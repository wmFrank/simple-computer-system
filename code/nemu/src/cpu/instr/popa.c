#include "cpu/instr.h"

make_instr_func(popa)
{
	OPERAND obj;
	obj.type = OPR_MEM;
	obj.sreg = SREG_CS;
	obj.data_size = data_size;

	//pop edi
	obj.addr = cpu.esp;
	operand_read(&obj);
	cpu.edi = obj.val;
    cpu.esp += 4;

	//printf("pop.c edi == %x\n", cpu.edi);
	
	//pop esi
	obj.addr = cpu.esp;
	operand_read(&obj);
	cpu.esi = obj.val;
    cpu.esp += 4;

	//pop ebp
	obj.addr = cpu.esp;
	operand_read(&obj);
	cpu.ebp = obj.val;
    cpu.esp += 4;

	//pop esp
    cpu.esp += 4;

	//pop ebx
	obj.addr = cpu.esp;
	operand_read(&obj);
	cpu.ebx = obj.val;
    cpu.esp += 4;

	//pop edx
	obj.addr = cpu.esp;
	operand_read(&obj);
	cpu.edx = obj.val;
    cpu.esp += 4;

	//pop ecx
	obj.addr = cpu.esp;
	operand_read(&obj);
	cpu.ecx = obj.val;
    cpu.esp += 4;

	//pop eax
	obj.addr = cpu.esp;
	operand_read(&obj);
	cpu.eax = obj.val;
    cpu.esp += 4;

	//printf("pop.c eax == %x\n", cpu.eax);	
	
	return 1;
}
