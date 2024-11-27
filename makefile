include make.config

# Define variables
TARGET = PaybackOS.elf
INITRD_TARGET = initrd.bin

# Sources for initrd
INITRD_SRCS := $(shell find initrd -name '*.c' -o -name '*.s' -o -name '*.asm')
INITRD_OBJS := $(patsubst initrd/%.c, obj/initrd/%.o, $(patsubst initrd/%.s, obj/initrd/%.o, $(patsubst initrd/%.asm, obj/initrd/%.o, $(INITRD_SRCS))))

SRCS := $(shell find $(SRC_DIR) -name '*.c' -o -name '*.s' -o -name '*.asm' -not -path "$(SRC_DIR)/userspace/*")
USERSPACE_SRCS := $(shell find $(USERSPACE_DIR) -name '*.c' -o -name '*.s' -o -name '*.asm')
SHARED_SRCS := $(shell find $(SHARED_DIR) -name '*.c' -o -name '*.s' -o -name '*.asm')

# Object files for src/ and userspace/
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(patsubst $(SRC_DIR)/%.s,$(OBJ_DIR)/%.o,$(patsubst $(SRC_DIR)/%.asm,$(OBJ_DIR)/%.o,$(SRCS)))))
USERSPACE_OBJS := $(patsubst $(USERSPACE_DIR)/%.cpp,$(OBJ_DIR)/userspace/%.o,$(patsubst $(USERSPACE_DIR)/%.c,$(OBJ_DIR)/userspace/%.o,$(patsubst $(USERSPACE_DIR)/%.s,$(OBJ_DIR)/userspace/%.o,$(patsubst $(USERSPACE_DIR)/%.asm,$(OBJ_DIR)/userspace/%.o,$(USERSPACE_SRCS)))))
SHARED_OBJS := $(patsubst $(SHARED_DIR)/%.c,$(OBJ_DIR)/shared/%.o,$(patsubst $(SHARED_DIR)/%.s,$(OBJ_DIR)/shared/%.o,$(SHARED_SRCS)))

# Ensure the object directories exist
$(shell mkdir -p $(OBJ_DIR)/userspace)
$(shell mkdir -p $(OBJ_DIR)/shared)
$(shell mkdir -p $(OBJ_DIR)/initrd)

# Define the build rules
build: $(OBJS) $(USERSPACE_OBJS) $(SHARED_OBJS) initrd
	$(LD) -T linker.ld -o $(TARGET) $(LDFLAGS) $(OBJS) $(USERSPACE_OBJS) $(SHARED_OBJS)

initrd: $(INITRD_OBJS)
	$(LD) -r -o $(INITRD_TARGET) $(INITRD_OBJS)
	$(OBJCOPY) -O binary $(INITRD_TARGET) iso/boot/initrd.bin

# Pattern rules to build .o files from .s, .asm, and .c files for initrd/
obj/initrd/%.o: initrd/%.s
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

obj/initrd/%.o: initrd/%.asm
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

obj/initrd/%.o: initrd/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Pattern rules to build .o files for src/, userspace/, and shared
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/userspace/%.o: $(USERSPACE_DIR)/%.s
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJ_DIR)/userspace/%.o: $(USERSPACE_DIR)/%.asm
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJ_DIR)/userspace/%.o: $(USERSPACE_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(USERCFLAGS) -c $< -o $@

$(OBJ_DIR)/shared/%.o: $(SHARED_DIR)/%.s
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJ_DIR)/shared/%.o: $(SHARED_DIR)/%.asm
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJ_DIR)/shared/%.o: $(SHARED_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(USERCFLAGS) -c $< -o $@

# Create the GRUB iso
iso: build
	mkdir -p iso/boot/grub/
	cp grub.cfg iso/boot/grub/
	cp $(TARGET) iso/boot/
	cp $(INITRD_TARGET) iso/boot/
	grub-mkrescue iso/ -o PaybackOS.iso

# Run the OS
run:
	qemu-system-i386 -hda PaybackOS.iso

# Clean rule to remove generated files
clean:
	rm -rf obj $(TARGET) $(INITRD_TARGET) iso *.iso *.log

.PHONY: all build clean iso run initrd
