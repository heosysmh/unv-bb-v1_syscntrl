PROJ_NAME=unv-bb-v1_sysCntrl

STANDALONE=../mcu/software/standalone
BSP=efinix/EfxSapphireSoc

CFLAGS+=-DSMP

# Zusätzliche Linker-Flags für erweiterte Memory Map Informationen
LDFLAGS += -Wl,--print-map,--cref

# 1. Alle Unterordner von src/ finden und als Include-Pfade (-I) für Header hinzufügen
INC_DIRS := $(shell find src -type d)
CFLAGS += $(addprefix -I, $(INC_DIRS))

# 2. Rekursiv alle Unterordner von drivers/ und devices/ als Include-Pfade hinzufügen
DRIVERS_INC_DIRS := $(shell find ../drivers -type d 2>/dev/null)
DEVICES_INC_DIRS := $(shell find ../devices -type d 2>/dev/null)
CFLAGS += $(addprefix -I, $(DRIVERS_INC_DIRS))
CFLAGS += $(addprefix -I, $(DEVICES_INC_DIRS))

# 3. Alle Sourcen (.c, .cpp, .S) rekursiv in src/, drivers/ und devices/ erfassen
SRCS = $(shell find src -type f \( -name '*.c' -o -name '*.cpp' -o -name '*.S' \)) \
       $(shell find ../drivers -type f -name '*.c' 2>/dev/null) \
       $(shell find ../devices -type f -name '*.c' 2>/dev/null) \
       ${STANDALONE}/common/start.S \
       ${STANDALONE}/common/trap.S

# Include common makefiles
include ${STANDALONE}/common/bsp.mk
include ${STANDALONE}/common/riscv64-unknown-elf.mk
include ${STANDALONE}/common/standalone.mk

LDSCRIPT = ${BSP_PATH}/linker/default.ld

# =============================================================================
# Build Configurations
# =============================================================================

# Default target (backward compatible)
.PHONY: all debug release clean

all: debug

# Debug Build: Maximum debug information, no optimization for debugging
# Flags: -g3 -Og
# Output: build/debug/
debug:
	@echo "=== Building DEBUG configuration ==="
	@$(MAKE) -f $(firstword $(MAKEFILE_LIST)) build \
		DEBUG=yes \
		DEBUG_OG=yes \
		BENCH=no \
		OBJDIR=build/debug

# Release Build: Maximum optimization, no debug information
# Flags: -O3
# Output: build/release/
release:
	@echo "=== Building RELEASE configuration ==="
	@$(MAKE) -f $(firstword $(MAKEFILE_LIST)) build \
		DEBUG=no \
		BENCH=yes \
		OBJDIR=build/release

# Internal build target (called by debug/release)
.PHONY: build
build: $(OBJDIR)/$(PROJ_NAME).elf $(OBJDIR)/$(PROJ_NAME).hex $(OBJDIR)/$(PROJ_NAME).asm $(OBJDIR)/$(PROJ_NAME).bin
	@echo "=== Build complete: $(OBJDIR)/$(PROJ_NAME).elf ==="
	@echo "Configuration: DEBUG=$(DEBUG) BENCH=$(BENCH)"
	@echo "Compiler flags: $(CFLAGS)"
	@echo "=== Memory usage summary ==="
	@$(RISCV_PREFIX)size $(OBJDIR)/$(PROJ_NAME).elf
	@echo "=== Detailed memory usage per object file ==="
	@printf "%8s %8s %8s %8s %8s %s\n" "text" "data" "bss" "dec" "hex" "filename"
	@for obj in $(OBJDIR)/obj_files/*.o; do \
		$(RISCV_PREFIX)size $$obj | grep -v "text" | while read text data bss dec hex file; do \
			printf "%8s %8s %8s %8s %8s %s\n" "$$text" "$$data" "$$bss" "$$dec" "$$hex" "$$(basename $$obj)"; \
		done; \
	done 2>/dev/null | sort -k 1 -n -r || true

# Clean all build artifacts
clean:
	@echo "=== Cleaning all build artifacts ==="
	@rm -rf build/debug build/release build
	@echo "=== Clean complete ==="
