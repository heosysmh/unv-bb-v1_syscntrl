-- SPDX-License-Identifier: Apache-2.0
-- Copyright (c) 2026 Matthias Haefner - Heosys
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use IEEE.MATH_REAL.all;

--! @brief FMC supply power-up controller and power-good supervisor.
--! FSM sequence: pwrOff -> pwrOn (rising edge of pwrCntrl_i enables the
--! supply) -> delay (entered when the MCU releases pgood via pgoodRelease_i;
--! waits G_PGOOD_MONITOR_DELAY ticks for the rail to rise) -> monitor
--! (pgood_i is supervised). A low level on pwrCntrl_i in any state, or a
--! loss of pgood_i in the monitor state, immediately returns the FSM to
--! pwrOff and disables the supply. While the supply is off or ramping,
--! pgood_oe is high so the open-drain pgood line is actively pulled low,
--! preventing a false power-good indication towards the FMC module.
entity fmcPowerCntrl is
    generic (
        G_PGOOD_MONITOR_DELAY : natural := 8000 --! Number of clock ticks to wait after pgood release before pgood is monitored (accounts for rail rise time, 8000 ticks = 100 us @ 80 MHz)
    );
    port(
        clk_i          : in  std_logic; --! System clock, rising edge active
        pwrCntrl_i     : in  std_logic; --! Power control input from MCU (rising edge starts power-up, low = power off)
        pgoodRelease_i : in  std_logic; --! Power good release input from MCU (high = monitor pgood, low = force pgood low)
        pgood_i        : in  std_logic; --! Power good input from IO (synchronized supply power-good feedback)
        pgood_oe       : out std_logic; --! Power good output enable to IO (high = drive pgood line low, low = tristate pgood line)
        en_o           : out std_logic  --! Power enable output to IO (high = enable power, low = disable power)
    );
end entity fmcPowerCntrl;

architecture RTL of fmcPowerCntrl is   
    constant C_DELAY_CNT_WIDTH : natural := integer(ceil(log2(real(G_PGOOD_MONITOR_DELAY + 1))));

    type t_state is (pwrOff, pwrOn, delay,monitor);   
    signal s_state_r : t_state := pwrOff;
    
    signal s_dlyCnt_r : unsigned(C_DELAY_CNT_WIDTH-1 downto 0) := (others => '0');  

    signal s_pwrCntrl_r : std_logic := '0';
    
    signal s_en : std_logic;
    signal s_en_r : std_logic := '0';
    signal s_pgoodOe : std_logic;
    signal s_pgoodOe_r : std_logic := '1';

begin
    process(clk_i) is
    begin
        if rising_edge(clk_i) then
            s_pwrCntrl_r <= pwrCntrl_i;
            case s_state_r is
                when pwrOff =>
                    if pwrCntrl_i = '1' and s_pwrCntrl_r = '0' then
                        s_state_r <= pwrOn;
                    end if;
                when pwrOn =>                    
                    if pwrCntrl_i = '0' then
                        s_state_r <= pwrOff;                        
                    elsif pgoodRelease_i = '1' then
                        s_dlyCnt_r <= (others => '0');
                        s_state_r <= delay;                                            
                    end if;                
                when delay => 
                    if pwrCntrl_i = '0' then
                        s_state_r <= pwrOff;                        
                    elsif s_dlyCnt_r >= to_integer(to_unsigned(G_PGOOD_MONITOR_DELAY, s_dlyCnt_r'length)) then
                        s_state_r <= monitor;                        
                    else
                        s_dlyCnt_r <= s_dlyCnt_r + 1;
                    end if;
                when monitor =>
                    if pwrCntrl_i = '0' or pgood_i = '0' then
                        s_state_r <= pwrOff;                       
                    end if;
            end case;            
        end if;        
    end process;

    s_en <= '0' when s_state_r = pwrOff else '1';
    s_pgoodOe <= '0' when (s_state_r = delay or s_state_r = monitor) else '1';
    
    process(clk_i) is
    begin
       if rising_edge(clk_i) then
          s_en_r <= s_en;
           s_pgoodOe_r <= s_pgoodOe;
       end if;
    end process;
   
   en_o <= s_en_r;
   pgood_oe <= s_pgoodOe_r;

end architecture RTL;
