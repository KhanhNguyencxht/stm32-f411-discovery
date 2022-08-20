#include"stdint.h"
#include <main.h>

void pwm();

void main(){
	 pwm();

}

void pwm(){
	__HAL_RCC_GPIOD_CLK_ENABLE();  // bật chân GPIOD
	uint32_t* MODER = 0x40020c00;
	*MODER &= ~(0b11<<24);		//reset chân 12 về 0
	*MODER |= 0b10<<24;			// set chân 12 nhường quyền điều khiển cho bộ ngoại vi ob00 IN, 0b11 OUT, 0b10, ngoại vi; 157ref

	uint32_t* AFRH = 0x40020c24;
	*AFRH &= ~(0b1111<<16);		//seset chân ngoại vi 12 về 0 mỗi ngoại vi là 4 bit
	*AFRH |= 0b0010<<16;		//trường hợp 1 chân có nhiều bộ ngoại vi thì dùng bộ ngoại vi nào thì set bộ ngoại vi đó T50data, 162ref

	__HAL_RCC_TIM4_CLK_ENABLE();	// chân D12 bộ phát xung là TIM4
	uint32_t* PSC = 0x40000828;
	uint32_t* ARR = 0x4000082c;
	uint32_t* CCR1 = 0x40000834;
	*PSC = 16000 - 1; 			// BỘ CHIA 16000000mhz xuống 16000khz
	*ARR = 10 - 1;			// 1000 tương đương 1ms số càng nhỏ nháy càng nhanh
	*CCR1 = 2 - 1;			// độ rộng xung 500 tương đương 500ms, độ rộng càng lớn thì đèn càng sáng

	uint32_t* CCMR1 = 0x40000818; // địa chỉ của thanh mode sử dụng kênh CH1 thì địa chỉ là CCMR1....
	*CCMR1 &= ~(0b111<<4);	// reset OC1M, là đỉa chỉ của OC1M bit thứ 4 trang 362ref
	*CCMR1 |= 0b110<<4;		// bật chế đô mode1 lên xung thuận là mode1 xung nghịch là mode2

	uint32_t* CCER = 0x40000820;	// địa chỉ của kênh CH1
	*CCER |= 1<<0;				// bật kênh CH1 tên là CC1E  365ref
	uint32_t* CR1 = 0x40000800; // bộ đếm couter timer của TIM4;
	*CR1 |=1<<0; 				// Bật bộ đếm couter tên là CEN lên

}
