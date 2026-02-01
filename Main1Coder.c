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
************************************************************
* File name: MainCoder.c
* Compiler: MS Visual Studio 2026
* Course: CST 8152 – Compilers, Lab Section: [301]
* Assignment: A12, A22, A32.
* Date: Jan 31 2026
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

#include <time.h>
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

/*
************************************************************
* Coder starting method
* Params:
*	- Mode: Cypher/decypher
*	- Input: Filename
*   - Key: Encrypt word
*	- Output: Filename
************************************************************
*/

/* Main function to handle command - line arguments */
urizen_int main1Coder(urizen_int argc, urizen_str* argv) {

	urizen_str operation = "";
	urizen_str inputFileName = "";
	urizen_str key = STR_LANGNAME;
	urizen_str outputFileName = "";
	clock_t start = 0;
	clock_t end = 0;

	if (argc < 5) {
		printf("Usage: %s [cypher=1|decypher=0] <input_file> <output_file>\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (argc > 4) {
		urizen_int size;
		urizen_str output;
		urizen_int i;

		operation = argv[2]; /* operations on the 3rd row. */
		inputFileName = argv[3]; /* input file name on the 4th row. */
		outputFileName = argv[4]; /* output file name on the 5th row. */

		/* Start timing how long encryption/decryption takes. */
		start = clock();

		/* Call the appropriate function to file */
		if (atoi(operation) == CYPHER) /* converts the string "1" entered in the cmd line to an integer to cmpr to CYPHER. */
			cypher(inputFileName, outputFileName, key); /* If the user entered 1, encrypt the input file. */
		else if (atoi(operation) == DECYPHER)
			decypher(inputFileName, outputFileName, key);
		else {
			errorPrint("%s%s%s", "Error: Unknown operation ", operation, ". Use 'cypher' or 'decypher'.\n");
			return EXIT_FAILURE;
		}

		/* Save end clock cycle.*/
		end = clock();

		printf("Operation '%s' completed successfully in %.6f seconds.\n\n", operation, (double)(end - start) / CLOCKS_PER_SEC);

		/* Call the other operation in memory */
		size = (urizen_int)getSizeOfFile(outputFileName);

		if (atoi(operation) == CYPHER)
			output = vigenereMem(outputFileName, key, DECYPHER);
		else 
			output = vigenereMem(outputFileName, key, CYPHER);

		if (output) {
			for ( i = 0; i < size; i++) {
				printf("%c", output[i]);
			}
			printf("\n");
			free(output);
		}
		
	}
	return EXIT_SUCCESS;
}

