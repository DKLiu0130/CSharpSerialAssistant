/*
 * @file MusicPlayer.c
 * @brief Buzzer control for music playback
 * @author Ldk, InnoLegend team.
 * @date Mar.21st,2024
 * @details This file contains functions to control the buzzer for playing music notes.
 */

 #include "MusicPlayer.h"

 /**
  * @brief Initialize the music player
  * @details This function initializes the timer and enables the interrupt for the PWM instance.
  */
 void MusicPlayer_init(){
	 DL_Timer_startCounter(PWM_0_INST);
	 NVIC_EnableIRQ(PWM_0_INST_INT_IRQN);
	 BuzzON(0, 0, 0);
 }
 
 /**
  * @brief Turn on the buzzer
  * @param Period The period of the PWM signal
  * @param CCPvalue The capture/compare value for the PWM signal
  * @param length The duration for which the buzzer should be on, in milliseconds
  * @details This function sets the PWM signal to turn on the buzzer for a specified duration.
  */
 void BuzzON(uint16_t Period, uint16_t CCPvalue, size_t length){
	 // length_Unit: ms
	 if(length <= 0) length = 1;
	 if(Period <= 0) Period = CCPvalue = 1;
	 DL_Timer_setLoadValue(PWM_0_INST, Period);
	 DL_Timer_setCaptureCompareValue(PWM_0_INST, CCPvalue, DL_TIMER_CC_0_INDEX);
	 DL_Common_delayCycles(CPU_Frq * length); 
 }
 
 /**
  * @brief Turn off the buzzer
  * @param length The duration for which the buzzer should be off, in milliseconds
  * @details This function sets the PWM signal to turn off the buzzer for a specified duration.
  */
 void BuzzOFF(size_t length){
	 // Unit: ms
	 DL_Timer_setLoadValue(PWM_0_INST, 1);
	 DL_Timer_setCaptureCompareValue(PWM_0_INST, 1, DL_TIMER_CC_0_INDEX);
	 DL_Common_delayCycles(CPU_Frq * length);
 }
 
 /**
  * @brief Play a sequence of music notes
  * @param Score Array of music notes to be played
  * @param ScoreLength The number of notes in the Score array
  * @details This function plays a sequence of music notes by turning the buzzer on and off for each note.
  */
 void playMusic(struct MusicNote Score[], uint16_t ScoreLength){
	 for(uint16_t i = 0; i < ScoreLength; ++i){
		 BuzzON(Score[i].Frq, Score[i].Frq / 2, Score[i].length);
		 BuzzOFF(Score[i].length);
	 }
 }
 
 /**
  * @brief Play a specific range of music notes
  * @param Score Array of music notes to be played
  * @param ScoreLength The number of notes in the Score array
  * @param from The starting index of the notes to be played
  * @param to The ending index of the notes to be played
  * @details This function plays a specific range of music notes by turning the buzzer on for each note.
  */
 void playSpScoreNote(struct MusicNote Score[], uint16_t ScoreLength, uint16_t from, uint16_t to){
	 if(to > ScoreLength) to = ScoreLength;
	 for(uint16_t i = from; i <= to; ++i){
		 BuzzON(Score[i].Frq, Score[i].Frq / 2, Score[i].length);
		 //BuzzOFF(Score[i].length);
	 }
 }
 
 /**
  * @brief Emit a simple beep sound
  * @param Period The period of the PWM signal
  * @param Delaylength The duration of the beep, in milliseconds
  * @details This function emits a simple beep sound by setting the PWM signal for a specified duration.
  */
 void Beep(uint16_t Period, uint16_t Delaylength){
	 DL_Timer_setLoadValue(PWM_0_INST, Period);
	 DL_Timer_setCaptureCompareValue(PWM_0_INST, Period / 2, DL_TIMER_CC_0_INDEX);
	 delay_cycles(CPU_Frq * Delaylength);
	 DL_Timer_setLoadValue(PWM_0_INST, 1);
	 DL_Timer_setCaptureCompareValue(PWM_0_INST, 1, DL_TIMER_CC_0_INDEX);
 }
 
 /**
  * @brief Emit two consecutive beep sounds
  * @details This function emits two consecutive beep sounds with a short delay in between.
  */
 void Beep2(){
	 Beep(800, 500);
	 delay_cycles(CPU_Frq * 100);
	 Beep(800, 500);
 }
 
 /**
  * @brief Emit a series of beeps with increasing frequency
  * @param Period The initial period of the PWM signal
  * @param step The step by which the period decreases for each beep
  * @param length The number of beeps
  * @param Delaylength The duration of each beep, in milliseconds
  * @details This function emits a series of beeps with increasing frequency by decreasing the period of the PWM signal for each beep.
  */
 void BeepUp(uint16_t Period, uint16_t step, uint16_t length, uint16_t Delaylength){
	 for(uint16_t i = 0; i < length; ++i){
		 Beep(Period - i * step, Delaylength);
		 delay_cycles(CPU_Frq * Delaylength);
	 }
 }
 
 /**
  * @brief Emit a series of beeps with decreasing frequency
  * @param Period The initial period of the PWM signal
  * @param step The step by which the period increases for each beep
  * @param length The number of beeps
  * @param Delaylength The duration of each beep, in milliseconds
  * @details This function emits a series of beeps with decreasing frequency by increasing the period of the PWM signal for each beep.
  */
 void BeepDown(uint16_t Period, uint16_t step, uint16_t length, uint16_t Delaylength){
	 for(uint16_t i = 0; i < length; ++i){
		 Beep(Period + i * step, Delaylength);
		 delay_cycles(CPU_Frq * Delaylength);
	 }
 }