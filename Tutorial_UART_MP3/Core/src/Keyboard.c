/*
 * @file Keyboard.c
 * @brief Keyboard scanning and initialization
 * @details This file contains functions to initialize and scan the keyboard on the mspm0G3507.
 *      PA 0,1,7,12 : Signal receiver as KR(KeyboardReceiver);
 *		PA 13,14,17,18 : Signal Emitter as KE(KeyboardEmitter);
 * @date Mar.21st, 2024
 * @author Ldk, InnoLegend team.
 */

 #include "Keyboard.h"

 /**
  * @brief Initialize the keyboard
  * @details This function sets the pins for the keyboard emitters.
  */
 void Keyboard_init(){
	 DL_GPIO_setPins(Keyboard_PORT, Keyboard_KE_1_PIN);
	 DL_GPIO_setPins(Keyboard_PORT, Keyboard_KE_2_PIN);
	 DL_GPIO_setPins(Keyboard_PORT, Keyboard_KE_3_PIN);
	 DL_GPIO_setPins(Keyboard_PORT, Keyboard_KE_4_PIN);
 }
 
 /**
  * @brief Read the state of a button
  * @param B The pin number of the button to read
  * @return true if the button is not pressed, false if the button is pressed
  * @details This function reads the state of a button and returns whether it is pressed or not.
  */
 inline bool readButton(size_t B){
	 if(DL_GPIO_readPins(Keyboard_PORT, B) == 0){
		 if (delay_level != 0) DL_Common_delayCycles(CPU_Frq * delay_level);
		 if(DL_GPIO_readPins(Keyboard_PORT, B) == 0){
			 return 0;
		 }
	 }
	 return 1;
 }
 
 /**
  * @brief Scan the keyboard for input
  * @return The value of the pressed key
  * @details This function scans the keyboard and returns the value of the pressed key.
  */
 uint16_t KeySCInput(){
	 // delay_level Unit: ms
	 int x, y, cvt;
	 bool flag;
	 flag = 1; cvt = x = y = 0;
	 DL_GPIO_clearPins(Keyboard_PORT, Keyboard_KE_1_PIN);
	 DL_Common_delayCycles(3200);
	 x = 1;
	 if(readButton(Keyboard_KR_1_PIN) == 0) y = 1, flag = 0;
	 if(readButton(Keyboard_KR_2_PIN) == 0) y = 2, flag = 0;
	 if(readButton(Keyboard_KR_3_PIN) == 0) y = 3, flag = 0;
	 if(readButton(Keyboard_KR_4_PIN) == 0) y = 4, flag = 0;
	 DL_GPIO_setPins(Keyboard_PORT, Keyboard_KE_1_PIN);
	 if (flag){
		 DL_GPIO_clearPins(Keyboard_PORT, Keyboard_KE_2_PIN);
		 DL_Common_delayCycles(3200);
		 x = 2;
		 if(readButton(Keyboard_KR_1_PIN) == 0) y = 1, flag = 0;
		 if(readButton(Keyboard_KR_2_PIN) == 0) y = 2, flag = 0;
		 if(readButton(Keyboard_KR_3_PIN) == 0) y = 3, flag = 0;
		 if(readButton(Keyboard_KR_4_PIN) == 0) y = 4, flag = 0;
		 DL_GPIO_setPins(Keyboard_PORT, Keyboard_KE_2_PIN);
	 }
	 if (flag){
		 DL_GPIO_clearPins(Keyboard_PORT, Keyboard_KE_3_PIN);
		 DL_Common_delayCycles(3200);
		 x = 3;
		 if(readButton(Keyboard_KR_1_PIN) == 0) y = 1, flag = 0;
		 if(readButton(Keyboard_KR_2_PIN) == 0) y = 2, flag = 0;
		 if(readButton(Keyboard_KR_3_PIN) == 0) y = 3, flag = 0;
		 if(readButton(Keyboard_KR_4_PIN) == 0) y = 4, flag = 0;
		 DL_GPIO_setPins(Keyboard_PORT, Keyboard_KE_3_PIN);
	 }
	 if (flag){
		 DL_GPIO_clearPins(Keyboard_PORT, Keyboard_KE_4_PIN);
		 DL_Common_delayCycles(3200);
		 x = 4;
		 if(readButton(Keyboard_KR_1_PIN) == 0) y = 1, flag = 0;
		 if(readButton(Keyboard_KR_2_PIN) == 0) y = 2, flag = 0;
		 if(readButton(Keyboard_KR_3_PIN) == 0) y = 3, flag = 0;
		 if(readButton(Keyboard_KR_4_PIN) == 0) y = 4, flag = 0;
		 DL_GPIO_setPins(Keyboard_PORT, Keyboard_KE_4_PIN);
	 }
	 if(flag == 0) cvt = (x - 1) * 4 + y;
	 return cvt;
 }