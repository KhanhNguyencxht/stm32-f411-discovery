#include"stdint.h"
#include <main.h>
#include <string.h>

char giatri;

char vong_lap(char giatri);
void setup_TIM1();
void pwm_led(char giatri);

int main(){
	while(1){
		vong_lap(giatri);
	return 0;
}


char vong_lap(char giatri){
	giatri=0;
	for( ; giatri<=10; giatri++){};

}


/*
void setup_TIM1(){
	__HAL_RCC_TIM1_CLK_ENABLE();

	void* diachi_vector =	// địa chỉ bảng vector table
	void* diachi_ram =
}
*/

void pwm_led(char giatri){
	__HAL_RCC_GPIOD_CLK_ENABLE();	// BẬT CHÂN XUẤT TÍN HIỆU ĐẦU RA
	uint32_t* GPIOD_MODER = 0x40020c00;	// địa chỉ tín hiệu chân điều khiển
	uint32_t* GPIOD_AFRH = 0x40020c24;	// ĐỊA CHỈ BỘ NGOẠI VI, 1 chân vó nhiều bộ ngoại vi

	*GPIOD_MODER &= ~(0b11<<24);
	*GPIOD_MODER |= 0b10<<24;  // nhường quyền điều khiển cho bộ ngoại vi, 10 là bộ ngoại vi
	*GPIOD_AFRH &= ~(0b111<<16);	//reset bộ điều khiển ngoại vi
	*GPIOD_AFRH |= 0b0010<<16;		// 0010 là mã bit của AF12 Ở ĐÂY TIM1 ĐANG DÙNG Ở BỘ NGOẠI VI AF02

	__HAL_RCC_TIM4_CLK_ENABLE();	// chân D12 bộ phát xung là TIM4
	uint32_t* TIM4_PSC = 0x40000828;	// ĐỊA CHỈ BỘ CHIA TIMER CHIA mhz
	uint32_t* TIM4_ARR = 0x4000082c;	// ĐỊA CHỈ BỘ CHIA TIMER GIÂY
	uint32_t* TIM4_CCR1 = 0x40000834;	// ĐỊA CHỈ ĐỘ RỘNG XUNG
	uint32_t* TIM4_CCER = 0x40000820;	// ĐỊA CHỈ của kênh CH1
	uint32_t* TIM4_CR1 = 0x40000800;	// ĐỊA CHỈ COUTER KÍCH HOẠT ĐẾM TIMER
	uint32_t* TIM4_CCMR1 = 0x40000818;	// ĐỊA CHỈ CỦA THANH MODE

	*TIM4_PSC = 16000-1;	// chia từ 16(000000)mhz xuống 16(000)khz, -> nhỏ hơn 65535
	*TIM4_ARR = 10-1;		// 1000 tương đương 1s với bộ chia 16000 để càng nhỏ thì xung nháy càng nhanh
	*TIM4_CCR1 = giatri-1;			// ĐỘ RỘNG XUNG CÀNG NHỎ THÌ ĐÈN CÀNG MỜ
	*TIM4_CCER = 1<<0;		// bật kênh CH1 tên là CC1E  365ref
	*TIM4_CR1 |=1<<0;		// KÍCH HOẠT BỘ ĐẾM TIMER
	*TIM4_CCMR1 &= ~(0b111<<3);	// reset địa chỉ của thanh mode1
	*TIM4_CCMR1 |= 0b110<<3;	// kích hoạt thanh mode1 (xung thuận)
}


