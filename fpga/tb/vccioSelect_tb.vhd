-- SPDX-License-Identifier: Apache-2.0
-- Copyright (c) 2026 Matthias Haefner - Heosys 
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use std.env.all;

entity vccioSelect_tb is
end entity vccioSelect_tb;

architecture sim of vccioSelect_tb is

    constant C_CLK_PERIOD  : time    := 12.5 ns; -- 80 MHz clock
    constant C_DELAY_TICKS : natural := 10;      -- short make-before-break delay for simulation

    signal clk_i    : std_logic := '0';
    signal select_i : std_logic := '0';
    signal en1_o    : std_logic;
    signal en2_o    : std_logic;

begin

    uut : entity work.vccioSelect
        generic map(
            G_DELAY_TICKS => C_DELAY_TICKS
        )
        port map(
            clk_i    => clk_i,
            select_i => select_i,
            en1_o    => en1_o,
            en2_o    => en2_o
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

    -- Safety monitor: the two VCCIO rails must never be enabled at the
    -- same time (break-before-make). Samples the stable pre-edge values.
    monitor_proc : process(clk_i) is
    begin
        if rising_edge(clk_i) then
            assert not (en1_o = '1' and en2_o = '1')
                report "en1_o and en2_o are high simultaneously (break-before-make violated)"
                severity failure;
        end if;
    end process monitor_proc;

    -- Watchdog: abort if the stimulus does not finish in reasonable time
    watchdog_proc : process is
    begin
        wait for 50 us;
        assert false
            report "vccioSelect_tb: TIMEOUT - stimulus did not complete"
            severity failure;
    end process watchdog_proc;

    stim_proc : process is
        -- wait n rising edges, then settle past the edge so registered
        -- outputs are stable before checking
        procedure wait_cycles(n : positive) is
        begin
            for i in 1 to n loop
                wait until rising_edge(clk_i);
            end loop;
            wait for C_CLK_PERIOD / 4;
        end procedure wait_cycles;

        -- Wait until the expected enable pattern appears. While waiting,
        -- verify that both rails are off during the dead time (the previously
        -- active rail may stay high for one clock after select_i changed,
        -- because the change is detected synchronously). Also verify that
        -- the dead time lasts at least min_cycles and that the delay expires
        -- at all (timeout).
        procedure wait_enable(en1_exp    : std_logic;
                              en2_exp    : std_logic;
                              min_cycles : natural := C_DELAY_TICKS) is
            variable v_cnt : natural := 0;
        begin
            loop
                wait until rising_edge(clk_i);
                wait for C_CLK_PERIOD / 4;
                v_cnt := v_cnt + 1;
                exit when en1_o = en1_exp and en2_o = en2_exp;
                if v_cnt > 1 then
                    assert en1_o = '0' and en2_o = '0'
                        report "Both enables must be low during the delay phase"
                        severity error;
                end if;
                assert v_cnt <= C_DELAY_TICKS + 10
                    report "Timeout waiting for enable pattern - delay never expires"
                    severity failure;
            end loop;
            assert v_cnt >= min_cycles
                report "Enable asserted before the configured dead time elapsed"
                severity error;
        end procedure wait_enable;

        procedure check_rail(en1_exp : std_logic; en2_exp : std_logic; msg : string) is
        begin
            assert en1_o = en1_exp and en2_o = en2_exp
                report msg
                severity error;
        end procedure check_rail;
    begin
        -- 1) Power-up with select_i = '0': both rails off during the initial
        --    delay, then en1_o active after the dead time
        select_i <= '0';
        wait_cycles(2);
        check_rail('0', '0', "Initial: both enables must be low during power-up delay");
        wait_enable('1', '0');
        wait_cycles(5);
        check_rail('1', '0', "en1_o must stay active while select_i is stable at '0'");

        -- 2) Switch to select_i = '1': dead time with both rails off,
        --    then en2_o active
        select_i <= '1';
        wait_enable('0', '1');
        wait_cycles(5);
        check_rail('0', '1', "en2_o must stay active while select_i is stable at '1'");

        -- 3) Switch back to select_i = '0': dead time again, then en1_o active
        select_i <= '0';
        wait_enable('1', '0');
        wait_cycles(5);
        check_rail('1', '0', "en1_o must be active again after switching back to '0'");

        -- 4) Glitch during the delay phase: select_i toggles and returns
        --    before the delay expires. The latch tracks select_i during
        --    delay, so the final value ('0') must win and the running delay
        --    must NOT be restarted by the glitch.
        select_i <= '1';
        wait_cycles(3);
        check_rail('0', '0', "Both enables must be low during delay after select change");
        select_i <= '0'; -- glitch back before the delay expires
        wait_cycles(C_DELAY_TICKS + 2);
        check_rail('1', '0', "Glitch during delay: en1_o must be active without restarting the delay");

        -- 5) Short pulse on select_i while enabled: even a 2-cycle pulse
        --    must trigger a full dead time, afterwards the original rail
        --    (en1_o) must be re-enabled because select_i returned to '0'
        select_i <= '1';
        wait_cycles(2);
        select_i <= '0';
        wait_cycles(3);
        check_rail('0', '0', "Short select pulse must trigger the dead time (both rails off)");
        wait_enable('1', '0', min_cycles => 1); -- part of the dead time already elapsed above
        wait_cycles(5);
        check_rail('1', '0', "en1_o must be re-enabled after the short pulse and dead time");

        report "vccioSelect_tb: PASS";
        stop;
    end process stim_proc;

end architecture sim;
