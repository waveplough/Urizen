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
# ECHO "[SCANNER SCRIPT ........................]"
# ECHO "                                         "
*/ 

/*
************************************************************
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define the number of tokens */
#define NUM_TOKENS 21

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	ID_T,		/*  1: variable name identifier token (start: &) */
	INL_T,		/*  2: Integer literal token */
	STR_T,		/*  3: String literal token */
	LPR_T,		/*  4: Left parenthesis token */
	RPR_T,		/*  5: Right parenthesis token */
	LBR_T,		/*  6: Left brace token */
	RBR_T,		/*  7: Right brace token */
	KW_T,		/*  8: Keyword token */
	EOS_T,		/*  9: End of statement (semicolon) */
	RTE_T,		/* 10: Run-time error token */
	SEOF_T,		/* 11: Source end-of-file token */
	CMT_T,		/* 12: Comment token */
	LSB_T,		/* 13: Left square bracket token */
	RSB_T,		/* 14: Right square bracket token*/
	VARSUB_T,	/* 15: (variable substitution) Dollar sign token */
	ARITH_T,	/* 16: Arithmetic operator */
	REL_T,		/* 17: Relational operator */
	LOG_T,		/* 18: Logic operator */
	ASSIGN_T,	/* 19: Assignment */
	FPL_T		/* 20: Floating-point literal */
	
};

/* TO_DO: Define the list of keywords */
static urizen_str tokenStrTable[NUM_TOKENS] = {
	"ERR_T",
	"ID_T",
	"INL_T",
	"STR_T",
	"LPR_T",
	"RPR_T",
	"LBR_T",
	"RBR_T",
	"KW_T",
	"EOS_T",
	"RTE_T",
	"SEOF_T",
	"CMT_T",
	"LSB_T",
	"RSB_T",
	"VARSUB_T",
	"ARITH_T",
	"REL_T",
	"LOG_T",
	"ASSIGN_T",
	"FPL_T"
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	urizen_int codeType;				 /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	urizen_int intValue;				/* integer literal attribute (value) */
	urizen_int keywordIndex;			/* keyword index in the keyword table */
	urizen_int contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	urizen_float floatValue;				/* floating-point literal attribute (value) */
	urizen_char idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	urizen_char errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	urizen_byte flags;			/* Flags information */
	union {
		urizen_int intValue;				/* Integer value */
		urizen_float floatValue;			/* Float value */
		urizen_str stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	urizen_int code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	urizen_int scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* TO_DO: Define lexeme FIXED classes */
/* EOF definitions */
#define EOS_CHR '\0'	// CH00
#define EOF_CHR 0xFF	// CH01
#define UND_CHR '_'		// CH02
#define AMP_CHR '&'		// CH03
#define QUT_CHR '\''	// CH04
#define HST_CHR '#'		// CH05
#define TAB_CHR '\t'	// CH06
#define SPC_CHR ' '		// CH07
#define NWL_CHR '\n'	// CH08
#define SCL_CHR ';'		// CH09
#define LPR_CHR '('		// CH10
#define RPR_CHR ')'		// CH11
#define LBR_CHR '{'		// CH12
#define RBR_CHR '}'		// CH13
#define LSB_CHR '['		// CH14
#define RSB_CHR ']'		// CH15
#define VARSUB_CHR '$'	//CH16
#define CRG_CHR '\r'
#define PLUS_CHR '+'	
#define MINUS_CHR '-'
#define MULT_CHR '*'
#define DIV_CHR '/'
#define LT_CHR '<'
#define GT_CHR '>'
#define ASSIGN_CHR '='
#define DQT_CHR '"'
#define PRD_CHR '.'
#define EXP_CHR_UPPER 'E'
#define EXP_CHR_LOWER 'e'


/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */


/* TO_DO: Error states and illegal state */
#define ESNR	8		/* Error state with no retract */
#define ESWR	9		/* Error state with retract */
#define FS		20		/* Illegal state */

 /* TO_DO: State transition table definition */
#define NUM_STATES		20
#define CHAR_CLASSES	11

/* TO_DO: Transition table - type of states defined in separate table */
static urizen_int transitionTable[NUM_STATES][CHAR_CLASSES] = {
/*    [A-z],[0-9],    _,    \', SEOF,    #,  \n,	 .	  [eE]   +/-  other
	   L(0), D(1), U(2),  Q(3), E(4), C(5),  N(6)	D(7)  E(8)  S(09) O(9) */
	{     1,   10,    1,     4, ESWR,	  6,   0,	12,		1,	18,	  ESNR},		// S0: NOFS: Start State
	{     1,    1,    1, 	 3,    3,   3,   ESNR,	ESNR,	1,	 3,		3},			// S1: NOFS: Building Identifier
	{    FS,   FS,   FS,     FS,   FS,	 FS,   FS,	FS,		FS,	 FS,   FS},			// S2: FSWR: (VID) Identifier Accepting State (unused/placeholder)
	{    FS,   FS,   FS,     FS,   FS,	 FS, ESNR,	FS,		FS,	 FS,   FS},			// S3: FSWR: Keyword Accepting State (retract: char belongs to next token)
	{     4,    4,    4,      5, ESWR,	  4,    4,	4,	    4,	  4,	4},			// S4: NOFS: (SL) Building String Literal
	{    FS,   FS,   FS,     FS,   FS,	 FS,   FS,	FS,		FS,	 FS,   FS},			// S5: FSNR: (SL) String Literal Accepting State (no retract)
	{     6,    6,    6,      6,   7,	  6,    7,	6,		6,	  6,	6},			// S6: NOFS: (COM) Building Comment
	{    FS,   FS,   FS,     FS,   FS,	 FS,   FS,	FS,		FS,	 FS,   FS},			// S7: FSNR: (COM) Comment Accepting State (no retract)
	{    FS,   FS,   FS,     FS,   FS,	 FS,   FS,	FS,		FS,	 FS,   FS},			// S8: FSNR: (ES) Error, no retract
	{    FS,   FS,   FS,     FS,   FS,	 FS,   FS,	FS,		FS,	 FS,   FS},			// S9: FSWR: (ER) Error, with retract
	{	 ESWR, 10, ESWR,   ESWR,   11,   ESWR, ESWR, 12,	14,	 ESWR, 11},			// S10: NOFS: Building Integer
	{    FS,   FS,   FS,     FS,   FS,   FS,   FS,	FS,		FS,	 FS,   FS},			// S11: FSWR: Integer Accepting state (retract)
	{    ESWR, 12,   ESWR,  ESWR,  13, ESWR, ESWR, ESNR,    14,	ESNR,  13},			// S12: NOFS: Building Float (digits after dot)
	{    FS,   FS,   FS,     FS,   FS,	 FS,   FS,	FS,		FS,	 FS,   FS},			// S13: FSWR: Float Accepting State
	{	 ESWR, 16,   ESWR,  ESWR,  ESWR, ESWR, ESWR, ESNR,  ESNR, 15, ESNR},		// S14: NOFS: Seen [eE], expecting sign or digit
	{	 ESWR, 16,   ESWR,  ESWR,  ESWR, ESWR, ESWR, ESNR,  ESNR, ESNR, ESNR},		// S15: NOFS: Seen [eE][+-], expecting digit
	{	 ESWR, 16,   ESWR,  ESWR,  17, ESWR, ESWR, ESNR,  ESNR,	 ESNR,  17},		// S16: NOFS: Building Exponent Digits
	{    FS,   FS,   FS,     FS,   FS,	 FS,   FS,	FS,		FS,	 FS,    FS},		// S17: FSWR: Float with Exponent Accepting State
	{	 ESWR, 10,   ESWR,  ESWR,  ESWR, ESWR, ESWR, ESNR,  ESNR, ESNR, 19},		// S18  NOFS: Seen +/-, deciding ARITH or signed number
	{    FS,   FS,   FS,     FS,   FS,	 FS,   FS,	FS,		FS,	 FS,   FS}			// S19: FSWR: Arithmetic Operator Accepting State
	
};


/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static urizen_int stateType[NUM_STATES] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	FSWR, /* 02 (ID) - Variable ID */ //originally retractable FSNR now FSWR
	FSWR, /* 03 (KEY) */
	NOFS, /* 04 */
	FSNR, /* 05 (SL) */
	NOFS, /* 06 */
	FSNR, /* 07 (COM) */
	FSNR, /* 08 (Err1 - no retract) */
	FSWR,  /* 09 (Err2 - retract) */
	NOFS, /* 10 Building number  (not accepting) */
	FSWR, /* 11 number accepting */
	NOFS, /* 12 Building float */
	FSWR, /* 13 Accepting float */
	NOFS, /* 14 Building Exponent */
	NOFS, /* 15 Building SIGN */
	NOFS, /* 16 Building Exponent Power */
	FSWR, /* 17 Accepting Exponent */
	NOFS, /* 18 SIGN BUILD */
	FSWR /* 19 ARITHMETIC ACCEPTANCE STATE */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
urizen_int			startScanner(BufferPointer psc_buf);
static urizen_int	nextClass(urizen_char c);					/* character class function */
static urizen_int	nextState(urizen_int, urizen_char);		/* state machine function */
urizen_void			printScannerData(ScannerData scData);
Token				tokenizer(urizen_void);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(urizen_str lexeme);

/* Declare accepting states functions */
Token funcSL	(urizen_str lexeme);
Token funcFPL	(urizen_str lexeme);
Token funcIL	(urizen_str lexeme);
Token funcID	(urizen_str lexeme);
Token funcCMT   (urizen_str lexeme);
Token funcKEY	(urizen_str lexeme);
Token funcErr	(urizen_str lexeme);
Token funcARITH	(urizen_str lexeme);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcID,		/* MNID	[02] */
	funcKEY,	/* KEY  [03] */
	NULL,		/* -    [04] */
	funcSL,		/* SL   [05] */
	NULL,		/* -    [06] */
	funcCMT,	/* COM  [07] */
	funcErr,	/* ERR1 [06] */
	funcErr,	/* ERR2 [07] */
	NULL,		/* 10 - building number */
	funcIL,     /* 11 - number accepting */
	NULL,		/* 12 - building Float */
	funcFPL,    /* 13 - float accepting */
	NULL,		/* 14 - start building exponent */
	NULL,		/*15 building exponent sign */
	NULL,		/* 16 Building exponent power*/
	funcFPL,    /* 17 - number accepting w/ exponent */
	NULL,		/* 18 - SIGN BUILD */
	funcARITH	/* 19 - ARITHMETIC OPERATOR ACCEPTANCE STATE */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 33

/* TO_DO: Define the list of keywords */
static urizen_str keywordTable[KWT_SIZE] = {
	/* Core (9) */
	"set",      /* 00: Variable assignment */
	"expr",     /* 01: Expression evaluation */
	"puts",     /* 02: Output */
	"proc",     /* 03: Procedure definition */
	"return",   /* 04: Return from proc */
	"if",       /* 05: Conditional */
	"else",     /* 06: Alternative */
	"elseif",   /* 07: Chained conditional */
	"while",    /* 08: While loop */

	/* Loops and flow (4) */
	"for",      /* 09: For loop */
	"foreach",  /* 10: List iteration */
	"break",    /* 11: Exit loop */
	"continue", /* 12: Next iteration */

	/* Scope (4) */
	"global",   /* 13: Global variable */
	"variable", /* 14: Namespace variable */
	"upvar",    /* 15: Link variable */
	"uplevel",  /* 16: Execute in scope */

	/* Lists (8) */
	"list",     /* 17: Create list */
	"lappend",  /* 18: Append to list */
	"lindex",   /* 19: Get element */
	"llength",  /* 20: List length */
	"lrange",   /* 21: Range of list */
	"lsearch",  /* 22: Search list */
	"lsort",    /* 23: Sort list */
	"concat",   /* 24: Join lists */

	/* Strings (4) */
	"string",   /* 25: String ops */
	"regexp",   /* 26: Regex match */
	"regsub",   /* 27: Regex sub */
	"subst",    /* 28: Substitutions */

	/* Files (4) */
	"open",     /* 29: Open file */
	"close",    /* 30: Close file */
	"read",     /* 31: Read file */
	"gets"      /* 32: Read line */
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT TAB_CHR  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	urizen_char indentationCharType;
	urizen_int indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
urizen_int numScannerErrors;

/* Scanner data */
ScannerData scData;

#endif
