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
* File name: Step5Writer.h
* Compiler: MS Visual Studio 2026
* Course: CST 8152 - Compilers, Lab Section: [301]
* Assignment: A5.
* Date: Apr 17 2026
* Professor: Paulo Sousa
* Purpose: Header for the Writer (Semantic Analyzer / Runner)
*          adapted for the Urizen language (Tcl-style syntax).
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * The Writer interprets an Urizen (Tcl) source file.  It recognises:
 *     - comments starting with '#'
 *     - the `set <name> <value>` assignment command
 *     - the `puts <arg>` output command
 *     - `[expr { <arithmetic> }]` command substitution
 *     - `$<name>` variable substitution
 *     - double-quoted string literals (with `$<name>` interpolation)
 *     - numeric, string, boolean and character values
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef WRITER_H_
#define WRITER_H_

 /* ---------- Sizing ---------- */
#define MAX_VARS      100
#define MAX_EXPR_LEN  256
#define MAX_LINES     100
#define MAX_NAME_LEN   32
#define MAX_STR_LEN   256

/* ---------- Tcl / Urizen keywords ---------- */
#define KW_SET   "set"
#define KW_PUTS  "puts"
#define KW_EXPR  "expr"

/* Backwards-compatible synonym (spec uses WRITE as the output keyword define) */
#define WRITE    KW_PUTS

/* ---------- Character constants ---------- */
#define EOS      '\0'
#define ZERO      0.0
#define STREMPTY  ""

#define LPAR     '('
#define RPAR     ')'
#define LBRACE   '{'
#define RBRACE   '}'
#define LBRACK   '['
#define RBRACK   ']'
#define DOLLAR   '$'
#define HASH     '#'

#define PLUS     '+'
#define MINUS    '-'
#define MULT     '*'
#define DIV      '/'
#define MOD      '%'

#define EQUALS   '='
#define SPACE    ' '
#define TAB      '\t'
#define NEWLINE  '\n'
#define RETURN   '\r'
#define QUOTES   '"'
#define QUOTE    '\''

/* ---------- Literal spellings ---------- */
#define TRUE_STR   "true"
#define FALSE_STR  "false"

/* Legacy aliases kept for compatibility with the spec's define names.    */
/* (The spec lists TRUE / FALSE as defines; Windows headers also define   */
/*  TRUE / FALSE, so the project uses the suffixed names above in .c.)    */
#ifndef TRUE
#define TRUE   TRUE_STR
#endif
#ifndef FALSE
#define FALSE  FALSE_STR
#endif

/* ---------- Variable representation ---------- */
typedef enum { NUMERIC, STRING, BOOLEAN, CHAR } VarType;

typedef struct {
    urizen_char name[MAX_NAME_LEN];
    VarType     type;
    union {
        urizen_doub num_value;
        urizen_char str_value[MAX_STR_LEN];
        urizen_int  bool_value;   /* 1 for true, 0 for false */
        urizen_char char_value;
    } value;
} Variable;

/* ---------- Symbol-table helpers ---------- */
urizen_int  find_variable(const urizen_str name);

urizen_void assign_boolean_variable(const urizen_str name, urizen_int value);
urizen_void assign_char_variable   (const urizen_str name, urizen_char value);
urizen_void assign_numeric_variable(const urizen_str name, urizen_doub value);
urizen_void assign_string_variable (const urizen_str name, const urizen_str value);

urizen_int  get_boolean_value(const urizen_str name);
urizen_char get_char_value   (const urizen_str name);
urizen_doub get_numeric_value(const urizen_str name);
const urizen_str get_string_value(const urizen_str name);

/* ---------- Expression evaluation ---------- */
urizen_doub parse_term      (urizen_str* expr);
urizen_doub parse_expression(urizen_str* expr);

/* ---------- Top-level interpreter ---------- */
urizen_void handle_set      (urizen_str line);
urizen_void handle_puts     (urizen_str line);
urizen_void handle_write    (urizen_str expression);  /* alias kept for spec */
urizen_void calculate       (urizen_str expression);

urizen_void process_file    (const urizen_str filename);
urizen_void process_content (urizen_str content);

#endif
