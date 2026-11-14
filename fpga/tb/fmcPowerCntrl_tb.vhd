library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity fmcPowerCntrl_tb is
end entity fmcPowerCntrl_tb;

architecture sim of fmcPowerCntrl_tb is

    constant C_CLK_PERIOD          : time    := 10 ns;
    constant C_IGNORE_TICKS        : natural := 5;
    constant C_OVRRD_TICKS         : natural := 6;

    signal clk_i    : std_logic := '0';
    signal cntrl_i  : std_logic := '0';
    signal pgood_i  : std_logic := '1';
    signal pgood_oe : std_logic;
    signal en_o     : std_logic;

begin

    uut : entity work.fmcPowerCntrl
        generic map(
            G_PGOOD_OVRRD_TICKS => C_OVRRD_TICKS,
            G_PGOOD_IGNORE_TICKS => C_IGNORE_TICKS
        )
        port map(
            clk_i    => clk_i,
            cntrl_i  => cntrl_i,
            pgood_i  => pgood_i,
            pgood_oe => pgood_oe,
            en_o     => en_o
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
        procedure wait_cycles(n : positive) is
        begin
            for i in 1 to n loop
                wait until rising_edge(clk_i);
            end loop;
        end procedure wait_cycles;
    begin
        -- 1) initial state: disabled
        cntrl_i <= '0';
        pgood_i <= '1';
        wait_cycles(5);
        assert en_o = '0' report "Initial enable must be low" severity error;
        assert pgood_oe = '1' report "When control is low, pgood_oe must be high" severity error;

        -- 2) request startup; the debounce output should be forced low first
        cntrl_i <= '1';
        wait_cycles(2);
        assert pgood_oe = '1' report "pgood_oe should be high during the ignore phase" severity error;
        assert en_o = '1' report "Enable must be high after one clock cycle" severity error;

        -- 3) allow the ignore window to complete -> FSM reaches pwrOn
        wait_cycles(C_IGNORE_TICKS+1);
        assert pgood_oe = '0' report "pgood_oe should be low after the ignore phase" severity error;
        assert en_o = '1' report "Must remain enabled after the ignore period when pgood is high" severity error;
        
        -- 4) pgood failure while enabled must shut power down
        wait_cycles(1);
        pgood_i <= '0';
        wait_cycles(2);
        assert en_o = '0' report "Power must turn off when pgood_i goes low" severity error;
        
        -- 5) release control while in power-on state -> reset to pwrOff
        pgood_i <= '1';
        cntrl_i <= '0';
        wait_cycles(1);
        assert pgood_oe = '1' report "After release, pgood_oe should return high" severity error;
        assert en_o = '0' report "Power should be off after release" severity error;

        -- 6) second valid cycle
        cntrl_i <= '1';        
        wait_cycles(2);
        assert en_o = '1' report "Power should enable again in a valid second cycle" severity error;
        assert pgood_oe = '1' report "pgood_oe should be high during the ignore phase" severity error;
        
        -- 7) release control before the ignore window completes -> FSM returns to pwrOff
        cntrl_i <= '0';
        wait_cycles(2);
        assert pgood_oe = '1' report "pgood_oe should be high after early release" severity error;
        assert en_o = '0' report "Power should be off after early release" severity error;
        
        -- 8) third valid cycle
        wait_cycles(1); 
        cntrl_i <= '1';
        wait_cycles(2); 
        assert en_o = '1' report "Power should enable again in a valid third cycle" severity error;
        assert pgood_oe = '1' report "pgood_oe should be high during the ignore phase" severity error;
        
        -- 9) allow the ignore window to complete -> FSM reaches pwrOn
        wait_cycles(C_IGNORE_TICKS);
        assert pgood_oe = '0' report "pgood_oe should be low after the ignore phase" severity error;
        assert en_o = '1' report "Must remain enabled after the ignore period when pgood is high" severity error;
    
        -- 10) final release
        cntrl_i <= '0';
        wait_cycles(2);
        assert pgood_oe = '1' report "pgood_oe must go high after final release" severity error;
        assert en_o = '0' report "Final state must be off" severity error;

        report "fmcPowerCntrl_tb: PASS";
        wait;
    end process stim_proc;

end architecture sim;
