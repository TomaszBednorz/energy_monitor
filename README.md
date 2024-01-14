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
Multisensor_station/
├── .vscode                 // Visual Studio Code files with configuration
├── Core                    
│   ├── Inc                 // Header files
│   ├── PrivateDrivers      // Custom drivers for MCU peripherals
│   ├── Src                 // Source files
│   └── Startup             // Startup and system files
├── Datasheets              // Datasheets of MCU and LPS25H sensor
└── Drivers
    ├── CMSIS               // Common Microcontroller Software Interface Standard
    └── FreeRTOS            // FreeRTOS driver
```

```
energy_monitor/
├── APP                    
│   ├── Ecum                 // Header files
│   └── EnergyMonitor             // Startup and system files
├── Datasheets              // Datasheets of MCU and LPS25H sensor
└── Drivers
    ├── CMSIS               // Common Microcontroller Software Interface Standard
    └── FreeRTOS            // FreeRTOS driver
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
- [ ] Documentation
