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
#define _CRT_SECURE_NO_WARNINGS
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

	/* Input and output file pointers. */
	FILE* inputFile;
	FILE* outputFile;

	urizen_str output;
	urizen_size size;


	/* Create a file for reading and writing. */
	inputFile = fopen(inputFileName, "rb"); 
	outputFile = fopen(outputFileName, "wb"); 

	/* Null checks */
	if (!inputFile) {
		printf("ERROR: The input file %s could not be reached.\n",inputFileName);
		return NULL;
	}
	else if (!outputFile) {
		printf("ERROR: The output file %s could not be reached.\n",outputFileName);
		return NULL;
	}

	if (encode) {
		output = vigenereMem(inputFileName, key, CYPHER);
		size = getSizeOfFile(inputFileName);
		fwrite(output, sizeof(char), size, outputFile);
	}
	else {
		vigenereMem(inputFileName, key, DECYPHER);
	}

	fclose(inputFile);
	fclose(outputFile);

	// TO_DO: Define the input and output files (ex: FILE* inputFile, FILE* outputFile [x]
	// TO_DO: Use defensive programming (checking files) [x]
	// TO_DO: Define local variables
	// TO_DO: Logic: check if it is encode / decode to change the char (using Vigenere algorithm) - next function
	// TO_DO: Close the files
}

// Function to perform the Vigenère cipher (encoding or decoding)
urizen_str vigenereMem(const urizen_str inputFileName, const urizen_str key, urizen_int encode) {
	// TO_DO define the return type and local variables
	// TO_DO: Check defensive programming
	// TO_DO: Use the logic to code/decode - consider the logic about visible chars only

	FILE* inputFile;
	urizen_size size = getSizeOfFile(inputFileName);
	urizen_str output = (urizen_str)malloc(size + 1);
	urizen_int i = 0,j = 0;
	urizen_char offset;

	inputFile = fopen(inputFileName, "rb");

	/* Check defensive programming. */
	if (!inputFile) {
		printf("ERROR: The input file %s could not be reached.\n",inputFileName);
		return NULL;
	}

	/* If the file is empty simply return nothing. */
	if (size == 0) {
		printf("Warning: File %s is empty.\n",inputFileName);
		return output;
	}

	if (fread(output, sizeof(urizen_char), size, inputFile) != size) {
		printf("ERROR: the input file %s could not be read.\n", inputFileName);
		return NULL;
	}

	/////////////////Encypt Characters////////////////////
	
	while (i < size) {
		/* If key length has been reached point it to 0 once more. */
		if (j == strlen(key)) {
			j = 0;
		}
		/* Only shift if it is a visible character. */
		if (output[i] != '\n' && output[i] != ' ' && output[i] != '\r') {
			offset = key[j] - ASCII_START;
			if (output[i] + offset > ASCII_END) {
				urizen_char out = output[i] + offset;
				output[i] = out - ASCII_RANGE;
			}
			else {
				output[i] += offset;
			}
			j++;
		}
		i++;
	}

	return output;
}

// Function to encode (cypher)
void cypher(const urizen_str inputFileName, const urizen_str outputFileName, const urizen_str key) {
    vigenereFile(inputFileName, outputFileName, key, CYPHER); /* vigFile controller selects */
}

// Function to decode (decypher)
void decypher(const urizen_str inputFileName, const urizen_str outputFileName, const urizen_str key) {
    vigenereFile(inputFileName, outputFileName, key, DECYPHER);
}

// TO_DO: Get file size (util method)
urizen_size getSizeOfFile(const urizen_str filename) {
	urizen_int size = 0;
	FILE* inputFile;

	inputFile = fopen(filename, "rb");

	if (!inputFile) {
		printf("ERROR: The input file %s could not be reached.\n", filename);
		return 1;
	}

	if (fseek(inputFile, 0, SEEK_END)) {
		printf("ERROR: fseek operation failed for %s\n", filename);
	}
	size = ftell(inputFile);
	fclose(inputFile);
    // TO_DO: Use the logic to get the size of the file
    return size;
}
