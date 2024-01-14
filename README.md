# Energy Monitor

Project for the class "Operating systems for embedded systems"

## 1. Hardware
- STM32F767ZI Nucleo board
- INA226 sesor
- Other componenst like resistors, buttons etc.

## 2. Functionalities
- The MCU measures data (voltage, current, power) from an external circuit - an energy monitoring function.
- Processed information is sent via serial port.

## 3. Project structure

```
energy_monitor/
├── 1_APP                           // Application layer
│   ├── Ecum                        
│   └── EnergyMonitor
├── 2_HAL                           // Hardware abstraction layer
│   ├── EnergyMonitor
│   ├── Gpio
│   └── Uart
├── 3_DRV                           // Driver layer
│   ├── INA226                      // INA226 sensor driver
│   └── Irq
├── 4_Generated                     // Code in this layer was generated by an external tool
│   ├── Core                        // Configuration of peripherals
│   ├── Drivers    
|   |   ├── CMSIS                   // Common Microcontroller Software Interface Standard
|   |   └── STM32F7xx_HAL_Driver    // HAL driver for MCU peripherals
│   └── Middlewares(FreeRTOS)       // Real-time operating system
├── CmakeList.txt
├── STM32F7x7.svd                   // System View Description file - description of MCU registers for debugging
└── STM32F767ZITX_FLASH.ld          // Linker file
```

## 4. TODO list
### Software
- [x] INA226 driver
- [x] IRQ driver
- [x] Energy monitor hardware abstraction layer
- [x] GPIO hardware abstraction layer
- [x] UART hardware abstraction layer
- [x] Energy monitor application layer
### Others
- [x] Cmake file configuration
- [x] Generation of low-level drivers
- [x] Comments in the software
- [x] Documentation
