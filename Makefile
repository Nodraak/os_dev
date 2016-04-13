
SUBDIR = descriptor_tables io lib x86 shell

_OBJECTS_DT = gdt.s.o idt.s.o pic.c.o
_OBJECTS_IO = buffer.c.o keyboard.c.o screen.c.o serial.c.o
_OBJECTS_LIB = printf.c.o string.c.o malloc.c.o read_elf.c.o
_OBJECTS_X86 = boot.s.o io.s.o paging.s.o
_OBJECTS_SHELL = shell.c.o
_OBJECTS_MAIN = kmain.c.o timer.c.o page_frame.c.o bitfield.c.o task.c.o switch_intel.s.o paging.c.o utils.s.o utils.c.o

_OBJECTS = $(addprefix descriptor_tables/, $(_OBJECTS_DT)) \
	 $(addprefix io/, $(_OBJECTS_IO)) \
	 $(addprefix lib/, $(_OBJECTS_LIB)) \
	 $(addprefix x86/, $(_OBJECTS_X86)) \
	 $(addprefix shell/, $(_OBJECTS_SHELL)) \
	 $(_OBJECTS_MAIN)
OBJECTS = $(addprefix obj/, $(_OBJECTS))

CC = /opt/cross_os/bin/i686-elf-gcc
CFLAGS = -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	-nostartfiles -nodefaultlibs -ffreestanding -lgcc -Wall -Wextra -g -c -I inc/

LD = /opt/cross_os/bin/i686-elf-ld
LDFLAGS = -T link.ld

# at&t : as (gnu as) / masm
# intel : nasm
AS = nasm
ASFLAGS = -f elf32

all: init os.iso

init:
	mkdir -p $(addprefix obj/, $(SUBDIR))

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

obj/%.c.o: src/%.c
	$(CC) $(CFLAGS) $< -o $@

obj/%.s.o: src/%.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf obj/* os.iso iso/boot/kernel.elf bochslog.txt serial.txt

re: clean all

