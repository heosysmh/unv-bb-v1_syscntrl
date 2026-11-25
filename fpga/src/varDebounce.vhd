
-- SPDX-License-Identifier: Apache-2.0
-- Copyright (c) 2026 Matthias Haefner - Heosys

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

--! @brief Debounce / glitch filter with run-time configurable debounce time.
--! Identical functionality to debounce, but the number of required stable
--! clock cycles is supplied at run time via the nDebounceTicks input instead
--! of a generic. The output only follows the input after it has been stable
--! at the new level for nDebounceTicks consecutive clock cycles; shorter
--! pulses reset the internal counter.
entity varDebounce is
	generic(
		G_CNT_WIDTH : integer := 10 --! Bit width of the debounce counter and of the nDebounceTicks input (max. debounce time = 2**G_CNT_WIDTH - 1 ticks)
	);
	port(
		clk_i          : in  std_logic;                                  --! System clock, rising edge active
		nDebounceTicks : in  std_logic_vector(G_CNT_WIDTH - 1 downto 0); --! Number of consecutive stable clock cycles required before the output follows the input (unsigned, sampled continuously)
		signal_i       : in  std_logic;                                  --! Input signal to be debounced (may be asynchronous / bouncy)
		signal_o       : out std_logic                                   --! Debounced output signal (registered, initial value '0')
	);
end entity varDebounce;

architecture RTL of varDebounce is
	
	type t_state is (low,high);
	signal s_state_r : t_state := low;
	
	signal s_cnt_r : unsigned(G_CNT_WIDTH - 1 downto 0) := (others => '0');
	signal s_q_r : std_logic := '0';
	
begin

	fsm_proc : process(clk_i) is
	begin
		if rising_edge(clk_i) then
			case s_state_r is
				when low =>
					if signal_i = '1' then
						if s_cnt_r = unsigned(nDebounceTicks) then
							s_q_r <= '1';
							s_state_r <= high;
						else
							s_cnt_r <= s_cnt_r + 1;
						end if;						
					else
							s_cnt_r <= (others => '0');
					end if;
				when high =>
					if signal_i = '0' then
						if s_cnt_r = unsigned(nDebounceTicks) then
							s_q_r <= '0';
							s_state_r <= low;
						else
							s_cnt_r <= s_cnt_r + 1;
						end if;						
					else
							s_cnt_r <= (others => '0');
					end if;
			end case;
		end if;
	end process fsm_proc;
	
	signal_o <= s_q_r;

end architecture RTL;
