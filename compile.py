# Python script as a substitute for Makefile
# Makefiles are simple and much more efficient, but I can't be bothered learning it...

import glob
import os
import sys

# Directory locations
BUILD_DEST = "build/"
OBJ_DEST = "build/obj/"
GRUB_DEST = "build/grub/boot/grub/"
GRUB_ROOT_SRC = "build/grub/"
KERNEL_ROOT_SRC = "kernel/"

# Specific file locations
ISO_DEST = "build/talon.iso"
KERNEL_DEST = "build/talon.bin"
BOOT_SRC = "boot/boot.asm"
LINKER_SRC = "linker.ld"
GRUB_SRC = "grub.cfg"

# Compile command/parameters
GCC_COMP = "gcc -m32 -ffreestanding -c"
GPP_COMP = "g++ -m32 -ffreestanding -c"
NASM_COMP = "nasm -f elf32"
LINKER_COMP = "ld -m elf_i386 -T"

QEMU_ARGS = "-audiodev pa,id=speaker -machine pcspk-audiodev=speaker"


def cmdout(command):
    get_result = os.system(command)
    if get_result != 0:
        print(f"\nExecuting stopped at a command: {command}")
        print(f"[Exiting script]\n")
        exit()


def os_clean():
    cmdout(f"rm -rf {BUILD_DEST}")
    print(f"\n[Removed directory: {BUILD_DEST}]\n")


def os_build():
    # Compiles the system
    os_clean()
    print(f"\n[Compilation start]\n")

    os.makedirs(OBJ_DEST, exist_ok=True)
    cmdout(f"{NASM_COMP} {BOOT_SRC} -o {OBJ_DEST}boot.o")

    # Find source files and compile them into objects
    c_files = glob.glob(KERNEL_ROOT_SRC + '**/*.c', recursive=True)
    cpp_files = glob.glob(KERNEL_ROOT_SRC + '**/*.cpp', recursive=True)
    asm_files = glob.glob(KERNEL_ROOT_SRC + "**/*.asm", recursive=True)

    for file_list in [c_files, cpp_files, asm_files]:
        for file in file_list:
            basename = os.path.splitext(os.path.basename(file))[0]
            obj_file = os.path.join(OBJ_DEST, f"{basename}.o")
            compiler_args = ""

            if file in c_files:
                compiler_args = GCC_COMP;
            elif file in cpp_files:
                compiler_args = GPP_COMP
            elif file in asm_files:
                compiler_args = NASM_COMP
            cmdout(f"{compiler_args} {file} -o {obj_file}")

    # Link objects together
    objects = " ".join([
        f"{OBJ_DEST}{os.path.splitext(os.path.basename(file))[0]}.o" 
        for file in c_files + cpp_files + asm_files])

    cmdout(f"{LINKER_COMP} {LINKER_SRC} -o {KERNEL_DEST} {OBJ_DEST}boot.o {objects}")
    print(f"\n[Compilation end]\n")


def os_iso():
    # Creating a bootable ISO by utilizing grub-mkrescue
    print(f"\n[ISO generation start]\n")
    os.makedirs(GRUB_DEST, exist_ok=True)
    cmdout(f"cp {GRUB_SRC} {GRUB_DEST}")
    cmdout(f"cp {KERNEL_DEST} {GRUB_DEST}")
    cmdout(f"grub-mkrescue -o {ISO_DEST} {GRUB_ROOT_SRC}")
    print(f"\n[ISO generation end]\n")


def os_run():
    # Running the ISO in a virtual environment
    print(f"\n[Running ISO in QEMU]\n")
    cmdout(f"qemu-system-i386 -cdrom {ISO_DEST} {QEMU_ARGS}")


def os_all():
    # Builds the system + generates ISO + runs the ISO in QEMU
    os_build()
    os_iso()
    os_run()


if __name__ == "__main__":
    args = sys.argv
    globals()[args[1]](*args[2:])
