#include <stdint.h>
#include "main.h"

char tinhieu;
void setup();
void truyen(char dulieu);
char nhan();
unsigned char ID;

#define enable_spi *GPIOE_ODR &= ~(1<<3)
#define disable_spi *GPIOE_ODR |= (1<<3)

uint32_t* GPIOE_ODR = (uint32_t*)0x40021014;
char X_L, X_H, Y_L, Y_H, Z_L, Z_H;
short X, Y, Z;


int main(){
	HAL_Init();
	setup();
		while(1){
			enable_spi;
			truyen(0X0f |(1<<7));		// tìm địa chỉ ID cảm biến
			ID = nhan();
			disable_spi;

			enable_spi;
			truyen(0X20 & ~(1<<7));	// địa chỉ thanh ghi & với chế đô ghi W = 0
			truyen(0X0f);		// bật trục x y z, chế độ PD z y x lên 1(7.2)(0X20 | W)
			nhan();
			disable_spi;

			enable_spi;
			truyen(0x28 |(1<<7) |(1<<6));	// bật chế độ đọc dữ liệu, chế độ mode tự động tăng giá trị
			X_L = nhan();
			X_H = nhan();
			Y_L = nhan();
			Y_H = nhan();
			Z_L = nhan();
			Z_H = nhan();
			disable_spi;
			X = X_L | (X_H <<8);
			Y = Y_L | (Y_H <<8);
			Z = Z_L | (Z_H <<8);

			HAL_Delay(10);
		}
	return 0;
}

void setup(){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	uint32_t* MODERA = (uint32_t*)0x40020000;

	*MODERA &= ~(0b111111<<10);
	*MODERA |= 0b101010<<10;	// set chân ngoại vi xung clock

	uint32_t* AFRL = (uint32_t*)0x40020020;
	*AFRL &= ~(0b111111111111<<20);
	*AFRL |= 0b010101010101<<20;	//set chân ngoại vi PA5 PA6 PA7

	__HAL_RCC_GPIOE_CLK_ENABLE();
	uint32_t* MODERE = (uint32_t*)0x40021000;
	*MODERE &= ~(0b11<<6);
	*MODERE |= 0b01<<6;			// set chân bật tắt slave
	disable_spi	;				// bật chip trên kit

	__HAL_RCC_SPI1_CLK_ENABLE();
	uint32_t* I2SCFGR = (uint32_t*)0x4001301c;
	*I2SCFGR &= ~(0b11<<8);
	*I2SCFGR |= 0b10<<8;		// set master cho kit

	uint32_t* CR1 = (uint32_t*)0x40013000;
	*CR1 &= ~(0b111<<3);
	*CR1 |= 0b011<<3;			// set tốc bộ baud rate
	*CR1 &= ~(1<<0);
	*CR1 &=	~(1<<1);	// set chế độ mode 0 chế độ nghỉ SPI hoạt động, chưa đẩy dữ liệu

	*CR1 |= 1<<2;				// set master cho kit
	*CR1 |= 1<<9;				// bật Software slave management(SSM)
	*CR1 |= 1<<8;				// bật lên khi SSM được bật
	*CR1 |= 1<<6;				// bật SPI lên

}

void truyen( char dulieu){
	uint32_t* SR = (uint32_t*)0x40013008;
	uint8_t* DR = (uint8_t*)0x4001300c;

	while(((*SR>>1)&1)==0){}	// trạng thái chờ khi TXE có dữ liệu
	*DR = dulieu;				// set bit
	while(((*SR>>1)&1)==1){}	// trạng thái chờ khi TXE trống dữ liệu
	while(((*SR>>0)&1)==0){}	// trạng thái đợi khi RXNE không có dữ liệu
	while(((*SR>>7)&1)==1){}	// trạng thái đợi khi BSY đang bận hoặc TX có dữ liệu
	uint8_t clear =*DR;			// xóa rác trong thanh ghi DR

}
char nhan(){
	uint32_t* SR = (uint32_t*)0x40013008;
	uint8_t* DR = (uint8_t*)0x4001300c;

	while(((*SR>>1)&1)==0){}	// trạng thái chờ khi TXE có dữ liệu
	*DR = 0xff;					// dư liệu rác để làm xung clock
	while(((*SR>>1)&1)==1){}	// trạng thái đợi khi TXE không có dữ liệu
	while(((*SR>>0)&1)==0){}	// trạng thái đợi khi RXNE không có dữ liệu
	while(((*SR>>7)&1)==1){}	// trạng thái đợi khi BSY bận hoặc TX có dữ liệu
	char ketqua = *DR;
	return ketqua;
}
