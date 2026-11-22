# Build System Documentation

## Overview

The build system for the Efinix Sapphire SoC firmware supports multiple build configurations with different optimization levels and debug information.

## Build Configurations

### 1. Debug Build (Default)

**Purpose:** Development and debugging

**Compiler Flags:**
- `-g3` - Maximum debug information (including macros)
- `-O0` - No optimization (better debuggability)
- `-march=rv32im_zicsr_zifencei` - RISC-V ISA
- `-mabi=ilp32` - ABI
- `-DSMP` - SMP support

**Output Directory:** `app/build/debug/`

**Build Command:**
```bash
cd app
make debug
```

**VSCode Task:** `Build Debug` (default build task, Ctrl+Shift+B)

**Output Files:**
- `kria_sysCntrl.elf` - ELF binary with debug symbols
- `kria_sysCntrl.hex` - Intel HEX format
- `kria_sysCntrl.bin` - Raw binary
- `kria_sysCntrl.asm` - Disassembly
- `kria_sysCntrl.map` - Linker map

---

### 2. Release Build

**Purpose:** Production firmware

**Compiler Flags:**
- `-O3` - Maximum optimization (speed)
- No debug information
- `-march=rv32im_zicsr_zifencei` - RISC-V ISA
- `-mabi=ilp32` - ABI
- `-DSMP` - SMP support

**Output Directory:** `app/build/release/`

**Build Command:**
```bash
cd app
make release
```

**VSCode Task:** `Build Release`

**Output Files:**
- `kria_sysCntrl.elf` - Optimized ELF binary
- `kria_sysCntrl.hex` - Intel HEX format
- `kria_sysCntrl.bin` - Raw binary
- `kria_sysCntrl.asm` - Disassembly
- `kria_sysCntrl.map` - Linker map

---

### 3. Clean

**Purpose:** Remove all build artifacts

**Build Command:**
```bash
cd app
make clean
```

**VSCode Task:** `Clean Project`

**Removes:**
- `app/build/debug/` - All debug build files
- `app/build/release/` - All release build files
- `app/build/` - Build directory (if empty)

---

## VSCode Integration

### Available Tasks

1. **Build Debug** (Default, Ctrl+Shift+B)
   - Creates debug build
   - Output: `build/debug/`

2. **Build Release**
   - Creates release build
   - Output: `build/release/`

3. **Clean Project**
   - Removes all build artifacts

4. **Rebuild Debug**
   - Clean + Debug build

5. **Rebuild Release**
   - Clean + Release build

### Running Tasks

**Via Keyboard Shortcut:**
- `Ctrl+Shift+B` - Default build task (Debug)

**Via Command Palette:**
1. `Ctrl+Shift+P`
2. Type "Tasks: Run Task"
3. Select desired task

**Via Terminal Menu:**
- Terminal → Run Task → Select task

---

## Build System Architecture

### Makefile Hierarchy

```
app/makefile (Project-specific)
├── mcu/software/standalone/common/bsp.mk (BSP configuration)
├── mcu/software/standalone/common/riscv64-unknown-elf.mk (Compiler flags)
└── mcu/software/standalone/common/standalone.mk (Build rules)
```

### Important Variables

| Variable | Debug | Release | Description |
|----------|-------|---------|-------------|
| `DEBUG` | yes | no | Enable debug mode |
| `DEBUG_OG` | no | - | Optimization in debug mode |
| `BENCH` | no | yes | Benchmark optimization |
| `OBJDIR` | build/debug | build/release | Output directory |

### Compiler Flag Logic

Compiler flags are determined in [`riscv64-unknown-elf.mk`](../mcu/software/standalone/common/riscv64-unknown-elf.mk):

```makefile
# Debug configuration
ifeq ($(DEBUG),yes)
    ifneq ($(DEBUG_OG),yes)
        CFLAGS += -g3 -O0    # Maximum debug, no optimization
    else
        CFLAGS += -g3 -Og    # Debug with minimal optimization
    endif
endif

# Release configuration
ifneq ($(DEBUG),yes)
    ifneq ($(BENCH),yes)
        CFLAGS += -Os        # Size optimization
    else
        CFLAGS += -O3        # Speed optimization
    endif
endif
```

---

## Configuration Comparison

| Property | Debug | Release |
|----------|-------|---------|
| **Optimization** | None (-O0) | Maximum (-O3) |
| **Debug Info** | Full (-g3) | None |
| **Code Size** | Larger | Smaller |
| **Execution Speed** | Slower | Faster |
| **Debuggability** | Excellent | Difficult |
| **Use Case** | Development | Production |

---

## Toolchain

**Compiler:** `riscv-none-elf-gcc`

**Architecture:** RISC-V 32-bit
- ISA: `rv32im_zicsr_zifencei`
- ABI: `ilp32`
- Extensions: M (Multiply/Divide), Zicsr (CSR), Zifencei (Fence)

**Linker Script:** `mcu/bsp/efinix/EfxSapphireSoc/linker/default.ld`

---

## Tips & Best Practices

### Development
- Use **Debug builds** for daily development
- Debug builds enable better debugging with GDB/OpenOCD
- Variables and functions are not optimized away

### Testing
- Test critical features with **Release builds** as well
- Optimizations can reveal hidden bugs
- Performance tests should be done with release builds

### Deployment
- Use only **Release builds** for production
- Smaller code size and better performance
- Document the build configuration used

### Debugging
- For hard-to-reproduce bugs: Compare debug and release builds
- Use `-Og` instead of `-O0` if debug build is too slow:
  ```bash
  make debug DEBUG_OG=yes
  ```

---

## Troubleshooting

### Problem: "make: command not found"
**Solution:** Ensure GNU Make is installed
```bash
sudo apt-get install build-essential
```

### Problem: "riscv-none-elf-gcc: command not found"
**Solution:** RISC-V toolchain is not in PATH
- Check toolchain installation
- Set `RISCV_BIN` in environment or makefile

### Problem: Build artifacts not updating
**Solution:** Perform a clean build
```bash
make clean
make debug  # or make release
```

### Problem: Different behavior between debug and release
**Causes:**
- Uninitialized variables
- Race conditions
- Timing-dependent code
- Undefined behavior

**Solution:** Perform code review and static analysis

---

## Extensions

### Adding Additional Build Configuration

Example: Size-optimized configuration

1. **Add to `app/makefile`:**
```makefile
size:
	@echo "=== Building SIZE-OPTIMIZED configuration ==="
	@$(MAKE) -f $(firstword $(MAKEFILE_LIST)) build \
		DEBUG=no \
		BENCH=no \
		OBJDIR=build/size
```

2. **Add to `.vscode/tasks.json`:**
```json
{
  "label": "Build Size-Optimized",
  "type": "shell",
  "command": "make",
  "args": ["size"],
  "options": {
    "cwd": "${workspaceFolder}/app"
  },
  "group": "build",
  "problemMatcher": ["$gcc"],
  "detail": "Build size-optimized firmware (-Os) → build/size/"
}
```

---

## Quick Reference

### Command Line

```bash
# Debug build
cd app && make debug

# Release build
cd app && make release

# Clean all
cd app && make clean

# Rebuild debug
cd app && make clean && make debug

# Rebuild release
cd app && make clean && make release
```

### VSCode Shortcuts

- `Ctrl+Shift+B` - Build Debug (default)
- `Ctrl+Shift+P` → "Tasks: Run Task" → Select task

---

## References

- [RISC-V ISA Specification](https://riscv.org/technical/specifications/)
- [GCC Optimization Options](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)
- [Efinix Sapphire SoC Documentation](https://www.efinixinc.com/)
