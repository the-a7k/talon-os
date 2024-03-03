# Python script as a substitute for Makefile
# Makefiles are simple and much more efficient, but I cannot be bothered learning it...

import glob
import os
import sys

BOOT_SRC = "boot/boot.asm"
ROOT_SRC = "kernel/"
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


def clean():
    shellout(f"rm -rf {BUILD_DEST}")
    print(f"\n[Removed directory: {BUILD_DEST}]\n")


def build():
    # Compiles the system
    clean()
    print(f"\n[Compilation start]\n")

    os.makedirs(OBJ_DEST, exist_ok=True)
    shellout(f"nasm -f elf32 {BOOT_SRC} -o {OBJ_DEST}boot.o")

    # Compile C files into objects
    c_files = glob.glob(ROOT_SRC + '**/*.c', recursive=True)
    for c_file in c_files:
        obj_file = os.path.join(OBJ_DEST, os.path.splitext(os.path.basename(c_file))[0] + ".o")
        shellout(f"gcc -m32 -ffreestanding -c {c_file} -o {obj_file}")

    # Compile ASM files to objects
    asm_files = glob.glob(ROOT_SRC + "**/*.asm", recursive=True)
    for asm_file in asm_files:
        obj_file = os.path.join(OBJ_DEST, os.path.splitext(os.path.basename(asm_file))[0] + ".o")
        shellout(f"nasm -f elf32 {asm_file} -o {obj_file}")

    # Link objects together
    objects = " ".join([f"{OBJ_DEST}{os.path.splitext(os.path.basename(file))[0]}.o" for file in c_files + asm_files])
    shellout(f"ld -m elf_i386 -T {LINKER_SRC} -o {KERNEL_DEST} {OBJ_DEST}boot.o {objects}")
    print(f"\n[Compilation end]\n")

def iso():
    # Creating a bootable ISO by utilizing grub-mkrescue
    print(f"\n[ISO generation start]\n")
    os.makedirs(GRUB_DEST, exist_ok=True)
    shellout(f"cp grub.cfg {GRUB_DEST}")
    shellout(f"cp {KERNEL_DEST} {GRUB_DEST}")
    shellout(f"grub-mkrescue -o {ISO_DEST} build/iso")
    print(f"\n[ISO generation end]\n")


def run():
    # Running the ISO in a virtual environment
    print(f"\n[Running ISO in QEMU]\n")
    shellout(f"qemu-system-i386 -cdrom {ISO_DEST}")


def all():
    # Builds the system + generates ISO + runs the ISO in QEMU
    build()
    iso()
    run()
    

if __name__ == "__main__":
    args = sys.argv
    # args[1] = function name
    # args[2:] = function args
    globals()[args[1]](*args[2:])
