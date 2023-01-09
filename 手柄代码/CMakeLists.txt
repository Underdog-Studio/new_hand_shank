
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.19)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)




project(GD32F303 C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)


set(COMMON_FLAGS "-specs=nosys.specs -specs=nano.specs -u _printf_float -u _scanf_float")


set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/GF32F303RCTx_FLASH.ld)

add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)


add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)#生成map文件
add_link_options(-T ${LINKER_SCRIPT})



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")
SET(CMAKE_BUILD_TYPE "Debug")
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "速度最大化优化")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "速度最大优化，包括调试信息")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "尺寸最大化优化")
    add_compile_options(-Os)
else ()
    message(STATUS "最少的优化，包括调试信息")
    add_compile_options(-Og -g)
endif ()


include_directories(
        GD32F30x_standard_peripheral/Include
        Startup
        Software/Include
        Hardware_Abstraction_Layer
        Device_Drivers_Layer
        Core
        USER
        lvgl/lvgl
        lvgl/lvgl/src
        lvgl/lvgl/porting
        lvgl/lvgl/src/core
        lvgl/lvgl/src/extra
        FreeRTOS/Source/CMSIS_RTOS_V2
        FreeRTOS/Source/include
        FreeRTOS/Source/portable/GCC/ARM_CM4F
        FatFS
        USB/class/device/msc/Include
        USB/device/Include
        USB/usbd/Include
        USB/msc_udisk/inc
)

add_definitions(-DUSE_STDPERIPH_DRIVER -DGD32F30X_HD -D__TARGET_FPU_VFP -DARM_MATH_CM4)

file(GLOB_RECURSE SOURCES
        "*.s"
        "Core/*.c"
        "GD32F30x_standard_peripheral/Source/*.c"
        "Hardware_Abstraction_Layer/*.*"
        "Device_Drivers_Layer/*.c"
        "Device_Drivers_Layer/*.cpp"
        "Device_Drivers_Layer/si24r1/*.cpp"
        "Software/Source/*.c"
        "Software/Source/*.cpp"
        "Startup/*.c"
        "USER/*.c"
        "USER/*.cpp"
        "lvgl/lvgl/porting/*.c"
        "lvgl/lvgl/src/core/*.c"
        "lvgl/lvgl/src/draw/*.c"
        "lvgl/lvgl/src/font/*.c"
        "lvgl/lvgl/src/hal/*.c"
        "lvgl/lvgl/src/misc/*.c"
        "lvgl/lvgl/src/widgets/*.c"
        "lvgl/lvgl/src/extra/*.c"
        "FreeRTOS/Source/*.c"
        "FreeRTOS/Source/CMSIS_RTOS_V2/*.c"
        "FreeRTOS/Source/portable/GCC/ARM_CM4F/*.c"
        "FreeRTOS/Source/portable/MemMang/*.c"
        "FatFS/*.c"
        "USB/class/device/msc/Source/*.c"
        "USB/device/Source/*.c"
        "USB/usbd/Source/*.c"
        "USB/msc_udisk/src/*.c"
        )




add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)


add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE})
#Building ${BIN_FILE}")
