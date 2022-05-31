#include "cpu/instr.h"

make_instr_func(call_disp32_v)
{
	int len = 1;
	uint32_t disp32 = (uint32_t)instr_fetch(eip+1,4);
	len = disp32 + 4 + len;
	cpu.gpr[4]._32 = alu_sub(0x4, cpu.gpr[4]._32);
	OPERAND obj;
	obj.type = OPR_MEM;
	obj.sreg = SREG_CS;
	obj.data_size = data_size;
	obj.addr = cpu.gpr[4]._32;
	obj.val = eip + 5;
	operand_write(&obj);
	return len;
}

make_instr_func(call_v)
{
	OPERAND src;
	src.data_size = data_size;
	
	int len = 1;
	len += modrm_rm(eip + 1, &src);
	
	operand_read(&src);
	cpu.eip = src.val;
	
	cpu.esp -= 0x4;
	OPERAND obj;
	obj.type = OPR_MEM;
	obj.sreg = SREG_CS;
	obj.data_size = data_size;
	obj.addr = cpu.esp;
	obj.val = eip + len;
	operand_write(&obj);
	
	return 0;
}

