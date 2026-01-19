/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2026
* Author: David Jacob
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (W26)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@       @@@@        @@@@      @@    ”
# ECHO "    @@       @@@@        @@@@      @@    ”
# ECHO "    @@       @@@@        @@@@      @@    ”
# ECHO "    @@       @@@@        @@@@      @@    ”
# ECHO "    @@       @@@@        @@@@      @@    ”
# ECHO "    @@       @@@@        @@@@      @@    ”
# ECHO "    @@       @@@@        @@@@      @@    ”
# ECHO "    @@       @@@@@@@@@@@@@@@@      @@    ”
# ECHO "    @@        @@@@@@@@@@@@@@       @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@         U R I Z E N         @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "                                         "
# ECHO "[CODER SCRIPT ..........................]"
# ECHO "                                         "
*/

/*
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2026
* Course: CST 8152 – Compilers, Lab Section: [301]
* Assignment: A12.
* Date: Jan 31 2026
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

// Function to perform the Vigenère cipher (encoding or decoding)
void vigenereFile(const urizen_str inputFileName, const urizen_str outputFileName, const urizen_str key, urizen_int encode) {
	// TO_DO: Define the input and output files (ex: FILE* inputFile, FILE* outputFile
	// TO_DO: Use defensive programming (checking files)
	// TO_DO: Define local variables
	// TO_DO: Logic: check if it is encode / decode to change the char (using Vigenere algorithm) - next function
	// TO_DO: Close the files
}

// Function to perform the Vigenère cipher (encoding or decoding)
urizen_str vigenereMem(const urizen_str inputFileName, const urizen_str key, urizen_int encode) {
	// TO_DO define the return type and local variables
	urizen_str output = NULL;
	// TO_DO: Check defensive programming
	// TO_DO: Use the logic to code/decode - consider the logic about visible chars only
	return output;
}

// Function to encode (cypher)
void cypher(const urizen_str inputFileName, const urizen_str outputFileName, const urizen_str key) {
    vigenereFile(inputFileName, outputFileName, key, CYPHER);
}

// Function to decode (decypher)
void decypher(const urizen_str inputFileName, const urizen_str outputFileName, const urizen_str key) {
    vigenereFile(inputFileName, outputFileName, key, DECYPHER);
}

// TO_DO: Get file size (util method)
urizen_int getSizeOfFile(const urizen_str filename) {
	urizen_int size = 0;
    // TO_DO: Use the logic to get the size of the file
    return size;
}
