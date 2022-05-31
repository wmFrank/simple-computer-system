#include "cpu/instr.h"

make_instr_func(lgdt)
{
	//assert(0);
	/*OPERAND rel1;
	rel1.type = OPR_IMM;
	rel1.sreg = SREG_CS;
	rel1.data_size = 16;
	rel1.addr = eip + 2;
	operand_read(&rel1);
	
	OPERAND rel2;
	rel2.type = OPR_IMM;
	rel2.sreg = SREG_CS;
	rel2.data_size = 32;
	rel2.addr = eip + 4;
	operand_read(&rel2);*/
	//printf("%x\n",cpu.eip);
	OPERAND rel;
	rel.data_size = data_size;
	int len = 1;
	len += modrm_rm(eip + 1, &rel);

	//uint32_t limit = ((rel1.val << 24) >> 16) + (rel2.val >> 24);
	//uint32_t base = ((rel1.val >> 8) << 24) + ((rel2.val << 8) >> 8);
	
	uint32_t limit = laddr_read(rel.addr, 2);
	uint32_t base = 0;
	if(data_size == 32)
		base = laddr_read(rel.addr + 2, 4);
	else if(data_size == 16)
		base = laddr_read(rel.addr + 2, 3);
	//uint32_t limit = ((num1 & 0xff) << 8) + ((num2 >> 24) & 0xff);
	//uint32_t base = ((num1 >> 8) << 24) + (num2 & 0xffffff);
	
	cpu.gdtr.limit = limit;
	cpu.gdtr.base = base;

	//printf(".............................\n");
	//printf("%d\n",len);
	return len;
}
