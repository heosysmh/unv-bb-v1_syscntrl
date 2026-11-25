
# Entity: synchronizer 
- **File**: synchronizer.vhd
- **Brief:**  Single-bit clock-domain-crossing synchronizer.

## Diagram
![Diagram](synchronizer.svg "Diagram")
## Description

Synchronizes an asynchronous input into the clk_i clock domain using a
capture flip-flop followed by G_NUM_GUARD_FFS guard flip-flops
(2-FF synchronizer with G_NUM_GUARD_FFS = 1). The ASYNC_REG attribute
keeps the flip-flops close together and prevents X propagation on timing
violations, shreg_extract disables SRL inference. Asynchronous,
high-active reset initializes all stages to G_INIT_VAL.

## Generics

| Generic name    | Type      | Value | Description                                                                                              |
| --------------- | --------- | ----- | -------------------------------------------------------------------------------------------------------- |
| G_INIT_VAL      | std_logic | '0'   | Initial / reset value of all synchronizer flip-flops                                                     |
| G_NUM_GUARD_FFS | natural   | 1     | Number of guard flip-flops after the capture stage (1 = classic 2-FF synchronizer, >= 2 for higher MTBF) |

## Ports

| Port name | Direction | Type      | Description                                                     |
| --------- | --------- | --------- | --------------------------------------------------------------- |
| clk_i     | in        | std_logic | Destination clock domain, rising edge active                    |
| rst_i     | in        | std_logic | Asynchronous reset, high active (sets all stages to G_INIT_VAL) |
| async_i   | in        | std_logic | Asynchronous input signal (foreign clock domain)                |
| sync_o    | out       | std_logic | Synchronized output signal in the clk_i domain                  |

## Signals

| Name      | Type                                           | Description |
| --------- | ---------------------------------------------- | ----------- |
| s_sync_r  | std_logic                                      |             |
| s_guard_r | std_logic_vector(G_NUM_GUARD_FFS - 1 downto 0) |             |

## Processes
- unnamed: ( clk_i,rst_i )
