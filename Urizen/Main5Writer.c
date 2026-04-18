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
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    "
# ECHO "    @@                             @@    "
# ECHO "    @@         U R I Z E N         @@    "
# ECHO "    @@                             @@    "
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    "
# ECHO "                                         "
# ECHO "[WRITER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Main5Writer.c
* Compiler: MS Visual Studio 2026
* Course: CST 8152 - Compilers, Lab Section: [301]
* Assignment: A5.
* Date: Apr 17 2026
* Professor: Paulo Sousa
* Purpose: Entry point for the Writer (A5).
*          Loads the encoded Urizen source file, decrypts it
*          in memory via the Vigenere coder, and runs the
*          Writer's interpreter on the plain-text buffer.
* Function list: main5Writer().
*************************************************************/

/*
 *.............................................................................
 * ADVICE:
 * The #define _CRT_SECURE_NO_WARNINGS suppresses the warnings issued by
 * MS Visual Studio for "unsafe" CRT functions such as fopen().  It is a
 * no-op on other compilers (gcc / clang / VSCode).
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

#ifndef WRITER_H_
#include "Step5Writer.h"
#endif

/*
************************************************************
* Main function from the Writer
*   argc / argv - command line arguments
*                 expected: <program> 5 <sourceFile>
* Return value:
*	Success operation.
************************************************************
*/
urizen_int main5Writer(urizen_int argc, urizen_str* argv) {

	urizen_str  source  = NULL;
	urizen_str  content = NULL;
	urizen_int  size    = 0;

	/* Defensive: require at least <program> 5 <sourceFile>. */
	if (argc < 3) {
		errorPrint("Usage: %s 5 <source_file>", argv[0]);
		return EXIT_FAILURE;
	}

	source = argv[2];
	printf("Reading file %s ....Please wait\n", source);

	/* Size of the encoded file (defensive check only). */
	size = (urizen_int)getSizeOfFile(source);
	if (size <= 0) {
		printf("The input file %s %s\n", source, "is not completely loaded.");
		return EXIT_FAILURE;
	}

	/* Decrypt the file into a freshly-allocated buffer. */
	content = vigenereMem(source, STR_LANGNAME, DECYPHER);
	if (!content) {
		printf("The input file %s %s\n", source, "could not be decrypted.");
		return EXIT_FAILURE;
	}

	/* Run the Writer (semantic analyzer / runner). */
	process_content(content);

	/* Release the decrypted buffer allocated by vigenereMem. */
	free(content);

	return EXIT_SUCCESS;
}
