
OBJECTS = obj/loader.o obj/kmain.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

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
	ld $(LDFLAGS) $^ -o $@

run: os.iso
	bochs -f bochsrc.txt -q

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $< -o $@

obj/%.o: src/%.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf obj/* os.iso iso/boot/kernel.elf bochslog.txt

re: clean all

