-- SPDX-License-Identifier: Apache-2.0
-- Copyright (c) 2026 Matthias Haefner - Heosys
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use std.env.all;

entity fmcPowerCntrl_tb is
end entity fmcPowerCntrl_tb;

architecture sim of fmcPowerCntrl_tb is

    constant C_CLK_PERIOD   : time    := 12.5 ns; -- 80 MHz clock
    constant C_DELAY_TICKS  : natural := 10;      -- short delay for simulation

    signal clk_i          : std_logic := '0';
    signal pwrCntrl_i     : std_logic := '0';
    signal pgoodRelease_i : std_logic := '0';
    signal pgood_i        : std_logic := '1';
    signal pgood_oe       : std_logic;
    signal en_o           : std_logic;

begin

    uut : entity work.fmcPowerCntrl
        generic map(
            G_PGOOD_MONITOR_DELAY => C_DELAY_TICKS
        )
        port map(
            clk_i          => clk_i,
            pwrCntrl_i     => pwrCntrl_i,
            pgoodRelease_i => pgoodRelease_i,
            pgood_i        => pgood_i,
            pgood_oe       => pgood_oe,
            en_o           => en_o
        );

    clk_gen : process is
    begin
        while true loop
            clk_i <= '0';
            wait for C_CLK_PERIOD / 2;
            clk_i <= '1';
            wait for C_CLK_PERIOD / 2;
        end loop;
    end process clk_gen;

    stim_proc : process is
        -- wait n rising edges, then settle past the edge so registered
        -- outputs (one extra pipeline stage) are stable before checking
        procedure wait_cycles(n : positive) is
        begin
            for i in 1 to n loop
                wait until rising_edge(clk_i);
            end loop;
            wait for C_CLK_PERIOD / 4;
        end procedure wait_cycles;
    begin
        -- 1) initial state: pwrOff -> power disabled, pgood driven low
        pwrCntrl_i     <= '0';
        pgoodRelease_i <= '0';
        pgood_i        <= '1';
        wait_cycles(5);
        assert en_o = '0' report "Initial enable must be low" severity error;
        assert pgood_oe = '1' report "In pwrOff, pgood_oe must be high (drive pgood low)" severity error;

        -- 2) rising edge on pwrCntrl_i -> pwrOn: power enabled, pgood still forced low
        pwrCntrl_i <= '1';
        wait_cycles(3);
        assert en_o = '1' report "Enable must be high in pwrOn" severity error;
        assert pgood_oe = '1' report "pgood_oe must stay high in pwrOn (release not asserted)" severity error;

        -- 3) pgood is NOT monitored in pwrOn: a low pgood must not disable power
        pgood_i <= '0';
        wait_cycles(3);
        assert en_o = '1' report "pgood_i must be ignored while in pwrOn" severity error;
        assert pgood_oe = '1' report "pgood_oe must stay high in pwrOn" severity error;
        pgood_i <= '1';
        wait_cycles(1);

        -- 4) assert pgoodRelease_i -> delay state: pgood released (tristated)
        pgoodRelease_i <= '1';
        wait_cycles(3);
        assert en_o = '1' report "Power must stay enabled during delay" severity error;
        assert pgood_oe = '0' report "pgood_oe must be low during delay (tristate pgood)" severity error;

        -- 5) abort during delay: pwrCntrl_i low -> back to pwrOff
        pwrCntrl_i     <= '0';
        pgoodRelease_i <= '0';
        wait_cycles(3);
        assert en_o = '0' report "Power must turn off when aborted during delay" severity error;
        assert pgood_oe = '1' report "pgood_oe must return high after abort during delay" severity error;

        -- 6) full startup cycle: pwrOn again
        pwrCntrl_i <= '1';
        wait_cycles(3);
        assert en_o = '1' report "Power should enable again in second cycle" severity error;
        assert pgood_oe = '1' report "pgood_oe must be high in pwrOn of second cycle" severity error;

        -- 7) release pgood -> delay; pgood low during delay must be ignored
        pgoodRelease_i <= '1';
        wait_cycles(3);
        assert pgood_oe = '0' report "pgood_oe must be low during delay of second cycle" severity error;
        pgood_i <= '0';
        wait_cycles(2);
        assert en_o = '1' report "pgood_i must be ignored during delay (rise time window)" severity error;
        pgood_i <= '1'; -- restore before delay window expires

        -- 8) delay expires -> monitor: power stays on while pgood is high
        wait_cycles(C_DELAY_TICKS + 4);
        assert en_o = '1' report "Power must stay enabled in monitor with pgood high" severity error;
        assert pgood_oe = '0' report "pgood_oe must be low in monitor" severity error;
        wait_cycles(5);
        assert en_o = '1' report "Power must remain enabled while monitored pgood is high" severity error;

        -- 9) pgood failure in monitor -> pwrOff
        pgood_i <= '0';
        wait_cycles(3);
        assert en_o = '0' report "Power must turn off when pgood_i fails in monitor" severity error;
        assert pgood_oe = '1' report "pgood_oe must return high after pgood failure" severity error;

        -- 10) no auto-restart: pwrCntrl_i still high, but a new rising edge is required
        pgood_i <= '1';
        wait_cycles(5);
        assert en_o = '0' report "No restart without a new rising edge on pwrCntrl_i" severity error;

        -- 11) restart with a fresh rising edge on pwrCntrl_i
        pwrCntrl_i     <= '0';
        pgoodRelease_i <= '0';
        wait_cycles(2);
        pwrCntrl_i <= '1';
        wait_cycles(3);
        assert en_o = '1' report "Power should enable again after new rising edge" severity error;
        assert pgood_oe = '1' report "pgood_oe must be high in pwrOn of third cycle" severity error;

        -- 12) release pgood and run through delay into monitor
        pgoodRelease_i <= '1';
        wait_cycles(C_DELAY_TICKS + 6);
        assert en_o = '1' report "Power must be enabled in monitor of third cycle" severity error;
        assert pgood_oe = '0' report "pgood_oe must be low in monitor of third cycle" severity error;

        -- 13) final release: pwrCntrl_i low while in monitor -> pwrOff
        pwrCntrl_i <= '0';
        wait_cycles(3);
        assert en_o = '0' report "Final state must be off" severity error;
        assert pgood_oe = '1' report "pgood_oe must go high after final release" severity error;

        report "fmcPowerCntrl_tb: PASS";
        stop;
    end process stim_proc;

end architecture sim;
