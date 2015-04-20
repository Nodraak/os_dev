
OBJECTS = obj/boot.o obj/int_s.o obj/io.o obj/kmain.o obj/screen.o obj/int_c.o obj/keyboard.o
CC = /opt/cross_os/bin/i686-elf-gcc
CFLAGS = -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
-nostartfiles -nodefaultlibs -Wall -Wextra -c
LD = /opt/cross_os/bin/i686-elf-ld
LDFLAGS = -T link.ld
AS = nasm
ASFLAGS = -f elf32

all: os.iso

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
	bochs -f bochsrc.txt -q

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $< -o $@

obj/%.o: src/%.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf obj/* os.iso iso/boot/kernel.elf bochslog.txt

re: clean all

