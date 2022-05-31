#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "device/mm_io.h"
#include <memory.h>
#include <stdio.h>
#include "memory/cache.h"

uint8_t hw_mem[MEM_SIZE_B];

uint32_t hw_mem_read(paddr_t paddr, size_t len) {
	uint32_t ret = 0;
	memcpy(&ret, hw_mem + paddr, len);
	return ret;
}

void hw_mem_write(paddr_t paddr, size_t len, uint32_t data) {
	memcpy(hw_mem + paddr, &data, len);
}

/*uint32_t paddr_read(paddr_t paddr, size_t len) {
	uint32_t ret = 0;
	ret = hw_mem_read(paddr, len);
	return ret;
}*/

/*void paddr_write(paddr_t paddr, size_t len, uint32_t data) {
	hw_mem_write(paddr, len, data);
}*/

uint32_t paddr_read(paddr_t paddr, size_t len) {
	uint32_t ret = 0;
	int map_no = is_mmio(paddr);
	if(map_no == -1)
	{
	#ifdef CACHE_ENABLED
		ret = cache_read(paddr, len, cache_block);
	#else
		ret = hw_mem_read(paddr, len);
	#endif
	}
	else
	{
		ret = mmio_read(paddr, len, map_no);
	}
	return ret;
}

void paddr_write(paddr_t paddr, size_t len, uint32_t data) {
	int map_no = is_mmio(paddr);
	if(map_no == -1)
	{
	#ifdef CACHE_ENABLED
		cache_write(paddr, len, data, cache_block);
	#else
		hw_mem_write(paddr, len, data);
	#endif
	}
	else
	{
		mmio_write(paddr, len, data, map_no);
	}
}

/*uint32_t laddr_read(laddr_t laddr, size_t len) {
	return paddr_read(laddr, len);
}

void laddr_write(laddr_t laddr, size_t len, uint32_t data) {
	paddr_write(laddr, len, data);
}*/

uint32_t laddr_read(laddr_t laddr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	//printf("read pg == %x  pe == %x\n",cpu.cr0.pg, cpu.cr0.pe);
	if(cpu.cr0.pg == 1 && cpu.cr0.pe == 1) {
	//assert((laddr >>12) == ((laddr + len - 1) >> 12));
	//	printf("........................\n");
	//	if ((laddr>>12) != ((laddr + len) >> 12)) {
		if(0){
			/* this is a special case, you can handle it later. */
			assert(0);
		/*	printf("............................");
			size_t len1 = 0xfff - (laddr & 0xfff) + 1;
			size_t len2 = len - len1;
			printf("len1 == %x  len2 == %x\n", len1, len2);
			laddr_t laddr1 = laddr;
			laddr_t laddr2 = laddr + len1;
			if(len1 == 3)
				return laddr_read(laddr1, 1) + laddr_read(laddr1 + 1, 2) + laddr_read(laddr2, 1);
			else if(len2 == 3)
				return laddr_read(laddr1, 1) + laddr_read(laddr)
			return laddr_read(laddr1, len1) + laddr_read(laddr2, len2);*/
		}
		 else {
			paddr_t paddr = page_translate(laddr);
			return paddr_read(paddr, len);
		}
	} 
	else {
		 return paddr_read(laddr, len);
	}
}

void laddr_write(laddr_t laddr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	//printf("write pg == %x  pe == %x\n",cpu.cr0.pg, cpu.cr0.pe);
	if(cpu.cr0.pg == 1 && cpu.cr0.pe == 1) {
	//assert((laddr >>12) == ((laddr + len - 1) >> 12));
	//	printf("........................\n");
		if (0) {
			/* this is a special case, you can handle it later. */
			assert(0);
		}
		 else {
			paddr_t paddr = page_translate(laddr);
			paddr_write(paddr, len, data);
		}
	} 
	else {
		 paddr_write(laddr, len, data);
	}
}

/*uint32_t vaddr_read(vaddr_t vaddr, uint8_t sreg, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return laddr_read(vaddr, len);
}*/


/*void vaddr_write(vaddr_t vaddr, uint8_t sreg, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	laddr_write(vaddr, len, data);
}*/

uint32_t vaddr_read(vaddr_t vaddr, uint8_t sreg, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	#ifndef IA32_SEG
		return laddr_read(vaddr, len);
	#else
		uint32_t laddr = vaddr;
		if( cpu.cr0.pe == 1 ) {
//			printf("ddddddddddddddddddd\n");
			laddr = segment_translate(vaddr, sreg);
		}
		return laddr_read(laddr, len);
	#endif
}

void vaddr_write(vaddr_t vaddr, uint8_t sreg, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	#ifndef IA32_SEG
		laddr_write(vaddr, len, data);
	#else
		uint32_t laddr = vaddr;
		if( cpu.cr0.pe == 1 ) {
			laddr = segment_translate(vaddr, sreg);
		}
		laddr_write(laddr, len, data);
	#endif
}

void init_mem() {
	// clear the memory on initiation
	#ifdef CACHE_ENABLED
		init_cache();
	#endif
	memset(hw_mem, 0, MEM_SIZE_B);

#ifdef TLB_ENABLED
	//init_all_tlb();
	make_all_tlb();
	init_all_tlb();
#endif
}

uint32_t instr_fetch(vaddr_t vaddr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return vaddr_read(vaddr, SREG_CS, len);
}

uint8_t * get_mem_addr() {
	return hw_mem;
}
