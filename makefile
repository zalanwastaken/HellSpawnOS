.PHONY: all run debug clean

CC = i386-elf-gcc
LD = i386-elf-ld
AS = nasm

CFLAGS = -m32 -ffreestanding -pedantic -msse2 -c
LDFLAGS = -T link.ld -m elf_i386 -nostdlib --oformat=binary

# ---- sources ----
SRC_C   := $(shell find src/os/kernel -name "*.c")
SRC_ASM := $(shell find src/os/kernel -name "*.asm")

# ---- objects ----
OBJ_C   := $(patsubst src/%.c, build/%.o, $(SRC_C))
OBJ_ASM := $(patsubst src/%.asm, build/%.o, $(SRC_ASM))
OBJS    := $(OBJ_C) $(OBJ_ASM)

# ---- main target ----
all: build/kernel.bin build/os.img

# ---- compile C ----
build/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $< -o $@
	@echo "CC $<"

# ---- compile ASM ----
build/%.o: src/%.asm
	@mkdir -p $(dir $@)
	@$(AS) -f elf $< -o $@
	@echo "AS $<"

# ---- link kernel ----
build/kernel.bin: $(OBJS)
	@$(LD) $(LDFLAGS) -o $@ $(OBJS)
	@echo "LD $<"

# ---- bootloader ----
build/bootloader.bin: src/os/bootloader.asm build/kernel.bin
	@mkdir -p build
	$(AS) -DSECTORS=$(shell stat -c%b build/kernel.bin) -f bin $< -o $@

# ---- disk image ----
build/os.img: build/bootloader.bin build/kernel.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=build/bootloader.bin of=$@ conv=notrunc
	dd if=build/kernel.bin of=$@ bs=1 seek=4096 conv=notrunc

# ---- run ----
run: all
	qemu-system-x86_64 -drive format=raw,file=build/os.img -m 4G -serial stdio -audiodev alsa,id=snd0 -machine pcspk-audiodev=snd0 -vga qxl -machine pc

# ---- debug ----
debug: all
	qemu-system-x86_64 -drive format=raw,file=build/os.img -m 4G -serial stdio -audiodev alsa,id=snd0 -machine pcspk-audiodev=snd0 -vga qxl -machine pc -s -S

# ---- clean ----
clean:
	rm -rf build
