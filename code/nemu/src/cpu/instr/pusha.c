#include "cpu/instr.h"

make_instr_func(pusha)
{
	uint32_t temp = cpu.esp;
	
	OPERAND obj;
	obj.type = OPR_MEM;		
	obj.sreg = SREG_CS;
    obj.data_size = data_size;

	//push eax
	cpu.esp -= 0x4;
	obj.addr = cpu.esp;
	obj.val = cpu.eax; 
	operand_write(&obj);

	//printf("push.c eax == %x\n", cpu.eax);
	
	//push ecx
	cpu.esp -= 0x4;
	obj.addr = cpu.esp;
	obj.val = cpu.ecx; 
	operand_write(&obj);
	
	//push edx
	cpu.esp -= 0x4;
	obj.addr = cpu.esp;
	obj.val = cpu.edx; 
	operand_write(&obj);

	//push ebx
	cpu.esp -= 0x4;
	obj.addr = cpu.esp;
	obj.val = cpu.ebx; 
	operand_write(&obj);

	//push esp
	cpu.esp -= 0x4;
	obj.addr = cpu.esp;
	obj.val = temp; 
	operand_write(&obj);
	
	//printf("push.c.c esp == %x\n", cpu.edi);

	//push ebp
	cpu.esp -= 0x4;
	obj.addr = cpu.esp;
	obj.val = cpu.ebp; 
	operand_write(&obj);
	
	//push esi
	cpu.esp -= 0x4;
	obj.addr = cpu.esp;
	obj.val = cpu.esi; 
	operand_write(&obj);

	//push edi
	cpu.esp -= 0x4;
	obj.addr = cpu.esp;
	obj.val = cpu.edi; 
	operand_write(&obj);

	//printf("push.c edi == %x\n", cpu.edi);	
	
	return 1;
}
