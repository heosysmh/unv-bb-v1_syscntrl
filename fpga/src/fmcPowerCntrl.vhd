library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use IEEE.MATH_REAL.all;

entity fmcPowerCntrl is
    generic(       
        G_PGOOD_OVRRD_TICKS : integer := 500000;
        G_PGOOD_IGNORE_TICKS  : integer := 1000000
    );
    port(
        clk_i    : in  std_logic;        
        cntrl_i  : in  std_logic;        
        pgood_i  : in  std_logic;
        pgood_oe : out std_logic;
        en_o     : out std_logic       
    );
end entity fmcPowerCntrl;

architecture RTL of fmcPowerCntrl is
    constant C_PGOOD_CNT_WIDTH : natural := integer(ceil(log2(real(G_PGOOD_IGNORE_TICKS + 1))));
   
    type t_state is (pwrOff, ovrrdPgood, ignorePgood, pwrOn);
    signal s_state_r : t_state := pwrOff;

    signal s_pgoodCnt_r : unsigned(C_PGOOD_CNT_WIDTH - 1 downto 0) := (others => '0');    

    signal s_cntrl_r : std_logic := '0';
begin
    assert (G_PGOOD_IGNORE_TICKS > G_PGOOD_OVRRD_TICKS)
        report "G_PGOOD_IGNORE_TICKS must be greater than G_PGOOD_OVRRD_TICKS"
        severity failure;
  

    process(clk_i) is
    begin
        if rising_edge(clk_i) then
            s_cntrl_r <= cntrl_i;
            case s_state_r is
                when pwrOff =>
                    if cntrl_i = '1' and s_cntrl_r = '0' then
                        s_state_r <= ovrrdPgood;
                    end if;
                when ovrrdPgood =>
                    s_pgoodCnt_r <= s_pgoodCnt_r + 1;
                    if cntrl_i = '0' then
                        s_state_r <= pwrOff;
                        s_pgoodCnt_r <= (others => '0');
                    elsif s_pgoodCnt_r = to_unsigned(G_PGOOD_OVRRD_TICKS-1, C_PGOOD_CNT_WIDTH) then
                        s_state_r <= ignorePgood;                                            
                    end if;
                when ignorePgood =>                
                    if cntrl_i = '0' then
                        s_state_r <= pwrOff;
                        s_pgoodCnt_r <= (others => '0');
                    elsif s_pgoodCnt_r = to_unsigned(G_PGOOD_IGNORE_TICKS-1, C_PGOOD_CNT_WIDTH) then
                        s_state_r <= pwrOn;
                    else
                        s_pgoodCnt_r <= s_pgoodCnt_r + 1;
                    end if;
                when pwrOn =>
                    if cntrl_i = '0' or pgood_i = '0' then
                        s_state_r <= pwrOff;
                        s_pgoodCnt_r <= (others => '0');
                    end if;
            end case;
        end if;
    end process;


    en_o <= '0' when s_state_r = pwrOff else '1';
    pgood_oe <= '1' when s_state_r = pwrOff or s_state_r = ovrrdPgood else '0';

end architecture RTL;