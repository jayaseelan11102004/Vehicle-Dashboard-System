# Vehicle Dashboard System

A 3-node embedded automotive dashboard built on **LPC2129 (ARM7TDMI-S)** microcontrollers that communicate over **CAN bus** (the "automotive industry standard" mainline protocol) and read analog sensors over **SPI**. The system displays live temperature and fuel level on an LCD and drives a servo-based fuel gauge / warning indicator.

## Overview

The dashboard is split across three LPC2129 boards ("nodes"), each with a dedicated role:

| Node | File | Role |
|------|------|------|
| **Node 0 – Dashboard/Master** | `node0.c` | Receives sensor data over CAN, updates the LCD dashboard, drives a servo motor via PWM |
| **Node 1 – Temperature Node** | `node1.c` | Reads an LM35 temperature sensor through an MCP3204 ADC over SPI, transmits the value over CAN |
| **Node 2 – Fuel Node** | `node2.c` | Reads a potentiometer (fuel level simulation) through an MCP3204 ADC over SPI, transmits the value over CAN |

All three nodes share a common driver layer (`can-driver-fp.c`, `spi-fp.c`, `mcp3204-fp.c`, `dashboard.c`) and a common header (`headerfp.h`).

## System Architecture

```
                CAN Bus (125 kbps, ID filters accept all frames)
   ┌──────────────────────────┼───────────────────────────┐
   │                          │                            │
┌──┴───────────┐      ┌───────┴────────┐          ┌────────┴───────┐
│   Node 1     │      │   Node 0       │          │   Node 2       │
│ LM35 Temp    │      │ LCD Dashboard  │          │ Potentiometer  │
│ Sensor       │─CAN─▶│ + Servo Gauge  │◀─CAN─────│ (Fuel Level)   │
│ (via SPI/ADC)│      │                │          │ (via SPI/ADC)  │
│ CAN ID 0x150 │      │  LPC2129       │          │ CAN ID 0x160   │
│  LPC2129     │      │  (Master)      │          │  LPC2129       │
└──────────────┘      └────────────────┘          └────────────────┘
```

- **Node 1** and **Node 2** are transmit-only sensor nodes: they sample an analog sensor through an **MCP3204** 12-bit ADC over SPI and broadcast the reading on the CAN bus every **750 ms**.
- **Node 0** is the display/master node: it has a CAN receive ISR that decodes incoming frames by CAN ID and updates the LCD accordingly, and it drives a PWM output (servo motor) as a mechanical fuel/warning gauge.

## Hardware

- **3 × NXP LPC2129** microcontrollers (ARM7TDMI-S)
- **LM35** analog temperature sensor
- **Potentiometer** (simulating a fuel-tank sender unit)
- **MCP3204** 12-bit SPI ADC (one per sensor node, used to digitize the LM35 / potentiometer output)
- **Character LCD** (HD44780-type, 4-bit interface) — dashboard display
- **Servo motor** — driven by PWM as a fuel-level indicator
- **CAN transceivers** on each board, wired into a common CAN bus

## Communication Protocols

### CAN Bus
- Peripheral: on-chip **CAN2** controller of the LPC2129
- Bit rate: **125 kbps** (`C2BTR = 0x001C001D`, PCLK = 60 MHz)
- Acceptance filter: accepts all data/remote frames (`AFMR = 0x2`)
- Message IDs used:
  - `0x150` — Temperature reading (from Node 1)
  - `0x160` — Fuel level reading (from Node 2)
- Payload: sensor value sent as a `float` in the first 4 data bytes (`byteA`), `dlc = 4`

### SPI
- Peripheral: on-chip **SPI0**
- Mode: **Mode 3** (CPOL = 1, CPHA = 1), Master mode, MSB first
- Used to talk to the MCP3204 ADC: a control byte selects the channel, and two response bytes are combined into a 12-bit conversion result, converted to a voltage (`(adcVal * 3.3) / 4096`)

## File Structure

```
.
├── headerfp.h        # Common types, register/bit macros, function prototypes
├── can-driver-fp.c   # CAN2 init, transmit, and receive ISR
├── spi-fp.c          # SPI0 init and byte transfer routine
├── mcp3204-fp.c       # MCP3204 ADC channel read over SPI
├── dashboard.c        # LCD driver, splash screen, integer/float display helpers
├── node0.c            # Master node: dashboard display + servo gauge
├── node1.c            # Sensor node: LM35 temperature over CAN (ID 0x150)
└── node2.c            # Sensor node: potentiometer/fuel over CAN (ID 0x160)
```

## How It Works

1. **Node 1** samples the LM35 sensor via the MCP3204 (channel 0), packages the value into a `CAN2_MSG` with ID `0x150`, and transmits it every 750 ms.
2. **Node 2** samples the potentiometer via its own MCP3204 (channel 0), packages the value into a `CAN2_MSG` with ID `0x160`, and transmits it every 750 ms.
3. **Node 0** listens on the CAN bus via an interrupt-driven receive handler (`can2_rx`):
   - On ID `0x150`, it writes the value to the LCD as `TEMP: xx.x C`
   - On any other ID (`0x160`), it writes the value to the LCD as `FUEL: xx.x %`
4. **Node 0** also runs a PWM output on `MR5` to drive a servo motor, acting as a physical fuel-level/warning gauge, with the position changing when a digital input (`P0.20`) is triggered.
5. On startup, Node 0 scrolls a splash message ("VEHICLE DASHBOARD USING AUTOMOTIVE INDUSTRY STANDARD MAINLINE COMMUNICATION PROTOCOL") across the LCD before showing live readings.

## Building

This code targets the **LPC2129** and is written for the **Keil µVision (ARM)** toolchain using the `LPC21xx.h` device header. To build:

1. Create a Keil µVision project targeting LPC2129 (or your preferred ARM7 toolchain with LPC21xx support).
2. Add `headerfp.h` and the shared driver sources (`can-driver-fp.c`, `spi-fp.c`, `mcp3204-fp.c`, `dashboard.c`) to each node's project.
3. Add the corresponding `main()` file per board:
   - Node 0 board → `node0.c`
   - Node 1 board → `node1.c`
   - Node 2 board → `node2.c`
4. Flash each board with its respective build.
5. Wire all three CAN2 transceivers onto a shared CAN bus (with proper termination resistors) and power each board.

## Notes / Known Limitations

- `#include <LPC21xx.H>` is commented out in `can-driver-fp.c` but included via `headerfp.h`, so it resolves correctly when compiled as part of the full project.
- Node 0's fuel-servo trigger condition (`IOPIN0 & (1<<20)`) is a placeholder digital input and can be adapted to a low-fuel threshold or warning condition.
- CAN acceptance filtering is fully open (`AFMR = 0x2`); in a larger network you may want to restrict this to the specific IDs each node needs.

