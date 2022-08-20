#include <stdint.h>
#include "main.h"

void xoa_flash();
void ghi_flash();

int main(){
	HAL_Init();
	xoa_flash();
	ghi_flash();
	return 0;
}

void xoa_flash(){
	uint32_t* SR = (uint32_t*)0x40023c0c;
	uint32_t* KEYR = (uint32_t*)0x40023c04;
	uint32_t* CR = (uint32_t*)0x40023c10;

	if(((*CR>>31)&1) == 1){				// kiểm tra bit LOCK có đang khóa hay không =1 là đang bị khóa
		*KEYR = 0x45670123;				// key mở khóa
		*KEYR = 0xCDEF89AB;				// key mở khóa
	}
	while(((*SR>>16)&1) == 1){};		// kiểm tra bit BSY trong thanh ghi STRT có bận không
	*CR |= 1<<1;						// bật sector ease mode
	*CR |= 7<<3;						// chọn sector cần xóa (section 7)
	*CR |= 1<<16;						// bật bit STRT để xóa dữ liệu sector
	while(((*SR>>16)&1) ==1){}			// kiểm tra bit BSY trong thanh ghi STRT có bận không
	*CR &= ~(1<<1);						// tắt sector ease mode
}
void ghi_flash(){
	uint32_t* SR = (uint32_t*)0x40023c0c;
	uint32_t* KEYR = (uint32_t*)0x40023c04;
	uint32_t* CR = (uint32_t*)0x40023c10;

	if(((*CR>>31)&1) == 1){				// kiểm tra bit LOCK có đang khóa hay không =1 là đang bị khóa
		*KEYR = 0x45670123;				// key mở khóa
		*KEYR = 0xCDEF89AB;				// key mở khóa
	}
	while(((*SR>>16)&1) == 1){};		// kiểm tra bit BSY trong thanh ghi STRT có bận không
	*CR |= 1;							// bật, kích hoạt PG

	uint8_t* flash = (uint8_t*)0x08060000;	// địa chỉ SECTOR 7 (3.1)
	*flash = 'x';
	while(((*SR>>16)&1) == 1){};		// kiểm tra bit BSY trong thanh ghi STRT có bận không
}
