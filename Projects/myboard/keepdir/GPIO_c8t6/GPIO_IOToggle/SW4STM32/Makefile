# 编译器设置
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)as
LD = $(PREFIX)ld
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump
SIZE = $(PREFIX)size

# 项目名称
PROJECT = GPIO_IOToggle

# 编译选项
MCU = -mcpu=cortex-m3 -mthumb
CFLAGS = $(MCU) -Wall -g -O0 -ffunction-sections -fdata-sections
CFLAGS += -DUSE_HAL_DRIVER -DSTM32F103xB
ASFLAGS = $(MCU) -Wall -g
# LDFLAGS = $(MCU) -T$(PROJECT).ld -Wl,-Map=$(PROJECT).map -Wl,--gc-sections
LDFLAGS = $(MCU) -TSTM3210E_EVAL/STM32F103ZGHx_FLASH.ld -Wl,-Map=$(PROJECT).map -Wl,--gc-sections

# 源文件目录
SRC_DIR = ../Src
# INC_DIR = ../Inc

SRCS = $(wildcard Src/*.c)
# 添加启动文件
SRCS += startup_stm32f103xb.s
# 添加HAL库文件
SRCS += ../../../../../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
SRCS += ../../../../../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
SRCS += ../../../../../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
SRCS += ../../../../../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c
SRCS += ../../../../../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c

# 包含路径
INC_DIR = -I../Inc
INC_DIR += -I../../../../../../Drivers/STM32F1xx_HAL_Driver/Inc
INC_DIR += -I../../../../../../Drivers/CMSIS/Device/ST/STM32F1xx/Include
INC_DIR += -I../../../../../../Drivers/CMSIS/Include
INC_DIR += -I../../../../../../Drivers/BSP/STM32F1xx_c8t6_mini

# 源文件
SRCS = $(wildcard $(SRC_DIR)/*.c)
ASRCS = ../startup_stm32f103xb.s

# 目标文件
OBJS = $(SRCS:.c=.o) $(ASRCS:.s=.o)

# 默认目标
all: $(PROJECT).elf $(PROJECT).hex $(PROJECT).bin

# 编译规则
%.o: %.c
	$(CC) $(CFLAGS) $(INC_DIR) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) -c $< -o $@

# 链接
$(PROJECT).elf: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@
	$(SIZE) $@

# 生成hex文件
$(PROJECT).hex: $(PROJECT).elf
	$(OBJCOPY) -O ihex $< $@

# 生成bin文件
$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

# 清理
clean:
	rm -f $(OBJS) $(PROJECT).elf $(PROJECT).hex $(PROJECT).bin $(PROJECT).map

.PHONY: all clean 