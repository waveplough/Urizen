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
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

 /* TIP: Do not change pragmas, unless necessary .......................................*/
 /*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
 /*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

 /* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Constants about controls (not need to change) */
#define READER_ERROR		(-1)		/* General error message */
#define READER_TERMINATOR	'\0'		/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE		INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE		100			/* default initial buffer reader capacity */
#define READER_DEFAULT_FACTOR	0.5f		/* default factor */
#define DEFAULT_MAX_LIMIT		INT_MAX-1
#define MODE_MULTI				
#define MODE_ADDIT				

/* TO_DO: Add your bit-masks constant definitions here */
/*
// BITS                             (    76543210)
#define READER_DEFAULT_FLAG 0x00 	// (0b00000000) = (0x00)_16 = (000)_10
// TO_DO: BIT 3: END = End of buffer flag
#define READER_SET_FLAG_END 0x08	// (0b00001000) = (0x08)_16 = (008)_10
// TO_DO: BIT 2: REL = Rellocation memory flag
#define READER_SET_FLAG_REL 0x04	// (0b00000100) = (0x04)_16 = (004)_10
// TO_DO: BIT 1: EMP = Buffer empty flag
#define READER_SET_FLAG_FUL 0x02	// (0b00000010) = (0x02)_16 = (002)_10
// TO_DO: BIT 0: FUL = Buffer full flag
#define READER_SET_FLAG_EMP 0x01	// (0b00000001) = (0x01)_16 = (001)_10
*/

#define NCHAR				128			/* Chars from 0 to 127 */
#define ASCII_FIRST			  0
#define ASCII_LAST			127
#define CHARSEOF			(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	urizen_int		write;					/* the offset to the add chars (in chars) */
	urizen_int		read;					/* the offset to the get a char position (in chars) */
	urizen_int		mark;					/* the offset to the mark position (in chars) */
} Position;

/* Flags declaration */
typedef struct flag {
	urizen_bool		isEmpty;					/* checks if there is no content */
	urizen_bool		isFull;						/* the content is using all size */
	urizen_bool		isRead;						/* all content was read */
	urizen_bool		isMoved;					/* the content was moved in reallocation */
} Flag;

/* Buffer structure */
typedef struct bufferReader {
	urizen_str		content;			/* pointer to the beginning of character array (character buffer) */
	urizen_int		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	urizen_long		maxLimit;			/* the maximum size the reader can expand to */
	urizen_float	factor;				/* factor for increase the buffer */
	Flag			flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		position;			/* Offset / position field */
	urizen_int		histogram[NCHAR];	/* Statistics of chars */
	urizen_int		numReaderErrors;	/* Number of errors from Reader */
	urizen_int		checkSum;			/* Sum of bytes(chars) */
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */

/* General Operations */
BufferPointer	readerCreate(urizen_int, urizen_float, urizen_long);
BufferPointer	readerAddChar(BufferPointer const, urizen_char);
urizen_bool		readerClear(BufferPointer const);
urizen_bool		readerFree(BufferPointer const);
urizen_bool		readerIsFull(BufferPointer const);
urizen_bool		readerIsEmpty(BufferPointer const);
urizen_bool		readerSetMark(BufferPointer const, urizen_int);
urizen_int		readerPrint(BufferPointer const);
urizen_int		readerLoad(BufferPointer const, urizen_str);
urizen_bool		readerRecover(BufferPointer const);
urizen_bool		readerRetract(BufferPointer const);
urizen_bool		readerRestore(BufferPointer const);
urizen_int		readerChecksum(BufferPointer const);
/* Getters */
urizen_char		readerGetChar(BufferPointer const);
urizen_str		readerGetContent(BufferPointer const, urizen_int);
urizen_int		readerGetPosRead(BufferPointer const);
urizen_int		readerGetPosWrite(BufferPointer const);
urizen_int		readerGetPosMark(BufferPointer const);
urizen_int		readerGetSize(BufferPointer const);
urizen_void		readerPrintFlags(BufferPointer const);
urizen_void		readerPrintStat(BufferPointer const);
urizen_int		readerNumErrors(BufferPointer const);
urizen_void		validateTokens(BufferPointer const);

#endif