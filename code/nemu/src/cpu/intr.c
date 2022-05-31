#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"


void raise_intr(uint8_t intr_no) {
#ifdef IA32_INTR
	//printf("Please implement raise_intr()");
	//assert(0);*/
	OPERAND obj;
	obj.type = OPR_MEM;		
	obj.sreg = SREG_CS;
    obj.data_size = data_size;
	
	//push eflags
	cpu.esp -= 0x4;
	obj.addr = cpu.esp;
	obj.val = cpu.eflags.val; 
	operand_write(&obj);

//	printf("int eflags == %x\n", cpu.eflags.val);
	
	//push cs
	OPERAND assist;
	assist.type = OPR_MEM;		
	assist.sreg = SREG_CS;
    assist.data_size = 16;
	cpu.esp -= 0x2;
	assist.addr = cpu.esp;
	assist.val = cpu.cs.val; 
	operand_write(&assist);

//	printf("int cs == %x\n", cpu.cs.val);
	
	//push eip
	cpu.esp -= 0x4;
	obj.addr = cpu.esp;
	obj.val = cpu.eip; 
	operand_write(&obj);

//	printf("int eip == %x\n", cpu.eip);
	
	uint32_t addr = cpu.idtr.base + intr_no * 8;
	GateDesc gatedesc;
	gatedesc.val[0] = vaddr_read(addr, 0, 4);
	gatedesc.val[1] = vaddr_read(addr + 4, 0, 4);

	//IF = 0 if it is interrupt	
	//if(intr_no >= 32)
	uint32_t  intr_type = 0x0;
	intr_type |= ((gatedesc.val[1] >> 8 ) & 0xf);
	if(intr_type == 14)
		cpu.eflags.IF = 0;
	
	cpu.cs.val = gatedesc.selector;
	cpu.eip = (gatedesc.offset_15_0 & 0xffff) + ((gatedesc.offset_31_16 << 16) & 0xffff0000);
#endif
}

void raise_sw_intr(uint8_t intr_no) {
	// return address is the next instruction
	//printf("insteip:%x\n", cpu.eip);
	cpu.eip += 2;
	raise_intr(intr_no);
}
