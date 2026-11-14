# Si5332 Clock Generator Driver - Implementation Plan

## Executive Summary

This plan details the implementation of a complete, production-ready Si5332 clock generator driver according to the specifications in [`agents/si5332.md`](../agents/si5332.md). The driver will be modular, platform-independent, and integrate seamlessly with the existing I2C infrastructure.

---

## Current State Analysis

### Existing Implementation
- **Location**: [`src/driver/si5332/`](../src/driver/si5332/)
- **Files**: 
  - [`si5332.h`](../src/driver/si5332/si5332.h) - Basic header with incomplete API
  - [`si5332.c`](../src/driver/si5332/si5332.c) - Minimal implementation with only `si5332_apply_config()`
  - [`si5332_regs.h`](../src/driver/si5332/si5332_regs.h) - ClockBuilder Pro register export (61 registers)

### Gaps Identified
1. ❌ No device ID verification
2. ❌ No PLL lock status checking
3. ❌ No output enable/disable control
4. ❌ Incomplete error code enumeration
5. ❌ Missing delays for device initialization
6. ❌ No example/template code
7. ❌ Insufficient documentation

### I2C Infrastructure
- **Driver**: [`src/driver/i2c/i2cCom.h`](../src/driver/i2c/i2cCom.h)
- **Functions Available**:
  - `int i2c_writeReg(i2cHandle_t *pInst, u8 slaveAddr, u8 regAddr, u8 *data)` - Single byte write
  - `int i2c_readReg(i2cHandle_t *pInst, u8 slaveAddr, u8 regAddr, u8 *data)` - Single byte read
- **Pattern**: Function pointer abstraction (see [`tpsm8286x.h`](../src/driver/tpsm8286x/tpsm8286x.h))

---

## Si5332 Register Map Analysis

### Critical Registers (from ClockBuilder Pro export and datasheet)

| Register | Name | Purpose | Access |
|----------|------|---------|--------|
| `0x0000` | DEVICE_ID | Device identification | Read |
| `0x0001` | DEVICE_REV | Device revision | Read |
| `0x0006` | DEVICE_READY | Device state control (0x01=Ready, 0x02=Active) | R/W |
| `0x000C` | STATUS | PLL lock status, LOS flags | Read |
| `0x000D` | STATUS_STICKY | Sticky status bits | Read |
| `0x0011` | LOS_CLKIN | Loss of signal for clock inputs | Read |
| `0x0012` | LOL | Loss of lock for PLL | Read |
| `0x0017-0x001C` | DESIGN_ID[0:5] | Design identification (KRIABB) | R/W |
| `0x00B6` | OUT0-5_OE | Output enable bits 0-5 | R/W |
| `0x00B7` | OUT6-7_OE | Output enable bits 6-7 | R/W |

### Configuration Sequence (from si5332_regs.h)
1. **Preamble**: Set device to Ready mode (`0x0006 = 0x01`)
2. **Configuration**: Write 59 configuration registers
3. **Postamble**: Set device to Active mode (`0x0006 = 0x02`)

### Output Enable Control
- **Register 0xB6**: Controls OUT0-OUT5 (bits 0,1,3,4,6,7)
- **Register 0xB7**: Controls OUT6-OUT7 (bits 1,2)
- **Current Config**: Only OUT6 enabled (0xB7[1] = 1)

---

## Architecture Design

### Data Structures

```c
// Error codes enumeration
typedef enum {
    SI5332_OK = 0,                    // Success
    SI5332_ERR_I2C = -1,              // I2C communication error
    SI5332_ERR_INVALID_DEV_ID = -2,   // Device ID mismatch
    SI5332_ERR_NOT_LOCKED = -3,       // PLL not locked
    SI5332_ERR_INVALID_ARG = -4       // Invalid argument
} si5332_status_t;

// Register configuration structure (already exists in si5332_regs.h)
typedef struct {
    unsigned int address;   // 8-bit register address
    unsigned char value;    // 8-bit register data
} si5332_reg_setting_t;

// Device handle structure
typedef struct {
    u8 i2c_addr;                      // I2C 7-bit address (0x6A)
    u8 is_initialized;                // Initialization flag
    void *hal_ctx;                    // HAL context (i2cHandle_t*)
    si5332_i2c_write_f i2c_write;     // Write function pointer
    si5332_i2c_read_f i2c_read;       // Read function pointer
} si5332_dev_t;
```

### Function Pointer Types

```c
typedef si5332_status_t (*si5332_i2c_write_f)(void *ctx, u8 i2c_addr, u8 reg_addr, const u8 *data);
typedef si5332_status_t (*si5332_i2c_read_f)(void *ctx, u8 i2c_addr, u8 reg_addr, u8 *data);
```

---

## Implementation Details

### 1. Device Initialization Function

**Signature**: `si5332_status_t si5332_init(si5332_dev_t *dev, const si5332_reg_setting_t *reg_map, size_t map_size)`

**Algorithm**:
```
1. Validate input parameters (dev != NULL, reg_map != NULL, map_size > 0)
2. Read Device ID from register 0x0000
   - Expected value: 0x02 (Si5332)
   - Return SI5332_ERR_INVALID_DEV_ID if mismatch
3. Read Device Revision from register 0x0001
   - Expected: Rev D (0x04)
   - Log warning if different but continue
4. Apply register configuration:
   - Iterate through reg_map array
   - Write each register using i2c_write()
   - Check return status after each write
   - Special handling for register 0x0006:
     * After writing 0x01 (Ready mode): delay 300ms
     * After writing 0x02 (Active mode): delay 25ms
5. Set is_initialized flag
6. Return SI5332_OK
```

**Key Considerations**:
- Device ID verification prevents misconfiguration
- Delays are critical for internal NVM operations
- Error propagation on any I2C failure

### 2. PLL Lock Status Function

**Signature**: `si5332_status_t si5332_check_lock(si5332_dev_t *dev, bool *is_locked)`

**Algorithm**:
```
1. Validate input parameters
2. Read STATUS register (0x000C)
   - Bit 0: LOL (Loss of Lock) - active high
   - Bit 1: LOS_CLKIN - active high
3. Read LOL register (0x0012) for detailed PLL status
4. Determine lock status:
   - is_locked = true if LOL bit is 0 (PLL locked)
   - is_locked = false if LOL bit is 1 (PLL unlocked)
5. Return SI5332_OK or SI5332_ERR_I2C
```

**Usage Pattern**:
```c
bool locked;
si5332_status_t status = si5332_check_lock(dev, &locked);
if (status == SI5332_OK && locked) {
    // PLL is stable
}
```

### 3. Output Enable Control Function

**Signature**: `si5332_status_t si5332_output_enable(si5332_dev_t *dev, uint8_t output_channel, bool enable)`

**Algorithm**:
```
1. Validate input parameters
   - output_channel must be 0-7
   - Return SI5332_ERR_INVALID_ARG if out of range
2. Determine register and bit position:
   - OUT0-5: Register 0xB6, bits [0,1,3,4,6,7]
   - OUT6-7: Register 0xB7, bits [1,2]
3. Read current register value
4. Modify appropriate bit:
   - Set bit to 1 for enable
   - Clear bit to 0 for disable
5. Write modified value back
6. Return status
```

**Bit Mapping**:
```
OUT0 -> 0xB6[0]
OUT1 -> 0xB6[1]
OUT2 -> 0xB6[3]
OUT3 -> 0xB6[4]
OUT4 -> 0xB6[6]
OUT5 -> 0xB6[7]
OUT6 -> 0xB7[1]
OUT7 -> 0xB7[2]
```

### 4. Helper Functions (Optional but Recommended)

```c
// Read device ID and revision
si5332_status_t si5332_get_device_info(si5332_dev_t *dev, u8 *device_id, u8 *revision);

// Read design ID string (6 bytes: "KRIABB")
si5332_status_t si5332_get_design_id(si5332_dev_t *dev, char *design_id, size_t len);

// Soft reset
si5332_status_t si5332_soft_reset(si5332_dev_t *dev);
```

---

## File Structure

### Updated si5332.h

```c
/*
 * si5332.h - Silicon Labs Si5332 Clock Generator Driver
 * 
 * Platform-independent driver for Si5332 via I2C
 */

#ifndef SRC_SI5332_H_
#define SRC_SI5332_H_

#include <type.h>
#include <stdbool.h>

/* I2C Address */
#define SI5332_I2C_ADDR_DEFAULT     0x6A

/* Device ID */
#define SI5332_DEVICE_ID            0x02
#define SI5332_REVISION_D           0x04

/* Register Addresses */
#define SI5332_REG_DEVICE_ID        0x0000
#define SI5332_REG_DEVICE_REV       0x0001
#define SI5332_REG_DEVICE_READY     0x0006
#define SI5332_REG_STATUS           0x000C
#define SI5332_REG_STATUS_STICKY    0x000D
#define SI5332_REG_LOS_CLKIN        0x0011
#define SI5332_REG_LOL              0x0012
#define SI5332_REG_DESIGN_ID0       0x0017
#define SI5332_REG_OUT0_5_OE        0x00B6
#define SI5332_REG_OUT6_7_OE        0x00B7

/* Device Ready States */
#define SI5332_READY_MODE           0x01
#define SI5332_ACTIVE_MODE          0x02

/* Status Register Bits */
#define SI5332_STATUS_LOL_BIT       (1 << 0)
#define SI5332_STATUS_LOS_BIT       (1 << 1)

/* Timing Constants (milliseconds) */
#define SI5332_READY_DELAY_MS       300
#define SI5332_ACTIVE_DELAY_MS      25

/* Error Codes */
typedef enum {
    SI5332_OK = 0,
    SI5332_ERR_I2C = -1,
    SI5332_ERR_INVALID_DEV_ID = -2,
    SI5332_ERR_NOT_LOCKED = -3,
    SI5332_ERR_INVALID_ARG = -4
} si5332_status_t;

/* Register Configuration Structure */
typedef struct {
    unsigned int address;
    unsigned char value;
} si5332_reg_setting_t;

/* HAL Function Pointers */
typedef si5332_status_t (*si5332_i2c_write_f)(void *ctx, u8 i2c_addr, u8 reg_addr, const u8 *data);
typedef si5332_status_t (*si5332_i2c_read_f)(void *ctx, u8 i2c_addr, u8 reg_addr, u8 *data);

/* Device Handle */
typedef struct {
    u8 i2c_addr;
    u8 is_initialized;
    void *hal_ctx;
    si5332_i2c_write_f i2c_write;
    si5332_i2c_read_f i2c_read;
} si5332_dev_t;

/* Public API */
si5332_dev_t* si5332_initialize(u8 i2c_addr, si5332_i2c_write_f w, si5332_i2c_read_f r, void *ctx);
si5332_status_t si5332_init(si5332_dev_t *dev, const si5332_reg_setting_t *reg_map, size_t map_size);
si5332_status_t si5332_check_lock(si5332_dev_t *dev, bool *is_locked);
si5332_status_t si5332_output_enable(si5332_dev_t *dev, u8 output_channel, bool enable);

/* Helper Functions */
si5332_status_t si5332_get_device_info(si5332_dev_t *dev, u8 *device_id, u8 *revision);

#endif /* SRC_SI5332_H_ */
```

### si5332.c Implementation Structure

```c
/*
 * si5332.c - Silicon Labs Si5332 Clock Generator Driver Implementation
 */

#include "si5332.h"
#include "si5332_regs.h"
#include "userDef.h"
#include "bsp.h"  // For bsp_uDelay()

/* Static device instance */
static si5332_dev_t gDevice;

/* Private helper functions */
static si5332_status_t si5332_write_reg(si5332_dev_t *dev, u8 reg_addr, u8 value);
static si5332_status_t si5332_read_reg(si5332_dev_t *dev, u8 reg_addr, u8 *value);
static void si5332_delay_ms(u32 ms);

/* Public API implementations */
// ... (detailed implementations follow)
```

### si5332_example.c Template

```c
/*
 * si5332_example.c - Si5332 Driver Usage Example
 * 
 * Demonstrates initialization, configuration, and status monitoring
 */

#include "si5332.h"
#include "si5332_regs.h"
#include "i2cCom.h"
#include "bsp.h"

void si5332_example(i2cHandle_t *i2c_handle)
{
    si5332_status_t status;
    bool is_locked;
    u8 device_id, revision;
    
    // 1. Initialize device handle
    si5332_dev_t *clock_gen = si5332_initialize(
        CLOCK_GEN_I2C_ADDR,
        (si5332_i2c_write_f)i2c_writeReg,
        (si5332_i2c_read_f)i2c_readReg,
        (void*)i2c_handle
    );
    
    // 2. Verify device presence
    status = si5332_get_device_info(clock_gen, &device_id, &revision);
    if (status != SI5332_OK) {
        bsp_printf("ERROR: Cannot communicate with Si5332\r\n");
        return;
    }
    bsp_printf("Si5332 Device ID: 0x%02X, Revision: 0x%02X\r\n", device_id, revision);
    
    // 3. Apply configuration from ClockBuilder Pro
    status = si5332_init(
        clock_gen,
        si5332_gm2_revd_registers,
        SI5332_GM2_REVD_REG_CONFIG_NUM_REGS
    );
    
    if (status == SI5332_ERR_INVALID_DEV_ID) {
        bsp_printf("ERROR: Invalid device ID\r\n");
        return;
    } else if (status == SI5332_ERR_I2C) {
        bsp_printf("ERROR: I2C communication failed\r\n");
        return;
    } else if (status == SI5332_OK) {
        bsp_printf("Si5332 configuration applied successfully\r\n");
    }
    
    // 4. Wait for PLL to lock
    bsp_uDelay(100000); // 100ms
    
    // 5. Check PLL lock status
    status = si5332_check_lock(clock_gen, &is_locked);
    if (status == SI5332_OK) {
        if (is_locked) {
            bsp_printf("PLL is LOCKED - clock outputs are stable\r\n");
        } else {
            bsp_printf("WARNING: PLL is NOT LOCKED\r\n");
        }
    }
    
    // 6. Enable/disable specific outputs
    // Enable OUT6 (25 MHz output)
    status = si5332_output_enable(clock_gen, 6, true);
    if (status == SI5332_OK) {
        bsp_printf("OUT6 enabled\r\n");
    }
    
    // Disable OUT0 (if needed)
    status = si5332_output_enable(clock_gen, 0, false);
    if (status == SI5332_OK) {
        bsp_printf("OUT0 disabled\r\n");
    }
}
```

---

## Implementation Sequence

### Phase 1: Header File Enhancement
1. Update [`si5332.h`](../src/driver/si5332/si5332.h) with:
   - Complete error enumeration
   - Register address definitions
   - Timing constants
   - Updated data structures
   - All function prototypes

### Phase 2: Core Driver Implementation
1. Update [`si5332.c`](../src/driver/si5332/si5332.c) with:
   - Helper functions for register access
   - `si5332_init()` with device ID verification
   - `si5332_check_lock()` for PLL status
   - `si5332_output_enable()` for output control
   - Comprehensive error handling

### Phase 3: Example Code
1. Create `si5332_example.c` demonstrating:
   - Device initialization
   - Configuration application
   - Status monitoring
   - Output control

### Phase 4: Integration
1. Update [`main.c`](../src/main.c) to use new API
2. Test with existing hardware
3. Verify PLL lock behavior
4. Validate output enable/disable

---

## Testing Strategy

### Unit Tests
1. **Device ID Verification**: Confirm correct ID read
2. **Register Write/Read**: Verify I2C communication
3. **Configuration Application**: Check all 61 registers written
4. **PLL Lock Detection**: Test lock status reading
5. **Output Control**: Verify enable/disable for all 8 outputs

### Integration Tests
1. **Full Initialization Sequence**: From power-up to locked
2. **Error Handling**: Test I2C failures, invalid parameters
3. **Timing Verification**: Confirm delays are adequate
4. **Multi-Output Control**: Enable/disable multiple outputs

### Hardware Validation
1. Measure output frequencies with oscilloscope
2. Verify PLL lock indicator
3. Test output enable/disable transitions
4. Validate power-up sequence timing

---

## Error Handling Strategy

### I2C Communication Errors
- Propagate immediately to caller
- Log error details if debugging enabled
- Do not retry automatically (let application decide)

### Device ID Mismatch
- Return `SI5332_ERR_INVALID_DEV_ID`
- Prevent further configuration
- Log expected vs. actual ID

### PLL Lock Failures
- Return status via `is_locked` parameter
- Allow application to decide retry strategy
- Consider timeout mechanism

### Invalid Arguments
- Return `SI5332_ERR_INVALID_ARG`
- Validate all inputs before I2C operations
- Document valid ranges in header

---

## Documentation Requirements

### Code Comments
- Function headers with purpose, parameters, return values
- Inline comments for complex logic
- Register bit field explanations

### User Documentation
- API reference guide
- Integration examples
- Troubleshooting section
- Timing diagrams for initialization

---

## Compatibility Considerations

### Existing Code
- Maintain backward compatibility with current `si5332_initialize()`
- Extend rather than replace existing API
- Keep `si5332_apply_config()` for legacy support

### Project Patterns
- Follow naming conventions from [`tpsm8286x`](../src/driver/tpsm8286x/tpsm8286x.h)
- Use consistent error handling
- Match function pointer patterns

### Build System
- No changes to makefile required
- All files already in build (see build/obj_files/)

---

## Risk Mitigation

### Risk: Incorrect Timing Delays
- **Mitigation**: Use conservative delays from datasheet
- **Validation**: Measure with logic analyzer

### Risk: I2C Communication Failures
- **Mitigation**: Comprehensive error checking
- **Validation**: Test with intentional failures

### Risk: PLL Lock Issues
- **Mitigation**: Provide detailed status reporting
- **Validation**: Test with various input conditions

### Risk: Register Configuration Errors
- **Mitigation**: Use ClockBuilder Pro export directly
- **Validation**: Read-back verification option

---

## Success Criteria

1. ✅ Device ID verification working
2. ✅ All 61 registers configured correctly
3. ✅ PLL lock status accurately reported
4. ✅ Output enable/disable functional for all channels
5. ✅ Error handling comprehensive and tested
6. ✅ Example code demonstrates all features
7. ✅ Integration with existing codebase seamless
8. ✅ Documentation complete and clear

---

## Next Steps

After plan approval, switch to **Code Mode** to implement:
1. Enhanced [`si5332.h`](../src/driver/si5332/si5332.h)
2. Complete [`si5332.c`](../src/driver/si5332/si5332.c)
3. New `si5332_example.c`
4. Updated [`main.c`](../src/main.c) integration

---

## References

- **Task Specification**: [`agents/si5332.md`](../agents/si5332.md)
- **Datasheet**: [`docs/si5332/si5332-rm.pdf`](../docs/si5332/si5332-rm.pdf)
- **Register Export**: [`si5332_regs.h`](../src/driver/si5332/si5332_regs.h)
- **I2C Driver**: [`i2cCom.h`](../src/driver/i2c/i2cCom.h)
- **Similar Driver**: [`tpsm8286x.h`](../src/driver/tpsm8286x/tpsm8286x.h)
