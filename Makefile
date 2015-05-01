
SUBDIR = descriptor_tables io x86 shell

_OBJECTS_DT = gdt.o idt.o pic.o
_OBJECTS_IO = buffer.o keyboard.o printf.o screen.o serial.o
_OBJECTS_X86 = boot.o io.o
_OBJECTS_SHELL = shell.o
_OBJECTS_MAIN = kmain.o string.o paging_low.o

_OBJECTS = $(addprefix descriptor_tables/, $(_OBJECTS_DT)) $(addprefix io/, $(_OBJECTS_IO)) $(addprefix x86/, $(_OBJECTS_X86)) $(addprefix shell/, $(_OBJECTS_SHELL)) $(_OBJECTS_MAIN)
OBJECTS = $(addprefix obj/, $(_OBJECTS))

CC = /opt/cross_os/bin/i686-elf-gcc
CFLAGS = -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	-nostartfiles -nodefaultlibs -Wall -Wextra -c $(addprefix -I inc/ -I inc/, $(SUBDIR))

LD = /opt/cross_os/bin/i686-elf-ld
LDFLAGS = -T link.ld

AS = nasm
ASFLAGS = -f elf32

all: init os.iso

init:
	mkdir -p obj
	mkdir -p obj/descriptor_tables
	mkdir -p obj/io
	mkdir -p obj/x86
	mkdir -p obj/shell

os.iso: obj/kernel.elf
	cp obj/kernel.elf iso/boot/kernel.elf
	genisoimage -R \
		-b boot/grub/stage2_eltorito \
		-no-emul-boot \
		-boot-load-size 4 \
		-A os \
		-input-charset utf8 \
		-quiet \
		-boot-info-table \
		-o os.iso \
		iso

obj/kernel.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@

run: os.iso
	rm -f serial.txt
	bochs -f bochsrc.txt

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $< -o $@

obj/%.o: src/%.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf obj/* os.iso iso/boot/kernel.elf bochslog.txt serial.txt

re: clean all

