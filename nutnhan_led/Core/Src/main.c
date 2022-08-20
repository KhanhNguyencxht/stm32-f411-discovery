#include"main.h"
#include<stdint.h>

void set_led() {
	__HAL_RCC_GPIOD_CLK_ENABLE();
	uint32_t *GPIOD_MODER = 0x40020c00;
	*GPIOD_MODER &= ~(0b11 << 24);
	*GPIOD_MODER |= 0b01 << 24;
	*GPIOD_MODER &= ~(0b11 << 26);
	*GPIOD_MODER |= 0b01 << 26;
	*GPIOD_MODER &= ~(0b11 << 28);
	*GPIOD_MODER |= 0b01 << 28;
	*GPIOD_MODER &= ~(0b11 << 30);
	*GPIOD_MODER |= 0b01 << 30;
}
void set_nutnhan() {
	__HAL_RCC_GPIOA_CLK_ENABLE();
	uint32_t *GPIOA_MODER = 0x40020000;
	*GPIOA_MODER &= ~(0b11 << 0);
	uint32_t *GPIOA_PUPDR = 0x4002000c;
	*GPIOA_PUPDR &= ~(0b11 << 0);
}

char trang_thai_nutnhan() {
	uint32_t *GPIOA_IDR = 0x40020010;
	if (((*GPIOA_IDR >> 0) & 1) == 1) {
		return 1;
	} else {
		return 0;
	}
}

char led_nhay(char tinhieu) {
	uint32_t *GPIOD_ODR = 0x40020c14;

	if (tinhieu == 1) {
		for (int i = 12; i <= 15; i++) {
			*GPIOD_ODR |= 1 << i;
			HAL_Delay(500);
			*GPIOD_ODR &= ~(1 << i);
		}
	}

/*	else if (tinhieu == 2) {

		for (int x = 15; x >= 12; x--) {
			*GPIOD_ODR |= 1 << x;
			HAL_Delay(500);
			*GPIOD_ODR &= ~(1 << x);
		}
	}	*/
	 else {
		*GPIOD_ODR &= ~(1 << 12);
	}

}

unsigned char giatri;
//unsigned char tinhieu;

int main() {
	HAL_Init();
	set_led();
	set_nutnhan();
	while (1) {
		if (trang_thai_nutnhan() == 1) {
			while (trang_thai_nutnhan() == 1) {
			}
			giatri++;
			if (giatri == 3) {
				giatri = 0;
			}
		}

		led_nhay(giatri);
		HAL_Delay(10);
	}
	return 0;
}
