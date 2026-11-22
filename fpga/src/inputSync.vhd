-- SPDX-License-Identifier: Apache-2.0
-- Copyright (c) 2026 Matthias Haefner - Heosys

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity inputSync is
	generic (
		WIDTH            : positive := 1;
		G_INIT_VAL       : std_logic_vector(WIDTH - 1 downto 0) := (others => '0');
		G_NUM_GUARD_FFS  : natural  := 1
	);
	port (
		clk_i    : in  std_logic;
		rst_i    : in  std_logic;
		signals_i : in  std_logic_vector(WIDTH - 1 downto 0);
		signals_o : out std_logic_vector(WIDTH - 1 downto 0)
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
