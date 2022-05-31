#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg) {
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	//printf("%d\n",sreg);
	//printf("%d\n",cpu.segReg[sreg].limit);
	//sreg = 1;
	//assert(cpu.segReg[sreg].base == 0);
	//assert(cpu.segReg[sreg].limit == 0xfffff);
	uint32_t ret = 0;
	ret = cpu.segReg[sreg].base + offset;
	//printf("base = %x,  offset = %x\n",cpu.segReg[sreg].base,offset);
	//assert(offset != 0);
	return ret;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg) {
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	//sreg = 1;
	//printf("base == %x\n",cpu.gdtr.base);
	//printf("sreg == %x\n",sreg);
	//printf("index == %x\n", cpu.segReg[sreg].index);
	uint32_t addr = cpu.gdtr.base + sreg * 8;
	SegDesc desc;
	desc.val[0] = laddr_read(addr, 4);
	//printf("des0 == %x\n", desc.val[0]);
	//printf("des1 == %x\n", desc.val[1]);
	desc.val[1] = laddr_read(addr + 4, 4);

	cpu.segReg[sreg].base = desc.base_15_0 + (desc.base_23_16 << 16) + (desc.base_31_24 << 24);
	cpu.segReg[sreg].limit = desc.limit_15_0 + ((desc.limit_19_16 << 16) & 0xfffff);
	cpu.segReg[sreg].type = desc.type;
	cpu.segReg[sreg].privilege_level = desc.privilege_level;
	cpu.segReg[sreg].soft_use = desc.soft_use;
	
	/*uint32_t temp_val[2];
	temp_val[1] = paddr_read(addr, 4);
	temp_val[2] = paddr_read(addr+4,4);
	//printf("\ntemp_val:%x %x\n",temp_val[1],temp_val[2]);
	cpu.segReg[sreg].base = ((temp_val[1]>>16)&0x0000ffff) + ((temp_val[2]<<16)&0x00ff0000) + (temp_val[2]&0xff000000);
	cpu.segReg[sreg].limit = (temp_val[1]&0xffff) + (temp_val[2]&0xf0000);
	cpu.segReg[sreg].type = (temp_val[2]>>8)&0xf;
	cpu.segReg[sreg].privilege_level = (temp_val[2]>>13)&0x11;
	cpu.segReg[sreg].soft_use = (temp_val[2]>>20)&0x1;*/
	//printf("sreg = %d\n",sreg);
	//printf("limit = %x\n",cpu.segReg[sreg].limit);
	assert(cpu.segReg[sreg].base == 0);
	assert(cpu.segReg[sreg].limit == 0xfffff);
}
