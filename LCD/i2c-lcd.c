
/** Put this in the src folder **/

#include "i2c-lcd.h"
extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly

#define SLAVE_ADDRESS_LCD 0x4E	// change this according to ur setup
								// địa chỉ module lcd

void lcd_send_cmd (char cmd)		// dửi lệnh cài đặt
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);			// set lcd chế độ 8 bit 4 bit dưới kết nối mức thấp D0, D1, D2, D3
	data_l = ((cmd<<4)&0xf0);		// gửi lệnh 8 bit thành 2 nửa 4 bit trên được kết nối mức cao D4. D5. D6. D7
	data_t[0] = data_u|0x0C;  //en=1, rs=0 7 bít địa chỉ đi, cùng 1 bit là đọc, ghi thành 8 bit 1 đọc 0 ghi
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_send_data (char data)			// gửi data dữ liệu
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);				// set lcd chế độ 8 bit 4 bit dưới kết nối mức cao
	data_l = ((data<<4)&0xf0);			// gửi lệnh 8 bit thành 2 nửa 4 bit trên được kết nối mức thấp
	data_t[0] = data_u|0x0D;  //en=1, rs=0  7 bít địa chỉ đi, cùng 1 bit là đọc, ghi thành 8 bit 1 đọc 0 ghi
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_clear (void)				// xóa dữ liệu trên màn hình
{
	lcd_send_cmd (0x80);
	for (int i=0; i<70; i++)
	{
		lcd_send_data (' ');
	}
}

void lcd_put_cur(int row, int col)
{
    switch (row)		// hàng
    {
        case 0:
            col |= 0x80;	// 0X80 địa chỉ ô đầu tiên cột trên cùng
							// cột1 dòng tùy vào mình chon để in ra trên màn hình dòng 2 hay dòng 1
            break;
        case 1:
            col |= 0xC0;	//0xc0 địa chỉ dòng đầu tiên cột 2
            break;			// cột 2 dòng tùy vào mình chọn
    }

    lcd_send_cmd (col);
}


void lcd_init (void)
{
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd (0x30);
	HAL_Delay(5);  // wait for >4.1ms
	lcd_send_cmd (0x30);
	HAL_Delay(1);  // wait for >100us
	lcd_send_cmd (0x30);
	HAL_Delay(10);
	lcd_send_cmd (0x20);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
						 // set DL=0 4 bit, N 2 cột hoặc 1 cột, F kích thước ô
	HAL_Delay(1);
	lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off hiện thị con trỏ hoặc tắt
	HAL_Delay(1);
	lcd_send_cmd (0x01);  // clear display(xóa ký tự trên màn hình)
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
							// bật tắt con trỏ D=1 thì bật,  con trỏ nhấp nháy khi c và b = 0
}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}


int main() {
	while (1) {
		lcd_put_cur(0, 1);		// HÀNG 1 CỘT 1
		lcd_send_string("HELLO");	// XUẤT CHỮ
	}
}