library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use IEEE.MATH_REAL.all;

entity vccioSelect is
    generic (
        G_DELAY_TICKS : natural := 80000 -- number of clock ticks for make before break delay (for 80MHz clock, 80000 ticks = 1ms)
    );
    port(
        clk_i     : in  std_logic;        
        select_i  : in  std_logic;  -- select input
        en1_o    : out  std_logic;  -- enable output for VCCIO1 (high = enable power, low = disable power)
        en2_o    : out  std_logic   -- enable output for VCCIO2 (high = enable power, low = disable power)     
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