
My very own x86 operating system, just for fun.

# Features

Currently implemented:

* Multiboot bootloader (using Grub)
* Screen driver
* Serial driver (for debug)
* Global Descriptor Table (GDT)
* Memory segmentation / virtual memory / paging
* Interrupts: IDT, PIC, Interrupts Handlers
* Clock (through interrupts)
* Keyboard driver (through interrupts, with a basic shell)
* Multitasking
* Basic shell

On the todo list:

* Better shell
* Better keyboard driver
* Filesystem
* User mode

# Ressources

Must read

* http://wiki.osdev.org/Main_Page
* https://littleosbook.github.io/
* http://www.osdever.net/tutorials/

Might be usefull

* http://www.brokenthorn.com/
* https://github.com/levex/osdev
* http://www.geezer.osdevbrasil.net/osd/index.htm
* http://osdev.trvx.org/
* http://www.cs.rutgers.edu/~pxk/416/notes/
* http://fogus.me/thunks/osdev.html
* https://github.com/mit-pdos/xv6-public

Doc

* [Asm (wikibooks)](https://en.wikibooks.org/wiki/X86_Assembly)
* [Asm (opcodes)](http://ref.x86asm.net/)
* [Asm (datatypes)](http://www.c-jump.com/CIS77/ASM/DataTypes/lecture.html)
* [Nasm](http://www.posix.nl/linuxassembly/nasmdochtml/nasmdoca.html)
* [multiboot](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
* [BIOS interrupt call](https://en.wikipedia.org/wiki/BIOS_interrupt_call)
* [Interrupts](http://www.ousob.com/ng/asm/ng1f806.php)
* [ELF](http://refspecs.linuxbase.org/elf/elf.pdf)
* [Syscalls](http://docs.cs.up.ac.za/programming/asm/derick_tut/syscalls.html)

# License

[WTFPL](http://www.wtfpl.net/)
