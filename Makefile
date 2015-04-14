
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

obj/kernel.elf: obj/loader.o
	ld $(LDFLAGS) $< -o $@

run: os.iso
	bochs -f bochsrc.txt -q

obj/loader.o: src/loader.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf obj/* os.iso iso/boot/kernel.elf bochslog.txt

re: clean all

