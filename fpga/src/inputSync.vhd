-- SPDX-License-Identifier: Apache-2.0
-- Copyright (c) 2026 Matthias Haefner - Heosys

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

--! @brief Multi-bit input synchronizer (vector wrapper around synchronizer).
--! Instantiates one single-bit synchronizer per bit of the input vector.
--! Each bit is synchronized independently into the clk_i clock domain;
--! there is no coherency guarantee between bits, so the module is intended
--! for unrelated status / control inputs, not for multi-bit buses that must
--! be sampled atomically.
entity inputSync is
	generic (
		WIDTH            : positive := 1;                            --! Number of input signals to synchronize (vector width)
		G_INIT_VAL       : std_logic_vector(WIDTH - 1 downto 0) := (others => '0'); --! Per-bit initial / reset value of the synchronizer flip-flops
		G_NUM_GUARD_FFS  : natural  := 1                             --! Number of guard flip-flops per bit after the capture stage (1 = classic 2-FF synchronizer)
	);
	port (
		clk_i     : in  std_logic;                            --! Destination clock domain, rising edge active
		rst_i     : in  std_logic;                            --! Asynchronous reset, high active (sets all synchronizers to G_INIT_VAL)
		signals_i : in  std_logic_vector(WIDTH - 1 downto 0); --! Asynchronous input vector (foreign clock domain)
		signals_o : out std_logic_vector(WIDTH - 1 downto 0)  --! Synchronized output vector in the clk_i domain
	);
end entity inputSync;

architecture rtl of inputSync is
begin
	synchronizers : for index in signals_i'range generate
		synchronizer_inst : entity work.synchronizer
			generic map (
				G_INIT_VAL      => G_INIT_VAL(index),
				G_NUM_GUARD_FFS => G_NUM_GUARD_FFS
			)
			port map (
				clk_i   => clk_i,
				rst_i   => rst_i,
				async_i => signals_i(index),
				sync_o  => signals_o(index)
			);
	end generate synchronizers;
end architecture rtl;
