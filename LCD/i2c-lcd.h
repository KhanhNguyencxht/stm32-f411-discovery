#include "stm32f1xx_hal.h"

void lcd_init (void);				// initialize lcd
									//khởi tạo lcd

void lcd_send_cmd (char cmd);		// send command to the lcd
									// gửi lệnh đến màn hình LCD

void lcd_send_data (char data);		// send data to the lcd
									// gửi dữ liệu đến màn hình LCD

void lcd_send_string (char *str);	// send string to the lcd
									// gửi chuỗi đến màn hình LCD

void lcd_put_cur(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);
									// đặt con trỏ vào hàng vị trí đã cột (0 hoặc 1), dong  (0-15);
void lcd_clear (void);





lcd_claer - led_send_cmd
	  - led_send_data


lcd_put_cur - lcd_send_cmd

lcd_init - lcd_send_cmd

lcd_send_string - lcd_send_data