.PHONY: run clean

CC = gcc
CCFLAGS = -march=native -O3
BUILD = build

make: $(BUILD)/built

$(BUILD)/built: main.c $(BUILD)
	$(CC) $(CCFLAGS) $< -o $@

$(BUILD):
	mkdir -p $(BUILD)
	mkdir -p fs
	mkdir -p fs/dir
	touch fs/file
	touch fs/dir/insidefile

clean: $(BUILD)
	rm -rf $(BUILD)

run: $(BUILD)/built $(BUILD)
	./$(BUILD)/built
	xxd output.bin
