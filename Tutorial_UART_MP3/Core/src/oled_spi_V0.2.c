/*
 * @file oled_spi_V0.2.c
 * @brief OLED display control using SPI
 * @details This file contains functions to control the OLED display using SPI communication.
 * @date Mar.21st, 2024
 * @author Ldk, InnoLegend team.
 */

 #include "oled_spi_V0.2.h"
 #include "oledfont.h"
 #include <string.h>
 
 // OLED display buffer
 u8 OLED_GRAM[130][8];
 
 /**
  * @brief Write a byte to the OLED display
  * @param dat The data/command byte to write
  * @param cmd The data/command flag (0 for command, 1 for data)
  * @details This function writes a byte to the OLED display. The cmd parameter indicates whether the byte is a command or data.
  */
 void OLED_WR_Byte(u8 dat, u8 cmd)
 {
     if (cmd)
     {
         OLED_DC_Set();
     }
     else
     {
         OLED_DC_Clr();
     }
     while (DL_SPI_isBusy(SPI_OLED_INST)){
     }
     DL_SPI_transmitData8(SPI_OLED_INST, dat);
     while (DL_SPI_isBusy(SPI_OLED_INST)){
     }
     OLED_DC_Set();
 }
 
 /**
  * @brief Set the position of the OLED cursor
  * @param x The x-coordinate (0-127)
  * @param y The y-coordinate (0-7)
  * @details This function sets the position of the OLED cursor to the specified coordinates.
  */
 void OLED_Set_Pos(unsigned char x, unsigned char y)
 {
     OLED_WR_Byte(0xaf + y + YOFFSET, OLED_CMD);
     OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
     OLED_WR_Byte((x & 0x0f) | 0x01, OLED_CMD);
 }
 
 /**
  * @brief Turn on the OLED display
  * @details This function turns on the OLED display.
  */
 void OLED_Display_On(void)
 {
     OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC command
     OLED_WR_Byte(0X14, OLED_CMD); // DCDC ON
     OLED_WR_Byte(0XAF, OLED_CMD); // DISPLAY ON
 }
 
 /**
  * @brief Turn off the OLED display
  * @details This function turns off the OLED display.
  */
 void OLED_Display_Off(void)
 {
     OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC command
     OLED_WR_Byte(0X10, OLED_CMD); // DCDC OFF
     OLED_WR_Byte(0XAE, OLED_CMD); // DISPLAY OFF
 }
 
 /**
  * @brief Clear the OLED display
  * @details This function clears the OLED display by setting all pixels to off.
  */
 void OLED_Clear(void)
 {
     u8 i, n;
     for (i = 0; i < 8; i++)
     {
         OLED_WR_Byte(0xb0 + i, OLED_CMD); // Set page address
         OLED_WR_Byte(0x02, OLED_CMD);     // Set lower column address
         OLED_WR_Byte(0x10, OLED_CMD);     // Set higher column address
         for (n = 0; n < 128; n++)
             OLED_WR_Byte(0, OLED_DATA);
     }
     memset(OLED_GRAM, 0, sizeof(OLED_GRAM));
 }
 
 /**
  * @brief Display a character at the specified position
  * @param x The x-coordinate (0-127)
  * @param y The y-coordinate (0-7)
  * @param chr The character to display
  * @details This function displays a character at the specified position on the OLED display.
  */
 void OLED_ShowChar(u8 x, u8 y, char chr)
 {
     unsigned char c = 0, i = 0;
     c = chr - ' '; // Get the offset value
     if (x > Max_Column - 1)
     {
         x = 0;
         y = y + 2;
     }
     if (SIZE == 16)
     {
         OLED_Set_Pos(x, y);
         for (i = 0; i < 8; i++)
             OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
         OLED_Set_Pos(x, y + 1);
         for (i = 0; i < 8; i++)
             OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
     }
     else
     {
         OLED_Set_Pos(x, y + 1);
         for (i = 0; i < 6; i++)
             OLED_WR_Byte(F6x8[c][i], OLED_DATA);
     }
 }
 
 /**
  * @brief Calculate the power of a number
  * @param m The base
  * @param n The exponent
  * @return The result of m raised to the power of n
  * @details This function calculates the power of a number.
  */
 u32 oled_pow(u8 m, u8 n)
 {
     u32 result = 1;
     while (n--)
         result *= m;
     return result;
 }
 
 /**
  * @brief Display a number at the specified position
  * @param x The x-coordinate (0-127)
  * @param y The y-coordinate (0-7)
  * @param num The number to display
  * @param len The length of the number
  * @param size2 The font size
  * @details This function displays a number at the specified position on the OLED display.
  */
 void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size2)
 {
     u8 t, temp;
     u8 enshow = 0;
     for (t = 0; t < len; t++)
     {
         temp = (num / oled_pow(10, len - t - 1)) % 10;
         if (enshow == 0 && t < (len - 1))
         {
             if (temp == 0)
             {
                 OLED_ShowChar(x + (size2 / 2) * t, y, ' ');
                 continue;
             }
             else
                 enshow = 1;
         }
         OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0');
     }
 }
 
 /**
  * @brief Display a signed number at the specified position
  * @param x The x-coordinate (0-127)
  * @param y The y-coordinate (0-7)
  * @param num The signed number to display
  * @param len The length of the number
  * @param size2 The font size
  * @details This function displays a signed number at the specified position on the OLED display.
  */
 void OLED_ShowSignedNum(u8 x, u8 y, int32_t num, u8 len, u8 size2)
 {
     u8 t, temp;
     u8 enshow = 0;
     u8 isNegative = 0;
 
     // Check if the number is negative
     if (num < 0)
     {
         isNegative = 1;
         num = -num;  // Convert to positive for processing
     }
 
     // Display the negative sign
     if (isNegative)
     {
         OLED_ShowChar(x, y, '-');
         x += size2 / 2;  // Update the starting position
         len--;  // Reduce the length by 1 to account for the negative sign
     }
 
     for (t = 0; t < len; t++)
     {
         temp = (num / oled_pow(10, len - t - 1)) % 10;
         if (enshow == 0 && t < (len - 1))
         {
             if (temp == 0)
             {
                 OLED_ShowChar(x + (size2 / 2) * t, y, ' ');
                 continue;
             }
             else
                 enshow = 1;
         }
         OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0');
     }
 }
 
 /**
  * @brief Display a string at the specified position
  * @param x The x-coordinate (0-127)
  * @param y The y-coordinate (0-7)
  * @param chr The string to display
  * @details This function displays a string at the specified position on the OLED display.
  */
 void OLED_ShowString(u8 x, u8 y, char *chr)
 {
     unsigned char j = 0;
     while (chr[j] != '\0')
     {
         OLED_ShowChar(x, y, chr[j]);
         x += 8;
         if (x > 120)
         {
             x = 0;
             y += 2;
         }
         j++;
     }
 }
 
 /**
  * @brief Display a Chinese character at the specified position
  * @param x The x-coordinate (0-127)
  * @param y The y-coordinate (0-7)
  * @param no The index of the Chinese character in the font array
  * @details This function displays a Chinese character at the specified position on the OLED display.
  */
 void OLED_ShowCHinese(u8 x, u8 y, u8 no)
 {
     u8 t, adder = 0;
     OLED_Set_Pos(x, y);
     for (t = 0; t < 16; t++)
     {
         OLED_WR_Byte(Hzk[2 * no][t], OLED_DATA);
         adder += 1;
     }
     OLED_Set_Pos(x, y + 1);
     for (t = 0; t < 16; t++)
     {
         OLED_WR_Byte(Hzk[2 * no + 1][t], OLED_DATA);
         adder += 1;
     }
 }
 
 /**
  * @brief Draw a BMP image on the OLED display
  * @param x0 The starting x-coordinate
  * @param y0 The starting y-coordinate
  * @param x1 The ending x-coordinate
  * @param y1 The ending y-coordinate
  * @param BMP The BMP image data
  * @details This function draws a BMP image on the OLED display at the specified coordinates.
  */
 void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, const unsigned char BMP[])
 {
     unsigned int j = 0;
     unsigned char x, y;
 
     if (y1 % 8 == 0)
         y = y1 / 8;
     else
         y = y1 / 8 + 1;
     for (y = y0; y < y1; y++)
     {
         OLED_Set_Pos(x0, y + (!YOFFSET));
         for (x = x0; x < x1; x++)
         {
             OLED_WR_Byte(BMP[j++], OLED_DATA);
         }
     }
 }
 
 /**
  * @brief Invert the display colors
  * @param i The inversion flag (0 for normal, 1 for inverted)
  * @details This function inverts the display colors based on the provided flag.
  */
 void OLED_ColorTurn(u8 i)
 {
     if (i == 0)
     {
         OLED_WR_Byte(0xA6, OLED_CMD); // Normal display
     }
     if (i == 1)
     {
         OLED_WR_Byte(0xA7, OLED_CMD); // Inverted display
     }
 }
 
 /**
  * @brief Rotate the display 180 degrees
  * @param i The rotation flag (0 for normal, 1 for 180 degrees)
  * @details This function rotates the display 180 degrees based on the provided flag.
  */
 void OLED_DisplayTurn(u8 i)
 {
     if (i == 0)
     {
         OLED_WR_Byte(0xC8, OLED_CMD); // Normal display
         OLED_WR_Byte(0xA1, OLED_CMD);
     }
     if (i == 1)
     {
         OLED_WR_Byte(0xC0, OLED_CMD); // 180 degrees display
         OLED_WR_Byte(0xA0, OLED_CMD);
     }
 }
 
 /**
  * @brief Refresh the OLED display
  * @details This function refreshes the OLED display by updating the display buffer.
  */
 void OLED_Refresh(void)
 {
     u8 i, n;
     for (i = 0; i < 8; i++)
     {
         OLED_WR_Byte(0xb0 + i, OLED_CMD); // Set page address
         OLED_WR_Byte(0x00, OLED_CMD);     // Set lower column address
         OLED_WR_Byte(0x10, OLED_CMD);     // Set higher column address
         while (DL_SPI_isBusy(SPI_OLED_INST)){}
         DL_SPI_transmitData8(SPI_OLED_INST, 0x78);
         while (DL_SPI_isBusy(SPI_OLED_INST)){}
         DL_SPI_transmitData8(SPI_OLED_INST, 0x78);
         while (DL_SPI_isBusy(SPI_OLED_INST)){}
         DL_SPI_transmitData8(SPI_OLED_INST, 0x40);
         while (DL_SPI_isBusy(SPI_OLED_INST)){}
         for (n = 0; n < 128; n++)
         {
             DL_SPI_transmitData8(SPI_OLED_INST, OLED_GRAM[n][i]);
             while (DL_SPI_isBusy(SPI_OLED_INST)){}
         }
     }
 }
 
 /**
  * @brief Draw a point on the OLED display
  * @param x The x-coordinate (0-127)
  * @param y The y-coordinate (0-63)
  * @param t The point value (1 for on, 0 for off)
  * @details This function draws a point on the OLED display at the specified coordinates.
  */
 void OLED_DrawPoint(u8 x, u8 y, u8 t)
 {
     u8 i, m, n;
     i = y / 8;
     m = y % 8;
     n = 1 << m;
     if (t)
     {
         OLED_GRAM[x][i] |= n;
     }
     else
     {
         OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
         OLED_GRAM[x][i] |= n;
         OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
     }
 }
 
 /**
  * @brief Draw a line on the OLED display
  * @param x1 The starting x-coordinate
  * @param y1 The starting y-coordinate
  * @param x2 The ending x-coordinate
  * @param y2 The ending y-coordinate
  * @param mode The drawing mode (1 for on, 0 for off)
  * @details This function draws a line on the OLED display from the starting coordinates to the ending coordinates.
  */
 void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2, u8 mode)
 {
     uint16_t t;
     int xerr = 0, yerr = 0, delta_x, delta_y, distance;
     int incx, incy, uRow, uCol;
     delta_x = x2 - x1; // Calculate the difference in x-coordinates
     delta_y = y2 - y1;
     uRow = x1; // Set the starting x-coordinate
     uCol = y1;
     if (delta_x > 0)
         incx = 1; // Set the increment for x
     else if (delta_x == 0)
         incx = 0; // No increment for x
     else
     {
         incx = -1;
         delta_x = -delta_x;
     }
     if (delta_y > 0)
         incy = 1;
     else if (delta_y == 0)
         incy = 0; // No increment for y
     else
     {
         incy = -1;
         delta_y = -delta_y;
     }
     if (delta_x > delta_y)
         distance = delta_x; // Calculate the distance
     else
         distance = delta_y;
     for (t = 0; t < distance + 1; t++)
     {
         OLED_DrawPoint(uRow, uCol, mode); // Draw the point
         xerr += delta_x;
         yerr += delta_y;
         if (xerr > distance)
         {
             xerr -= distance;
             uRow += incx;
         }
         if (yerr > distance)
         {
             yerr -= distance;
             uCol += incy;
         }
     }
 }
 
 /**
  * @brief Draw a circle on the OLED display
  * @param x The x-coordinate of the center
  * @param y The y-coordinate of the center
  * @param r The radius of the circle
  * @details This function draws a circle on the OLED display with the specified center and radius.
  */
 void OLED_DrawCircle(u8 x, u8 y, u8 r)
 {
     int a, b, num;
     a = 0;
     b = r;
     while (2 * b * b >= r * r)
     {
         OLED_DrawPoint(x + a, y - b, 1);
         OLED_DrawPoint(x - a, y - b, 1);
         OLED_DrawPoint(x - a, y + b, 1);
         OLED_DrawPoint(x + a, y + b, 1);
 
         OLED_DrawPoint(x + b, y + a, 1);
         OLED_DrawPoint(x + b, y - a, 1);
         OLED_DrawPoint(x - b, y - a, 1);
         OLED_DrawPoint(x - b, y + a, 1);
 
         a++;
         num = (a * a + b * b) - r * r; // Calculate the error
         if (num > 0)
         {
             b--;
             a--;
         }
     }
 }
 
 /**
  * @brief Initialize the OLED display
  * @details This function initializes the OLED display by setting up the necessary commands and configurations.
  */
 void OLED_Init(void)
 {
     OLED_RST_Set();
     delay_cycles(CPU_Frq * 100);
     OLED_RST_Clr();
     delay_cycles(CPU_Frq * 100);
     OLED_RST_Set();
 
     OLED_WR_Byte(0xAE, OLED_CMD); // Turn off OLED panel
     OLED_WR_Byte(0x02, OLED_CMD); // Set low column address
     OLED_WR_Byte(0x10, OLED_CMD); // Set high column address
     OLED_WR_Byte(0x40, OLED_CMD); // Set start line address
     OLED_WR_Byte(0x81, OLED_CMD); // Set contrast control register
     OLED_WR_Byte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
     OLED_WR_Byte(0xA1, OLED_CMD); // Set SEG/Column Mapping
     OLED_WR_Byte(0xC8, OLED_CMD); // Set COM/Row Scan Direction
    OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
	

 }


//#if OLED_MODE==1
////鍚慡SD1106鍐欏叆涓?涓瓧鑺傘??
////dat:瑕佸啓鍏ョ殑鏁版嵁/鍛戒护
////cmd:鏁版嵁/鍛戒护鏍囧織 0,琛ㄧず鍛戒护;1,琛ㄧず鏁版嵁;
//void OLED_WR_Byte(u8 dat,u8 cmd)
//{
//	DATAOUT(dat);	    
//	if(cmd)
//	  OLED_DC_Set();
//	else 
//	  OLED_DC_Clr();		   
//	OLED_CS_Clr();
//	OLED_WR_Clr();	 
//	OLED_WR_Set();
//	OLED_CS_Set();	  
//	OLED_DC_Set();	 
//} 	    	    
//#else
////鍚慡SD1306鍐欏叆涓?涓瓧鑺傘??
////dat:瑕佸啓鍏ョ殑鏁版嵁/鍛戒护
////cmd:鏁版嵁/鍛戒护鏍囧織 0,琛ㄧず鍛戒护;1,琛ㄧず鏁版嵁;
//void OLED_WR_Byte(u8 dat,u8 cmd)
//{	
//	u8 i;			  
//	if(cmd)
//	  OLED_DC_Set();
//	else 
//	  OLED_DC_Clr();		  
//	OLED_CS_Clr();
//	for(i=0;i<8;i++)
//	{			  
//		OLED_SCLK_Clr();
//		if(dat&0x80)
//			{
//		   OLED_SDIN_Set();
//			}
//else
//		   OLED_SDIN_Clr();
//				OLED_SCLK_Set();
//		dat<<=1;   
//	}				 		  
//	OLED_CS_Set();
//	OLED_DC_Set();   	  
//} 
//#endif
//	void OLED_Set_Pos(unsigned char x, unsigned char y) 
//{ 
//	OLED_WR_Byte(0xb0+y,OLED_CMD);
//	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
//	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
//}   	  
////寮?鍚疧LED鏄剧ず
//void OLED_Display_On(void)
//{
//	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC鍛戒护
//	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
//	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
//}
////鍏抽棴OLED鏄剧ず
//void OLED_Display_Off(void)
//{
//	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC鍛戒护
//	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
//	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
//}		   			 
////娓呭睆鍑芥暟,娓呭畬灞?,鏁翠釜灞忓箷鏄粦鑹茬殑!鍜屾病鐐逛寒涓?鏍?!!!
//void OLED_Clear(void)  
//{  
//	u8 i,n;		    
//	for(i=0;i<8;i++)  
//	{  
//		OLED_WR_Byte (0xb0+i,OLED_CMD);    //璁剧疆椤靛湴鍧?锛?0~7锛?
//		OLED_WR_Byte (0x02,OLED_CMD);      //璁剧疆鏄剧ず浣嶇疆鈥斿垪浣庡湴鍧?
//		OLED_WR_Byte (0x10,OLED_CMD);      //璁剧疆鏄剧ず浣嶇疆鈥斿垪楂樺湴鍧?
//		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
//	} //鏇存柊鏄剧ず
//}


////鍦ㄦ寚瀹氫綅缃樉绀轰竴涓瓧绗?,鍖呮嫭閮ㄥ垎瀛楃
////x:0~127
////y:0~63
////mode:0,鍙嶇櫧鏄剧ず;1,姝ｅ父鏄剧ず
////size:閫夋嫨瀛椾綋 16/12
//void OLED_ShowChar(u8 x,u8 y,u8 chr)
//{      	
//	unsigned char c=0,i=0;	
//		c=chr-' ';//寰楀埌鍋忕Щ鍚庣殑鍊?
//		if(x>Max_Column-1){x=0;y=y+2;}
//		if(SIZE ==16)
//			{
//			OLED_Set_Pos(x,y);	
//			for(i=0;i<8;i++)
//			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
//			OLED_Set_Pos(x,y+1);
//			for(i=0;i<8;i++)
//			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
//			}
//			else {	
//				OLED_Set_Pos(x,y+1);
//				for(i=0;i<6;i++)
//				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
//				
//			}
//}
////m^n鍑芥暟
//u32 oled_pow(u8 m,u8 n)
//{
//	u32 result=1;	 
//	while(n--)result*=m;    
//	return result;
//}				  
////鏄剧ず2涓暟瀛?
////x,y :璧风偣鍧愭爣
////len :鏁板瓧鐨勪綅鏁?
////size:瀛椾綋澶у皬
////mode:妯″紡	0,濉厖妯″紡;1,鍙犲姞妯″紡
////num:鏁板??(0~4294967295);
//void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
//{         	
//	u8 t,temp;
//	u8 enshow=0;						   
//	for(t=0;t<len;t++)
//	{
//		temp=(num/oled_pow(10,len-t-1))%10;
//		if(enshow==0&&t<(len-1))
//		{
//			if(temp==0)
//			{
//				OLED_ShowChar(x+(size2/2)*t,y,' ');
//				continue;
//			}else enshow=1; 
//		 	 
//		}
//	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0'); 
//	}
//} 
////鏄剧ず涓?涓瓧绗﹀彿涓?
//void OLED_ShowString(u8 x,u8 y,u8 *chr)
//{
//	unsigned char j=0;
//	while (chr[j]!='\0')
//	{		OLED_ShowChar(x,y,chr[j]);
//			x+=8;
//		if(x>120){x=0;y+=2;}
//			j++;
//	}
//}
////鏄剧ず姹夊瓧
//void OLED_ShowCHinese(u8 x,u8 y,u8 no)
//{      			    
//	u8 t,adder=0;
//	OLED_Set_Pos(x,y);	
//    for(t=0;t<16;t++)
//		{
//				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
//				adder+=1;
//     }	
//		OLED_Set_Pos(x,y+1);	
//    for(t=0;t<16;t++)
//			{	
//				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
//				adder+=1;
//      }					
//}

//void OLED_Show_int32(u8 x,u8 y,int32_t num,u8 len,u8 size2)//瀹為檯鏄剧ず浣嶆暟姣旇緭鍏ヤ綅鏁板1锛岀敤浜庣鍙锋樉绀?
//{
//	
//	if(num<0)
//	{
//    OLED_ShowChar(x,y,'-');
//		OLED_ShowNum(x+8,y,-num,len,size2);	
//	}
//		else
//		{
//			OLED_ShowNum(x,y,num,len+1,size2);
//		}
//}
//void OLED_Show_float(u8 x,u8 y,float num,u8 len_1,u8 len_2)//len_1涓烘暣鏁颁綅鏁帮紝len_2涓哄皬鏁颁綅鏁?,瀹為檯鏄剧ず浣嶆暟涓簂en_1+len_2+2
//{
//	if(num<0)
//	{
//    OLED_ShowChar(x,y,'-');
//	  OLED_ShowNum(x+8,y,-num,len_1,16);
//    OLED_ShowChar(x+len_1*8+8,y,'.');
//	  OLED_ShowNum(x+len_1*8+16,y,-(num-(int32_t)num)*oled_pow(10,len_2),len_2,16);
//	}
//		else
//		{			
//			OLED_ShowNum(x,y,num,len_1+1,16);
//			OLED_ShowChar(x+len_1*8+8,y,'.');
//			OLED_ShowNum(x+len_1*8+16,y,(num-(int32_t)num)*oled_pow(10,len_2),len_2,16);
//		}
//}
///***********鍔熻兘鎻忚堪锛氭樉绀烘樉绀築MP鍥剧墖128脳64璧峰鐐瑰潗鏍?(x,y),x鐨勮寖鍥?0锝?127锛寉涓洪〉鐨勮寖鍥?0锝?7*****************/
//void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
//{ 	
// unsigned int j=0;
// unsigned char x,y;
//  
//  if(y1%8==0) y=y1/8;      
//  else y=y1/8+1;
//	for(y=y0;y<y1;y++)
//	{
//		OLED_Set_Pos(x0,y);
//    for(x=x0;x<x1;x++)
//	    {      
//	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
//	    }
//	}
//} 


////鍒濆鍖朣SD1306
//void OLED_Init(void)
//{
// 

// 
//  OLED_RST_Set();
//	system_delay_ms(100);
//	OLED_RST_Clr();
//	system_delay_ms(100);
//	OLED_RST_Set(); 
//	/*				  
//	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
//	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
//	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
//	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
//	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
//	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
//	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0宸﹀彸鍙嶇疆 0xa1姝ｅ父
//	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0涓婁笅鍙嶇疆 0xc8姝ｅ父
//	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
//	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
//	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
//	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
//	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
//	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
//	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
//	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
//	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
//	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
//	OLED_WR_Byte(0x12,OLED_CMD);
//	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
//	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
//	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
//	OLED_WR_Byte(0x02,OLED_CMD);//
//	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
//	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
//	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
//	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
//	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
//	*/

//	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
//	OLED_WR_Byte(0x02,OLED_CMD);//---set low column address
//	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
//	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
//	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
//	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
//	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0宸﹀彸鍙嶇疆 0xa1姝ｅ父
//	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0涓婁笅鍙嶇疆 0xc8姝ｅ父
//	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
//	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
//	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
//	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
//	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
//	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
//	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
//	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
//	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
//	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
//	OLED_WR_Byte(0x12,OLED_CMD);
//	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
//	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
//	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
//	OLED_WR_Byte(0x02,OLED_CMD);//
//	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
//	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
//	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
//	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
//	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
//	
//	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
//	OLED_Clear();
//	OLED_Set_Pos(0,0); 	
//}  
