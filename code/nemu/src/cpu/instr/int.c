#include "cpu/instr.h"
//#include "cpu/intr.h"

make_instr_func(int_b)
{
	/*uint32_t imm8 = (uint32_t)instr_fetch(eip + 1, 1);
	uint32_t addr = cpu.idtr.base + imm8 * 8;
	GateDesc gatedesc;
	gatedesc.val[0] = laddr_read(addr, 4);
	//printf("des0 == %x\n", desc.val[0]);
	//printf("des1 == %x\n", desc.val[1]);
	gatedesc.val[1] = laddr_read(addr + 4, 4);

	uint32_t sreg = (gatedesc.selector >> 3) & 0x1fff;
    uint32_t vaddr = (gatedesc.offset_15_0 & 0xffff) + ((gatedesc.offset_31_16 << 16) & 0xffff0000);
	uint32_t laddr = segment_translate(vaddr, sreg);
	uint32_t paddr = page_translate(laddr);
	
	cpu.eip = paddr;
	printf("cpu.eip == %x\n", cpu.eip);	
	
	return 0;*/

//	printf("eip ====== %x\n",cpu.eip);
	uint8_t imm8 = (uint8_t)instr_fetch(eip + 1, 1);
//	printf("imm8 == %x\n",imm8);
	raise_sw_intr(imm8);
	return 0;
}
