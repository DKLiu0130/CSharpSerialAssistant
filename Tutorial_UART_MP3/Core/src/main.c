/*
 * @file main.c
 * @brief 
 *		UART:
 *			Receive command and play music;
 *			Transmit the botton value user have input.
 * 			Notice that the Msg will automatically send out when you input 16 numbers;
 *			KeyBoard:
 *				Button Numbers:
 *				| 1 | 5 | 9  | 13 |
 *				| 2 | 6 | 10 | 14 |
 *				| 3 | 7 | 11 | 15 |
 *				| 4 | 8 | 12 | 16 |
 * 				Button Meanings:
 *				| 7 | 8 | 9 | backspace |
 *				| 4 | 5 | 6 |           |
 *				| 1 | 2 | 3 |           |
 *				|   | 0 |   | sendMsg   |
 * @details This program receives commands via UART to play music and transmits button values input by the user.
 * @date Feb.25th, 2025
 * @author Ldk, InnoLegend team.
 */

#include "ti_msp_dl_config.h"
#include "Keyboard.h"
#include "oled_spi_V0.2.h"
#include "oledpicture.h"
#include "MusicPlayer.h"
#include "MusicScore.h"
#include "eeprom_emulation_type_a.h"
#include "UART.h"
#include "CommandLine.h"
#include <math.h>

//Definitions&Variables:
#define CPU_Frq 80000	//Unit:kHz;
#define CPU_Frq_MHz 80	//Unit:MHz;
#define PI 3.1415926

//Simulated EEPROM on Flash:
uint32_t EEPROMEmulationState;
uint32_t EEPROMEmulationBuffer[EEPROM_EMULATION_DATA_SIZE / sizeof(uint32_t)];

//UART:
#define TxLength 16
uint8_t TxMsg[TxLength],InCTL = 0;
uint16_t Cmd = 0;

//Functions:
void Initialization();
void other_value_input(uint8_t key_value);

//EEPROM:
void SaveData(uint32_t * State);
void LoadData();

//MusicPlayer:
void BeepWarning();

uint8_t key_value = 0;

/**
 * @brief The main function
 * @note Transmit the button value user have input. 
 *       Receive command and play music.
 */
int main(void)
{
	
		Initialization();
		
		while(1)
		{
			//printf("HelloWorld!");
			key_value = 0;  
			key_value = KeySCInput();
			delay_cycles(32000000);	    //Delay 0.4s;
		    OLED_ShowNum(0,0,key_value,2,12);
			
			// Backspace
			if (key_value == 13 && InCTL > 0) 
			{
				--InCTL;
				OLED_Clear();
			}
			
			// Send message if press "sendMsg". Relocation function printf();
			if (key_value == 16 && InCTL > 0)
			{
				OLED_ShowString(0,4,"Message Sent!");
				delay_cycles(16000000);	//Delay 0.2s;
				printf("Password:");
				for(uint8_t i = 0;i < InCTL;++i)
				{
					printf("%d",TxMsg[i]);
				}
				printf("\n");
				OLED_Clear();
				InCTL = 0;
			}

			// Send message if input 16 numbers.
			if (InCTL == 16)
			{
				OLED_ShowString(0,4,"Message Sent!");
				delay_cycles(16000000);	//Delay 0.2s;
				printf("Password:");
				for(uint8_t i = 0;i < InCTL;++i)
				{
					printf("%d",TxMsg[i]);
				}
				printf("\n");
				OLED_Clear();
				InCTL = 0;
			}

			// Other buttons input.
			other_value_input(key_value);
			
			// Show the input message.
			for(uint8_t i = 0;i < InCTL;++i) 
			{
				OLED_ShowNum(6*i,1,TxMsg[i],1,12);
			}

			// Play music according to the password.
			if (Cmd == 1) 
				Cmd = 0,playMusic(Sakura,244);
			if (Cmd == 2) 
				Cmd = 0,playMusic(MEGALOVANIA,206);
			if (Cmd == 3) 
				Cmd = 0,playMusic(KAMI,113);
			if (Cmd == 4) 
				Cmd = 0,playMusic(SkyWeakness,92);
			if (Cmd == 5) 
				Cmd = 0,playMusic(NightOfNights,95);
			if (Cmd == 6) 
				Cmd = 0,playMusic(FunkyStar,75);
		}
		while(1){__WFI();}
		
}

/**
 * @brief Handle other button inputs
 * @param key_value The value of the key pressed
 * @details This function handles the input from other buttons and updates the TxMsg array.
 */
void other_value_input(uint8_t key_value)
{
	if (key_value == 1)
	{
		TxMsg[InCTL] = 7;
		++InCTL;
	}
	if (key_value == 2)
	{
		TxMsg[InCTL] = 4;
		++InCTL;
	}
	if (key_value == 3)
	{
		TxMsg[InCTL] = 1;
		++InCTL;
	}
	if (key_value == 5)
	{
		TxMsg[InCTL] = 8;
		++InCTL;
	}
	if (key_value == 6)
	{
		TxMsg[InCTL] = 5;
		++InCTL;
	}
	if (key_value == 7)
	{
		TxMsg[InCTL] = 2;
		++InCTL;
	}
	if (key_value == 8)
	{
		TxMsg[InCTL] = 0;
		++InCTL;
	}
	if (key_value == 9)
	{
		TxMsg[InCTL] = 9;
		++InCTL;
	}
	if (key_value == 10)
	{
		TxMsg[InCTL] = 6;
		++InCTL;
	}
	if (key_value == 11)
	{
		TxMsg[InCTL] = 3;
		++InCTL;
	}
}


/**
 * @brief Initialize the system
 * @details This function initializes the MCU, storage, keyboard, OLED, buzzer, UART, and command line.
 */
void Initialization()
{
    SYSCFG_DL_init(); 							//Initialize MCU;
	EEPROMEmulationState = EEPROM_TypeA_init(&EEPROMEmulationBuffer[0]);	//Initialize Storage;
    Keyboard_init();							//Initialize Keyboard;
    OLED_Init();								//Initialize OLED;
    MusicPlayer_init();							//Initialize Buzzer;
	UART_init();								//Initialize UART;
	OLED_DrawBMP(9,0,119,8,Genshin);			//LOGO;
	delay_cycles(CPU_Frq*1000);
	OLED_Clear();
	CommandLineON();							//Initialize complicated uart interaction;
	BeepWarning();
}

/**
 * @brief Emit a warning beep sound
 * @details This function emits a warning beep sound using the buzzer.
 */
void BeepWarning()
{
	BeepUp(800,50,3,100);
	BeepUp(800,50,3,100);
}

/**
 * @brief UART interrupt handler
 * @details This function handles the UART interrupt and updates the command based on received data.
 */
void UART_0_INST_IRQHandler()
{
	volatile uint8_t data;
	switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)){
		case  DL_UART_MAIN_IIDX_RX:
			data = DL_UART_Main_receiveDataBlocking(UART_0_INST);
			Cmd = embedding(&data);
			DL_UART_clearInterruptStatus(UART_0_INST,DL_UART_INTERRUPT_RX);
			break;
	default:
		break;
    }
}

/*
void SysTick_Handler(){                                 
	//Milisecond Source
	SysTick->CTRL &= ~(1<<16);
}


//EEPROM:
void SaveData(uint32_t * State){
	//uint32_t data[EEPROM_EMULATION_DATA_SIZE / sizeof(uint32_t)];
	if (gEEPROMTypeAEraseFlag == 1) {
		// In this demo, when the sector is full, it will be erased immediately
		EEPROM_TypeA_eraseLastSector();
		gEEPROMTypeAEraseFlag = 0;
	}
	//EEPROM_TypeA_eraseLastSector();
	
	//Note: Starting from 1;
	//EEPROMEmulationBuffer[1] = [Contents];
	//EEPROMEmulationBuffer[1] = utick;
	
	
	//*State = EEPROM_TypeA_writeData(data);
	*State = EEPROM_TypeA_writeData(EEPROMEmulationBuffer);
	if (*State != EEPROM_EMULATION_WRITE_OK) {
		__BKPT(0);
	}
}
	
void LoadData(){
	EEPROM_TypeA_readData(EEPROMEmulationBuffer);
	//[Contents] = EEPROMEmulationBuffer[1]...;
	//utick = EEPROMEmulationBuffer[1];
	
}
*/


