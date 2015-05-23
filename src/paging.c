
#include "paging.h"
#include "paging_att.h"
#include "page_frame.h"
#include "types.h"
#include "printf.h"
#include "kmain.h"


void paging_map_frame_virtual_to_phys(uint32 *page_directory, void *virt, void *phys, bool reload)
{
    uint32 *page_table = NULL;
    uint32 pd_id = (uint32)virt >> 22;
    uint32 pt_id = ((uint32)virt >> 12) & 0x03FF;
    uint32 phys_rounded = (uint32)phys & 0xFFFFF000;

    printf("\tmapping %p to %p (pd_id=%d / pt_id=%d)\n", virt, phys, pd_id, pt_id);

    /* page table */
    if (page_directory[pd_id] & 0xFFFFF000)
    {
        page_table = (uint32*)(page_directory[pd_id] & 0xFFFFF000);
    }
    else
    {
        uint32 i;

        page_table = page_frame_alloc_addr(PAGE_SIZE);
        printf("\tnew page_table at %p\n", page_table);

        for (i = 0; i < 1024; i++)
            page_table[i] = PF_FLAG_BASE;
    }

    /* register page frame */
    page_directory[pd_id] = (uint32)page_table | PF_FLAG_BASE | PF_FLAG_PRESENT;
    page_table[pt_id] = phys_rounded | PF_FLAG_BASE | PF_FLAG_PRESENT;
    page_frame_set_addr((uint8*)phys_rounded, 1);

    /* update cache */
    if (reload)
        paging_reload_page_directory();
}


void paging_init(void)
{
    uint32 *page_directory = NULL;
    uint8 *ptr = NULL;
    uint32 i;

    printf("Setting up paging :\n");

    /*
        page dir
    */
    page_directory = page_frame_alloc_addr(PAGE_SIZE);

    for (i = 0; i < 1024; i++)
        page_directory[i] = PF_FLAG_BASE;

    printf("\tpage_directory at %p\n", page_directory);

    /*
        page_tables
    */

    /* map lower 1 MiB */
    uint8 *one_meg = (uint8*)0x100000;
    for (ptr = 0; ptr < one_meg; ptr += PAGE_SIZE)
        paging_map_frame_virtual_to_phys(page_directory, ptr, ptr, 0);

    /* map kernel */
    for (ptr = kdata.kernel_start; ptr < kdata.kernel_end; ptr += PAGE_SIZE)
        paging_map_frame_virtual_to_phys(page_directory, ptr, ptr, 0);

    /* pd / pt */
    for (ptr = (uint8*)page_directory; ptr <= (uint8*)page_directory+2*PAGE_SIZE; ptr += PAGE_SIZE)
        paging_map_frame_virtual_to_phys(page_directory, ptr, ptr, 0);

    /*
        enable
    */

    paging_load_and_enable_page_directory(page_directory);

    printf("Paging was successfully enabled !\n");
}
