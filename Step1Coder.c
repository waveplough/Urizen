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
#include <limits.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif


// Function to perform the Vigenère cipher (encoding or decoding)
void vigenereFile(const urizen_str inputFileName, const urizen_str outputFileName, const urizen_str key, urizen_int encode) {

	/* output file pointers. */
	FILE* outputFile;

	urizen_str output;
	urizen_size size;


	/* Create a file for reading and writing. */
	outputFile = fopen(outputFileName, "wb"); 

	/* Null check */
	if (!outputFile) {
		printf("ERROR: The output file %s could not be reached.\n",outputFileName);
		return;
	}

	if (encode) {
		output = vigenereMem(inputFileName, key, CYPHER);
		if (!output) {
			fclose(outputFile);
			return;
		}
		size = getSizeOfFile(inputFileName);
		fwrite(output, sizeof(char), size, outputFile);
		free(output);
	}
	else {
		output = vigenereMem(inputFileName, key, DECYPHER);
		if (!output) { 
			fclose(outputFile);
			return;
		}
		size = getSizeOfFile(inputFileName);
		fwrite(output, sizeof(char), size, outputFile);
		free(output);
	}

	fclose(outputFile);
}

// Function to perform the Vigenère cipher (encoding or decoding)
urizen_str vigenereMem(const urizen_str inputFileName, const urizen_str key, urizen_int encode) {

	FILE* inputFile;
	urizen_int size;
	urizen_str output;

	/* Check key first (cheap check) */
	if (!key || strlen(key) == 0) {
		printf("ERROR: The key is empty, enter a valid key.\n");
		return NULL;
	}

	/* Get file size */
	size = (urizen_int)getSizeOfFile(inputFileName);

	if (size == 0) {
		printf("Warning: File %s is empty.\n", inputFileName);
		return NULL;
	}

	/* Open file */
	inputFile = fopen(inputFileName, "rb");
	if (!inputFile) {
		printf("ERROR: The input file %s could not be reached.\n", inputFileName);
		return NULL;
	}

	/* Allocate memory */
	output = (urizen_str)malloc(size + 1);
	if (!output) {
		printf("ERROR: Couldn't allocate memory for the output.\n");
		fclose(inputFile);
		return NULL;
	}

	/* Read file */
	if ((urizen_int)fread(output, sizeof(urizen_char), size, inputFile) != size) {
		printf("ERROR: the input file %s could not be read.\n", inputFileName);
		free(output);
		fclose(inputFile);
		return NULL;
	}

	output[size] = '\0';
	fclose(inputFile);

	output = vigenereImpl(output, key, encode, size);

	return output;
}

/* Implements the vigenere cypher. */
urizen_str vigenereImpl(urizen_str output,const urizen_str key,urizen_int encode, urizen_int size) {
	urizen_char offset;
	urizen_int i = 0;
	urizen_int j = 0;

	while (i < size) {

		if (j == (urizen_int)strlen(key)) {
			j = 0;
		}

		if (output[i] != '\n' && output[i] != '\r' ) {
			offset = key[j] - ASCII_START + 1;
			if (encode) {
				urizen_int sum = output[i] + offset;
				if (sum > ASCII_END) {
					output[i] = (urizen_char)(sum - ASCII_RANGE);
				}
				else {
					output[i] = (urizen_char)sum;
				}
			}
			else {
				urizen_int diff = output[i] - offset;
				if (diff < ASCII_START) {
					output[i] = (urizen_char)(diff + ASCII_RANGE);
				}
				else {
					output[i] = (urizen_char)diff;
				}
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

// Computes the file size (util method)
urizen_size getSizeOfFile(const urizen_str filename) {
	urizen_int size = 0;
	FILE* inputFile;

	inputFile = fopen(filename, "rb");

	if (!inputFile) {
		printf("ERROR: The input file %s could not be reached.\n", filename);
		return 0;
	}

	if (fseek(inputFile, 0, SEEK_END)) {
		printf("ERROR: fseek operation failed for %s\n", filename);
		fclose(inputFile);
		return 0;
	}
	size = ftell(inputFile);
	fclose(inputFile);

    return size;
}
