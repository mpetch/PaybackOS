# Define the compiler and flags
CC = i686-elf-gcc
AS = i686-elf-as
LD = i686-elf-gcc
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Isrc/headers/
LDFLAGS = -ffreestanding -O2 -nostdlib -lgcc

# Find all source files in src/ and its subdirectories
SRCS_S = $(shell find src/ -name '*.s')
SRCS_ASM = $(shell find src/ -name '*.asm')
SRCS_C = $(shell find src/ -name '*.c')

# Find all object files in src/ and its subdirectories
OBJ_S = $(SRCS_S:src/%.s=src/%.o)
OBJ_ASM = $(SRCS_ASM:src/%.asm=src/%.o)
OBJ_C = $(SRCS_C:src/%.c=src/%.o)
OBJ = $(OBJ_S) $(OBJ_ASM) $(OBJ_C)

# Define the kernel binary
TARGET = PaybackOS.bin

# Define the build target
ALL: build iso

# Define the build rules
build: $(OBJ)
	$(LD) -T linker.ld -o $(TARGET) $(LDFLAGS) $(OBJ)

# Pattern rule to build .o files from .s files
src/%.o: src/%.s
	$(AS) -o $@ $<

# Pattern rule to build .o files from .asm files
src/%.o: src/%.asm
	$(AS) -o $@ $<

# Pattern rule to build .o files from .c files
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Create the GRUB iso
iso:
	mkdir -p iso/boot/grub/
	cp grub.cfg iso/boot/grub/
	mv $(TARGET) iso/boot/
	grub-mkrescue iso/ -o PaybackOS.iso
	rm -rf iso

# Run the OS
run:
	qemu-system-x86_64 -cdrom PaybackOS.iso 

# Clean rule to remove generated files
clean:
	rm -rf $(OBJ) $(TARGET) iso *.iso

.PHONY: ALL build clean
