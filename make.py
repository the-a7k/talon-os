# Python script as a substitue for Makefile
# Makefiles are simple and more efficient, but I cannot be bothered learning it...

import glob
import os

BOOT_SRC = "boot/boot.asm"
ROOT_SRC = "kernel/"
LINKER_SRC = "linker.ld"
OBJ_DEST = "build/obj/"
ISO_DEST = "build/talon.iso"
GRUB_DEST = "build/iso/boot/grub"
KERNEL_DEST = "build/talon.bin"

c_filenames = []
c_filepaths = []
asm_filenames = []
asm_filepaths = []
obj_files = []


def clean():
     os.system("rm -rf build")


def build():
    os.makedirs(OBJ_DEST)
    os.system(f"nasm -f elf32 {BOOT_SRC} -o {OBJ_DEST}boot.o")

    # Get C source files
    for c_file in glob.iglob(ROOT_SRC + '**/*.c', recursive=True):
        c_filepaths.append(c_file)
        c_filenames.append(os.path.basename(c_file))

    # Get assembly files
    for asm_file in glob.iglob(ROOT_SRC + "**/*.asm", recursive=True):
        asm_filepaths.append(asm_file)
        asm_filenames.append(os.path.basename(asm_file))

    # Compile C files into objects
    for file, path in zip(c_filenames, c_filepaths):
        obj_file = ""
        name, extension = os.path.splitext(file)
        if extension != ".o":
            obj_file = name + ".o"
        else:
            obj_file = name
        os.system(f"gcc -m32 -ffreestanding -c {path} -o {OBJ_DEST}{obj_file}")
        obj_files.append(obj_file)

    # Compile ASM files to objects
    for file, path in zip(asm_filenames, asm_filepaths):
        obj_file = ""
        name, extension = os.path.splitext(file)
        if extension != ".o":
            obj_file = name + ".o"
        else:
            obj_file = name
        os.system(f"nasm -f elf32 {path} -o {OBJ_DEST}{obj_file}")
        obj_files.append(obj_file)

    # Link objects together
    linker_prompt = (f"ld -m elf_i386 -T {LINKER_SRC} -o {KERNEL_DEST} {OBJ_DEST}boot.o")
    for object in obj_files:
        linker_prompt += f" {OBJ_DEST}{object}"
    
    os.system(linker_prompt)


def iso():
	os.makedirs(GRUB_DEST)
	os.system(f"cp grub.cfg {GRUB_DEST}")
	os.system(f"cp {KERNEL_DEST} {GRUB_DEST}")
	os.system(f"grub-mkrescue -o {ISO_DEST} build/iso")


def run():
	os.system(f"qemu-system-i386 -cdrom {ISO_DEST}")


clean()
build()
iso()
run()