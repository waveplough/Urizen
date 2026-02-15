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
# ECHO "[READER SCRIPT ..........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: MainReader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
* Function list: (...).
*************************************************************/

/*
 *.............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: Preprocessor directives
 * The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
 * to suppress the warnings about using "unsafe" functions like fopen()
 * and standard sting library functions defined in string.h.
 * The define directive does not have any effect on other compiler projects 
 * (Gcc, VSCode, Codeblocks, etc.).
 *.............................................................................
 */

#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

 /* Check for ANSI C compliancy */
#define ANSI_C 0
#if defined(__STDC__)
#undef ANSI_C
#define ANSI_C 1
#endif

/*
 * TODO .......................................................................
 * Basically, change all datatypes to your language definitions
 * (see "Compilers.h")
 */

/*
 * -------------------------------------------------------------
 *  Function declarations
 * -------------------------------------------------------------
 */
urizen_void displayBuffer(BufferPointer ptr_Buffer);
urizen_long getFileSize(urizen_str fname);
urizen_int isNumber(const urizen_str ns);
urizen_void startReader(urizen_str, urizen_str, urizen_int, urizen_float);

/*
************************************************************
* Main function from Buffer
* Parameters:
*   argc / argv = Parameters from command prompt
* Return value:
*	Success operation.
************************************************************
*/

urizen_int main2Reader(urizen_int argc, urizen_str* argv) {

	/* Create source input buffer */
	urizen_str program = argv[0];
	urizen_str input = argv[2];
	urizen_int size = READER_DEFAULT_SIZE; /*originally 0.*/
	urizen_float factor = READER_DEFAULT_FACTOR; /*originally 0.0f*/

	/* Missing file name or/and mode parameter */
	if (argc <= 2) {
		errorPrint("\nDate: %s  Time: %s", __DATE__, __TIME__);
		errorPrint("\nRuntime error at line %d in file %s\n", __LINE__, __FILE__);
		errorPrint("%s\b\b\b\b%s%s", argv[0], ": ", "Missing parameters.");
		errorPrint("Usage: <Option=2> <SourceFile>");
		exit(EXIT_FAILURE);
	}

	/* TODO: Update all other options about parameters */
	startReader(program, input, size, factor);

	/* Return success */
	return (EXIT_SUCCESS);
}

/*
************************************************************
* Buffer starting method
* Params:
*	- Program: Name of the program
*	- Input: Filename
*	- Mode: Operational mode
*	- Size: Buffer capacity
*	- Increment: buffer increment.
************************************************************
*/
urizen_void startReader(urizen_str program, urizen_str input, urizen_int size, urizen_float factor) {

	BufferPointer pBuffer;		/* pointer to Buffer structure */
	urizen_int loadSize = 0;	/* the size of the file loaded in the buffer */

	/* Create buffer */
	pBuffer = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_FACTOR);

	if (pBuffer == NULL) {
		errorPrint("%s%s", program, ": Cannot allocate buffer - Use: buffer <input>.");
		errorPrint("Filename: %s \n", input);
		exit(1);
	}

	/* Load source file into input buffer  */
	printf("Reading file %s ....Please wait\n", input);
	loadSize = readerLoad(pBuffer, input);

	/* Sets the checksum */
	readerChecksum(pBuffer);

	/* If the input file has not been completely loaded, find the file size and print the last symbol loaded */
	if (loadSize == READER_ERROR) {
		printf("The input file %s %s\n", input, "has not been completely loaded.");
		printf("Current size of buffer: %d.\n", readerGetSize(pBuffer));
		printf("Input file size: %ld\n", getFileSize(input));
	}

	/* Finishes the buffer: add end of file character (EOF) to the buffer display again */
	if ((loadSize != READER_ERROR) && (loadSize != 0)) {
		if (!readerAddChar(pBuffer, READER_TERMINATOR)) {
			errorPrint("%s%s%s", program, ": ", "Error in compacting buffer.");
		}
	}

	/* Prints the buffer property and content */
	displayBuffer(pBuffer);

	/* Free the dynamic memory used by the buffer */
	readerFree(pBuffer);
	pBuffer = NULL;
}

/*
************************************************************
* Get buffer size
* Params:
*	- Filename: Name of the file
************************************************************
*/

urizen_long getFileSize(urizen_str fname) {
	FILE* input;
	urizen_long length;
	input = fopen(fname, "r");

	if (input == NULL) {
		errorPrint("%s%s", "Cannot open file: ", fname);
		return 0;
	}

	if (fseek(input, 0L, SEEK_END)) {
		printf("error moving the file pointer\n");
	}

	length = ftell(input);
	fclose(input);

	return length;
}

/*
************************************************************
 * Tests for decimal-digit character string
 * Params:
 *		- String to be evaluated as numeric
 * Return:
 *		- Number value: Returns nonzero (true) if ns is a number; 0 (False) otherwise
************************************************************
*/

urizen_int isNumber(const urizen_str ns) {
	urizen_char c; urizen_int i = 0;
	if (ns == NULL) return 0;
	while ((c = ns[i++]) == 0) {
		if (!isdigit(c)) return 0;
	}
	return 1;
}


/*
************************************************************
* Print function
*	- Params: buffer to print all properties.
************************************************************
*/
urizen_void displayBuffer(BufferPointer ptr_Buffer) {
	printf("\nPrinting buffer parameters:\n\n");
	printf("The size of the buffer is:  %d\n",
		readerGetSize(ptr_Buffer));
	printf("The current size of the buffer is:  %d\n",
		readerGetPosWrite(ptr_Buffer));
	printf("The first symbol in the buffer is:  %c\n",
		readerGetPosWrite(ptr_Buffer) ? *readerGetContent(ptr_Buffer, 0) : ' ');
	printf("The value of the flags:\n");
	readerPrintFlags(ptr_Buffer);
	printf("Checksum: %d\n", readerChecksum(ptr_Buffer));
	printf("%s", "Reader statistics : \n");
	readerPrintStat(ptr_Buffer);
	printf("Number of errors: %d\n",
		readerNumErrors(ptr_Buffer));
	printf("\nPrinting buffer contents:\n\n");
	readerRecover(ptr_Buffer);
	if (!readerPrint(ptr_Buffer))
		printf("Empty buffer\n");
}

