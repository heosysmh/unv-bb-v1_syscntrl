--! use standard library
library IEEE;
--! use logic elements
use IEEE.STD_LOGIC_1164.all;

use IEEE.NUMERIC_STD.all;
use IEEE.MATH_REAL.all;

entity sysCntrl_Top is
  port(
--MISC/PLL/Reset    
    pll_locked  : in  std_logic;
    sysClk_i    : in  std_logic;    
-- SPI
    spi_0_io_sclk_o               : out std_logic;
    spi_0_io_data_0_read_i        : in  std_logic;
    spi_0_io_data_0_write_o       : out std_logic;
    spi_0_io_data_0_writeEnable_o : out std_logic;
    spi_0_io_data_1_read_i        : in  std_logic;
    spi_0_io_data_1_write_o       : out std_logic;
    spi_0_io_data_1_writeEnable_o : out std_logic;
    spi_0_io_ss_o                 : out std_logic;
    spi_0_io_hold_o               : out std_logic;
    spi_0_io_wp_o                 : out std_logic;
--JTAG
    jtag_tck     : in  std_logic;  
    jtag_tdi     : in  std_logic;
    jtag_tdo     : out std_logic;
    jtag_capture : in  std_logic;
    jtag_sel     : in  std_logic;
    jtag_shift   : in  std_logic;
    jtag_update  : in  std_logic;
    jtag_reset   : in  std_logic;
--Unused JTAG signals
    --jtag_drck    : in  std_logic;
    --jtag_tms     : in  std_logic;
    --jtag_runtest : in  std_logic;
-- SoM power control 
    som_vccoPsEn_i : in std_logic;
    som_vccoPlEn_i : in std_logic;
    som_sdPwrEn_i : in std_logic;
    som_hpa14p_i : in std_logic;
    som_hpa14n_i : in std_logic;    
    pmu_mio35GPO_i : in std_logic;
    pmu_mio28GPI_o : out std_logic;
    pmu_mio29GPI_o : out std_logic;
    pmu_mio31GPI_o : out std_logic;        
-- PL power control    
    hpc_vadj1En_o  : out std_logic;
    hpc_vadj2En_o  : out std_logic;
    hdb_vadj2En_o  : out std_logic;
    hdb_2v5En_o    : out std_logic;
    pl_pwrEn_o     : out std_logic;
    pl_2v5Pgood_i  : in std_logic;
    pl_2v5Pgood_oe : out std_logic;
-- PS power control
    ps_pwrEn_o : out std_logic;
-- System power control
    sys_pwrEn_o  : out std_logic;
    sys_pgood_i  : in std_logic;
    sys_pgood_oe : out std_logic;
-- SYZYGY power control
    szg_pwrEn_o  : out std_logic;
    szg_pgood_i  : in std_logic;
    szg_pgood_oe : out std_logic;
-- FMC power control
    fmc1_supPwrEn_o : out std_logic;
    fmc1_supPgood_i : in std_logic;
    fmc1_supPgood_oe : out std_logic;
    fmc2_supPwrEn_o : out std_logic;
    fmc2_supPgood_i : in std_logic;
    fmc2_supPgood_oe : out std_logic;
    -- fmc1_ModPresent_i : in std_logic; -- Low active / missing on the board
    fmc1_modPgood_i : in std_logic;
    fmc1_modPgood_oe : out std_logic;
    -- fmc2_ModPresent_i : in std_logic; -- Low active / missing on the board
    fmc2_modPgood_i : in std_logic;
    fmc2_modPgood_oe : out std_logic;
-- Clock gnererator control
    si5332_cntrl_o : out std_logic_vector(4 downto 0);
    lmk05028_en_o : out std_logic;
    lmk05028_intr_i : in std_logic;
-- Ethernet PHY
    eth_pme_i : in std_logic;
-- reset signals
    sd_reset_o : out std_logic;   -- high active (open drain in interface)
    sd_reset_i : in std_logic;    -- dummy signal (not connected to anything, used to avoid synthesis warnings)
    eth_reset_o : out std_logic;  -- high active (open drain in interface)
    eth_reset_i : in std_logic;   -- dummy signal (not connected to anything, used to avoid synthesis warnings)
    i2c0_reset_o : out std_logic; -- High active
    ps_resetn_o : out std_logic;  -- Low active
    ps_porn_o : out std_logic;    -- Low active
-- I2C interfaces
    sda0_i     : in std_logic;
    sda0_o     : out std_logic;    
    scl0_i     : in std_logic;
    scl0_o     : out std_logic;    
    sda1_i     : in std_logic;
    sda1_o     : out std_logic;    
    scl1_i     : in std_logic;
    scl1_o     : out std_logic;    
-- UART 
    uart_rxd_i : in std_logic;
    uart_txd_o : out std_logic;
-- Fan control
    fan_pwm_o : out std_logic;
-- User
    config_i : in std_logic_vector(5 downto 0);
-- LED naming on silkscreen is switched with respect to the schematic. Corrected in Interface designer.
    leds_o : out std_logic_vector(1 downto 0) 
  );
end sysCntrl_Top;

architecture rtl of sysCntrl_Top is
    
    component mcu is
    port (
        io_systemClk : in std_logic;
        jtagCtrl_enable : in std_logic;
        jtagCtrl_tdi : in std_logic;
        jtagCtrl_capture : in std_logic;
        jtagCtrl_shift : in std_logic;
        jtagCtrl_update : in std_logic;
        jtagCtrl_reset : in std_logic;
        jtagCtrl_tdo : out std_logic;
        jtagCtrl_tck : in std_logic;
        system_spi_0_io_data_0_read : in std_logic;
        system_spi_0_io_data_0_write : out std_logic;
        system_spi_0_io_data_0_writeEnable : out std_logic;
        system_spi_0_io_data_1_read : in std_logic;
        system_spi_0_io_data_1_write : out std_logic;
        system_spi_0_io_data_1_writeEnable : out std_logic;
        system_spi_0_io_data_2_read : in std_logic;
        system_spi_0_io_data_2_write : out std_logic;
        system_spi_0_io_data_2_writeEnable : out std_logic;
        system_spi_0_io_data_3_read : in std_logic;
        system_spi_0_io_data_3_write : out std_logic;
        system_spi_0_io_data_3_writeEnable : out std_logic;
        system_spi_0_io_sclk_write : out std_logic;
        userInterruptB : in std_logic;
        userInterruptA : in std_logic;
        io_asyncReset : in std_logic;
        io_systemReset : out std_logic;
        system_uart_0_io_txd : out std_logic;
        system_uart_0_io_rxd : in std_logic;
        system_i2c_1_io_sda_read : in std_logic;
        system_i2c_1_io_scl_write : out std_logic;
        system_i2c_1_io_scl_read : in std_logic;
        system_i2c_1_io_sda_write : out std_logic;
        system_i2c_0_io_scl_read : in std_logic;
        system_i2c_0_io_scl_write : out std_logic;
        system_i2c_0_io_sda_read : in std_logic;
        system_i2c_0_io_sda_write : out std_logic;
        system_gpio_0_io_writeEnable : out std_logic_vector(31 downto 0);
        system_gpio_0_io_write : out std_logic_vector(31 downto 0);
        system_gpio_1_io_write : out std_logic_vector(31 downto 0);
        system_gpio_1_io_read : in std_logic_vector(31 downto 0);
        system_gpio_1_io_writeEnable : out std_logic_vector(31 downto 0);
        system_gpio_0_io_read : in std_logic_vector(31 downto 0);
        system_spi_0_io_ss : out std_logic_vector(0 to 0);
        system_watchdog_hardPanic : out std_logic
    );
    end component mcu;

    constant C_100US_DELAY_TICKS : integer :=    8000;  -- 100us delay
    constant C_1MS_DELAY_TICKS   : integer :=   80000;  --   1ms delay
    constant C_5MS_DELAY_TICKS   : integer :=  400000;  --   5ms delay
    constant C_10MS_DELAY_TICKS  : integer :=  800000;  --  10ms delay
    constant C_20MS_DELAY_TICKS  : integer := 1600000;  --  20ms delay
    constant C_100MS_DELAY_TICKS : integer := 8000000;  -- 100ms delay

    signal s_srst : std_logic;
    signal s_clk : std_logic;

    signal s_usrIntrA : std_logic;
    signal s_usrIntrB : std_logic;
    signal s_gpio0_wre : std_logic_vector(31 downto 0);
    signal s_gpio0_wr : std_logic_vector(31 downto 0);
    signal s_gpio0_rd : std_logic_vector(31 downto 0);
    signal s_gpio1_wre : std_logic_vector(31 downto 0);
    signal s_gpio1_wr : std_logic_vector(31 downto 0);
    signal s_gpio1_rd : std_logic_vector(31 downto 0);
    signal s_wd_hardPanic : std_logic;        
    
    -- Signals and parameters for input synchronization
    signal s_inputs : std_logic_vector(20 downto 0);
    signal s_inputs_sync : std_logic_vector(20 downto 0);
    constant C_SYNC_INPUTS_INIT : std_logic_vector(20 downto 0) := (others => '0');

    signal s_som_vccoPsEn : std_logic;
    signal s_som_vccoPlEn : std_logic;
    signal s_som_sdPwrEn : std_logic;
    signal s_som_hpa14p : std_logic;
    signal s_som_hpa14n : std_logic;
    signal s_pmu_mio35GPO : std_logic;
    signal s_fmc1_supPgood : std_logic;
    signal s_fmc1_supPgood_dlyd : std_logic;
    signal s_fmc2_supPgood : std_logic;
    signal s_fmc2_supPgood_dlyd : std_logic;
    signal s_fmc1_modPgood : std_logic;
    signal s_fmc2_modPgood : std_logic;
    signal s_pl_2v5Pgood : std_logic;
    signal s_sys_pgood : std_logic;
    signal s_szg_pgood : std_logic;    
    signal s_lmk05028_intr : std_logic;
    signal s_eth_pme : std_logic;
    signal s_config_sync : std_logic_vector(5 downto 0);
    signal s_config : std_logic_vector(5 downto 0);
    
    signal s_ledCount_r : unsigned(25 downto 0) := (others => '0');

    signal s_sys_pwrEn      : std_logic;
    signal s_sys_pwrEn_dlyd : std_logic;

    signal s_pl_pwrEn       : std_logic;
    signal s_pl_pwrEn_dlyd  : std_logic;

    signal s_ps_pwrEn       : std_logic;
    signal s_ps_pwrEn_dlyd  : std_logic;
    
    signal s_szg_pwrEn      : std_logic;
    signal s_szg_pwrEn_dlyd : std_logic;

    signal s_porn     : std_logic;
    signal s_eth_rstn : std_logic;

    signal s_mcu_hpc_VccioSelect : std_logic;
    signal s_mcu_hdb_VccioSelect : std_logic;

    signal s_mcu_fmc1_supCntrl : std_logic;
    signal s_mcu_fmc2_supCntrl : std_logic;

    signal s_mcu_fmc1_supPgoodRelease : std_logic;
    signal s_mcu_fmc2_supPgoodRelease : std_logic;

    signal s_mcu_fmc1_modCntrl : std_logic;
    signal s_mcu_fmc2_modCntrl : std_logic;

    signal s_mcu_porn : std_logic;
    signal s_mcu_eth_rstn : std_logic;

    signal s_mcu_led : std_logic;

    signal s_i2c0_sda_out : std_logic;
    signal s_i2c0_scl_out : std_logic;

    signal s_i2c1_sda_out : std_logic;
    signal s_i2c1_scl_out : std_logic;
begin 

    -- Tie unused SPI signals to default values to avoid synthesis warnings
    spi_0_io_hold_o <= '1';
    spi_0_io_wp_o <= '1';    

    s_clk <= sysClk_i;

    -- Create a vector of input signals to be synchronized
    s_inputs(0)  <= som_vccoPsEn_i;
    s_inputs(1)  <= som_vccoPlEn_i;
    s_inputs(2)  <= som_sdPwrEn_i;
    s_inputs(3)  <= som_hpa14p_i;
    s_inputs(4)  <= som_hpa14n_i;
    s_inputs(5)  <= pmu_mio35GPO_i;
    s_inputs(6)  <= fmc1_supPgood_i;
    s_inputs(7)  <= fmc2_supPgood_i;
    s_inputs(8)  <= fmc1_modPgood_i;
    s_inputs(9)  <= fmc2_modPgood_i;
    s_inputs(10) <= pl_2v5Pgood_i;
    s_inputs(11) <= sys_pgood_i;
    s_inputs(12) <= szg_pgood_i;
    s_inputs(13) <= lmk05028_intr_i;
    s_inputs(14) <= eth_pme_i;
    s_inputs(20 downto 15) <= config_i;

    -- Synchronize the input signals using the inputSync component
    inputSync_inst : entity work.inputSync
    generic map (
        WIDTH            => 21,
        G_INIT_VAL       => C_SYNC_INPUTS_INIT,
        G_NUM_GUARD_FFS  => 2
    )
    port map (
        clk_i     => s_clk,
        rst_i     => s_srst,
        signals_i => s_inputs,
        signals_o => s_inputs_sync
    );

    s_gpio0_rd(20 downto 0) <= s_inputs_sync(20 downto 0);
    s_gpio0_rd(31 downto 21) <= (others => '0');
    
    -- Split the synchronized signals into individual signals for easier access
    s_som_vccoPsEn  <= s_inputs_sync(0);
    s_som_vccoPlEn  <= s_inputs_sync(1);
    s_som_sdPwrEn   <= s_inputs_sync(2);
    s_som_hpa14p    <= s_inputs_sync(3);
    s_som_hpa14n    <= s_inputs_sync(4);
    s_pmu_mio35GPO  <= s_inputs_sync(5);
    s_fmc1_supPgood <= s_inputs_sync(6);
    s_fmc2_supPgood <= s_inputs_sync(7);
    s_fmc1_modPgood <= s_inputs_sync(8);
    s_fmc2_modPgood <= s_inputs_sync(9);
    s_pl_2v5Pgood   <= s_inputs_sync(10);
    s_sys_pgood     <= s_inputs_sync(11);
    s_szg_pgood     <= s_inputs_sync(12);
    s_lmk05028_intr <= s_inputs_sync(13);
    s_eth_pme       <= s_inputs_sync(14);
    s_config_sync   <= s_inputs_sync(20 downto 15);
       
config_debounce:
    for i in 0 to 5 generate
        config_debounce_inst : entity work.debounce
        generic map (
            G_DEBOUNCE_TICKS => C_10MS_DELAY_TICKS
        )
        port map (
            clk_i          => s_clk,            
            signal_i       => not s_config_sync(i), -- config pins are active low
            signal_o       => s_config(i)
        );
    end generate config_debounce;
    
    s_sys_pwrEn <= '1';

    /* Control signals from the MCU */
    s_mcu_hpc_VccioSelect       <= s_gpio1_wr(0);
    s_mcu_hdb_VccioSelect       <= s_gpio1_wr(1);    
    s_mcu_fmc1_supCntrl         <= s_gpio1_wr(2);
    s_mcu_fmc2_supCntrl         <= s_gpio1_wr(3);
    s_mcu_fmc1_supPgoodRelease  <= s_gpio1_wr(4);
    s_mcu_fmc2_supPgoodRelease  <= s_gpio1_wr(5);
    s_mcu_fmc1_modCntrl         <= s_gpio1_wr(6);
    s_mcu_fmc2_modCntrl         <= s_gpio1_wr(7);
    s_mcu_porn                  <= s_gpio1_wr(8);
    s_mcu_eth_rstn              <= s_gpio1_wr(9);
    s_mcu_led                   <= s_gpio1_wr(31);

    s_gpio1_rd(31 downto 0) <= (others => '0');

    s_usrIntrA <= '0';
    s_usrIntrB <= '0';
    
    mcu_inst : mcu
    port map (
        io_systemClk   => sysClk_i,
        io_asyncReset  => not pll_locked,
        io_systemReset => s_srst,
        
        jtagCtrl_enable  => jtag_sel,
        jtagCtrl_tdi     => jtag_tdi,
        jtagCtrl_capture => jtag_capture,
        jtagCtrl_shift   => jtag_shift,
        jtagCtrl_update  => jtag_update,
        jtagCtrl_reset   => jtag_reset,
        jtagCtrl_tdo     => jtag_tdo,
        jtagCtrl_tck     => jtag_tck,
        
        system_spi_0_io_data_0_read        => spi_0_io_data_0_read_i,
        system_spi_0_io_data_0_write       => spi_0_io_data_0_write_o,
        system_spi_0_io_data_0_writeEnable => spi_0_io_data_0_writeEnable_o,
        system_spi_0_io_data_1_read        => spi_0_io_data_1_read_i,
        system_spi_0_io_data_1_write       => spi_0_io_data_1_write_o,
        system_spi_0_io_data_1_writeEnable => spi_0_io_data_1_writeEnable_o,
        system_spi_0_io_data_2_read        => '0',
        system_spi_0_io_data_2_write       => open,
        system_spi_0_io_data_2_writeEnable => open,
        system_spi_0_io_data_3_read        => '0',
        system_spi_0_io_data_3_write       => open,
        system_spi_0_io_data_3_writeEnable => open,
        system_spi_0_io_sclk_write         => spi_0_io_sclk_o,
        system_spi_0_io_ss(0)              => spi_0_io_ss_o,
        
        userInterruptB => s_usrIntrB,
        userInterruptA => s_usrIntrA,
                
        system_uart_0_io_txd => uart_txd_o,
        system_uart_0_io_rxd => uart_rxd_i,
        
        system_i2c_0_io_sda_read  => sda0_i,
        system_i2c_0_io_sda_write => s_i2c0_sda_out,
        system_i2c_0_io_scl_read  => scl0_i,
        system_i2c_0_io_scl_write => s_i2c0_scl_out,
        system_i2c_1_io_sda_read  => sda1_i,
        system_i2c_1_io_sda_write => s_i2c1_sda_out,
        system_i2c_1_io_scl_read  => scl1_i,
        system_i2c_1_io_scl_write => s_i2c1_scl_out,
                      
        system_gpio_0_io_writeEnable => s_gpio0_wre,
        system_gpio_0_io_write       => s_gpio0_wr,
        system_gpio_0_io_read        => s_gpio0_rd,
        system_gpio_1_io_writeEnable => s_gpio1_wre,
        system_gpio_1_io_write       => s_gpio1_wr,
        system_gpio_1_io_read        => s_gpio1_rd,
                       
        system_watchdog_hardPanic => s_wd_hardPanic
    );

    process(s_clk)
    begin
        if rising_edge(s_clk) then
            s_ledCount_r <= s_ledCount_r + 1;
        end if;
    end process;

    ----------------------------
    --- System power control ---
    ----------------------------
    -- Delay system power enable for power good signal release
    sysPwrEn_delay_inst : entity work.debounce
    generic map (
        G_DEBOUNCE_TICKS => C_5MS_DELAY_TICKS
    )
    port map (
        clk_i          => s_clk,
        signal_i       => s_sys_pwrEn,
        signal_o       => s_sys_pwrEn_dlyd
    );

    sys_PwrEn_o <= s_sys_pwrEn;
    --override power good signal when the power is disabled to avoid false power good indication
    sys_pgood_oe <= not s_sys_pwrEn_dlyd;  

    ----------------------------
    ----- PL power control -----
    ----------------------------
    -- Enable PL when system power is good and SoM VCCO_PL_EN is high    
    s_pl_pwrEn <= s_sys_pgood and s_som_vccoPlEn;    
    pl_PwrEn_o <= s_pl_pwrEn;
        
    -- Delay PL power enable for power good signal release
    plPwrEn_delay_inst : entity work.debounce
    generic map (
        G_DEBOUNCE_TICKS => C_5MS_DELAY_TICKS
    )
    port map (
        clk_i          => s_clk,
        signal_i       => s_pl_pwrEn,
        signal_o       => s_pl_pwrEn_dlyd
    ); 
    --override power good signal when the power is disabled to avoid false power good indication
    pl_2v5Pgood_oe <= not s_pl_pwrEn_dlyd;

    ----------------------------
    ----- PS power control -----
    ----------------------------
    -- Enable PS when system power is good and SoM VCCO_PS_EN is high
    s_ps_pwrEn <= s_sys_pgood and s_som_vccoPsEn;
    ps_PwrEn_o <= s_ps_pwrEn;

    ----------------------------
    ----- VCCIO selection ------
    ----------------------------
    -- HPC VCCIO can be selected between FMC Vadj1/2 power rails. The selection is controlled by the MCU via GPIOs.
    hpcVccioSelect : entity work.vccioSelect
        generic map(
            G_DELAY_TICKS => C_1MS_DELAY_TICKS
        )
        port map(
            clk_i    => s_clk,
            select_i => s_mcu_hpc_VccioSelect,
            en1_o    => hpc_Vadj2En_o,
            en2_o    => hpc_Vadj1En_o
        );

    -- HDB VCCIO can be selected between FMC Vadj2 and PL2V5 power rails. The selection is controlled by the MCU via GPIOs.
    hdbVccioSelect : entity work.vccioSelect
        generic map(
            G_DELAY_TICKS => C_1MS_DELAY_TICKS
        )
        port map(
            clk_i    => s_clk,
            select_i => s_mcu_hdb_VccioSelect,
            en1_o    => hdb_Vadj2En_o,
            en2_o    => hdb_2v5En_o
        );

    ----------------------------
    ---- FMC1 power control ----
    ----------------------------
    -- FMC power-up ist controlled and monitored by mcu
    fmc1PowerCntrl_inst : entity work.fmcPowerCntrl
    generic map (
        G_PGOOD_MONITOR_DELAY => C_100US_DELAY_TICKS
    )
    port map (
        clk_i           => s_clk,
        pwrCntrl_i      => s_mcu_fmc1_supCntrl,
        pgoodRelease_i  => s_mcu_fmc1_supPgoodRelease,
        pgood_i         => s_fmc1_supPgood,
        pgood_oe        => fmc1_supPgood_oe,
        en_o            => fmc1_supPwrEn_o
    );

    -- FMC1 supply power good is directly connected to "PG_C2M" on the board
    -- Delay supply power good signal release for FMC1 module enable
    fmc1ModEn_delay_inst : entity work.debounce
    generic map (
        G_DEBOUNCE_TICKS => C_5MS_DELAY_TICKS
    )
    port map (
        clk_i          => s_clk,
        signal_i       => s_fmc1_supPgood,
        signal_o       => s_fmc1_supPgood_dlyd
    ); 
    --override module power good signals when the power is disabled to avoid false power good indication  
    fmc1_modPgood_oe <= not s_fmc1_supPgood_dlyd;

    ----------------------------
    ---- FMC2 power control ----
    ----------------------------
    -- FMC power-up is controlled and monitored by mcu
    fmc2PowerCntrl_inst : entity work.fmcPowerCntrl
    generic map (
        G_PGOOD_MONITOR_DELAY => C_100US_DELAY_TICKS
    )
    port map (
        clk_i           => s_clk,
        pwrCntrl_i      => s_mcu_fmc2_supCntrl,
        pgoodRelease_i  => s_mcu_fmc2_supPgoodRelease,
        pgood_i         => s_fmc2_supPgood,
        pgood_oe        => fmc2_supPgood_oe,
        en_o            => fmc2_supPwrEn_o
    );

    -- FMC2 supply power good is directly connected to "PG_C2M" on the board
    -- Delay supply power good signal release for FMC2 module enable
    fmc2ModEn_delay_inst : entity work.debounce
    generic map (
        G_DEBOUNCE_TICKS => C_5MS_DELAY_TICKS
    )
    port map (
        clk_i          => s_clk,
        signal_i       => s_fmc2_supPgood,
        signal_o       => s_fmc2_supPgood_dlyd
    ); 
    --override power good signals when the power is disabled to avoid false power good indication  
    fmc2_modPgood_oe <= not s_fmc2_supPgood_dlyd;

    ----------------------------
    --- SYZYGY power control ---
    ----------------------------
    s_szg_pwrEn <= s_pl_pwrEn_dlyd and s_ps_pwrEn;
    -- Delay SYZYGY power enable for power good signal release
    szgPwrEn_delay_inst : entity work.debounce
    generic map (
        G_DEBOUNCE_TICKS => C_5MS_DELAY_TICKS
    )
    port map (
        clk_i          => s_clk,
        signal_i       => s_szg_pwrEn,
        signal_o       => s_szg_pwrEn_dlyd
    ); 
    szg_PwrEn_o <= s_szg_pwrEn;
    --override power good signal when the power is disabled to avoid false power good indication
    szg_pgood_oe <= not s_szg_pwrEn_dlyd;
    
    ----------------------------
    --- POR Control ---
    ----------------------------
    -- Delay POR signal until all voltage rails are stable and power good signals are asserted
    por_delay_inst : entity work.debounce
    generic map (
        G_DEBOUNCE_TICKS => C_100MS_DELAY_TICKS
    )
    port map (
        clk_i          => s_clk,
        signal_i       => s_pl_pwrEn and s_ps_pwrEn,
        signal_o       => s_porn
    ); 
        
    -- Keep POR signal asserted until the MCU releases it (s_porn is relased first on HW-level)
    ps_porn_o <= s_porn and s_mcu_porn;
    
    -- Release Ethernet PHY reset after mcu has finshed initialization of the clock generator
    eth_reset_o <= not s_mcu_eth_rstn; 

    pmu_Mio28GPI_o <= '0';
    pmu_Mio29GPI_o <= '0';
    pmu_Mio31GPI_o <= '0';

    si5332_Cntrl_o <= (others => '0');

    lmk05028_En_o <= '0';
    sd_reset_o <= '0';
    
    i2c0_reset_o <= '0';
    ps_resetn_o <= '1';
   
    fan_pwm_o <= '0';

    sda0_o <= not s_i2c0_sda_out;
    scl0_o <= not s_i2c0_scl_out;

    sda1_o <= not s_i2c1_sda_out;
    scl1_o <= not s_i2c1_scl_out;
            
    leds_o(0) <= not s_mcu_led;
    leds_o(1) <= '1';

end rtl;