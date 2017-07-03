PROJECT = fongerOS

PWD ?= ${CURDIR}
EXECUTABLE = $(PROJECT).elf
BIN_IMAGE = $(PROJECT).bin


CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
SIZE = $(CROSS_COMPILE)size

CPU = cortex-m3
CFLAGS = -mcpu=$(CPU)
CFLAGS += -fno-common -O0 -g -mcpu=cortex-m3 -mthumb  -fno-stack-protector

LDFLAGS += -T./kernel/kernel.ld

CFLAGS += -I$(PWD)/libraries/CMSIS/CM3/CoreSupport
CFLAGS += -I$(PWD)/libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
CFLAGS += -I$(PWD)/libraries/STM32F10x_StdPeriph_Driver/inc
CFLAGS += -I$(PWD)/common
CFLAGS += -I$(PWD)/include

OBJS += ./libraries/CMSIS/CM3/CoreSupport/core_cm3.o \
		./libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.o \
		./libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_md.o \
		./common/stm32_p103.o \
		./libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.o \
		./libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.o  \
		./libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.o \
		./libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.o \
		./libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_adc.o  \
		./libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.o \
		./libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rtc.o \
		./libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_dac.o \
		./libraries/STM32F10x_StdPeriph_Driver/src/misc.o \
		./kernel/main.o 

OBJS += ./kernel/interrupt.o
OBJS += ./kernel/isr.o
OBJS += ./kernel/task.o
OBJS += ./kernel/keyboard.o
OBJS += ./kernel/user.o
OBJS += ./kernel/string.o

all: $(BIN_IMAGE)

$(BIN_IMAGE): $(EXECUTABLE)
	$(OBJCOPY) -O binary $^ $@
	$(OBJDUMP) -h -S -D $(EXECUTABLE) > $(PROJECT).lst

$(EXECUTABLE): $(OBJS)
	$(LD) -o $@ $(OBJS) \
		$(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(OBJS)
	rm -rf $(BIN_IMAGE)
	rm -rf *.lst
	rm -rf *.txt

gdb:
	./qemu/qemu-system-arm -M stm32-p103 -kernel $(BIN_IMAGE) -s -S
