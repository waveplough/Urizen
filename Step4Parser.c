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
	case NWL_T:
		if (tokenCode != NWL_T && tokenCode != NO_ATTR)
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
	else {
		syncErrorHandler(tokenCode);
	}
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
	extern numParserErrors;
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
	case LSB_T:           
		printf("LSB_T\n");
		break;
	case RSB_T:          
		printf("RSB_T\n");
		break;
	case ARITH_T:         
		printf("ARITH_T\n");
		break;
	case ASSIGN_T:      
		printf("ASSIGN_T\n");
		break;
	case VARSUB_T:        
		printf("VARSUB_T\n");
		break;
	case INL_T:           
		printf("INL_T: %d\n", t.attribute.intValue);
		break;
	case FPL_T: 
		printf("FPL_T: %f\n", t.attribute.floatValue);
		break;
	case NWL_T:
		printf("NWL_T\n");
		break;
	case EOS_T:
		printf("NA\n");
		break;
	case CMT_T:
		printf("CMT_T\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
		numParserErrors++;
	}
}


 /* Parse a sequence of commands until terminator token is reached */
void script(urizen_int terminator) {
	psData.parsHistogram[BNF_script]++;

	while (lookahead.code != terminator && lookahead.code != SEOF_T) {
		/* Skip comments and blank lines */
		while (lookahead.code == CMT_T || lookahead.code == NWL_T) {
			while (lookahead.code == CMT_T) comment();
			while (lookahead.code == NWL_T) matchToken(NWL_T, NO_ATTR);
		}

		if (lookahead.code == terminator || lookahead.code == SEOF_T) break;

		command();
	}

	printf("Urizen: Exiting script\n");
}

urizen_void command() {
	psData.parsHistogram[BNF_command]++;

	/* Special keywords */
	if (lookahead.code == KW_T) {
		switch (lookahead.attribute.codeType) {
		case KW_if:      parseIf(); return;
		case KW_while:   parseWhile(); return;
		case KW_foreach: parseForeach(); return;
		case KW_proc:    parseProc(); return;
		case KW_return:  parseReturn(); return;
		default: break;
		}
	}

	/* Generic command: parse words until terminator */
	word();  /* first word */
	while (lookahead.code != EOS_T && lookahead.code != NWL_T && lookahead.code != SEOF_T) {
		word();
	}

	/* Consume terminator if present */
	if (lookahead.code == EOS_T || lookahead.code == NWL_T) {
		matchToken(lookahead.code, NO_ATTR);
	}

	printf("%s: Command parsed\n", STR_LANGNAME);
}

/* For braced expressions (single value, like {$x > 0}) */
urizen_void bracedExpression() {
	printf("%s: Braced expression entered\n", STR_LANGNAME);
	psData.parsHistogram[BNF_braced]++;
	matchToken(LBR_T, NO_ATTR);

	/* Parse as a sequence of words/tokens (no commands) */
	while (lookahead.code != RBR_T && lookahead.code != SEOF_T) {
		word();  /* parse each token */
	}

	matchToken(RBR_T, NO_ATTR);
	printf("%s: Braced expression parsed\n", STR_LANGNAME);
}

/* For braced blocks (multiple commands, like { set x 10; puts $x }) */
urizen_void bracedBlock() {
	printf("%s: Braced block entered\n", STR_LANGNAME);
	psData.parsHistogram[BNF_braced]++;
	matchToken(LBR_T, NO_ATTR);

	/* Parse as a script of commands */
	while (lookahead.code != RBR_T && lookahead.code != SEOF_T) {
		while (lookahead.code == CMT_T) comment();
		while (lookahead.code == NWL_T) matchToken(NWL_T, NO_ATTR);
		if (lookahead.code == RBR_T || lookahead.code == SEOF_T) break;
		command();  /* parse commands, not just words */
	}

	matchToken(RBR_T, NO_ATTR);
	printf("%s: Braced block parsed\n", STR_LANGNAME);
}

urizen_void substWord() {
	printf("%s%s\n", STR_LANGNAME, ": Command substitution entered");
	psData.parsHistogram[BNF_subst]++;
	matchToken(LSB_T, NO_ATTR);

	while (lookahead.code != RSB_T && lookahead.code != SEOF_T) {
		word();  /* parse each token inside */
	}

	matchToken(RSB_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Command substitution parsed");
}

urizen_void parseWhile() {
	printf("%s%s\n", STR_LANGNAME, ": While loop entered");
	matchToken(KW_T, KW_while);

	/* Condition */
	if (lookahead.code == LBR_T) {
		bracedExpression();
	}
	else {
		word();
	}

	/* Body */
	bracedBlock();

	printf("%s%s\n", STR_LANGNAME, ": While loop parsed");
}

urizen_void parseForeach() {
	printf("%s%s\n", STR_LANGNAME, ": foreach loop entered");
	matchToken(KW_T, KW_foreach);

	/* Variable name */
	word();

	/* List (braced or word) */
	if (lookahead.code == LBR_T) {
		bracedExpression();
	}
	else {
		word();
	}

	/* Body */
	bracedBlock();

	printf("%s%s\n", STR_LANGNAME, ": Foreach loop parsed");
}

urizen_void parseProc() {
	printf("%s: Procedure entered\n", STR_LANGNAME);
	matchToken(KW_T, KW_proc);		// match proc keyword

	/* Procedure name */
	word();							// parse the name of the procedure

	parseParameterList();

	/* Procedure body — braced script */
	bracedBlock();

	printf("%s: Procedure parsed\n", STR_LANGNAME);
}


urizen_void parseParameterList() {
	psData.parsHistogram[BNF_parameterList]++;

	/* Parameter list — must be braced */
	matchToken(LBR_T, NO_ATTR);
	/* Parse parameters (just words) inside braces */
	while (lookahead.code != RBR_T && lookahead.code != SEOF_T) {
		word();
	}
	matchToken(RBR_T, NO_ATTR);

	printf("%s: Parameters parsed\n", STR_LANGNAME);
}

urizen_void parseBody() {
	psData.parsHistogram[BNF_body]++;
	matchToken(LBR_T, NO_ATTR);

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
	printf("%s%s\n", STR_LANGNAME, ": If statement entered");
	matchToken(KW_T, KW_if);

	/* Condition — could be braced or bare */
	if (lookahead.code == LBR_T) {
		bracedExpression();
	}
	else {
		word();
	}

	/* Optional 'then' */
	if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_then) {
		matchToken(KW_T, KW_then);
	}

	/* Then-body */
	bracedBlock();


	/* Optional elseif/else */
	while (lookahead.code == KW_T) {
		if (lookahead.attribute.codeType == KW_elseif) {
			printf("%s%s\n", STR_LANGNAME, ": Elseif statement entered");
			matchToken(KW_T, KW_elseif);
			if (lookahead.code == LBR_T) {
				bracedExpression();
			}
			else {
				word();
			}
			bracedBlock();
		}
		else if (lookahead.attribute.codeType == KW_else) {
			printf("%s%s\n", STR_LANGNAME, ": Else statement entered");

			matchToken(KW_T, KW_else);
			bracedBlock();
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
		matchToken(KW_T, lookahead.attribute.codeType);
		break;
	case ID_T:
		matchToken(ID_T, NO_ATTR);  
		break;
	case INL_T:
		matchToken(INL_T, NO_ATTR);  
		break;
	case FPL_T:
		matchToken(FPL_T, NO_ATTR); 
		break;
	case STR_T:
		matchToken(STR_T, NO_ATTR);  
		break;
	case VARSUB_T:
		matchToken(VARSUB_T, NO_ATTR);
		if (lookahead.code == ID_T) {
			word();
		}
		break;
	case LBR_T:
		bracedExpression();
		break;
	case LSB_T:
		substWord();
		break;
	case ARITH_T:       
		matchToken(ARITH_T, NO_ATTR);
		break;
	case ASSIGN_T:       
		matchToken(ASSIGN_T, NO_ATTR);
		break;
	case REL_T:          
		matchToken(REL_T, NO_ATTR);
		break;
	case LOG_T:         
		matchToken(LOG_T, NO_ATTR);
		break;
	default:
		printError();
		syncErrorHandler(SEOF_T);
		break;
	}
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
