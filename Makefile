CC = gcc
G++ = g++
LD = ld
CHMOD = chmod

all: build/join build/boot.bin build/loader.bin build/kernel.bin res/ch.font build/write_sector
	@echo Making kernel image...
	@build/join bin/af.img build/boot.bin build/loader.bin build/kernel.bin

	@echo Adding resources...
	@build/write_sector bin/af.img res/login_1.bmp 2 0 0
	@build/write_sector bin/af.img res/login_2.bmp 5 1 0
	@build/write_sector bin/af.img res/normal_cursor.bmp 9 0 0
	@build/write_sector bin/af.img res/down_cursor.bmp 9 0 8
	@build/write_sector bin/af.img res/hold_down_cursor.bmp 9 0 16
	@build/write_sector bin/af.img res/af.bmp 11 1 0
	@build/write_sector bin/af.img res/logout.bmp 13 1 0
	@build/write_sector bin/af.img res/titlebar.bmp 16 1 0
	@build/write_sector bin/af.img res/ch.font 20 0 0
	@build/write_sector bin/af.img res/asc16 20 0 1

	@echo Creating ISO file...
	@touch bin/af.iso
	@rm bin/af.iso
	@mkisofs -R -V "A-Framework-IA32" -o bin/af.iso -b af.img bin

# asm src
build/boot.bin: src/boot.asm
	@echo Compiling boot.bin...
	@nasm $^ -o $@
	
build/loader.bin: src/loader.asm
	@echo Compiling loader.bin...
	@nasm $^ -o $@

build/kernel.asmo: src/kernel.asm
	@echo Compiling kernel.asmo...
	@nasm -f elf $^ -o $@

build/interrupt.asmo: src/interrupt.asm
	@echo Compiling interrupt.asmo...
	@nasm -f elf $^ -o $@

build/mouse.asmo: src/mouse.asm
	@echo Compiling mouse.asmo...
	@nasm -f elf $^ -o $@

build/keyboard.asmo: src/keyboard.asm
	@echo Compiling keyboard.asmo...
	@nasm -f elf $^ -o $@

# C src
build/kernel.o: src/kernel.c
	@echo Compiling kernel.o with gcc...
	@$(CC) -c -fno-stack-protector -fno-builtin -fpack-struct -std=c99 -c $^ -o $@

build/system.o: src/system.c
	@echo Compiling system.o with gcc...
	@$(CC) -fpack-struct -std=c99 -c $^ -o $@
	
build/vesa.o: src/vesa.c
	@echo Compiling vesa.o with gcc...
	@$(CC) -fpack-struct -std=c99 -c $^ -o $@

build/graphic.o: src/graphic.c
	@echo Compiling graphic.o with gcc...
	@$(CC) -fpack-struct -std=c99 -c $^ -o $@

build/io.o: src/io.c
	@echo Compiling io.o with gcc...
	@$(CC) -fpack-struct -std=c99 -c $^ -o $@

build/interrupt.o: src/interrupt.c
	@echo Compiling interrupt.o with gcc...
	@$(CC) -fpack-struct -std=c99 -c $^ -o $@

build/mouse.o: src/mouse.c
	@echo Compiling mouse.o with gcc...
	@$(CC) -fpack-struct -std=c99 -c $^ -o $@

build/keyboard.o: src/keyboard.c
	@echo Compiling keyboard.o with gcc...
	@$(CC) -fpack-struct -std=c99 -c $^ -o $@

build/message.o: src/message.c
	@echo Compiling message.o with gcc...
	@$(CC) -fpack-struct -std=c99 -c $^ -o $@

build/ata.o: src/ata.c
	@echo Compiling ata.o with gcc...
	@$(CC) -fpack-struct -std=c99 -c $^ -o $@

# kernel
build/kernel.ld: build/kernel.asmo build/kernel.o build/system.o build/vesa.o build/io.o build/interrupt.o build/interrupt.asmo build/keyboard.o build/keyboard.asmo build/message.o build/mouse.o build/mouse.asmo build/graphic.o build/ata.o
	@echo Linking kernel...
	@$(LD) -o $@ -Ttext 0x90000 -e main $^

build/kernel.bin: build/kernel.ld
	@echo Removing extra sections added by 'ld -o'... 
	@objcopy -R .note -R .comment -S -O binary $^ $@

# utils
build/join: util/join.cpp
	@echo Compiling join...
	@$(G++) $^ -o $@
	@$(CHMOD) 755 build/join

build/write_sector: util/write_sector.cpp
	@echo Compiling write_sector...
	@$(G++) $^ -o $@
	@$(CHMOD) 755 build/write_sector

build/make_font: util/make_font.cpp
	@echo Compiling make_font...
	@$(G++) $^ -o $@
	@$(CHMOD) 755 build/make_font

# res font
res/ch.font: build/make_font
	build/make_font res/hzk16 res/ch.font

clean:
	rm bin -r
	rm build -r
	mkdir bin
	mkdir build

