#include"main.h"
#include<stdint.h>
#include<string.h>

unsigned char giatri;
unsigned char tinhieu;

void setup_timer();
void setup_led();
void tim1();
void nhay_led(char giatri);

int main(){

	HAL_Init();
	setup_timer();
	setup_led();
	void* diachi_ram = 0x20000000;
	void* diachi_flash = 0x08000000;
	memcpy(diachi_ram, diachi_flash, 0x198);  //COPY DỮ LIỆU BẢNG VECTOR TABLE LÊN RAM

	uint32_t* VTOR = 0xe000ed08;  // báo với arm địa chỉ bảng vector table dời lên ram
	*VTOR = 0X20000000;

	uint32_t* dangky_timer = 0x200000a4;  	// đăng ký địa chỉ tim1, 0x000000a4 là địa chỉ cũ tim1 trong bảng vetor table,
	*dangky_timer = (uint32_t)tim1 | 1;	//0x20000a4 là địa chỉ mới trên ram 0x200000A4

	dangky_timer = 0x200000a4;
	*dangky_timer = (uint32_t)tim1 | 1;	//0x20000a4 là địa chỉ mới trên ram 0x200000A4

	while(1){

	}
	return 0;
}

void setup_timer(){
	__HAL_RCC_TIM1_CLK_ENABLE();
	uint32_t* TIM1_PSC = 0x40010028;
	uint32_t* TIM1_ARR = 0x4001002c;
	uint32_t* TIM1_CR1 = 0x40010000; 	// BẬT BỘ ĐẾM COUTER
	*TIM1_PSC = 16000-1;				//THIẾT LẬP BỘ CHIA, PSC(16000) =1KHZ(1MS)
	*TIM1_ARR = 1000-1; 				 // THIẾT LẬP BỘ ĐẾM (1000MS) = 1S
	*TIM1_CR1 |= 1<<0;					// BẬT CHẾ ĐỘ ĐẾM COUTER
	uint32_t* TIM1_DIER = 0x4001000c;
	*TIM1_DIER = 1<<0;					// BẬT CHẾ ĐỘ NGẮT TRONG TIM1
	uint32_t* NVIC_ISER0 = 0xe000e100;
	*NVIC_ISER0 |= 1<<25;				// BẬT CHẾ ĐỘ ƯU TIÊM TIM1 TRONG BẢNG VECTOR TABLE
}

void setup_led(){
	__HAL_RCC_GPIOD_CLK_ENABLE();
	uint32_t* GPIOD_MOER = 0x40020c00;
	*GPIOD_MOER &= ~(0b11111111<<24);
	*GPIOD_MOER |= 0b01010101<<24;
}

void tim1(){
	uint32_t* TIM1_SR = 0x40010010;
	__asm("nop");
	giatri++;
	tinhieu = 1-tinhieu;
	nhay_led(tinhieu);
	*TIM1_SR = ~(1<<0);
}

void TIM1_UP_TIM10_IRQHandler(){
	uint32_t* TIM1_SR = 0x40010010;
	*TIM1_SR = ~(1<<0);
}

void nhay_led(char tinhieu){
	uint32_t* GPIOD_ODR = 0x40020c14;
	 if (tinhieu == 1){
		  *GPIOD_ODR |= (1 << 12);
	 }
	  else{
		  *GPIOD_ODR &= ~(1 << 12);
	  }
	}
