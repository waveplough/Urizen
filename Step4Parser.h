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
# ECHO "[PARSER SCRIPT ........................]"
# ECHO "                                         "
*/


/*
************************************************************
* File name: Parser.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
************************************************************
*/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Reader.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
extern Token			lookahead;
extern BufferPointer	stringLiteralTable;
extern urizen_int		line;
extern Token			tokenizer(urizen_void);
extern urizen_str		keywordTable[KWT_SIZE];
extern urizen_int		syntaxErrorNumber;

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,	// -1
	/* Core */
	KW_set,			// 00
	KW_expr,		// 01
	KW_puts,		// 02
	KW_proc,		// 03
	KW_return,		// 04
	KW_if,			// 05
	KW_else,		// 06
	KW_elseif,		// 07

	/* Loops */
	KW_while,		// 08
	KW_for,			// 09
	KW_foreach,		// 10
	KW_break,		// 11
	KW_continue,	// 12
	KW_then,

	/* String */
	KW_string		// 13
	

};

/* TO_DO: Define the number of BNF rules */
#define NUM_BNF_RULES 12

/* Parser */
typedef struct parserData {
	urizen_int parsHistogram[NUM_BNF_RULES];	/* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
extern urizen_int numParserErrors;

/* Scanner data */
extern ParserData psData;

/* Function definitions */
urizen_void startParser();
urizen_void matchToken(urizen_int, urizen_int);
urizen_void syncErrorHandler(urizen_int);
urizen_void printError();
urizen_void printBNFData(ParserData psData);

/* List of BNF statements */
enum BNF_RULES {
	BNF_error,           /* 0 */
	BNF_script,          /* 1 */
	BNF_command,         /* 2 */
	BNF_word,            /* 3 */
	BNF_braced,          /* 4 */
	BNF_subst,           /* 5 */
	BNF_expression,      /* 6 */
	BNF_comment,          /* 7 */
	BNF_newline,			/* 8 */
	BNF_parameterList,		/* 9 */
	BNF_block,				/* 10 */
	BNF_floatLiteral

};


/* TO_DO: Define the list of keywords */
extern urizen_str BNFStrTable[NUM_BNF_RULES];

/* TO_DO: Place ALL non-terminal function declarations */
urizen_void script(urizen_int terminator);
urizen_void word();
urizen_void bracedExpression();
urizen_void bracedBlock();
urizen_void substWord();
urizen_void parseIf();
urizen_void parseWhile();
urizen_void parseForeach();
urizen_void parseProc();
urizen_void parseParameterList();
urizen_void parseBody();
urizen_void parseReturn();
urizen_void comment();
urizen_void command();




#endif
