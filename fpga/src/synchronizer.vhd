-- SPDX-License-Identifier: Apache-2.0
-- Copyright (c) 2026 Matthias Haefner - Heosys

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

--! @brief Single-bit clock-domain-crossing synchronizer.
--! Synchronizes an asynchronous input into the clk_i clock domain using a
--! capture flip-flop followed by G_NUM_GUARD_FFS guard flip-flops
--! (2-FF synchronizer with G_NUM_GUARD_FFS = 1). The ASYNC_REG attribute
--! keeps the flip-flops close together and prevents X propagation on timing
--! violations, shreg_extract disables SRL inference. Asynchronous,
--! high-active reset initializes all stages to G_INIT_VAL.
entity synchronizer is
    generic (          
            G_INIT_VAL      : std_logic := '0'; --! Initial / reset value of all synchronizer flip-flops
            G_NUM_GUARD_FFS : natural := 1      --! Number of guard flip-flops after the capture stage (1 = classic 2-FF synchronizer, >= 2 for higher MTBF)
            );
    port ( 
            clk_i   : in  std_logic; --! Destination clock domain, rising edge active
            rst_i   : in  std_logic; --! Asynchronous reset, high active (sets all stages to G_INIT_VAL)
            async_i : in  std_logic; --! Asynchronous input signal (foreign clock domain)
            sync_o  : out std_logic  --! Synchronized output signal in the clk_i domain
			 );
end synchronizer;

architecture Behavioral of synchronizer is
	signal s_sync_r   : std_logic                                      := '0';
	signal s_guard_r  : std_logic_vector(G_NUM_GUARD_FFS - 1 downto 0) := (others => G_INIT_VAL);

  -- Disable shift register (SRL) extraction (Xilinx XST)  
  attribute shreg_extract : string;
  attribute shreg_extract of s_sync_r : signal is "no";
  attribute shreg_extract of s_guard_r : signal is "no";

  -- Disable X propagation during simulation. In the event of 
  -- a timing violation, the previous value is retained on the output instead 
  -- of going unknown (see Xilinx UG625)
  attribute ASYNC_REG : string;
  attribute ASYNC_REG of s_sync_r : signal is "TRUE";


begin
	process(clk_i,rst_i)
	begin
		if(rst_i = '1') then
			s_sync_r  <= G_INIT_VAL;
			s_guard_r <= (others => G_INIT_VAL);
		elsif(rising_edge(clk_i)) then
      s_sync_r <= async_i;			
			if s_guard_r'length = 1 then
         s_guard_r(0) <= s_sync_r;
      else
        s_guard_r <= s_guard_r(s_guard_r'high - 1 downto 0) & s_sync_r;
      end if;  
		end if;
	end process;		
	-- output logic
	sync_o <= s_guard_r(s_guard_r'high);
	
end Behavioral;
