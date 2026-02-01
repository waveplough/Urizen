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


/* Function to perform the Vigenère cipher(encoding or decoding) */
urizen_void vigenereFile(const urizen_str inputFileName, const urizen_str outputFileName, const urizen_str key, urizen_int encode) {

	/* Declare variable at the top of the block. */
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

	/* Set the output to the value of the returned string post encoding/decoding. */
	output = vigenereMem(inputFileName, key, encode ? CYPHER : DECYPHER);

	/* Validate the output. */
	if (!output) {
		fclose(outputFile);
		return;
	}
	/* retrieve the size of the file. */
	size = getSizeOfFile(inputFileName);

	/* Use the size to write the encoding/decoding to the user-specified file. */
	fwrite(output, sizeof(urizen_char), size, outputFile);

	/* Free the file data store in memory. */
	free(output);

	fclose(outputFile);
}

/* Function to perform the Vigenère cipher (encoding or decoding) */
urizen_str vigenereMem(const urizen_str inputFileName, const urizen_str key, urizen_int encode) {
	/* Declare variable at the top of the block. */
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

	/* Ensure the file isn't empty. */
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
	/* Set null terminator so that that function such as printf know when the string ends. */
	output[size] = '\0';
	/* Close the file. */
	fclose(inputFile);

	/* Set the output to the value of the returned string post encoding/decoding. */
	output = vigenereImpl(output, key, encode, size);

	return output;
}

/* Implements the vigenere cypher. */
urizen_str vigenereImpl(urizen_str output,const urizen_str key,urizen_int encode, urizen_int size) {
	/* Declare variable at the top of the block. */
	urizen_char offset;
	urizen_int i = 0;
	urizen_int j = 0;
	urizen_int keyLen = (urizen_int)strlen(key);

	/* Loop while the character index @i hasn't reached the file size @size. */
	while (i < size) {
		/* If key index @j has reached the end of the key's length (past the final character) then set it to 0. */
		if (j == keyLen) {
			j = 0;
		}
		/* Encrypt all characters besides new lines and carriage returns. */
		if (output[i] != '\n' && output[i] != '\r' ) {
			/* Calculate the offset by subtracting the value of the first visible ASCII character's index @ASCII_START from the key's index. */
			offset = key[j] - ASCII_START + 1;
			if (encode) {
				/* If its an encoding, calculate the sum of the current character @output[i] and the offset @offset. */
				urizen_int sum = output[i] + offset;

				/* If the sum exceeds the range of the visible ASCII characters @ASCII_RANGE, then subtract the range's value from the sum to fix disproportionality. */
				if (sum > ASCII_END) {
					output[i] = (urizen_char)(sum - ASCII_RANGE);
				}
				else {
					/* Otherwise set the character to the character value of the sum. */
					output[i] = (urizen_char)sum;
				}
			}
			else {
				/* If its a decoding, calculate the difference between the current character @output[i] and the offset @offset. */
				urizen_int diff = output[i] - offset;

				/* If the difference precedes the start of the visible ASCII characters @ASCII_START, then add the range's value to the sum to fix disproportionality. */
				if (diff < ASCII_START) {
					output[i] = (urizen_char)(diff + ASCII_RANGE);
				}
				else {
					/* Otherwise set the character to the character value of the difference. */
					output[i] = (urizen_char)diff;
				}
			}
			j++;
		}
		i++;
		
	}

	return output;

}

/* Function to encode(cypher) */
urizen_void cypher(const urizen_str inputFileName, const urizen_str outputFileName, const urizen_str key) {
    vigenereFile(inputFileName, outputFileName, key, CYPHER); /* vigFile controller selects */
}

/* Function to decode(decypher) */
urizen_void decypher(const urizen_str inputFileName, const urizen_str outputFileName, const urizen_str key) {
    vigenereFile(inputFileName, outputFileName, key, DECYPHER);
}

/* Computes the file size(util method) */
urizen_size getSizeOfFile(const urizen_str filename) {
	/* Declare variable at the top of the block. */
	urizen_long size = 0;
	FILE* inputFile;

	/* Open the input file. */
	inputFile = fopen(filename, "rb");

	/* Validate the input file. */
	if (!inputFile) {
		printf("ERROR: The input file %s could not be reached.\n", filename);
		return 0;
	}

	/* Move the file pointer to EOF. */
	if (fseek(inputFile, 0, SEEK_END)) {
		printf("ERROR: fseek operation failed for %s\n", filename);
		fclose(inputFile);
		return 0;
	}
	/* Assign byte-size to size. */
	size = ftell(inputFile);

	/* Validate size value. */
	if (size < 0) {
		fclose(inputFile);
		return 0;
	}
	/* Close the input file. */
	fclose(inputFile);

    return (urizen_size)size;
}
