.PHONY: build

build:
	mkdir -p build

	# Compile all C files to .o in build/
	find src/os/kernel -type f -name "*.c" | while read file; do \
		out="build/$${file#src/}"; \
		out="$${out%.c}.o"; \
		mkdir -p "$$(dirname "$$out")"; \
		i386-elf-gcc -m32 -ffreestanding -pedantic -c "$$file" -o "$$out"; \
	done
	# Compile kernel.c
	#i386-elf-gcc -m32 -ffreestanding -pedantic -c src/os/kernel/kernel.c -o build/os/kernel.o

	# Compile all ASM files to .o in build/
	find src/os/kernel -type f -name "*.asm" | while read file; do \
		out="build/$${file#src/}"; \
		out="$${out%.asm}.o"; \
		mkdir -p "$$(dirname "$$out")"; \
		nasm -f elf "$$file" -o "$$out"; \
	done

	# Link everything into kernel.bin
	i386-elf-ld -T link.ld -m elf_i386 -nostdlib --oformat=binary \
		-o build/kernel.bin $$(find build -type f -name "*.o")

	# Bootloader (raw binary for sector 0 only)
	nasm -DSECTORS=$$(stat -c%b build/kernel.bin) -f bin src/os/bootloader.asm -o build/bootloader.bin

	# Create disk image
	dd if=/dev/zero of=build/os.img bs=512 count=2880
	dd if=build/bootloader.bin of=build/os.img conv=notrunc
	dd if=build/kernel.bin of=build/os.img bs=1 seek=4096 conv=notrunc

run:
	qemu-system-x86_64 -drive format=raw,file=build/os.img -m 4G -serial stdio -audiodev alsa,id=snd0 -machine pcspk-audiodev=snd0 -vga qxl -machine pc 

debug:
	qemu-system-x86_64 -drive format=raw,file=build/os.img -m 4G -serial stdio -audiodev alsa,id=snd0 -machine pcspk-audiodev=snd0 -vga qxl -s -S

clean:
	rm -rf build
