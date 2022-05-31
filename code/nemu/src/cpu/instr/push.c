#include "cpu/instr.h"

static void instr_execute_1op() {
	operand_read(&opr_src);
//	opr_dest.val = opr_src.val;
//	operand_write(&opr_dest);
//	uint32_t value = opr_src.val;
//	cpu.gpr[4]._32 = alu_sub(0x4, cpu.gpr[4]._32);
	cpu.esp -= 0x4;
//	opr_src.addr = cpu.gpr[4]._32;
//	operand_write(&opr_src);
   	OPERAND obj;
	obj.type = OPR_MEM;		
	obj.sreg = SREG_CS;
    obj.data_size = data_size;
	obj.addr = cpu.esp;
	obj.val = opr_src.val; 
	operand_write(&obj);
}

make_instr_impl_1op(push,r,v)
make_instr_impl_1op(push,rm,v)
make_instr_impl_1op(push,i,b)
make_instr_impl_1op(push,i,v)

/*make_instr_func(push_i_b)
{
	cpu.esp -= 1;

	OPERAND obj;
	obj.type = OPR_MEM;
	obj.sreg = SREG_CS;
	obj.data_size = 8;
	obj.addr = cpu.esp;
	obj.val = (uint32_t)instr_fetch(eip + 1, 1);
	operand_write(&obj);
	
	return 2;
}*/
