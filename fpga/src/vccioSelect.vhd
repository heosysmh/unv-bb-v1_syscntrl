-- SPDX-License-Identifier: Apache-2.0
-- Copyright (c) 2026 Matthias Haefner - Heosys
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use IEEE.MATH_REAL.all;

--! @brief Break-before-make selector between two VCCIO power rails.
--! Selects which of two mutually exclusive power rails is enabled
--! (select_i = '0' -> en1_o, select_i = '1' -> en2_o). On every change of
--! select_i both enables are deasserted for G_DELAY_TICKS clock cycles
--! before the newly selected rail is enabled, so the two rails are never
--! active at the same time. After reset the module starts in the delay
--! state with both rails disabled.
entity vccioSelect is
    generic (
        G_DELAY_TICKS : natural := 80000 --! Number of clock ticks with both rails disabled during a switch-over (break-before-make delay, 80000 ticks = 1 ms @ 80 MHz)
    );
    port(
        clk_i    : in  std_logic; --! System clock, rising edge active
        select_i : in  std_logic; --! Rail select input ('0' = enable rail 1 via en1_o, '1' = enable rail 2 via en2_o)
        en1_o    : out std_logic; --! Enable output for VCCIO rail 1 (high = enable power, low = disable power)
        en2_o    : out std_logic  --! Enable output for VCCIO rail 2 (high = enable power, low = disable power)
    );
end entity vccioSelect;

architecture RTL of vccioSelect is   
    constant C_DELAY_CNT_WIDTH : natural := integer(ceil(log2(real(G_DELAY_TICKS + 1))));

    type t_state is (delay, enable);   
    signal s_state_r : t_state := delay;
    
    signal s_dlyCnt_r : unsigned(C_DELAY_CNT_WIDTH-1 downto 0) := (others => '0');  

    signal s_selectLatch_r : std_logic := '0';

    
    signal s_en1_r : std_logic := '0';
    signal s_en2_r : std_logic := '0';
    

begin
    process(clk_i) is
    begin
        if rising_edge(clk_i) then
            case s_state_r is
                when delay =>
                    s_selectLatch_r <= select_i;
                    s_en1_r <= '0';
                    s_en2_r <= '0';
                    if s_dlyCnt_r >= to_integer(to_unsigned(G_DELAY_TICKS, s_dlyCnt_r'length)) then
                        s_state_r <= enable;
                    else
                        s_dlyCnt_r <= s_dlyCnt_r + 1;
                    end if;
                when enable =>                    
                    if s_selectLatch_r = '0' then
                        s_en1_r <= '1';
                        s_en2_r <= '0';                       
                    else
                        s_en1_r <= '0';
                        s_en2_r <= '1';                                          
                    end if;          
                    if s_selectLatch_r /= select_i then
                        s_state_r <= delay;
                        s_dlyCnt_r <= (others => '0');
                    end if;
            end case;                      
        end if;        
    end process;

    en1_o <= s_en1_r;
    en2_o <= s_en2_r;
    

end architecture RTL;
