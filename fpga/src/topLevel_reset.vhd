-- SPDX-License-Identifier: Apache-2.0
-- Copyright (c) 2026 Matthias Haefner - Heosys
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

--! @brief Top-level reset generator with watchdog pulse stretcher.
--! Generates a global, high-active synchronous reset for the core logic.
--! The reset is asserted while the PLL is unlocked or for at least
--! STRETCH_CYCLES clock cycles after a watchdog event (single-cycle
--! watchdog pulses are stretched to a guaranteed minimum duration).
--! Release of the reset is synchronized through a 4-stage shift register
--! to avoid metastability (asynchronous assert, synchronous release).
entity topLevel_reset is
    generic (
        -- Mindestdauer des Resets in Taktzyklen nach einem Watchdog-Event
        STRETCH_CYCLES : integer := 255 --! Minimum reset duration in clock cycles after a watchdog event (pulse stretcher reload value)
    );
    port (
        clk_i            : in  std_logic; --! System clock, rising edge active
        pll_locked_i     : in  std_logic; --! LOCKED signal from PLL (high = locked; low = asynchronously assert reset)
        watchdog_reset_i : in  std_logic; --! Watchdog reset request from softcore (high active, may be a single-cycle pulse)
        srst_o           : out std_logic  --! Global synchronous reset for the core logic (high active, released synchronously)
    );
end entity topLevel_reset;

architecture rtl of topLevel_reset is
    -- Signals for pulse stretcher
    signal s_wd_cnt : integer range 0 to STRETCH_CYCLES := 0;
    signal s_wd_rst : std_logic := '1'; 
    
    -- Shift-Register for synchronous release of the reset signal. 
    -- The reset is initialized to '1' to represent the active reset state.
    signal s_rst_shift : std_logic_vector(3 downto 0) := (others => '1');
begin

    ---------------------------------
    -- 1. Watchdog Pulse Stretcher --
    ---------------------------------
    process(clk_i, pll_locked_i)
    begin        
        if pll_locked_i = '0' then
            s_wd_cnt <= 0;
            s_wd_rst <= '1';
        elsif rising_edge(clk_i) then
            if watchdog_reset_i = '1' then                
                s_wd_cnt <= STRETCH_CYCLES;
                s_wd_rst <= '1';
            elsif s_wd_cnt > 0 then                
                s_wd_cnt <= s_wd_cnt - 1;
                s_wd_rst <= '1';
            else                
                s_wd_rst <= '0';
            end if;
        end if;
    end process;

    ------------------------------------------------------------------
    -- 2. Synchronisierungsstufe: Vermeidet Metastabilität beim Release
    ------------------------------------------------------------------
    process(clk_i, pll_locked_i)
    begin
        -- Assert reset when PLL is not locked 
        if pll_locked_i = '0' then
            s_rst_shift <= (others => '1');
        elsif rising_edge(clk_i) then
            --  Shift the wd-reset signal through a 4-stage shift register to synchronize it with the clock domain.
            s_rst_shift <= s_rst_shift(2 downto 0) & s_wd_rst;
        end if;
    end process;
    
    srst_o <= s_rst_shift(3);

end architecture rtl;
