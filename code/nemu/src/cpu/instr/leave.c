#include "cpu/instr.h"

make_instr_func(leave)
{
/*	OPERAND r, rm;
	rm.data_size = r.data_size = data_size;
	int len = 1;
	r.type = OPR_MEM;
	r.addr = cpu.gpr[4]._32;
	rm.type = OPR_MEM;
	rm.addr = cpu.gpr[5]._32;
	cpu.gpr[5]._32 = r.addr;*/
	cpu.gpr[4]._32 = cpu.gpr[5]._32;

/*	operand_read(&r);
	rm.val = r.val;
	operand_write(&rm);*/
	
	OPERAND obj;
	obj.type = OPR_MEM;
	obj.sreg = SREG_CS;
	obj.data_size = data_size;
	obj.addr = cpu.gpr[4]._32;
	operand_read(&obj);
	cpu.gpr[5]._32 = obj.val;
	
	cpu.gpr[4]._32 = alu_add(0x4, cpu.gpr[4]._32);
	
	int len = 1;
	return len;
}
	
