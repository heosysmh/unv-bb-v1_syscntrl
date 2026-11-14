# Efinix Sapphire SoC Firmware - Build System

## Quick Start

### Prerequisites

You need the RISC-V toolchain installed and in your PATH:
- `riscv-none-elf-gcc` (or `riscv64-unknown-elf-gcc`)

If not installed, you can:
1. Download from [RISC-V GNU Toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain)
2. Or set `RISCV_BIN` environment variable to point to your toolchain prefix

### Build Commands

```bash
cd app

# Debug build (default) - Maximum debug info, no optimization
make debug

# Release build - Maximum optimization, no debug info
make release

# Clean all build artifacts
make clean
```

### VSCode Integration

Press `Ctrl+Shift+B` to build debug configuration (default).

Or use Command Palette (`Ctrl+Shift+P`) → "Tasks: Run Task" and select:
- **Build Debug** - Debug build with `-g3 -O0`
- **Build Release** - Release build with `-O3`
- **Clean Project** - Remove all build artifacts
- **Rebuild Debug** - Clean + Debug build
- **Rebuild Release** - Clean + Release build

## Build Configurations

| Configuration | Optimization | Debug Info | Output Directory |
|---------------|--------------|------------|------------------|
| **Debug** | `-O0` (none) | `-g3` (full) | `app/build/debug/` |
| **Release** | `-O3` (max) | none | `app/build/release/` |

## Output Files

Each build produces:
- `kria_sysCntrl.elf` - ELF executable
- `kria_sysCntrl.hex` - Intel HEX format
- `kria_sysCntrl.bin` - Raw binary
- `kria_sysCntrl.asm` - Disassembly listing
- `kria_sysCntrl.map` - Linker map file

## Documentation

See [`docs/BUILD_SYSTEM.md`](docs/BUILD_SYSTEM.md) for detailed documentation.

## Troubleshooting

### "riscv-none-elf-gcc: command not found"

The RISC-V toolchain is not in your PATH. Options:

1. **Add toolchain to PATH:**
   ```bash
   export PATH=/path/to/riscv/toolchain/bin:$PATH
   ```

2. **Set RISCV_BIN variable:**
   ```bash
   export RISCV_BIN=riscv64-unknown-elf-
   make debug
   ```

3. **Or modify `app/makefile`:**
   ```makefile
   # Add before includes
   RISCV_BIN = /path/to/toolchain/bin/riscv-none-elf-
   ```

### Build warnings about recipe override

These warnings are expected and can be ignored:
```
makefile:57: Warnung: Das Rezept für das Ziel „build" wird überschrieben
```

This is intentional to support multiple build configurations.
