cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD                11)
set(CMAKE_C_STANDARD_REQUIRED       ON)
set(CMAKE_C_EXTENSIONS              ON)
set(CMAKE_CXX_STANDARD              20)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            ON)
set(PROJ_PATH                       ${CMAKE_CURRENT_SOURCE_DIR})
message("Build type: "              ${CMAKE_BUILD_TYPE})

#
# Core project settings
#
project(energy_monitor)
enable_language(C CXX ASM)

#
# Core MCU flags, CPU, instruction set and FPU setup
# Needs to be set properly for your MCU
#
set(CPU_PARAMETERS
    -mthumb

    # This needs attention to properly set for used MCU
    -mcpu=cortex-m7
    -mfpu=fpv5-d16
    -mfloat-abi=hard
)

# Set linker script

# Define a list of linker scripts
set(linker_script_SRC
    ${PROJ_PATH}/STM32F767ZITX_FLASH.ld
    # Add more linker scripts as needed
)

set_property(TARGET ${EXECUTABLE} PROPERTY LINK_DEPENDS ${linker_script_SRC})

set(EXECUTABLE                      ${CMAKE_PROJECT_NAME})

#
# List of source files to compile
#
set(sources_SRCS
    # Put here your source files, one in each line, relative to CMakeLists.txt file location
    ${PROJ_PATH}/3_DRV/INA226/Src/ina226.c
    ${PROJ_PATH}/3_DRV/Irq/Src/irq.c
    ${PROJ_PATH}/2_HAL/EnergyMonitor/Src/hal_energy_monitor.c
    ${PROJ_PATH}/2_HAL/Gpio/Src/hal_gpio.c
    ${PROJ_PATH}/2_HAL/Uart/Src/hal_uart.c
    ${PROJ_PATH}/1_APP/Ecum/Src/main.c
    ${PROJ_PATH}/1_APP/EnergyMonitor/Src/app_energy_monitor.c
    ${PROJ_PATH}/4_Generated/Core/Src/freertos.c
    ${PROJ_PATH}/4_Generated/Core/Src/gpio.c
    ${PROJ_PATH}/4_Generated/Core/Src/i2c.c
    ${PROJ_PATH}/4_Generated/Core/Src/stm32f7xx_hal_msp.c
    ${PROJ_PATH}/4_Generated/Core/Src/stm32f7xx_it.c
    ${PROJ_PATH}/4_Generated/Core/Src/system_stm32f7xx.c
    ${PROJ_PATH}/4_Generated/Core/Src/usart.c
    ${PROJ_PATH}/4_Generated/Core/Startup/startup_stm32f767zitx.s
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.c
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_exti.c
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.c
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.c
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart_ex.c
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c
    ${PROJ_PATH}/4_Generated/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c
    ${PROJ_PATH}/4_Generated/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c
    ${PROJ_PATH}/4_Generated/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    ${PROJ_PATH}/4_Generated/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    ${PROJ_PATH}/4_Generated/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    ${PROJ_PATH}/4_Generated/Middlewares/Third_Party/FreeRTOS/Source/list.c
    ${PROJ_PATH}/4_Generated/Middlewares/Third_Party/FreeRTOS/Source/queue.c
    ${PROJ_PATH}/4_Generated/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    ${PROJ_PATH}/4_Generated/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    ${PROJ_PATH}/4_Generated/Middlewares/Third_Party/FreeRTOS/Source/timers.c
)

#
# Include directories
#
set(include_path_DIRS
    # Put here your include dirs, one in each line, relative to CMakeLists.txt file location
    ${PROJ_PATH}/3_DRV/INA226/Cfg
    ${PROJ_PATH}/3_DRV/INA226/Src
    ${PROJ_PATH}/2_HAL/EnergyMonitor/Src
    ${PROJ_PATH}/2_HAL/EnergyMonitor/Cfg
    ${PROJ_PATH}/2_HAL/Gpio/Src
    ${PROJ_PATH}/2_HAL/Uart/Src
    ${PROJ_PATH}/2_HAL/Uart/Cfg
    ${PROJ_PATH}/1_APP/Ecum/Src
    ${PROJ_PATH}/1_APP/EnergyMonitor/Src
    ${PROJ_PATH}/1_APP/EnergyMonitor/Cfg
    ${PROJ_PATH}/4_Generated/Core/Inc
    ${PROJ_PATH}/4_Generated/Drivers/CMSIS/Device/ST/STM32F7xx/Include
    ${PROJ_PATH}/4_Generated/Drivers/CMSIS/Include
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Inc
    ${PROJ_PATH}/4_Generated/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy
    ${PROJ_PATH}/4_Generated/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
    ${PROJ_PATH}/4_Generated/Middlewares/Third_Party/FreeRTOS/Source/include
    ${PROJ_PATH}/4_Generated/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1
)

#
# Symbols definition
#
set(symbols_SYMB
    # Put here your symbols (preprocessor defines), one in each line
    # Encapsulate them with double quotes for safety purpose
    "DEBUG"
    "STM32F767xx"
    "USE_HAL_DRIVER"
    "HSE_VALUE=8000000"
)

# Executable files
add_executable(${EXECUTABLE} ${sources_SRCS})

# Include paths
target_include_directories(${EXECUTABLE} PRIVATE ${include_path_DIRS})

# Project symbols
target_compile_definitions(${EXECUTABLE} PRIVATE ${symbols_SYMB})

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    # Full debug configuration
    # -Og -g3 -ggdb
)

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
    -T${linker_script_SRC}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    --specs=nosys.specs
    -u _printf_float                # STDIO float formatting support
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -Wl,--print-memory-usage
)

# Execute post-build to print size
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
)

# Convert output to hex and binary
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex
)

# Conver to bin file -> add conditional check?
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin
)