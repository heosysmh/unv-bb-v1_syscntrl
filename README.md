# unv-bb-v1-syscntrl

System controller for the UNV-BB v1 Kria SoM carrier board. An Efinix FPGA
handles power sequencing, VCCIO selection, and reset/debounce logic, while a
Sapphire SoC (VexRiscv RISC-V) soft MCU runs the board bring-up and supervision
firmware: clock generator configuration, power monitoring, and FMC Vadj
regulation.

## Repository Layout

| Path | Contents |
|------|----------|
| [`fpga/`](fpga/) | VHDL sources, testbenches, constraints, and the Sapphire SoC MCU IP |
| [`sw/app/`](sw/app/) | System controller application (bring-up sequence and supervision loop) |
| [`sw/devices/`](sw/devices/) | Device drivers: Si5332 clock generator, INA745A power monitors, TPSM8286x Vadj regulators, PI4MSD5V945x I2C switch |
| [`sw/drivers/`](sw/drivers/) | Peripheral wrappers (I2C) |
| [`sw/mcu/`](sw/mcu/) | Sapphire SoC BSP, bootloader, and standalone drivers |
| [`sw/tests/`](sw/tests/) | Host-side unit tests for device drivers |
| [`docs/`](docs/) | MkDocs documentation sources |
| [`tools/`](tools/) | Helper configurations (FTDI/OpenOCD) |

## Quick Start

```bash
cd sw/app
make debug
```

See [`sw/README_BUILD.md`](sw/README_BUILD.md) for toolchain prerequisites,
build configurations, VSCode tasks, and troubleshooting.

## Configuration

All board-specific settings (FMC Vadj voltages, VCCIO source selection, clock
generator outputs, startup timing) are centralized in
[`sw/app/src/boardConfig.h`](sw/app/src/boardConfig.h).

## Documentation

- [FTDI & JTAG configuration](docs/setup/ftdi-jtag.md)
- [Firmware build guide](docs/setup/firmware-build.md)
- [Writing the flash](docs/setup/write-flash.md)
- [Build system notes](docs/sw-notes/BUILD_SYSTEM.md)

The full documentation site (including the generated C API reference) is built
with [MkDocs](https://www.mkdocs.org/) from [`mkdocs.yml`](mkdocs.yml):

```bash
mkdocs serve
```

## License

Licensed under the Apache License, Version 2.0 — see [`LICENSE.txt`](LICENSE.txt).

Copyright (c) 2026 Matthias Haefner - Heosys
