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
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Jan 01 2025
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

#include <ctype.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

 /*
 ***********************************************************
 * Function name: readerCreate
 * Purpose: Creates the buffer reader according to capacity, increment
	 factor and operational mode ('f', 'a', 'm')
 * Author: Svillen Ranev / Paulo Sousa
 * History/Versions: S22
 * Called functions: calloc(), malloc()
 * Parameters:
 *   size = initial capacity
 *   increment = increment factor
 *   mode = operational mode
 * Return value: BufferPointer @readerPointer (pointer to reader)
 * Algorithm: Allocation of memory according to iniial (default) values.
 * TODO ......................................................
 *	- Adjust datatypes for your LANGUAGE.
 *   - Use defensive programming
 *	- Check boundary conditions
 *	- Check flags.
 *************************************************************
 */

BufferPointer readerCreate(urizen_int size, urizen_float factor) {

	BufferPointer readerPointer = NULL;
	int i = 0;

	/* TO_DO: Defensive programming: size */
	if (size <= 0 || factor <= 0) {
		printf("%s:%d error: the size '%d' and factor '%f' must be positive\n", __FILE__, __LINE__, size, factor);
		return NULL;
	}
	/* TO_DO: readerPointer allocation */
	readerPointer = calloc(1, sizeof(Buffer));

	/* TO_DO: content allocation */
	urizen_str content = malloc(size * sizeof(char));

	/* TO_DO: Defensive programming: content // TO_DO: Defensive programming: readerPointer*/
	if (!readerPointer || !content) {
		printf("%s:%d error: couldn't allocate memory for either readerPointer or content\n", __FILE__, __LINE__);
		return NULL;
	}

	/* TO_DO: Initialize the histogram */
	for (i = 0; i < NCHAR; i++) {
		readerPointer->histogram[i] = 0; /* set every value to 0 */
	}

	/* TO_DO: Initialize errors */
	readerPointer->numReaderErrors = 0;
	readerPointer->checkSum = 0;

	/* TO_DO: Update the properties */
	readerPointer->content = content;
	readerPointer->size = size;

	readerPointer->position.mark = 0;
	readerPointer->position.read = 0;
	readerPointer->position.write = 0;

	readerPointer->factor = factor;


	/* TO_DO: Initialize flags */
	readerPointer->flags.isEmpty = URIZEN_TRUE; /* TO_DO: The created flag must be signalized as EMP */
	readerPointer->flags.isFull = URIZEN_FALSE;
	readerPointer->flags.isMoved = URIZEN_FALSE;
	readerPointer->flags.isRead = URIZEN_FALSE;

	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

BufferPointer readerAddChar(BufferPointer const readerPointer, urizen_char ch) {
	urizen_str tempReader = NULL;
	urizen_int newSize = 0;
	urizen_int pos = readerGetPosWrite(readerPointer);

	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return NULL;
	}

	if (ch < ASCII_FIRST || ch > ASCII_LAST) {
		readerPointer->numReaderErrors++;
		printf("%s:%d | valid ASCII values range from [0-127]\n", __FILE__, __LINE__);
		return NULL;
	}


	/* TO_DO: Test the inclusion of chars */
	if (readerPointer->position.write * (urizen_int)sizeof(urizen_char) < readerPointer->size) {
		/* TO_DO: Buffer not full: set flag */
		readerPointer->flags.isFull = URIZEN_FALSE;
	}
	else {
		urizen_int ratio;

		/* TO_DO: Reset Full flag */
		readerPointer->flags.isFull = URIZEN_TRUE;

		/* TO_DO: Adjust the size to be duplicated */
		ratio = (urizen_float)(1.0 + readerPointer->factor);
		newSize = (urizen_int)(readerPointer->size * ratio);

		/* TO_DO: Defensive programming */
		if (newSize <= 0 || newSize > READER_MAX_SIZE) {
			printf("%s:%d | warning: error increasing reader size\n", __FILE__, __LINE__);
			return NULL;
		}

		/* Realloc content memory and check if the memory address was duplicated. */
		tempReader = (urizen_str)realloc(readerPointer->content, newSize);

		if (!tempReader) {
			printf("%s:%d | error: realloc failed, 'tempReader' is NULL\n", __FILE__, __LINE__);
			return NULL;
		}

		/* If memory address has changed following realloc, set the isMoved flag */
		if (tempReader != readerPointer->content) {
			readerPointer->flags.isMoved = URIZEN_TRUE;
		}

		readerPointer->content = tempReader;
		readerPointer->factor = ratio;
		readerPointer->size = newSize;

		printf("\nThe new size is %d bytes\n\n", newSize);

		readerPointer->flags.isFull = URIZEN_FALSE;
	}

	/* TO_DO: Add the char */
	if (pos >= 0 && pos <= readerPointer->size) {
		readerPointer->content[pos] = ch;
		readerPointer->position.write++;
	}
	else {
		printf("%s:%d | warning: write out of bounds\n", __FILE__, __LINE__);
		return NULL;
	}

	/* TO_DO: Updates histogram */
	readerPointer->histogram[ch]++;

	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
urizen_bool readerClear(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Adjust positions to zero */
	/* TO_DO: Adjust flags original */
	return URIZEN_FALSE;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
urizen_bool readerFree(BufferPointer const readerPointer) {

	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return URIZEN_FALSE;
	}

	if (readerPointer->content) {
		free(readerPointer->content);
		readerPointer->content = NULL;
	}

	/* Free memory (buffer/content) */
	free(readerPointer);

	return URIZEN_TRUE;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
urizen_bool readerIsFull(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return URIZEN_FALSE;
	}

	/* TO_DO: Check flag if buffer is FUL */
	return readerPointer->flags.isFull ? URIZEN_TRUE : URIZEN_FALSE;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
urizen_bool readerIsEmpty(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return URIZEN_FALSE;
	}
	/* TO_DO: Check flag if buffer is EMP */
	return readerPointer->flags.isEmpty ? URIZEN_TRUE : URIZEN_FALSE;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
urizen_bool readerSetMark(BufferPointer const readerPointer, urizen_int mark) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Adjust mark */
	if (!readerPointer || mark < 0 || mark > readerPointer->position.write) {
		return READER_ERROR;
	}

	readerPointer->position.mark = mark;

	return URIZEN_FALSE;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
urizen_int readerPrint(BufferPointer const readerPointer) {
	int i = 0;
	int num = 0;
	/* TO_DO: Defensive programming (including invalid chars) */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return READER_ERROR;
	}
	/* TO_DO: Print the buffer content */
	while (readerPointer->content[i]) {
		printf("%c", readerPointer->content[i]);
		num++;
		i++;
	}

	printf("\n\n");

	return num;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	a specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
urizen_int readerLoad(BufferPointer const readerPointer, urizen_str inputFileName) {
	int c = 0;
	int num = 0;
	const urizen_str key = STR_LANGNAME;
	const urizen_str outputFileName = "DECRYPT.txt";

	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return READER_ERROR;
	}

	/* TO_DO: Loads and decrypt the input file */
	vigenereFile(inputFileName, outputFileName, key, DECYPHER);

	printf("\nFile decrypted!\n");

	/* Open the written decrypted file */
	FILE* fOut = fopen(outputFileName, "rb");

	/* check that the output file was properly opened */
	if (!fOut) {
		printf("%s:%d | error: file could not be opened\n", __FILE__, __LINE__);
		return READER_ERROR;
	}

	printf("\nNow reading file %s .....please wait\n", outputFileName);

	/* read character by character until you reach EOF */
	while ((c = fgetc(fOut)) != EOF) {
		/* for each character, call readerAddChar to validate and add it to content */
		if (readerAddChar(readerPointer, (urizen_char)c) != NULL) {
			num++;
		}

	}

	fclose(fOut);
	return num;
}

/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
urizen_bool readerRecover(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return URIZEN_FALSE;
	}

	/* TO_DO: Adjust read and mark to zero */
	readerPointer->position.mark = 0;
	readerPointer->position.read = 0;

	return URIZEN_TRUE;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
urizen_bool readerRetract(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return URIZEN_FALSE;
	}

	if (readerPointer->position.read < 0) {
		printf("%s:%d | warning: read position cannot be negative\n", __FILE__, __LINE__);
		return URIZEN_FALSE;
	}

	/* TO_DO: Retract (return 1 pos read) */
	--readerPointer->position.read;

	return URIZEN_TRUE;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
urizen_bool readerRestore(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return URIZEN_FALSE;
	}

	/* TO_DO: Restore read to mark */
	readerPointer->position.read = readerPointer->position.mark;

	return URIZEN_TRUE;
}



/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
urizen_char readerGetChar(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return READER_ERROR;
	}

	/* TO_DO: Returns size in the read position and updates read */
	return '\0';
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
urizen_str readerGetContent(BufferPointer const readerPointer, urizen_int pos) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return NULL;
	}

	if (pos < 0 || pos > readerGetPosWrite(readerPointer)) {
		printf("%s:%d | warning: 'pos' is out of bounds\n", __FILE__, __LINE__);
		return NULL;
	}

	/* TO_DO: Return content (string) */
	return &(readerPointer->content[pos]);
}

/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
urizen_int readerGetPosRead(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return READER_ERROR;
	}
	/* TO_DO: Return read */
	return 0;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
urizen_int readerGetPosWrite(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return READER_ERROR;
	}

	if (readerPointer->position.write < 0) {
		printf("%s:%d | warning: 'write' should be positive\n", __FILE__, __LINE__);
		return READER_ERROR;
	}

	/* TO_DO: Return write */
	return readerPointer->position.write;

}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
urizen_int readerGetPosMark(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Return mark */
	return 0;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
urizen_int readerGetSize(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return READER_ERROR;
	}

	if (readerPointer->size < 0) {
		printf("%s:%d | warning: 'size' should be positive\n", __FILE__, __LINE__);
		return READER_ERROR;
	}

	/* TO_DO: Return size */
	return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
#define FLAGS_
#undef FLAGS_
#ifndef FLAGS_
urizen_void readerPrintFlags(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return;
	}

	/* TO_DO: Return flags */
	printf(
		"Flag.isEmpty = %d\n"
		"Flag.isFull = %d\n"
		"Flag.isMoved = %d\n"
		"Flag.isRead = %d\n",
		readerPointer->flags.isEmpty,
		readerPointer->flags.isFull,
		readerPointer->flags.isMoved,
		readerPointer->flags.isRead
	);
}
#else
#define bGetFlags(readerPointer) ((readerPointer)?(readerPointer->flags):(RT_FAIL_1))
#endif

/*
***********************************************************
* Function name: readerShowStat
* Purpose: Shows the char statistic.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
urizen_void readerPrintStat(BufferPointer const readerPointer) {
	int printed = 0;

	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return;
	}

	/* TO_DO: Print statistics */
	for (urizen_int i = 0; i < 128; i++) {
		if (readerPointer->histogram[i] > 0) {

			if (isprint(i)) {
				printf("Char [%c] = %d ", i, readerPointer->histogram[i]);
			}
			else {
				printf("Char [0x%02X] = %d ", i, readerPointer->histogram[i]);
			}

			printed++;

			if (printed % 10 == 0) {
				printf("\n");
			}
		}
	}

	if (printed % 10 != 0) {
		printf("\n");
	}
}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
urizen_int readerNumErrors(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return READER_ERROR;
	}

	/* validate the number */
	if (readerPointer->numReaderErrors < 0) {
		printf("%s:%d | warning: 'numReaderErrors' should be positive\n", __FILE__, __LINE__);
		return READER_ERROR;
	}

	/* TO_DO: Return the number of errors */
	return readerPointer->numReaderErrors;
}

/*
***********************************************************
* Function name: readerChecksum
* Purpose: Sets the checksum of the reader (4 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	[None]
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

urizen_int readerChecksum(BufferPointer readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer || !readerPointer->content) {
		printf("%s:%d | error: BufferPointer 'readerPointer' or 'content' is NULL\n", __FILE__, __LINE__);
		return READER_ERROR;
	}

	// Initialize checksum
	urizen_int checksum = 0;

	// Iterate through the content of the reader
	for (urizen_int i = 0; i < readerPointer->size; i++) {
		// Add the value of the character to the checksum 
		checksum += (urizen_int)readerPointer->content[i];

	}

	// Update the checksum field of the reader
	readerPointer->checkSum = checksum;

	// Return the checksum
	return checksum;
}