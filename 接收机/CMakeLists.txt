#THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.19)

# specify cross compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings


project(GD32E230_demo C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# for use printf & scanf with float
set(COMMON_FLAGS "-specs=nosys.specs -specs=nano.specs -u _printf_float -u _scanf_float")

#       Flash程序入口文件 CMAKE_SOURCE_DIR -- 为包含PROJECT()命令的最近一个CMakeLists.txt文件所在的文件夹路径。
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F031C6_FLASH.ld)

#       add_link_options --添加链接选项
#       链接选项查询网站https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html#index-z
#       add_compile_options  --添加编译选项
#       编译选项查询网站https://gcc.gnu.org/onlinedocs/gcc/Option-Index.html#Option-Index_op_letter-M
#Uncomment for hardware floating point取消硬件浮点的注释
add_compile_options(-mfloat-abi=soft)
add_link_options(-mfloat-abi=soft)
#    -mfloat-abi=soft
#    -mfloat-abi=softfp
#    -mfloat-abi=hard
#    "soft"选项：表明不使用FPU硬件，而是使用GCC的整数算术运算来模拟浮点运算。
#    "softfp"选项：表明要使用FPU硬件来做浮点运算，只是，函数的参数传递到整数寄存器（r0-r3）中，然后再传递到FPU中。
#    "hard"选项：表明要使用FPU硬件来做浮点运算，并且，函数的参数直接传递到FPU的寄存器（s0、d0）中。
#    armcc编译器使–cpu 7-A或者–cpu Cortex-A8来指定指令集架构和CPU类型。
#    gcc编译器的处理器选项-mfpu=neon和-mcpu来指定cpu类型。如-mcpu=cortex-a5
#    gcc选择用-mfpu=vfpv3-fp16来指定为vfp协处理，而-mfpu=neon-vfpv4等就能指定为NEON+VFP结构。
#    -mfpu代表硬件浮点运算单元的名称，具体的值可在《Cortex M3与M4权威指南》的17章节可找到。

#Uncomment for software floating point取消软件浮点的注释
add_compile_options(-mcpu=cortex-m23 -mthumb -mthumb-interwork)
add_link_options(-mcpu=cortex-m23 -mthumb -mthumb-interwork)
#    -mcpu表示内核是cortex-m4内核；
#    -mthumb 代表编译为thumb指令集
#    -mthumb-interwork 的意义是：使用这个编译选项生成的目标文件，允许在ARM和Thumb之间交叉调用。但需要说明的是，加入这个选项之后，编译器生成的目标文件既可能是ARM的，也有可能是Thumb的，这个要根据其他的编译选项的情况而定。
#    于是乎，一个自然而然的问题就是："-mthumb-interwork -mthumb”编译选项生成的目标文件就是thumb的，但是可以被其他的ARM的目标文件交叉调用。
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
#    GCC在编译时可以使用 -ffunction-sections 和 -fdata-sections 将每个函数或符号创建为一个sections，其中每个sections名与function或data名保持一致。而在链接阶段， -Wl,–gc-sections 指示链接器去掉不用的section（其中-wl, 表示后面的参数 -gc-sections 传递给链接器），这样就能减少最终的可执行程序的大小了。
#    -fno-common选项来禁止将未初始化的全局变量放入到common段
#    对于全局变量来说，如果初始化了不为0的值，那么该全局变量则被保存在data段，如果初始化的值为0，那么将其保存在bss段，如果没有初始化，则将其保存在common段，等到链接时再将其放入到BSS段。关于第三点不同编译器行为会不同，有的编译器会把没有初始化的全局变量直接放到BSS段。
#    -fmessage-length=0后，输出信息会根据控制台的宽度自动换行，这样就能看全输出信息了。


add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)#生成map文件
#   作为选项传递到链接器。如果包含逗号，则会在逗号处将其拆分为多个选项。可以使用此语法将参数传递给选项。例如，传递到链接器。使用 GNU 链接器时，您也可以获得 与 相同的效果。
#   --gc-sections表示依赖库中不使用的sections将不被链接，可以减小链接目标的大小，一般跟-ffunction-sections一起使用。用法：-Wl,--gc-sections。
#   这为我们提供了链接器文件中定义的每个内存区域中使用的内存的细分。这对于定义具有不同空间限制的多个内存区域的嵌入式系统特别有用。
add_link_options(-T ${LINKER_SCRIPT})
#   -T 用作链接器脚本。大多数使用 GNU 链接器的系统都支持此选项。在某些目标上，例如没有操作系统的裸板目标，链接时可能需要该选项，以避免引用未定义的符号。script-T



#       单独设置C++编译器编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")
#除了修改该变量的值之外，还可以通过add_compile_options命令来设置编译选项。它们的区别是：
#add_compile_options命令添加的编译选项是针对所有编译器的(包括c和c++编译器)
#CMAKE_C_FLAGS或CMAKE_CXX_FLAGS变量则是分别只针对c和c++编译器


# uncomment to mitigate c++17 absolute addresses warnings设置编译优化及类型
SET(CMAKE_BUILD_TYPE "Debug")
#1 Debug：用于在没有优化的情况下，使用带有调试符号构建库或可执行文件 在线调试一定要选择Debug 否则无法断点
#2 Release：用于构建的优化的库或可执行文件，不包含调试符号
#3 RelWithDebInfo：由于构建较少的优化库或可执行文件，包含调试符号
#4 MinSizeRel：用于不增加目标代码大小的优化方式，来构建或可执行文件
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
    #add_compile_options(-O0)
endif ()

#       头文件路径
include_directories(
        CMSIS/Include
        GD32E23x_standard_peripheral/Include
        Hardware_Abstraction_Layer
        Device_Drivers_Layer
        Software_Layer
        USER
)

#       添加宏
add_definitions(-DGD32E230)

#       源文件路径
file(GLOB_RECURSE SOURCES
        "*.s"
        "CMSIS/Source/*.c"
        "GD32E23x_standard_peripheral/Source/*.c"
        "Hardware_Abstraction_Layer/*.cpp"
        "Software_Layer/*.c"
        "Software_Layer/*.cpp"
        "Device_Drivers_Layer/*.cpp"
        "USER/*.c"
        "USER/*.cpp"
        )




add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})#生成目标可执行文件 SOURCES - 所有源文件及启动文件 LINKER_SCRIPT - Flash程序入口文件 *.ld

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)#将HEX路径定义为变量PROJECT_BINARY_DIR - 运行cmake命令的目录，即工程编译发生的路径  PROJECT_NAME - 工程名 由 project（）设置
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)#将HEX路径定义为变量PROJECT_BINARY_DIR - 运行cmake命令的目录，即工程编译发生的路径  PROJECT_NAME - 工程名 由 project（）设置

#       自动命令
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD#生成目标后编译
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}#生成HEX文件
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}#生成BIN文件
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")#输出路径
