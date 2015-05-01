#ifndef MULTIBOOT_HEADER_INCLUDED
#define MULTIBOOT_HEADER_INCLUDED

/* How many bytes from the start of the file we search for the header. */
#define MULTIBOOT_SEARCH                        0x2000
/* The magic field should contain this. */
#define MULTIBOOT_HEADER_MAGIC                  0x1BADB002
/* The bits in the required part of flags field we don't support. */
#define MULTIBOOT_UNSUPPORTED                   0x0000fffc
/* Alignment of multiboot modules. */
#define MULTIBOOT_MOD_ALIGN                     0x00001000
/* Alignment of the multiboot info structure. */
#define MULTIBOOT_INFO_ALIGN                    0x00000004

/* FLAGS set in the 'flags' member of the MULTIBOOT HEADER. */

/* Align all boot modules on i386 page (4KB) boundaries. */
#define MULTIBOOT_PAGE_ALIGN                    0x00000001
/* Must pass memory information to OS. */
#define MULTIBOOT_MEMORY_INFO                   0x00000002
/* Must pass video information to OS. */
#define MULTIBOOT_VIDEO_MODE                    0x00000004
/* This flag indicates the use of the address fields in the header. */
#define MULTIBOOT_AOUT_KLUDGE                   0x00010000

/* FLAGS to be set in the 'flags' member of the MULTIBOOT INFO STRUCTURE. */

/* is there basic lower/upper memory information ? */
#define MULTIBOOT_INFO_MEMORY                   0x00000001
/* is there a boot device set ? */
#define MULTIBOOT_INFO_BOOTDEV                  0x00000002
/* is the command-line defined ? */
#define MULTIBOOT_INFO_CMDLINE                  0x00000004
/* are there modules to do something with ? */
#define MULTIBOOT_INFO_MODS                     0x00000008

/* These next two are mutually exclusive */

/* is there a symbol table loaded ? */
#define MULTIBOOT_INFO_AOUT_SYMS                0x00000010
/* is there an ELF section header table ? */
#define MULTIBOOT_INFO_ELF_SHDR                 0X00000020

/* is there a full memory map ? */
#define MULTIBOOT_INFO_MEM_MAP                  0x00000040
/* Is there drive info ? */
#define MULTIBOOT_INFO_DRIVE_INFO               0x00000080
/* Is there a config table ? */
#define MULTIBOOT_INFO_CONFIG_TABLE             0x00000100
/* Is there a boot loader name ? */
#define MULTIBOOT_INFO_BOOT_LOADER_NAME         0x00000200
/* Is there a APM table ? */
#define MULTIBOOT_INFO_APM_TABLE                0x00000400
/* Is there video information ? */
#define MULTIBOOT_INFO_VIDEO_INFO               0x00000800

typedef unsigned short          multiboot_uint16_t;
typedef unsigned int            multiboot_uint32_t;

#if 0
struct multiboot_header
{
    /* Must be MULTIBOOT_MAGIC - see above. */
    multiboot_uint32_t magic;

    /* Feature flags. */
    multiboot_uint32_t flags;

    /* The above fields plus this one must equal 0 mod 2^32. */
    multiboot_uint32_t checksum;

    /* These are only valid if MULTIBOOT_AOUT_KLUDGE is set. */
    multiboot_uint32_t header_addr;
    multiboot_uint32_t load_addr;
    multiboot_uint32_t load_end_addr;
    multiboot_uint32_t bss_end_addr;
    multiboot_uint32_t entry_addr;

    /* These are only valid if MULTIBOOT_VIDEO_MODE is set. */
    multiboot_uint32_t mode_type;
    multiboot_uint32_t width;
    multiboot_uint32_t height;
    multiboot_uint32_t depth;
};
#endif

/* The symbol table for a.out. */
typedef struct multiboot_aout_symbol_table
{
    multiboot_uint32_t tabsize;
    multiboot_uint32_t strsize;
    multiboot_uint32_t addr;
    multiboot_uint32_t reserved;
} multiboot_aout_symbol_table_t;

/* The section header table for ELF. */
typedef struct multiboot_elf_section_header_table
{
    multiboot_uint32_t num;
    multiboot_uint32_t size;
    multiboot_uint32_t addr;
    multiboot_uint32_t shndx;
} multiboot_elf_section_header_table_t;

typedef struct multiboot_info
{
    /* Multiboot info version number */
    multiboot_uint32_t flags;

    /* Available memory from BIOS */
    multiboot_uint32_t mem_lower;
    multiboot_uint32_t mem_upper;

    /* "root" partition */
    multiboot_uint32_t boot_device;

    /* Kernel command line */
    multiboot_uint32_t cmdline;

    /* Boot-Module list */
    multiboot_uint32_t mods_count;
    multiboot_uint32_t mods_addr;

    union
    {
        multiboot_aout_symbol_table_t aout_sym;
        multiboot_elf_section_header_table_t elf_sec;
    };

    /* Memory Mapping buffer */
    multiboot_uint32_t mmap_length;
    multiboot_uint32_t mmap_addr;

    /* Drive Info buffer */
    multiboot_uint32_t drives_length;
    multiboot_uint32_t drives_addr;

    /* ROM configuration table */
    multiboot_uint32_t config_table;

    /* Boot Loader Name */
    multiboot_uint32_t boot_loader_name;

    /* APM table */
    multiboot_uint32_t apm_table;

    /* Video */
    multiboot_uint32_t vbe_control_info;
    multiboot_uint32_t vbe_mode_info;
    multiboot_uint16_t vbe_mode;
    multiboot_uint16_t vbe_interface_seg;
    multiboot_uint16_t vbe_interface_off;
    multiboot_uint16_t vbe_interface_len;
} multiboot_info_t;

#define MB_MMAP_TYPE_AVAILABLE 1
typedef struct multiboot_mmap_entry
{
    multiboot_uint32_t size;
    multiboot_uint32_t addr_low;
    multiboot_uint32_t addr_high;
    multiboot_uint32_t len_low;
    multiboot_uint32_t len_high;
    multiboot_uint32_t type;
} __attribute__((packed)) multiboot_memory_map_t;

#if 0
typedef struct multiboot_mod_list
{
    /* the memory used goes from bytes 'mod_start' to 'mod_end-1' inclusive */
    multiboot_uint32_t mod_start;
    multiboot_uint32_t mod_end;

    /* Module command line */
    multiboot_uint32_t cmdline;

    /* padding to take it to 16 bytes (must be zero) */
    multiboot_uint32_t pad;
} multiboot_module_t;
#endif

#endif
