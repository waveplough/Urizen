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

BufferPointer readerCreate(urizen_int size, urizen_float factor, urizen_int maxLimit) {

	BufferPointer readerPointer = NULL;
	int i = 0;

	/* Defensive programming: size */
	if (size <= 0 || factor <= 0) {
		printf("%s:%d error: the size '%d' and factor '%f' must be positive\n", __FILE__, __LINE__, size, factor);
		return NULL;
	}
	/* readerPointer allocation */
	readerPointer = calloc(1, sizeof(Buffer));

	/* content allocation */
	urizen_str content = malloc(size * sizeof(char));

	/* Defensive programming: */
	if (!readerPointer || !content) {
		printf("%s:%d error: couldn't allocate memory for either readerPointer or content\n", __FILE__, __LINE__);
		return NULL;
	}

	/* Initialize the histogram */
	for (i = 0; i < NCHAR; i++) {
		readerPointer->histogram[i] = 0; /* set every value to 0 */
	}

	/* Initialize errors */
	readerPointer->numReaderErrors = 0;
	readerPointer->checkSum = 0;

	/* Update the properties */
	readerPointer->content = content;
	readerPointer->size = size;
	readerPointer->maxLimit = maxLimit;

	readerPointer->position.mark = 0;
	readerPointer->position.read = 0;
	readerPointer->position.write = 0;

	readerPointer->factor = factor;


	/* Initialize flags */
	readerPointer->flags.isEmpty = URIZEN_TRUE; /* The created flag must be signalized as EMP */
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
	urizen_int pos;

	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return NULL;
	}

	pos = readerGetPosWrite(readerPointer);

	/* if the write position is negative return null. */
	if (pos < 0) {
		return NULL;
	}

	if (ch < ASCII_FIRST) {
		readerPointer->numReaderErrors++;
		printf("%s:%d | valid ASCII values range from [0-127]\n", __FILE__, __LINE__);
		return NULL;
	}


	/* Test the inclusion of chars */
	if (readerPointer->position.write * (urizen_int)sizeof(urizen_char) < readerPointer->size) {
		/* Buffer not full: set flag */
		readerPointer->flags.isFull = URIZEN_FALSE;
	}
	else {
		urizen_float ratio;

		/* Reset Full flag */
		readerPointer->flags.isFull = URIZEN_TRUE;
		
		/* Adjust the size to be duplicated */
		ratio = (urizen_float)(1.0 + readerPointer->factor);
		newSize = (urizen_int)(readerPointer->size * ratio);


		/* Defensive programming */
		if (newSize <= 0) {
			printf("%s:%d | warning: error increasing reader size\n", __FILE__, __LINE__);
			return NULL;
		}

		/* If new size exceeds the readers max size, truncate the value to match it. */
		if (newSize > readerPointer->maxLimit) {
			newSize -= (newSize - readerPointer->maxLimit);
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
		readerPointer->size = newSize;

		printf("\n..............\n");
		printf("* New size: %d bytes", newSize);
		printf("\n..............");

		readerPointer->flags.isFull = URIZEN_FALSE;
	}

	/* Add the char */
	readerPointer->content[pos] = ch;
	readerPointer->position.write++;
	

	/* Updates histogram */
	readerPointer->histogram[(urizen_int)ch]++;

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
	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return URIZEN_FALSE;
	}

	/* Adjust positions to zero */
	readerPointer->position.read = 0;
	readerPointer->position.write = 0;

	/* Adjust flags original */
	readerPointer->flags.isEmpty = URIZEN_TRUE; /* The created flag must be signalized as EMP */
	readerPointer->flags.isFull = URIZEN_FALSE;
	readerPointer->flags.isMoved = URIZEN_FALSE;
	readerPointer->flags.isRead = URIZEN_FALSE;

	return URIZEN_TRUE;
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

	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return URIZEN_FALSE;
	}
	/* free the content buffer */
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
	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return URIZEN_FALSE;
	}

	/* Check flag if buffer is FUL */
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
	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return URIZEN_FALSE;
	}
	/* Check flag if buffer is EMP */
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
	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return URIZEN_FALSE;
	}

	/* Adjust mark */
	if ( mark < 0 || mark > readerPointer->position.write) {
		printf("%s:%d | warning: 'mark' out of bounds\n", __FILE__, __LINE__);
		return URIZEN_FALSE;
	}

	readerPointer->position.mark = mark;

	return URIZEN_TRUE;
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
	/* Defensive programming (including invalid chars) */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return READER_ERROR;
	}

	/* Print the buffer content */
	while (readerPointer->content[i]) {
		printf("%c",readerPointer->content[i]);
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
	const urizen_str outputFileName = "DECRYPTED.txt";

	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return READER_ERROR;
	}

	/* Loads and decrypt the input file */
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

		/* If max size is reached, break out of the loop */
		if (readerGetPosWrite(readerPointer) >= readerPointer->maxLimit - 1) {
			printf("\nwarning: max size has been reached\n");
			break;
		}

		/* for each character, call readerAddChar to validate and add it to content */
		if (readerAddChar(readerPointer, (urizen_char)c) != NULL) {
			num++;
		}

	}

	/* check the number of chars and set isEmpty as false if it exceeds 0 */
	if (num > 0) {
		readerPointer->flags.isEmpty = URIZEN_FALSE;
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
	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return URIZEN_FALSE;
	}

	/* Adjust read and mark to zero */
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
	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return URIZEN_FALSE;
	}

	if (readerPointer->position.read == 0) {
		printf("%s:%d | warning: cannot retract past the beginning\n", __FILE__, __LINE__);
		return URIZEN_FALSE;
	}

	/* Retract (return 1 pos read) */
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
	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return URIZEN_FALSE;
	}

	/* Restore read to mark */
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
	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return READER_ERROR;
	}

	/* check if read position is invalid */
	if (readerPointer->position.read < 0) {
		printf("%s:%d | warning: read position cannot be negative\n", __FILE__, __LINE__);
		return READER_ERROR;
	}

	/* check if read position has reached EOF */
	if (readerPointer->position.read == readerPointer->position.write) {
		readerPointer->flags.isRead = URIZEN_TRUE;
		return EOF_CHAR;
	}

	/* if it hasn't reached EOF thrn set the isRead flag to false */
	readerPointer->flags.isRead = URIZEN_FALSE;

	/* Returns size in the read position and updates read */
	return readerPointer->content[readerPointer->position.read++];
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
	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return NULL;
	}

	if (pos < 0 || pos > readerGetPosWrite(readerPointer)) {
		printf("%s:%d | warning: 'pos' is out of bounds\n", __FILE__, __LINE__);
		return NULL;
	}

	/* Return content (string) */
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
	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return READER_ERROR;
	}
	/* Return read */
	return readerPointer->position.read;
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
	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return READER_ERROR;
	}

	/* Return write */
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
	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return READER_ERROR;
	}

	/* Return mark */
	return readerPointer->position.mark;
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
	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return READER_ERROR;
	}

	/* Return size */
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
	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return;
	}

	/* Return flags */
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

	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return;
	}

	/* Print statistics */
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
	/* Defensive programming */
	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return READER_ERROR;
	}

	/* Return the number of errors */
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
	/* Defensive programming */
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

urizen_void validateTokens(BufferPointer const readerPointer) {

	urizen_bool flag = URIZEN_FALSE;
	
	const urizen_int sqrL = '[';
	const urizen_int sqrR = ']';

	const urizen_int parL = '(';
	const urizen_int parR = ')';

	const urizen_int quotes = '\"';

	const urizen_int curlyL = '{';
	const urizen_int curlyR = '}';

	if (!readerPointer) {
		printf("%s:%d | error: BufferPointer 'readerPointer' is NULL\n", __FILE__, __LINE__);
		return;
	}

	printf("VALIDATOR:\n");

	if (readerPointer->histogram[sqrL] != readerPointer->histogram[sqrR]) {
		flag = URIZEN_TRUE;
		printf("warning: the number of square brackets is uneven\n");
	}
	if (readerPointer->histogram[parL] != readerPointer->histogram[parR]) {
		flag = URIZEN_TRUE;
		printf("warning: the number of parenthesis is uneven\n");
	}
	if ((readerPointer->histogram[quotes] % 2) != 0) {
		flag = URIZEN_TRUE;
		printf("warning: the number of quotes is uneven\n");
	}
	if (readerPointer->histogram[curlyL] != readerPointer->histogram[curlyR]) {
		flag = URIZEN_TRUE;
		printf("warning: the number of curly braces is uneven\n");
	}

	if (!flag) {
		printf("\n* * * Code is valid * * *\n");
	}
	

	
}