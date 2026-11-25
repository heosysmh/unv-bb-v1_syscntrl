
# Entity: inputSync 
- **File**: inputSync.vhd
- **Brief:**  Multi-bit input synchronizer (vector wrapper around synchronizer).

## Diagram
![Diagram](inputSync.svg "Diagram")
## Description

Instantiates one single-bit synchronizer per bit of the input vector.
Each bit is synchronized independently into the clk_i clock domain;
there is no coherency guarantee between bits, so the module is intended
for unrelated status / control inputs, not for multi-bit buses that must
be sampled atomically.

## Generics

| Generic name    | Type                                 | Value           | Description                                                                                |
| --------------- | ------------------------------------ | --------------- | ------------------------------------------------------------------------------------------ |
| WIDTH           | positive                             | 1               | Number of input signals to synchronize (vector width)                                      |
| G_INIT_VAL      | std_logic_vector(WIDTH - 1 downto 0) | (others => '0') | Per-bit initial / reset value of the synchronizer flip-flops                               |
| G_NUM_GUARD_FFS | natural                              | 1               | Number of guard flip-flops per bit after the capture stage (1 = classic 2-FF synchronizer) |

## Ports

| Port name | Direction | Type                                 | Description                                                            |
| --------- | --------- | ------------------------------------ | ---------------------------------------------------------------------- |
| clk_i     | in        | std_logic                            | Destination clock domain, rising edge active                           |
| rst_i     | in        | std_logic                            | Asynchronous reset, high active (sets all synchronizers to G_INIT_VAL) |
| signals_i | in        | std_logic_vector(WIDTH - 1 downto 0) | Asynchronous input vector (foreign clock domain)                       |
| signals_o | out       | std_logic_vector(WIDTH - 1 downto 0) | Synchronized output vector in the clk_i domain                         |
