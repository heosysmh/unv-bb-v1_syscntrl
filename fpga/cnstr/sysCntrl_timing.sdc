
## SPDX-License-Identifier: Apache-2.0
## Copyright (c) 2026 Matthias Haefner - Heosys


# Device: T20Q100F3
# Project: sysCntrl
# Timing Model: C4 (final)

# PLL Constraints
###################
create_clock -period 12.500 -name sysClk_i [get_ports {sysClk_i}]

# GPIO Constraints
####################
# Set false-path for synchronized inputs
set_false_path -to [get_cells {inputSync_inst/synchronizers[*].synchronizer_inst/s_sync_r~FF}]


# JTAG Constraints
####################
create_clock -period 33.333 -name jtag_tck [get_ports {jtag_tck}]
set_clock_groups -asynchronous -group {jtag_tck} -group {sysClk_i}
set_output_delay -clock jtag_tck -max 0.111 [get_ports {jtag_tdo}]
set_output_delay -clock jtag_tck -min -0.054 [get_ports {jtag_tdo}]
set_input_delay -clock_fall -clock jtag_tck -max 0.267 [get_ports {jtag_capture}]
set_input_delay -clock_fall -clock jtag_tck -min 0.134 [get_ports {jtag_capture}]
set_input_delay -clock_fall -clock jtag_tck -max 0.267 [get_ports {jtag_reset}]
set_input_delay -clock_fall -clock jtag_tck -min 0.134 [get_ports {jtag_reset}]
set_input_delay -clock_fall -clock jtag_tck -max 0.231 [get_ports {jtag_sel}]
set_input_delay -clock_fall -clock jtag_tck -min 0.116 [get_ports {jtag_sel}]
set_input_delay -clock_fall -clock jtag_tck -max 0.267 [get_ports {jtag_update}]
set_input_delay -clock_fall -clock jtag_tck -min 0.134 [get_ports {jtag_update}]
set_input_delay -clock_fall -clock jtag_tck -max 0.321 [get_ports {jtag_shift}]
set_input_delay -clock_fall -clock jtag_tck -min 0.161 [get_ports {jtag_shift}]

# SPI Flash Constraints
#########################
set_input_delay -clock spi_clk -clock_fall -max 7.0 [get_ports {spi_0_io_data_0_read_i}]
set_input_delay -clock spi_clk -clock_fall -min 3.0 [get_ports {spi_0_io_data_0_read_i}]
set_input_delay -clock spi_clk -clock_fall -max 7.0 [get_ports {spi_0_io_data_1_read_i}]
set_input_delay -clock spi_clk -clock_fall -min 3.0 [get_ports {spi_0_io_data_1_read_i}]

set_output_delay -clock sysClk_i -max 0.610  [get_ports {spi_0_io_sclk_o}]
set_output_delay -clock sysClk_i -min -0.085 [get_ports {spi_0_io_sclk_o}]

set_output_delay -clock sysClk_i -max 0.610 [get_ports {spi_0_io_ss_o}]
set_output_delay -clock sysClk_i -min -0.085 [get_ports {spi_0_io_ss_o}]

set_output_delay -clock sysClk_i -max 0.610 [get_ports {spi_0_io_data_0_write_o}]
set_output_delay -clock sysClk_i -min -0.085 [get_ports {spi_0_io_data_0_write_o}]
set_output_delay -clock sysClk_i -max 0.610 [get_ports {spi_0_io_data_1_write_o}]
set_output_delay -clock sysClk_i -min -0.085 [get_ports {spi_0_io_data_1_write_o}]

# Write Protect and hold (not used):
set_false_path -to [get_ports {spi_0_io_wp_o}]
set_false_path -to [get_ports {spi_0_io_hold_o}]


