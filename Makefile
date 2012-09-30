make: build/join build/boot.bin build/loader.bin build/kernel.bin res/ch.font build/write_sector
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

build/boot.bin: src/boot.asm
	@echo Compiling boot.bin...
	@nasm src/boot.asm -o build/boot.bin
	
build/loader.bin: src/loader.asm
	@echo Compiling loader.bin...
	@nasm src/loader.asm -o build/loader.bin

build/kernel.asmo: src/kernel.asm
	@echo Compiling kernel.asmo...
	@nasm -f elf src/kernel.asm -o build/kernel.asmo

build/interrupt.asmo: src/interrupt.asm
	@echo Compiling interrupt.asmo...
	@nasm -f elf src/interrupt.asm -o build/interrupt.asmo

build/mouse.asmo: src/mouse.asm
	@echo Compiling mouse.asmo...
	@nasm -f elf src/mouse.asm -o build/mouse.asmo

build/keyboard.asmo: src/keyboard.asm
	@echo Compiling keyboard.asmo...
	@nasm -f elf src/keyboard.asm -o build/keyboard.asmo

build/kernel.o: src/kernel.c
	@echo Compiling kernel.o with gcc...
	@gcc -c -fno-stack-protector -fno-builtin -fpack-struct -std=c99 -c src/kernel.c -o build/kernel.o 

build/system.o: src/system.c
	@echo Compiling system.o with gcc...
	@gcc -fpack-struct -std=c99 -c src/system.c -o build/system.o
	
build/vesa.o: src/vesa.c
	@echo Compiling vesa.o with gcc...
	@gcc -fpack-struct -std=c99 -c src/vesa.c -o build/vesa.o

build/graphic.o: src/graphic.c
	@echo Compiling graphic.o with gcc...
	@gcc -fpack-struct -std=c99 -c src/graphic.c -o build/graphic.o

build/io.o: src/io.c
	@echo Compiling io.o with gcc...
	@gcc -fpack-struct -std=c99 -c src/io.c -o build/io.o

build/interrupt.o: src/interrupt.c
	@echo Compiling interrupt.o with gcc...
	@gcc -fpack-struct -std=c99 -c src/interrupt.c -o build/interrupt.o

build/mouse.o: src/mouse.c
	@echo Compiling mouse.o with gcc...
	@gcc -fpack-struct -std=c99 -c src/mouse.c -o build/mouse.o

build/keyboard.o: src/keyboard.c
	@echo Compiling keyboard.o with gcc...
	@gcc -fpack-struct -std=c99 -c src/keyboard.c -o build/keyboard.o

build/message.o: src/message.c
	@echo Compiling message.o with gcc...
	@gcc -fpack-struct -std=c99 -c src/message.c -o build/message.o

build/ata.o: src/ata.c
	@echo Compiling ata.o with gcc...
	@gcc -fpack-struct -std=c99 -c src/ata.c -o build/ata.o

build/kernel.ld: build/kernel.asmo build/kernel.o build/system.o build/vesa.o build/io.o build/interrupt.o build/interrupt.asmo build/keyboard.o build/keyboard.asmo build/message.o build/mouse.o build/mouse.asmo build/graphic.o build/ata.o
	@echo Linking kernel...
	@ld -o build/kernel.ld -Ttext 0x90000 -e main build/kernel.asmo build/kernel.o build/system.o build/vesa.o build/io.o build/interrupt.o build/interrupt.asmo build/keyboard.o build/keyboard.asmo build/message.o build/mouse.o build/mouse.asmo build/graphic.o  build/ata.o

build/kernel.bin: build/kernel.ld
	@echo Removing extra sections added by 'ld -o'... 
	@objcopy -R .note -R .comment -S -O binary build/kernel.ld build/kernel.bin
	
build/join: util/join.cpp
	@echo Compiling join...
	@g++ util/join.cpp -o build/join
	@chmod 755 build/join

build/write_sector: util/write_sector.cpp
	@echo Compiling write_sector...
	@g++ util/write_sector.cpp -o build/write_sector
	@chmod 755 build/write_sector

build/make_font: util/make_font.cpp
	@echo Compiling make_font...
	@g++ util/make_font.cpp -o build/make_font
	@chmod 755 build/make_font

res/ch.font: build/make_font
	build/make_font res/hzk16 res/ch.font
	
clean:
	-rm bin -r
	-rm build -r
	-mkdir bin
	-mkdir build