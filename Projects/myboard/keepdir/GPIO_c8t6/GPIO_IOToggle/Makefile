# 工具链设置
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
CFLAGS = $(MCU) -Wall -g -O0
CFLAGS += -fno-common -ffunction-sections -fdata-sections
CFLAGS += -DUSE_HAL_DRIVER -DSTM32F103xB

# 包含路径
INCLUDES = -I./Inc
INCLUDES += -I../../../../../Drivers/STM32F1xx_HAL_Driver/Inc
INCLUDES += -I../../../../../Drivers/CMSIS/Device/ST/STM32F1xx/Include
INCLUDES += -I../../../../../Drivers/CMSIS/Include
INCLUDES += -I../../../../../Drivers/BSP/STM32F1xx_c8t6_mini

# 源文件
SRCS = $(wildcard Src/*.c)
# 添加启动文件
SRCS += startup_stm32f103xb.s
# 添加HAL库文件
SRCS += ../../../../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
SRCS += ../../../../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
SRCS += ../../../../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
SRCS += ../../../../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c
SRCS += ../../../../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c

OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)

# 链接脚本
LDSCRIPT = STM32F103C8Tx_FLASH.ld

# 链接选项
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) -Wl,-Map=$(PROJECT).map,--cref -Wl,--gc-sections

# 默认目标
all: $(PROJECT).elf $(PROJECT).hex $(PROJECT).bin
	$(SIZE) $(PROJECT).elf

# 编译规则
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.s
	$(AS) $(MCU) -c $< -o $@

# 链接
$(PROJECT).elf: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

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