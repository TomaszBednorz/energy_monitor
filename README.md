# energy_monitor

Project for the class "Operating systems for embedded systems"

## 1. Hardware
- STM32F767ZI Nucleo board
- INA226 sesor
- Other componenst like resistors, buttons etc.

## 2. Functionalities
- The MCU measures data (voltage, current, power) from an external circuit - an energy monitoring function.
- Processed information is sent via serial port.

## 3. TODO list
### Software
- [x] INA226 driver
- [x] IRQ driver
- [x] Energy monitor hardware abstraction layer
- [x] GPIO hardware abstraction layer
- [x] UART hardware abstraction layer
- [ ] Energy monitor application layer
### Others
- [x] Cmake configuration
- [x] Generation of low-level drivers
- [x] Comments in the software
- [ ] Documentation
