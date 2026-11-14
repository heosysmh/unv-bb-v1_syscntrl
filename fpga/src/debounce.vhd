library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use IEEE.MATH_REAL.all;

entity debounce is
	generic(
		G_DEBOUNCE_TICKS : integer := 1000000
	);
	port(
		clk_i          : in  std_logic;		
		signal_i       : in  std_logic;
		signal_o       : out std_logic
	);
end entity debounce;

architecture RTL of debounce is
	
    constant C_CNT_WIDTH : natural := integer(ceil(log2(real(G_DEBOUNCE_TICKS + 1))));

	type t_state is (low,high);
	signal s_state_r : t_state := low;
	
	signal s_cnt_r : unsigned(C_CNT_WIDTH - 1 downto 0) := (others => '0');
	signal s_q_r : std_logic := '0';
	
begin

	fsm_proc : process(clk_i) is
	begin
		if rising_edge(clk_i) then
			case s_state_r is
				when low =>
					if signal_i = '1' then
						if s_cnt_r = to_unsigned(G_DEBOUNCE_TICKS, C_CNT_WIDTH) then
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
						if s_cnt_r = to_unsigned(G_DEBOUNCE_TICKS, C_CNT_WIDTH) then
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
