run: build
	qemu-system-arm -machine lm3s811evb -nographic -semihosting -kernel app.elf

build:
	arm-none-eabi-gcc -mcpu=cortex-m3 -o main.o -c main.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -o semihosting.o -c ../semihosting.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -nostartfiles -specs=nosys.specs \
		-Tlm3s811.ld  -L. -lopencm3_lm3s -o app.elf main.o semihosting.o

clean:
	rm -f main.o
	rm -f semihosting.o
	rm -f app.elf
