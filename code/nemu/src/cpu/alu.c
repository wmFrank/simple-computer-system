#include "cpu/cpu.h"

void set_CF_add(uint32_t result, uint32_t src){
	cpu.eflags.CF = result < src;
}

void set_CF_adc(uint32_t result, uint32_t src, uint32_t dest){
	if(result == src + dest)
		cpu.eflags.CF = result < src;
	else
		cpu.eflags.CF = result <= src;
}

void set_CF_sub(uint32_t src, uint32_t dest){
	cpu.eflags.CF = dest < src;
}

void set_CF_sbb(uint32_t result, uint32_t src, uint32_t dest){
	if(result == dest - src)
		cpu.eflags.CF = dest < src;
	else
		cpu.eflags.CF = dest <= src;
}

void set_CF_zero(){
	cpu.eflags.CF = 0;
}


void set_OF_add(uint32_t result, uint32_t src, uint32_t dest){
	if(sign(src) == sign(dest)){
		if(sign(src) != sign(result))
			cpu.eflags.OF = 1;
		else
			cpu.eflags.OF = 0;
	}else{
		cpu.eflags.OF = 0;
	}
}

void set_OF_sub(uint32_t result, uint32_t src, uint32_t dest){
	if(sign(src) != sign(dest)){
		if(sign(src) == sign(result))
			cpu.eflags.OF = 1;
		else
			cpu.eflags.OF = 0;
	}else{
		cpu.eflags.OF = 0;
	}
}

void set_OF_zero(){
	cpu.eflags.OF = 0;
}

void set_PF(uint32_t result){
	uint32_t counter = 0;
	uint32_t num = result;
	for(uint32_t i = 0; i < 8; i++,num = num >> 1)
	{
		if((num & 0x1)==1)
			counter++;
	}
	if(counter%2 == 1)
		cpu.eflags.PF = 0;
	else
		cpu.eflags.PF = 1;
}

void set_ZF(uint32_t result){
	cpu.eflags.ZF = (result == 0);
}

void set_SF(uint32_t result){
	cpu.eflags.SF = sign(result);
}



uint32_t alu_add(uint32_t src, uint32_t dest) {
        uint32_t res = 0;
	res = dest + src;

	set_CF_add(res,src);
	set_PF(res);
	set_ZF(res);
	set_SF(res);
	set_OF_add(res,src,dest);

	return res;
}

uint32_t alu_adc(uint32_t src, uint32_t dest) {
	uint32_t res = 0;
	res = dest + src + cpu.eflags.CF;

	set_CF_adc(res,src,dest);
	set_PF(res);
	set_ZF(res);
	set_SF(res);
	set_OF_add(res,src,dest);

	return res;
}


uint32_t alu_sub(uint32_t src, uint32_t dest) {
	uint32_t res = 0;
	res = dest - src;

	set_CF_sub(src,dest);
	set_PF(res);
	set_ZF(res);
	set_SF(res);
	set_OF_sub(res,src,dest);

	return res;
}

uint32_t alu_sbb(uint32_t src, uint32_t dest) {
	uint32_t res = 0;
	res = dest - (src + cpu.eflags.CF);

	set_CF_sbb(res,src,dest);
	set_PF(res);
	set_ZF(res);
	set_SF(res);
	set_OF_sub(res,src,dest);

	return res;
}


uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size) {
	uint64_t res = 0;
	uint64_t n = 64 - data_size;
	uint64_t des = dest;
	uint64_t sr = src;
	if(data_size < 32){
		uint64_t y = (des<<n)>>n;
		uint64_t z = (sr<<n)>>n;
		uint64_t e = y * z;
	        res = e;
   		if((e<<(64 - 2 * data_size))>>n == 0)
		{
			cpu.eflags.CF = 0;
			cpu.eflags.OF = 0;
		}else{
			cpu.eflags.CF = 1;
			cpu.eflags.OF = 1;
		}
	}else{
		res = des * sr;
		if((res<<(64 - 2 * data_size))>>n == 0)
		{
			cpu.eflags.CF = 0;
			cpu.eflags.OF = 0;
		}else{
			cpu.eflags.CF = 1;
			cpu.eflags.OF = 1;
		}
	}

	return res;
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size) {
	int64_t res = 0;
	int64_t n = 64 - data_size;
	int64_t des = dest;
	int64_t sr = src;
	if(data_size < 32){
		int64_t y = (des<<n)>>n;
		int64_t z = (sr<<n)>>n;
		int64_t e = y * z;
	        res = e;
   		if((e<<(64 - 2 * data_size))>>n == 0)
		{
			cpu.eflags.CF = 0;
			cpu.eflags.OF = 0;
		}else{
			cpu.eflags.CF = 1;
			cpu.eflags.OF = 1;
		}
	}else{
		res = des * sr;
		if((res<<(64 - 2 * data_size))>>n == 0)
		{
			cpu.eflags.CF = 0;
			cpu.eflags.OF = 0;
		}else{
			cpu.eflags.CF = 1;
			cpu.eflags.OF = 1;
		}
	}
	
	return res;
}

uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size) {
	uint32_t res = 0;
	res = dest/src;

	return res;
}

int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size) {
	int32_t res = 0;
	res = dest/src;

	return res;
}

uint32_t alu_mod(uint64_t src, uint64_t dest) {
	uint32_t res = 0;
	res = dest%src;

	return res;
}

int32_t alu_imod(int64_t src, int64_t dest) {
	int32_t res = 0;
	res = dest%src;

	return res;
}

uint32_t alu_and(uint32_t src, uint32_t dest) {
	uint32_t res = 0;
	res = dest & src;

	set_CF_zero();
 	set_PF(res);
	set_ZF(res);
	set_SF(res);
	set_OF_zero();

	return res;
}

uint32_t alu_xor(uint32_t src, uint32_t dest) {
	uint32_t res = 0;
	res = dest ^ src;

	set_CF_zero();
 	set_PF(res);
	set_ZF(res);
	set_SF(res);
	set_OF_zero();

	return res;
}

uint32_t alu_or(uint32_t src, uint32_t dest) {
	uint32_t res = 0;
	res = dest | src;

	set_CF_zero();
 	set_PF(res);
	set_ZF(res);
	set_SF(res);
	set_OF_zero();

	return res;
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size) {	
	uint32_t res = 0;
	size_t m = 32 - data_size;
	uint32_t y = (dest<<m)>>m;
	if(data_size < 32){
	uint32_t x = (dest>>data_size)<<data_size;
	uint32_t temp = src;
	while(temp != 0){
		cpu.eflags.CF = y>>(data_size - 1);
		y = y << 1;
		temp = temp - 1;
		}
	y = (y<<m)>>m;
	res = x + y;
	}else{
		uint32_t temp = src;
		while(temp != 0){
			cpu.eflags.CF = y>>(data_size - 1);
			y = y << 1;
			temp = temp - 1;
		}
		res = y;
	}

	set_PF(y);
	set_ZF(y);
	set_SF(y<<m);

	return res;
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t res = 0;
	size_t m = 32 - data_size;
	uint32_t y = (dest<<m)>>m;
	if(data_size < 32){
	uint32_t x = (dest>>data_size)<<data_size;
	uint32_t temp = src;
	while(temp != 0){
		cpu.eflags.CF = y & 0x1;
		y = y / 2;
		temp = temp - 1;
		}
	y = (y<<m)>>m;
	res = x + y;
	}else{
		uint32_t temp = src;
		while(temp != 0){
			cpu.eflags.CF = y & 0x1;
			y = y / 2;
			temp = temp - 1;
		}
		res = y;
	}

	set_PF(y);
	set_ZF(y);
	set_SF(y<<m);

	return res;

}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t res = 0;
	size_t m = 32 - data_size;
	uint32_t y = (dest<<m)>>m;
	uint32_t test = sign(y<<m);
	if(data_size < 32){
	uint32_t x = (dest>>data_size)<<data_size;
	uint32_t temp = src;
	while(temp != 0){
		cpu.eflags.CF = y & 0x1;
		y = y / 2;
		temp = temp - 1;
		}
	if(test == 1)
		y = y | (0xffffffff<<(data_size - src));
	y = (y<<m)>>m;
	res = x + y;
	}else{
		uint32_t temp = src;
		while(temp != 0){
			cpu.eflags.CF = y & 0x1;
			y = y / 2;
			temp = temp - 1;
		}
		if(test == 1)
			y = y | (0xffffffff<<(data_size - src));
		res = y;
	}

	set_PF(y);
	set_ZF(y);
	set_SF(y<<m);

	return res;
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size) {	
	uint32_t res = 0;
	size_t m = 32 - data_size;
	uint32_t y = (dest<<m)>>m;
	if(data_size < 32){
	uint32_t x = (dest>>data_size)<<data_size;
	uint32_t temp = src;
	while(temp != 0){
		cpu.eflags.CF = y>>(data_size - 1);
		y = y << 1;
		temp = temp - 1;
		}
	y = (y<<m)>>m;
	res = x + y;
	}else{
		uint32_t temp = src;
		while(temp != 0){
			cpu.eflags.CF = y>>(data_size - 1);
			y = y << 1;
			temp = temp - 1;
		}
		res = y;
	}

	set_PF(y);
	set_ZF(y);
	set_SF(y<<m);

	return res;
}

