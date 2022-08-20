#include <main.h>
#include "stdint.h"

unsigned char giatri;
unsigned char caidat;

void setup_nutnhan();
void setup_led();
void setup_thongso();
void led(char tinhieu);

char nutnhan1();
char nutnhan2();
char nutnhan3();

int main() {
	HAL_Init();
	setup_nutnhan();
	setup_led();

	while (1) {
		setup_thongso();
		HAL_Delay(10);
	}
	return 0;
}

void setup_nutnhan() {
	__HAL_RCC_GPIOA_CLK_ENABLE();
	uint32_t *MODERA = 0x40020000;
	uint32_t *PUPDRA = 0x4002000c;

	*MODERA &= ~(0b111111 << 8);
	*PUPDRA &= ~(0b111111 << 8);		// gim điện áp chân 1 2 3
}

void setup_led() {
	__HAL_RCC_GPIOD_CLK_ENABLE();
	uint32_t *MODERD = 0x40020c00;

	*MODERD &= ~(0b11111111 << 24);
	*MODERD |= 0b01010101 << 24;		// bật chân
}

char nutnhan1() {
	uint32_t *IDR = 0x40020010;
	if (((*IDR >> 4) & 1) == 1) {
		return 1;
	} else {
		return 0;
	}
}
char nutnhan2() {
	uint32_t *IDR = 0x40020010;

	if (((*IDR >> 5) & 1) == 1) {
		return 1;
	} else {
		return 0;
	}
}
char nutnhan3() {
	uint32_t *IDR = 0x40020010;

	if (((*IDR >> 6) & 1) == 1) {
		return 1;
	} else {
		return 0;
	}
}

void setup_thongso() {
	if (nutnhan1() == 1) {
		while (nutnhan1() == 1) {
		}
		caidat++;
		if (caidat >= 2) {
			caidat = 2;
		}
	}

	if (caidat == 1) {
		if (nutnhan3() == 1) {
			while (nutnhan3() == 1) {
			}
			giatri++;
			if (giatri >= 4) {
				giatri = 4;
			}
		}

		else {
			if (nutnhan2() == 1) {
				while (nutnhan2() == 1) {
				}
				giatri--;
				if (giatri <= 1) {
					giatri = 1;
				}
			}
		}
	} else {
		if (caidat == 2) {
			led(giatri);
		}
	}
}

void led(char tinhieu) {
	uint32_t *ODR = 0x40020c14;
	if (tinhieu == 1) {
		*ODR |= 1 << 12;
	}
	else if (tinhieu == 2) {
		*ODR |= 1 << 13;
	}
	else if (tinhieu == 3) {
		*ODR |= 1 << 14;
	}
	else {
		*ODR |= 1 << 15;
	}
}

