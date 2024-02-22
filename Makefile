C_SOURCES = $(wildcard kernel/*.c kernel/drivers/*.c kernel/controller/*.c libc/*.c)
HEADERS = $(wildcard kernel/*.h kernel/drivers/*.h kernel/controller/*.h libc/*.h)

OBJ = ${C_SOURCES:.c=.o kernel/controller/interrupt.o} 

# Cross-compiler location
CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
CFLAGS = -g

# First rule is run by default
os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > os-image.bin

# Symbol removing with '--oformat binary'
kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# Debug
kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ 

run: os-image.bin
	qemu-system-i386 -fda os-image.bin

# Open the connection to qemu and load our kernel-object file with symbols
debug: os-image.bin kernel.elf
	qemu-system-i386 -s -fda os-image.bin -d guest_errors,int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards - to make an object, always compile from its .c
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

# Removes every compiled file
wipe:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin kernel/drivers/*.o boot/*.o kernel/controller/*.o libc/*.o

# Keeps os-image
clean:
	rm -rf *.dis *.o *.elf kernel.bin
	rm -rf kernel/*.o boot/*.bin kernel/drivers/*.o boot/*.o kernel/controller/*.o libc/*.o