C_SOURCES = $(wildcard 		$\
	kernel/*.c 			$\
	kernel/drivers/*.c 	$\
	kernel/controller/*.c 	$\
	kernel/libc/*.c 		$\
	kernel/usermode/*.c) 	$\

HEADERS = $(wildcard 		$\
	kernel/*.h 			$\
	kernel/drivers/*.h 	$\
	kernel/controller/*.h 	$\
	kernel/libc/*.h 		$\
	kernel/usermode/*.h 	$\
)

OBJ = ${C_SOURCES:.c=.o kernel/controller/interrupt.o} 


CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
CFLAGS = -g


talon.bin: boot/bootsect.bin kernel.bin
	cat $^ > talon.bin

kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ 

run: talon.bin
	qemu-system-i386 -fda talon.bin

debug: talon.bin kernel.elf
	qemu-system-i386 -s -fda talon.bin -d guest_errors,int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

clean:
	find . -type f \( -name '*.o' -o -name '*.bin' -o -name '*.dis' -o -name '*.elf' \) -not -name talon.bin -delete


# Generic rules for wildcards - to make an object, always compile from its .c
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@
