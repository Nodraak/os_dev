
#include "paging.c.h"
#include "paging.s.h"
#include "page_frame.h"
#include "types.h"
#include "printf.h"
#include "kmain.h"
#include "string.h"
#include "task.h"


void paging_map_frame_virtual_to_phys(void *virt, void *phys)
{
    uint32 pd_id = (uint32)virt >> 22;
    uint32 pt_id = ((uint32)virt >> 12) & 0x03FF;

    if ((uint32)virt % PAGE_SIZE)
        kpanic("Virtual address not aligned to PAGE_SIZE");
    if ((uint32)phys % PAGE_SIZE)
        kpanic("Physical address not aligned to PAGE_SIZE");

    /* page table doesnt exist yet, create a new one */
    if (!(kdata.page_directory[pd_id] & PF_FLAG_PRESENT))
    {
        uint32 *page_table = page_frame_alloc_addr(PAGE_SIZE);

        printf("\tnew page_table at %p\n", page_table);

        // todo : if no free page frame is in a mapped area, we are fucked. Use recursive mapping ?
        paging_map_frame_virtual_to_phys(page_table, page_table);

        /* init and map the page table */
        str_memset32(page_table, 1024, PF_FLAG_BASE);
        kdata.page_directory[pd_id] = (uint32)page_table | PF_FLAG_BASE | PF_FLAG_PRESENT;

        paging_reload_page_directory();
    }

    printf("\tmapping %p to %p (pd_id=%d / pt_id=%d)\n", virt, phys, pd_id, pt_id);

    uint32 *page_table = (uint32*)(kdata.page_directory[pd_id] & 0xFFFFF000);

    if (page_table[pt_id] & PF_FLAG_PRESENT)
    {
        printf("\tWarning : page_table_entry already mapped to %x (overwritting with %x)\n",
            page_table[pt_id] & 0xFFFFF000, (uint32)phys);
    }

    page_table[pt_id] = (uint32)phys | PF_FLAG_BASE | PF_FLAG_PRESENT;

    /* update paging */
    paging_reload_page_directory();
}


void paging_init(void)
{
    uint32 *page_table = NULL;
    uint8 *ptr = NULL;

    printf("Paging : init\n");

    /*
        page dir
    */
    /* new page_dir */
    kdata.page_directory = page_frame_alloc_addr(PAGE_SIZE);
    str_memset32(kdata.page_directory, 1024, PF_FLAG_BASE);

    kdata.page_directory[1023] = (uint32)kdata.page_directory | PF_FLAG_BASE | PF_FLAG_PRESENT;

    printf("\tpage_directory at %p\n", kdata.page_directory);

    /* init one new page_table for lower 4 MiB (lower 1 MiB + kernel + page dir + a few page tables) (pd_id == 0) */
    page_table = page_frame_alloc_addr(PAGE_SIZE);
    str_memset32(page_table, 1024, PF_FLAG_BASE);

    kdata.page_directory[0] = (uint32)page_table | PF_FLAG_BASE | PF_FLAG_PRESENT;

    /*
        map
    */

    /* map lower 1 MiB */
    printf("one meg\n");
    for (ptr = 0; ptr < (uint8*)0x100000; ptr += PAGE_SIZE)
        paging_map_frame_virtual_to_phys(ptr, ptr);

    /* map kernel */
    printf("kernel\n");
    for (ptr = kdata.kernel_start; ptr < kdata.kernel_end; ptr += PAGE_SIZE)
        paging_map_frame_virtual_to_phys(ptr, ptr);

    /* map page_dir + page_table */
    printf("page_dir + page_table\n");
    paging_map_frame_virtual_to_phys(kdata.page_directory, kdata.page_directory);
    paging_map_frame_virtual_to_phys(page_table, page_table);

    /*
        enable
    */
    printf("Paging : enabling ...\n");
    paging_write_cr3(kdata.page_directory);
    paging_write_cr0((uint32*)(paging_read_cr0() | (1 << 31)));
    printf(" ok\n");

    /*
        testing
    */

    printf("Paging: test\n");
    _paging_test_addr((char*)(2*1024*1024), (char*)(3*1024*1024));
    _paging_test_addr((char*)(30*1024*1024), (char*)(31*1024*1024));
}


void _paging_test_addr(char *ptr1, char *ptr2)
{
    printf("\tMapping %p->%p and %p->%p...\n", ptr1, ptr1, ptr2, ptr2);
    paging_map_frame_virtual_to_phys(ptr1, ptr1);
    paging_map_frame_virtual_to_phys(ptr2, ptr2);
    printf("\t-> ok\n");
    *ptr1 = 42, *ptr2 = 43;
    printf("\tCurrent value: %p=%d and %p=%d\n", ptr1, *ptr1, ptr2, *ptr2);
    printf("\tRemapping %p to %p...\n", ptr1, ptr2);
    paging_map_frame_virtual_to_phys(ptr1, ptr2);
    printf("\t-> ok\n");
    printf("\tnew value: %p=%d and %p=%d\n", ptr1, *ptr1, ptr2, *ptr2);
}
