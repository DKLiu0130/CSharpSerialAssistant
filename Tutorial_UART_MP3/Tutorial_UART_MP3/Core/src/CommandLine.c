/*
 * @file CommandLine.c
 * @brief Command line simulation
 * @details This file contains functions to simulate a command line interface.
 * @date Mar.21st, 2024
 * @author Ldk, InnoLegend team.
 */

 #include "CommandLine.h"

 #define DEFAULT_COMMAND_LENGTH 50
 
 static uint16_t Wcnt = 0, CmdNumber = 0, nowSize = DEFAULT_COMMAND_LENGTH;
 static char *CommandLine;
 
 /**
  * @brief Convert a string to uppercase
  * @param str The string to convert
  * @details This function converts all characters in the string to uppercase.
  */
 inline void UpperAlphabet(char *str) {
	 for (int i = 0; str[i] != '\0'; ++i) str[i] = toupper(str[i]);
 }
 
 /**
  * @brief Double the size of a string
  * @param content The string to double in size
  * @return A pointer to the new string with doubled size
  * @details This function doubles the size of the given string by allocating new memory and copying the content.
  */
 inline char* DoubleSize(char *content) {
	 uint16_t len = strlen(content);
	 char *newp = (char*)malloc(2 * len * sizeof(char));
	 for (uint16_t i = 0; i < len; ++i) {
		 newp[i] = content[i];
	 }
	 free(content);
	 return newp;
 }
 
 /**
  * @brief Initialize the command line
  * @details This function allocates memory for the command line and initializes it.
  */
 void CommandLineON() {
	 CommandLine = (char*)malloc(DEFAULT_COMMAND_LENGTH * sizeof(char));
	 memset(CommandLine, 0, DEFAULT_COMMAND_LENGTH * sizeof(char));
 }
 
 /**
  * @brief Deinitialize the command line
  * @details This function frees the memory allocated for the command line.
  */
 void CommandLineOFF() {
	 free(CommandLine);
 }
 
 /**
  * @brief Analyze the command line input
  * @param CommandLine The command line input to analyze
  * @return The command number corresponding to the input
  * @details This function analyzes the command line input and returns the corresponding command number.
  */
 uint16_t AnalyseCmd(char* CommandLine) {
	 uint16_t CodeCmdNumber = 0;
	 if (strstr(CommandLine, "1")) return 1;
	 if (strstr(CommandLine, "2")) return 2;
	 if (strstr(CommandLine, "3")) return 3;
	 if (strstr(CommandLine, "4")) return 4;
	 if (strstr(CommandLine, "5")) return 5;
	 if (strstr(CommandLine, "6")) return 6;
	 return CodeCmdNumber;
 }
 
 /**
  * @brief Embed a character into the command line
  * @param data The character to embed
  * @return The command number if a complete command is detected, otherwise 0
  * @details This function embeds a character into the command line and analyzes the command if a newline character is detected.
  */
 uint16_t embedding(char *data) {
	 char key = *data;
	 if (Wcnt == 0 && key == ' ') return 0; // Prevent former space
	 if (Wcnt > 0 && CommandLine[Wcnt - 1] == ' ' && key == ' ') return 0; // Prevent long space between words
	 if (key != '\n') {
		 CommandLine[Wcnt] = key;
		 ++Wcnt;
	 } // Push_back words
	 if (Wcnt == nowSize) {
		 nowSize *= 2;
		 CommandLine = DoubleSize(CommandLine);
	 }
	 if (Wcnt > 0 && key == '\n') {
		 CommandLine[Wcnt] = '\0';
		 ++Wcnt;
		 CmdNumber = AnalyseCmd(CommandLine);
		 Wcnt = 0;
		 memset(CommandLine, 0, nowSize * sizeof(char));
		 return CmdNumber;
	 }
	 return 0;
 }
 
 /*
 Old Cmd Deconstructing Method:
	 CodeCmdNumber: [00000000 00000000];
	 The first 4 bits use to locate command set;
	 The residual 12 bits are order number in the specific located command set;
	 In the 12 bits, the left 4 bits are the First Level Directories,
	 while the middle 4 bits are second level,
	 and the right 4 bits are third level.
	 Directories:
		 0. Help Manual;
		 1. OLED Command;
		 2. Sound Command;
			 2.1. play/pause;
			 2.2. song list;
			 2.3. set to play specific song;
			 2.4. Volume set;
		 3. Game Command:
			 3.1. keyboard sensitivity;
			 3.2. game difficulty;
 Programme:
 if (strstr(CommandLine, "/")) {
	 // Command_Side;
	 UpperAlphabet(CommandLine);
	 if (strstr(CommandLine, "HELP")) {
		 return 1;
	 }
	 if (strstr(CommandLine, "GAME")) {
		 CodeCmdNumber += 3 << 8;
		 if (strstr(CommandLine, "KEYBOARD") || strstr(CommandLine, "KST")) { // KST: KEYBOARD SENSITIVITY SETTING;
			 CodeCmdNumber += 1 << 4;
			 if (strstr(CommandLine, "0")) { CodeCmdNumber += 1; return CodeCmdNumber; }
			 if (strstr(CommandLine, "1")) { CodeCmdNumber += 2; return CodeCmdNumber; }
			 if (strstr(CommandLine, "2")) { CodeCmdNumber += 3; return CodeCmdNumber; }
			 if (strstr(CommandLine, "3")) { CodeCmdNumber += 4; return CodeCmdNumber; }
			 if (strstr(CommandLine, "4")) { CodeCmdNumber += 5; return CodeCmdNumber; }
		 } else if (strstr(CommandLine, "DIFFICULTY") || strstr(CommandLine, "GDS")) {
			 CodeCmdNumber += 2 << 4;
			 if (strstr(CommandLine, "0")) { CodeCmdNumber += 1; return CodeCmdNumber; }
			 if (strstr(CommandLine, "1")) { CodeCmdNumber += 2; return CodeCmdNumber; }
			 if (strstr(CommandLine, "2")) { CodeCmdNumber += 3; return CodeCmdNumber; }
			 if (strstr(CommandLine, "3")) { CodeCmdNumber += 4; return CodeCmdNumber; }
			 if (strstr(CommandLine, "4")) { CodeCmdNumber += 5; return CodeCmdNumber; }
		 }
	 } else if (strstr(CommandLine, "MUSIC")) {
		 CodeCmdNumber += 2 << 8;
		 if (strstr(CommandLine, "SET") || strstr(CommandLine, "SCORE")) {
			 CodeCmdNumber += 3 << 4;
			 if (strstr(CommandLine, "MEGALOVANIA") || strstr(CommandLine, "1")) { CodeCmdNumber += 1; return CodeCmdNumber; }
			 if (strstr(CommandLine, "SAKURA") || strstr(CommandLine, "2")) { CodeCmdNumber += 2; return CodeCmdNumber; }
			 if (strstr(CommandLine, "KAMI") || strstr(CommandLine, "3")) { CodeCmdNumber += 3; return CodeCmdNumber; }
		 } else if (strstr(CommandLine, "PLAY") || strstr(CommandLine, "PAUSE")) {
			 CodeCmdNumber += 1 << 4; return CodeCmdNumber;
		 } else if (strstr(CommandLine, "LIST")) {
			 CodeCmdNumber += 2 << 4; return CodeCmdNumber;
		 }
	 }
 } else {
	 // Message_Side;
	 CodeCmdNumber += 1 << 12;
 }
 */