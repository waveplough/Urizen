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
* File name: Parser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

#ifndef PARSER_H_
#include "Step4Parser.h"
#endif

/* Parser data */
extern ParserData psData; /* BNF statistics */

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
/* TO_DO: This is the function to start the parser - check your program definition */

urizen_void startParser() {
	/* TO_DO: Initialize Parser data */
	urizen_int i = 0;
	for (i = 0; i < NUM_BNF_RULES; i++) {
		psData.parsHistogram[i] = 0;
	}
	/* Proceed parser */
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {	// while the token isn't the eof designator of the source file then execute program()
		script(SEOF_T);
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
/* TO_DO: This is the main code for match - check your definition */
urizen_void matchToken(urizen_int tokenCode, urizen_int tokenAttribute) {
	urizen_int matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
		if (tokenAttribute != NO_ATTR && lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;
		break;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
		break;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
/* TO_DO: This is the function to handler error - adjust basically datatypes */
urizen_void syncErrorHandler(urizen_int syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
/* TO_DO: This is the function to error printing - adjust basically datatypes */
urizen_void printError() {
	extern numParserErrors;			/* link to number of errors (defined in Parser.h) */
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("*ERROR*: %s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case ID_T:
		printf("ID_T:\t\t%s\t\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
		numParserErrors++; // Updated parser error
	}
}


 /* Parse a sequence of commands until terminator token is reached */
void script(urizen_int terminator) {
	psData.parsHistogram[BNF_script]++;

	while (lookahead.code != terminator && lookahead.code != SEOF_T) {
		/* Skip comments */
		while (lookahead.code == CMT_T) {
			comment();
		}

		if (lookahead.code == terminator || lookahead.code == SEOF_T) {
			break;
		}

		command();
	}
}

urizen_void command() {
	psData.parsHistogram[BNF_command]++;
	
	/* Special keywords */
	if (lookahead.code == KW_T) {
		switch (lookahead.attribute.codeType) {
		case KW_if:
			parseIf();
			return;
		case KW_while:
			parseWhile();
			return;
		case KW_foreach:
			parseForeach();
			return;
		case KW_proc:
			parseProc();
			return;
		case KW_return:
			parseReturn();
			return;
		default:
			/* Fall through to generic command parsing */
			break;
		}
	}

	word();
	while (lookahead.code != NWL_T && lookahead.code != EOS_T && lookahead.code != SEOF_T) {
		word();
	}

	if (lookahead.code == EOS_T) {
		matchToken(EOS_T, NO_ATTR);
	}

	//printf("%s%s\n", STR_LANGNAME, ": Command parsed");'
	printf("%s: Command parsed\n", STR_LANGNAME);
}

urizen_void bracedWord() {
	psData.parsHistogram[BNF_braced]++;
	matchToken(LBR_T, NO_ATTR);
	script(RBR_T);
	matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Braced word parsed");
}

urizen_void substWord() {
	psData.parsHistogram[BNF_subst]++;
	matchToken(LSB_T, NO_ATTR);
	script(RSB_T);
	matchToken(RSB_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Command substitution parsed");
}

urizen_void parseWhile() {
	matchToken(KW_T, KW_while);

	/* Condition */
	if (lookahead.code == LBR_T) {
		matchToken(LBR_T, NO_ATTR);
		script(RBR_T);
		matchToken(RBR_T, NO_ATTR);
	}
	else {
		word();
	}

	/* Body */
	matchToken(LBR_T, NO_ATTR);
	script(RBR_T);
	matchToken(RBR_T, NO_ATTR);

	printf("%s%s\n", STR_LANGNAME, ": While loop parsed");
}

urizen_void parseForeach() {
	matchToken(KW_T, KW_foreach);

	/* Variable name */
	word();

	/* List (braced or word) */
	if (lookahead.code == LBR_T) {
		matchToken(LBR_T, NO_ATTR);
		script(RBR_T);  /* list contents */
		matchToken(RBR_T, NO_ATTR);
	}
	else {
		word();
	}

	/* Body */
	matchToken(LBR_T, NO_ATTR);
	script(RBR_T);
	matchToken(RBR_T, NO_ATTR);

	printf("%s%s\n", STR_LANGNAME, ": Foreach loop parsed");
}

urizen_void parseProc() {
	matchToken(KW_T, KW_proc);

	/* Procedure name */
	word();

	/* Parameter list (braced) */
	matchToken(LBR_T, NO_ATTR);
	script(RBR_T);  /* parameters inside braces */
	matchToken(RBR_T, NO_ATTR);

	/* Body (braced) */
	matchToken(LBR_T, NO_ATTR);
	script(RBR_T);
	matchToken(RBR_T, NO_ATTR);

	printf("%s%s\n", STR_LANGNAME, ": Procedure parsed");
}

urizen_void parseReturn() {
	matchToken(KW_T, KW_return);

	/* Optional return value */
	if (lookahead.code != EOS_T && lookahead.code != SEOF_T && lookahead.code != RBR_T) {
		word();
	}

	printf("%s%s\n", STR_LANGNAME, ": Return statement parsed");
}

urizen_void parseIf() {
	matchToken(KW_T, KW_if);

	/* Condition — could be braced or bare */
	if (lookahead.code == LBR_T) {
		matchToken(LBR_T, NO_ATTR);
		script(RBR_T);
		matchToken(RBR_T, NO_ATTR);
	}
	else {
		word();
	}

	/* Optional 'then' */
	if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_then) {
		matchToken(KW_T, KW_then);
	}

	/* Then-body */
	matchToken(LBR_T, NO_ATTR);
	script(RBR_T);
	matchToken(RBR_T, NO_ATTR);

	/* Optional elseif/else */
	while (lookahead.code == KW_T) {
		if (lookahead.attribute.codeType == KW_elseif) {
			matchToken(KW_T, KW_elseif);
			if (lookahead.code == LBR_T) {
				matchToken(LBR_T, NO_ATTR);
				script(RBR_T);
				matchToken(RBR_T, NO_ATTR);
			}
			else {
				word();
			}
			matchToken(LBR_T, NO_ATTR);
			script(RBR_T);
			matchToken(RBR_T, NO_ATTR);
		}
		else if (lookahead.attribute.codeType == KW_else) {
			matchToken(KW_T, KW_else);
			matchToken(LBR_T, NO_ATTR);
			script(RBR_T);
			matchToken(RBR_T, NO_ATTR);
			break;
		}
		else {
			break;
		}
	}

	printf("%s%s\n", STR_LANGNAME, ": If statement parsed");
}

urizen_void word() {
	psData.parsHistogram[BNF_word]++;

	switch (lookahead.code) {
	case KW_T:
		matchToken(KW_T, lookahead.attribute.codeType);	/* Keywords can appear as words too (e.g., 'set' as variable name) */
		break;
	case ID_T:
		matchToken(ID_T, lookahead.attribute.idLexeme);
		break;
	case INL_T:
		matchToken(INL_T, lookahead.attribute.intValue);
		break;
	case FPL_T:
		matchToken(FPL_T, lookahead.attribute.floatValue);
		break;
	case STR_T:
		matchToken(STR_T, lookahead.attribute.contentString);
		break;
	case VARSUB_T:
		matchToken(VARSUB_T, NO_ATTR);
		/* Variable name follows */
		if (lookahead.code == ID_T) {
			word();  /* consume the variable name */
		}
		break;
	case LBR_T:
		bracedWord();
		break;
	case LSB_T:
		substWord();
		break;
	default:
		printError();
		syncErrorHandler(SEOF_T);
		break;
	}

	//printf("%s%s\n", STR_LANGNAME, ": Word parsed");
}

/*
 ************************************************************
 * comment
 * BNF: comment
 * FIRST(<comment>)= {CMT_T}.
 ***********************************************************
 */
urizen_void comment() {
	psData.parsHistogram[BNF_comment]++;
	matchToken(CMT_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
	//line++;
}




/*
 ************************************************************
 * The function prints statistics of BNF rules
 * Param:
 *	- Parser data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
/*
sofia_void printBNFData(ParserData psData) {
}
*/
urizen_void printBNFData(ParserData psData) {
	/* Print Parser statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_BNF_RULES; cont++) {
		if (psData.parsHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", BNFStrTable[cont], "]=", psData.parsHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}
