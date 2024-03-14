# Python script as a substitute for Makefile
# Makefiles are simple and much more efficient, but I cannot be bothered learning it...

import glob
import os
import sys

BOOT_SRC = "boot/boot.asm"
KERNEL_ROOT_SRC = "kernel/"
LINKER_SRC = "linker.ld"
BUILD_DEST = "build"
OBJ_DEST = "build/obj/"
ISO_DEST = "build/talon.iso"
GRUB_DEST = "build/iso/boot/grub"
KERNEL_DEST = "build/talon.bin"


def shellout(command):
    get_result = os.system(command)
    if get_result != 0:
        print(f"\nExecuting stopped at a command: {command}")
        print(f"[Exiting script]\n")
        exit()


def os_clean():
    shellout(f"rm -rf {BUILD_DEST}")
    print(f"\n[Removed directory: {BUILD_DEST}]\n")


def os_build():
    # Compiles the system
    os_clean()
    print(f"\n[Compilation start]\n")

    os.makedirs(OBJ_DEST, exist_ok=True)
    shellout(f"nasm -f elf32 {BOOT_SRC} -o {OBJ_DEST}boot.o")

    # Find source files and compile them into objects
    c_files = glob.glob(KERNEL_ROOT_SRC + '**/*.c', recursive=True)
    cpp_files = glob.glob(KERNEL_ROOT_SRC + '**/*.cpp', recursive=True)
    asm_files = glob.glob(KERNEL_ROOT_SRC + "**/*.asm", recursive=True)

    for file_list in [c_files, cpp_files, asm_files]:
        for file in file_list:
            basename = os.path.splitext(os.path.basename(file))[0]
            obj_file = os.path.join(OBJ_DEST, f"{basename}.o")
            compiler_params = ""

            if file in c_files:
                compiler_params = "gcc -m32 -ffreestanding -c"
            elif file in cpp_files:
                compiler_params = "g++ -m32 -ffreestanding -c"
            elif file in asm_files:
                compiler_params = "nasm -f elf32"
            shellout(f"{compiler_params} {file} -o {obj_file}")

    # Link objects together
    objects = " ".join([
        f"{OBJ_DEST}{os.path.splitext(os.path.basename(file))[0]}.o" 
        for file in c_files + cpp_files + asm_files])

    shellout(f"ld -m elf_i386 -T {LINKER_SRC} -o {KERNEL_DEST} {OBJ_DEST}boot.o {objects}")
    print(f"\n[Compilation end]\n")


def os_iso():
    # Creating a bootable ISO by utilizing grub-mkrescue
    print(f"\n[ISO generation start]\n")
    os.makedirs(GRUB_DEST, exist_ok=True)
    shellout(f"cp grub.cfg {GRUB_DEST}")
    shellout(f"cp {KERNEL_DEST} {GRUB_DEST}")
    shellout(f"grub-mkrescue -o {ISO_DEST} build/iso")
    print(f"\n[ISO generation end]\n")


def os_run():
    # Running the ISO in a virtual environment
    print(f"\n[Running ISO in QEMU]\n")
    shellout(f"qemu-system-i386 -cdrom {ISO_DEST}")


def os_all():
    # Builds the system + generates ISO + runs the ISO in QEMU
    os_build()
    os_iso()
    os_run()


if __name__ == "__main__":
    args = sys.argv
    # args[1] = function name
    # args[2:] = function args
    globals()[args[1]](*args[2:])
