#include "cpu/instr.h"

make_instr_func(lidt)
{
	OPERAND rel;
	rel.data_size = data_size;
	int len = 1;
	len += modrm_rm(eip + 1, &rel);

	uint32_t limit = laddr_read(rel.addr, 2);
	uint32_t base = 0;
	if(data_size == 32)
		base = laddr_read(rel.addr + 2, 4);
	else if(data_size == 16)
		base = laddr_read(rel.addr + 2, 3);
	
	cpu.idtr.limit = limit;
	cpu.idtr.base = base;
	
	//printf("idtr.base == %x\n", cpu.idtr.base);

	return len;
}
