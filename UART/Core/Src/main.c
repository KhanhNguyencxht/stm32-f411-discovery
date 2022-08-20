#include <main.h>
#include "stdint.h"
#include <string.h>

char data;
char giatri[4];
unsigned int luu = 0;

void setup_uart2();
void truyen_uart2(char truyen[]);
char nhan_uart2();
void USART2_IRQHandler();
void uart_DMA1();

int main(){
	HAL_Init();
	setup_uart2();
	uart_DMA1();
	char truyen[] = "xin chao ban \r\n";

	while(1){
	//	truyen_uart2(truyen);
		HAL_Delay(1000);
		}
	return 0;
}

void setup_uart2(){
	__HAL_RCC_GPIOA_CLK_ENABLE();	// bật chân PA
	uint32_t* MODER = 0x40020000;
	*MODER &= ~(0b1111<<4);
	*MODER &= ~(0b1111<<6);
	*MODER |= 0b10<<4;	// nhường chân cho bộ ngoại vi
	*MODER |= 0b10<<6;

	uint32_t* AFRH = 0x40020020;	// địa chỉ bộ ngoại vi
	*AFRH &= ~(0b1111<<8);	//reset chân ngoại vi về 0
	*AFRH &= ~(0b1111<<12);	//reset chân ngoại vi về 0
	*AFRH |= 7<<8;		// bật chân ngoại vi PA2 PA3
	*AFRH |= 7<<12;

	__HAL_RCC_USART2_CLK_ENABLE();//bật USART2 lên
	uint32_t* BRR = 0x40004408;	// địa chỉ tốc độ truyền
	*BRR |=(104<<4) |3;			// set tốc độ truyền 9600 áp dụng công thức

	uint32_t* CR1 = 0x4000440c;
	*CR1 |= (1 << 2);				// set  RE lên 1 bât bộ truyền UART
	*CR1 |= 1<<3;				// set TE lên 1 bật bộ nhận UART
	*CR1 |= 1<<13;				// set UE lên 1 bật chạy UART
	*CR1 &= ~(1<<12);			// set M xuống 0 8 bit
	//*CR1 |=1<<5;				// set bit RXNEIE lên 1 sự kiện ngắt()tránh mất dữ liệu khi chip đang làm việc
								//  dùng DMA thì không cần sự kiện ngắt này nữa

	uint32_t* CR3 = 0x40004414;
	*CR3 |= 1<<6;				// bật DMA

	//uint32_t* NVIC_ISER1 = 0xe000e104;	địa chỉ ưu tiên của uart 2// dùng DMA thì không cần
	//*NVIC_ISER1 |= 1<<6;				// UART 38-32 =6  mỗi ISER LÀ 32 bit

}

void truyen_uart2(char truyen[]){
	uint32_t* SR = 0x40004400;
	uint32_t* DR = 0x40004404;

	char uart = 0;
	while(truyen[uart] !='\0'){
		while(((*SR>>7)&1)==0){};	// kiểm tra TXE nếu TXE bằng 0 thì đang có dữ liệu, chờ khi lên 1 đang trống để ghi dữ liệu
		*DR = truyen[uart++];					// SR lên TXE đang trống, ghi dữ liệu vào
		while(((*SR>>6)&1)==0){};	// kiểm TC, nếu TC chưa đẩy dữ liệu đi thì chờ
		*SR &= ~(1<<6);				// dũ liệu đi rồi thì clear về 0 dể tiếp tục đi dữ liệu
	}
}

void uart_DMA1(){
	__HAL_RCC_DMA1_CLK_ENABLE();
	uint32_t* S7CR = 0x400260b8;
	uint32_t* S7NDTR = 0x400260bc;
	uint32_t* S7PAR = 0x400260c0;
	uint32_t* S7M0AR = 0x400260c4;

	*S7NDTR= sizeof(giatri);	// kích thước  bộ nhớ data dữ liệu
	*S7M0AR = (uint32_t)giatri;	// chứa địa chỉ mảng
	*S7PAR = 0x40004404;		// gán địa chỉ thanh ghi DR chứa dữ liệu
	*S7CR &= ~(0b111);
	*S7CR |= 0b110<<25;			// bật CH6 UART2
	*S7CR |= (1<<10); 			// địa chỉ vùng nhớ tự động tăng lên lưu dữ liệu
	*S7CR |= 1<<0;				// bật EN(DMA)
	*S7CR |= 1<<8;				// bật CIRC đầy dữ liệu thì sẽ quay về địa chỉ đầu tiên để ghi đè dữ liệu
}

/*  dùng 1 trong 3; hàm ngắt; DMA(NGẮT DMA)
char nhan_uart(){
	uint32_t* SR = 0x40004400;
	uint32_t* DR = 0x40004404;
	uint32_t dulieu = 0;

	while(((*SR>>5)&1)==0){};		// tín hiệu data về đợi RXNE lên 1 là có dữ liệu về để nhận dữ liệu
	dulieu = *DR;
	*SR &=~(1<<5);
	return dulieu;
}


void USART2_IRQHandler(){
	uint32_t* SR = 0x40004400;
	uint32_t* DR = 0x40004404;
	uint32_t* PR = 0x40013c14;

	giatri[luu] = *DR;
	luu++;
	*SR &= ~(1<<5);
	//*PR |=1<<0;

}
*/
