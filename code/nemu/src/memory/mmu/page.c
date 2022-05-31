#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
/*paddr_t page_translate(laddr_t laddr) {
#ifndef TLB_ENABLED
	//printf("\nPlease implement page_translate()\n");
	//assert(0);
	//assert(laddr != 0);
	//printf("laddr == %x\n",laddr);
	//printf("cpu.cr3 == %x\n",cpu.cr3.val);
	uint32_t dir = (laddr>>22) & 0x3ff;
	uint32_t page = (laddr>>12) & 0x3ff;
	uint32_t offset = laddr & 0xfff;
	uint32_t paddr_pd = (cpu.cr3.pdbr << 12);
	//printf("cpu.cr3.pdbr == %x\n",cpu.cr3.pdbr);
	//printf("dir == %x, page == %x, offset == %x\n",dir,page,offset);
	//printf("paddr_pd == %x\n",paddr_pd);
	PDE pde;
	pde.val = paddr_read(paddr_pd + dir * 4, 4);
	//printf("pde.val = %x\n",pde.val);
	if(pde.present == 0)
		printf("cpu.eip == %x\n", cpu.eip);
	assert(pde.present == 1);
	//if(pde.present == 0)
		//printf("hhhhhhhhhhhhhhhhhhhhhhhhhh\n");
	//printf("hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n");
	uint32_t paddr_pt = (pde.page_frame << 12);
	PTE pte;
	pte.val = paddr_read(paddr_pt + page * 4, 4);
	assert(pte.present == 1);
	paddr_t paddr_ret = (pte.page_frame << 12) + offset;
	return paddr_ret;
	
#else	
	return tlb_read(laddr) | (laddr & PAGE_MASK);;
#endif
}*/

paddr_t page_translate(laddr_t laddr) {
#ifndef TLB_ENABLED
	//printf("\nPlease implement page_translate()\n");
	//assert(0);
	//assert(laddr != 0);
	//printf("laddr == %x\n",laddr);
	//printf("cpu.cr3 == %x\n",cpu.cr3.val);
	uint32_t dir = (laddr>>22) & 0x3ff;
	uint32_t page = (laddr>>12) & 0x3ff;
	uint32_t offset = laddr & 0xfff;
	uint32_t paddr_pd = (cpu.cr3.pdbr << 12);
	//printf("cpu.cr3.pdbr == %x\n",cpu.cr3.pdbr);
	//printf("dir == %x, page == %x, offset == %x\n",dir,page,offset);
	//printf("paddr_pd == %x\n",paddr_pd);
	PDE pde;
	pde.val = paddr_read(paddr_pd + dir * 4, 4);
	/*if(laddr == 0)
	{
		printf("pde.val == %x\n", pde.val);
	//	assert(0);
	}*/
	//printf("pde.val = %x\n",pde.val);
	if(pde.present == 0)
		printf("cpu.eip == %x\n", cpu.eip);
	assert(pde.present == 1);
	//if(pde.present == 0)
		//printf("hhhhhhhhhhhhhhhhhhhhhhhhhh\n");
	//printf("hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n");
	uint32_t paddr_pt = (pde.page_frame << 12);
	PTE pte;
	pte.val = paddr_read(paddr_pt + page * 4, 4);
	assert(pte.present == 1);
	paddr_t paddr_ret = (pte.page_frame << 12) + offset;
	return paddr_ret;
	
#else	
	return tlb_read(laddr) | (laddr & PAGE_MASK);;
#endif
}
