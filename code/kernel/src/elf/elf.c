#include "common.h"
#include "memory.h"
#include "string.h"

#include <elf.h>

#ifdef HAS_DEVICE_IDE
#define ELF_OFFSET_IN_DISK 0
#endif

#define STACK_SIZE (1 << 20)

void ide_read(uint8_t *, uint32_t, uint32_t);
void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {	
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph, *eph;

#ifdef HAS_DEVICE_IDE
	uint8_t buf[4096];
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
	elf = (void*)buf;
	Log("ELF loading from hard disk.");
#else
	elf = (void *)0x0;
	Log("ELF loading from ram disk.");
#endif

	/* Load each program segment */
	ph = (void *)elf + elf->e_phoff;
	eph = ph + elf->e_phnum;
	for(; ph < eph; ph ++) {
		if(ph->p_type == PT_LOAD) {
	

	//	BREAK_POINT;
	//		panic("Please implement the loader");

			/* TODO: copy the segment from the ELF file to its proper memory area */
	//		uint32_t k = 0;
	//		for(; k < ph->p_filesz; k++)
	//			Log("vaddr == %x    paddr == %x",ph->p_vaddr, pa_init);
				uint32_t pa_init = mm_malloc(ph->p_vaddr, ph->p_memsz);
	//			Log("vaddr == %x    paddr == %x",ph->p_vaddr, pa_init);
	//			BREAK_POINT;
#ifdef HAS_DEVICE_IDE
				ide_read((uint8_t *)pa_init, ph->p_offset, ph->p_filesz);
	//			memset((void*)pa_init,0,ph->p_memsz);
	//			Log("files = %d", ph->p_filesz);
#else
				memset((void*)pa_init,0,ph->p_memsz);
				memcpy((void*)pa_init,(void*)elf + ph->p_offset,ph->p_filesz);
#endif

				//memset((void*)ph->p_vaddr,0,ph->p_memsz);
				//memcpy((void*)ph->p_vaddr,(void*)ph->p_offset,ph->p_filesz);			

	//		/* TODO: zeror the memory area [vaddr + file_sz, vaddr + mem_sz) */
	//		uint8_t m = 0x0;
	//		for(uint32_t i = 0; i < ph->p_memsz - ph->p_filesz; i++)
	//			memcpy((void*)(ph->p_vaddr + ph->p_filesz + i),(void*)(&m),(uint32_t)(0x1));
       			
#ifdef IA32_PAGE
			/* Record the program break for future use */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
#endif
		}
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);
#ifdef HAS_DEVICE_VGA
	create_video_mapping();
#endif
	write_cr3(get_ucr3());
#endif
	return entry;
}
