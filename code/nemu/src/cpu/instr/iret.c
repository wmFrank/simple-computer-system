#include "cpu/instr.h"

make_instr_func(iret)
{
	OPERAND obj;
	obj.type = OPR_MEM;
	obj.sreg = SREG_CS;
	obj.data_size = data_size;

	OPERAND assist;
	assist.type = OPR_MEM;		
	assist.sreg = SREG_CS;
    assist.data_size = 16;

	//pop eip
	obj.addr = cpu.esp;
	operand_read(&obj);
	cpu.eip = obj.val;
    cpu.esp += 4;

//	printf("iret eip == %x\n", cpu.eip);

	//pop cs
	assist.addr = cpu.esp;
	operand_read(&assist);
	cpu.cs.val = assist.val;
	cpu.esp += 2;

//	printf("iret cs == %x\n", cpu.cs.val);	

	//pop eflags
	obj.addr = cpu.esp;
	operand_read(&obj);
	cpu.eflags.val = obj.val;
    cpu.esp += 4;
	
//	printf("iret eflags == %x\n", cpu.eflags.val);

/*	//pop esp
	obj.addr = cpu.esp;
	operand_read(&obj);
	cpu.esp = obj.val;
    cpu.esp += 4;

	//pop ss
	assist.addr = cpu.esp;
	operand_read(&assist);
	cpu.ss.val = assist.val;
	cpu.esp += 2;*/

	return 0;
}
