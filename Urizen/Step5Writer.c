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
***********************************************************
* File name: Step5Writer.c
* Compiler: MS Visual Studio 2026
* Course: CST 8152 - Compilers, Lab Section: [301]
* Assignment: A5.
* Date: Apr 17 2026
* Professor: Paulo Sousa
* Purpose: Writer (Semantic Analyzer / Runner) for the
*          Urizen language (Tcl-style syntax).
*
*          The writer interprets each source line and builds
*          a variable table plus an output buffer.  After the
*          whole file has been processed, it prints the
*          collected outputs (from `puts`) followed by the
*          full list of variables and their values.
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 *  - Defensive programming: every pointer is NULL-checked before use and
 *    every buffer copy respects the destination size.
 *  - The language grammar covered here is the minimum required by the spec:
 *         comment        : '#' <anything>\n
 *         set            : set <name> <value>
 *         puts           : puts <argument>
 *         value          : number | string | bool | char | $var | [expr {e}]
 *         argument       : "string"  |  $var  |  [expr {e}]  |  word
 *         expression e   : + - * / with ( ) grouping and $var references
 *.............................................................................
 */

#define _CRT_SECURE_NO_WARNINGS

#ifndef WRITER_H_
#include "Step5Writer.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ---------------------------------------------------------- */
/*  Global state                                              */
/* ---------------------------------------------------------- */

Variable     variables[MAX_VARS];
urizen_int   var_count     = 0;
urizen_int   initial_phase = 1;                                     /* unused in one-pass mode, kept for spec compatibility */
urizen_char  output_buffer[MAX_EXPR_LEN * MAX_LINES] = { 0 };       /* collected puts output */

/* Forward declarations of static helpers (not in header) */
static urizen_str  skip_ws(urizen_str p);
static urizen_int  read_name(urizen_str* pp, urizen_char* out, urizen_int max);
static urizen_void format_variable_by_name(const urizen_str name, urizen_str dst, urizen_int dst_size);
static urizen_void interpolate_string(const urizen_str src, urizen_str dst, urizen_int dst_size);
static urizen_doub parse_factor(urizen_str* expr);
static urizen_void extract_expr_block(urizen_str* pp, urizen_str out, urizen_int out_size);
static urizen_str* splitIntoLines(const urizen_str content, urizen_int* lineCount);
static urizen_void freeLines(urizen_str* lines, urizen_int lineCount);

/* ---------------------------------------------------------- */
/*  Small utilities                                           */
/* ---------------------------------------------------------- */

/* Advance past whitespace characters (space / tab / CR) */
static urizen_str skip_ws(urizen_str p) {
    if (!p) return p;
    while (*p != EOS && isspace((urizen_byte)*p)) p++;
    return p;
}

/* Read a Tcl identifier (letters, digits, underscore).
 * Advances *pp past the last consumed character and returns the length. */
static urizen_int read_name(urizen_str* pp, urizen_char* out, urizen_int max) {
    urizen_int i = 0;
    if (!pp || !*pp || !out || max <= 0) return 0;
    while (**pp != EOS
        && (isalnum((urizen_byte)**pp) || **pp == '_')
        && i < max - 1) {
        out[i++] = *(*pp)++;
    }
    out[i] = EOS;
    return i;
}

/* Extract the body of an `expr { ... }` clause (already positioned at `{`).
 * Matches nested braces and null-terminates `out`. Advances `*pp` past '}'. */
static urizen_void extract_expr_block(urizen_str* pp, urizen_str out, urizen_int out_size) {
    urizen_int depth = 0;
    urizen_int k     = 0;
    if (!pp || !*pp || !out || out_size <= 0) return;

    out[0] = EOS;
    if (**pp != LBRACE) return;

    (*pp)++;           /* consume opening '{' */
    depth = 1;
    while (**pp != EOS && depth > 0 && k < out_size - 1) {
        if (**pp == LBRACE) { depth++; out[k++] = *(*pp)++; }
        else if (**pp == RBRACE) {
            depth--;
            if (depth == 0) { (*pp)++; break; }
            out[k++] = *(*pp)++;
        }
        else out[k++] = *(*pp)++;
    }
    out[k] = EOS;
}

/* ---------------------------------------------------------- */
/*  Symbol-table helpers                                      */
/* ---------------------------------------------------------- */

/* Returns index of variable `name` in the table, or -1 if not found. */
urizen_int find_variable(const urizen_str name) {
    urizen_int i = 0;
    if (!name) return -1;
    for (i = 0; i < var_count; ++i) {
        if (strcmp(variables[i].name, name) == 0) return i;
    }
    return -1;
}

/* Allocate a new slot (or reuse existing) for `name` and return index. */
static urizen_int get_or_create(const urizen_str name) {
    urizen_int idx = find_variable(name);
    if (idx == -1) {
        if (var_count >= MAX_VARS) {
            printf("Error: variable table full (max %d).\n", MAX_VARS);
            return -1;
        }
        idx = var_count++;
        strncpy_s(variables[idx].name, sizeof(variables[idx].name),
            name, sizeof(variables[idx].name) - 1);
        variables[idx].name[sizeof(variables[idx].name) - 1] = EOS;
    }
    return idx;
}

urizen_void assign_numeric_variable(const urizen_str name, urizen_doub value) {
    urizen_int idx = get_or_create(name);
    if (idx == -1) return;
    variables[idx].type = NUMERIC;
    variables[idx].value.num_value = value;
}

urizen_void assign_string_variable(const urizen_str name, const urizen_str value) {
    urizen_int idx = get_or_create(name);
    if (idx == -1 || !value) return;
    variables[idx].type = STRING;
    strncpy_s(variables[idx].value.str_value, sizeof(variables[idx].value.str_value),
        value, sizeof(variables[idx].value.str_value) - 1);
    variables[idx].value.str_value[sizeof(variables[idx].value.str_value) - 1] = EOS;
}

urizen_void assign_boolean_variable(const urizen_str name, urizen_int value) {
    urizen_int idx = get_or_create(name);
    if (idx == -1) return;
    variables[idx].type = BOOLEAN;
    variables[idx].value.bool_value = value ? 1 : 0;
}

urizen_void assign_char_variable(const urizen_str name, urizen_char value) {
    urizen_int idx = get_or_create(name);
    if (idx == -1) return;
    variables[idx].type = CHAR;
    variables[idx].value.char_value = value;
}

urizen_int get_boolean_value(const urizen_str name) {
    urizen_int idx = find_variable(name);
    if (idx != -1 && variables[idx].type == BOOLEAN) return variables[idx].value.bool_value;
    return 0;
}

urizen_char get_char_value(const urizen_str name) {
    urizen_int idx = find_variable(name);
    if (idx != -1 && variables[idx].type == CHAR) return variables[idx].value.char_value;
    return EOS;
}

urizen_doub get_numeric_value(const urizen_str name) {
    urizen_int idx = find_variable(name);
    if (idx != -1 && variables[idx].type == NUMERIC) return variables[idx].value.num_value;
    return ZERO;
}

const urizen_str get_string_value(const urizen_str name) {
    urizen_int idx = find_variable(name);
    if (idx != -1 && variables[idx].type == STRING) return variables[idx].value.str_value;
    return STREMPTY;
}

/* Render variable `name` as text into dst. Works for every supported type. */
static urizen_void format_variable_by_name(const urizen_str name, urizen_str dst, urizen_int dst_size) {
    urizen_int idx = -1;
    if (!dst || dst_size <= 0) return;
    dst[0] = EOS;
    if (!name) return;
    idx = find_variable(name);
    if (idx == -1) return;
    switch (variables[idx].type) {
    case NUMERIC:
        snprintf(dst, dst_size, "%.2lf", variables[idx].value.num_value);
        break;
    case STRING:
        snprintf(dst, dst_size, "%s", variables[idx].value.str_value);
        break;
    case BOOLEAN:
        snprintf(dst, dst_size, "%s",
            variables[idx].value.bool_value ? TRUE_STR : FALSE_STR);
        break;
    case CHAR:
        snprintf(dst, dst_size, "%c", variables[idx].value.char_value);
        break;
    }
}

/* Copy `src` to `dst`, replacing $name occurrences with the variable's text. */
static urizen_void interpolate_string(const urizen_str src, urizen_str dst, urizen_int dst_size) {
    urizen_int di = 0;
    urizen_int i  = 0;
    if (!src || !dst || dst_size <= 0) return;
    while (src[i] != EOS && di < dst_size - 1) {
        if (src[i] == DOLLAR) {
            urizen_char name[MAX_NAME_LEN] = { 0 };
            urizen_int  k = 0;
            i++;
            while (src[i] != EOS
                && (isalnum((urizen_byte)src[i]) || src[i] == '_')
                && k < MAX_NAME_LEN - 1) {
                name[k++] = src[i++];
            }
            name[k] = EOS;
            if (k > 0) {
                urizen_char tmp[MAX_STR_LEN] = { 0 };
                urizen_int  tlen = 0;
                format_variable_by_name(name, tmp, (urizen_int)sizeof(tmp));
                tlen = (urizen_int)strlen(tmp);
                if (di + tlen >= dst_size) tlen = dst_size - 1 - di;
                memcpy(dst + di, tmp, (size_t)tlen);
                di += tlen;
            }
            else {
                dst[di++] = DOLLAR;     /* lone '$' - keep it */
            }
        }
        else {
            dst[di++] = src[i++];
        }
    }
    dst[di] = EOS;
}

/* ---------------------------------------------------------- */
/*  Arithmetic expression evaluator                           */
/*                                                            */
/*  Grammar:                                                  */
/*    expression := term ( (+|-) term )*                      */
/*    term       := factor ( (*|/) factor )*                  */
/*    factor     := [+|-] primary                             */
/*    primary    := number                                    */
/*                | $name                                     */
/*                | name                                      */
/*                | ( expression )                            */
/* ---------------------------------------------------------- */

static urizen_doub parse_factor(urizen_str* expr) {
    urizen_doub value = 0.0;
    urizen_int  neg   = 0;

    if (!expr || !*expr) return 0.0;

    *expr = skip_ws(*expr);
    if (**expr == MINUS) { neg = 1; (*expr)++; }
    else if (**expr == PLUS) { (*expr)++; }
    *expr = skip_ws(*expr);

    if (**expr == LPAR) {
        (*expr)++;
        value = parse_expression(expr);
        *expr = skip_ws(*expr);
        if (**expr == RPAR) (*expr)++;
    }
    else if (**expr == DOLLAR) {
        urizen_char name[MAX_NAME_LEN] = { 0 };
        (*expr)++;
        read_name(expr, name, MAX_NAME_LEN);
        value = get_numeric_value(name);
    }
    else if (isalpha((urizen_byte)**expr) || **expr == '_') {
        urizen_char name[MAX_NAME_LEN] = { 0 };
        read_name(expr, name, MAX_NAME_LEN);
        /* Bare identifier inside expr - treat as variable lookup. */
        value = get_numeric_value(name);
    }
    else {
        value = strtod(*expr, expr);
    }

    return neg ? -value : value;
}

urizen_doub parse_term(urizen_str* expr) {
    urizen_doub value = 0.0;
    if (!expr || !*expr) return 0.0;
    value = parse_factor(expr);
    for (;;) {
        *expr = skip_ws(*expr);
        if (**expr == MULT || **expr == DIV) {
            urizen_char op  = *(*expr)++;
            urizen_doub rhs = parse_factor(expr);
            if (op == MULT) {
                value *= rhs;
            }
            else {
                if (rhs != 0.0) value /= rhs;
                else {
                    printf("Error: division by zero!\n");
                    value = 0.0;
                }
            }
        }
        else break;
    }
    return value;
}

urizen_doub parse_expression(urizen_str* expr) {
    urizen_doub value = 0.0;
    if (!expr || !*expr) return 0.0;
    value = parse_term(expr);
    for (;;) {
        *expr = skip_ws(*expr);
        if (**expr == PLUS || **expr == MINUS) {
            urizen_char op  = *(*expr)++;
            urizen_doub rhs = parse_term(expr);
            if (op == PLUS) value += rhs;
            else            value -= rhs;
        }
        else break;
    }
    return value;
}

/* ---------------------------------------------------------- */
/*  Command handlers                                          */
/* ---------------------------------------------------------- */

/*
 *  handle_set
 *  ----------
 *   set <name> <value>
 *
 *   <value> is one of
 *     "literal"           -> string (with $var interpolation)
 *     'X'                 -> char
 *     true | false        -> boolean
 *     [expr { <arith> }]  -> arithmetic expression
 *     $otherVar           -> copy of another variable
 *     number | expr       -> numeric (evaluated as an arithmetic expression)
 */
urizen_void handle_set(urizen_str line) {
    urizen_str  p    = NULL;
    urizen_char name[MAX_NAME_LEN] = { 0 };

    if (!line) return;

    /* Skip the `set` keyword. */
    p = skip_ws(line);
    while (*p != EOS && !isspace((urizen_byte)*p)) p++;
    p = skip_ws(p);

    /* Variable name. */
    if (read_name(&p, name, MAX_NAME_LEN) == 0) {
        printf("Error: `set` expects a variable name.\n");
        return;
    }

    p = skip_ws(p);
    if (*p == EOS) {
        /* Bare `set x` - nothing to do beyond announcing the variable. */
        if (find_variable(name) == -1) assign_string_variable(name, STREMPTY);
        return;
    }

    /* String literal. */
    if (*p == QUOTES) {
        urizen_char raw[MAX_STR_LEN]   = { 0 };
        urizen_char interp[MAX_STR_LEN] = { 0 };
        urizen_int  i = 0;
        p++;
        while (*p != EOS && *p != QUOTES && i < MAX_STR_LEN - 1) {
            raw[i++] = *p++;
        }
        raw[i] = EOS;
        interpolate_string(raw, interp, (urizen_int)sizeof(interp));
        assign_string_variable(name, interp);
        return;
    }

    /* Character literal. */
    if (*p == QUOTE) {
        urizen_char c = EOS;
        p++;
        if (*p != EOS) c = *p;
        assign_char_variable(name, c);
        return;
    }

    /* Command substitution - only [expr {...}] is supported. */
    if (*p == LBRACK) {
        p = skip_ws(p + 1);
        if (strncmp(p, KW_EXPR, 4) == 0
            && (isspace((urizen_byte)p[4]) || p[4] == LBRACE)) {
            urizen_char expr_buf[MAX_EXPR_LEN] = { 0 };
            urizen_str  e = NULL;
            urizen_doub value = 0.0;
            p = skip_ws(p + 4);
            if (*p == LBRACE) {
                extract_expr_block(&p, expr_buf, (urizen_int)sizeof(expr_buf));
            }
            else {
                urizen_int k = 0;
                while (*p != EOS && *p != RBRACK && k < MAX_EXPR_LEN - 1) {
                    expr_buf[k++] = *p++;
                }
                expr_buf[k] = EOS;
            }
            e = expr_buf;
            value = parse_expression(&e);
            assign_numeric_variable(name, value);
        }
        else {
            printf("Error: only `[expr { ... }]` is supported for command substitution.\n");
        }
        return;
    }

    /* Boolean literals. */
    if (strncmp(p, TRUE_STR, 4) == 0
        && !isalnum((urizen_byte)p[4]) && p[4] != '_') {
        assign_boolean_variable(name, 1);
        return;
    }
    if (strncmp(p, FALSE_STR, 5) == 0
        && !isalnum((urizen_byte)p[5]) && p[5] != '_') {
        assign_boolean_variable(name, 0);
        return;
    }

    /* Copy from another variable:  set a $b */
    if (*p == DOLLAR) {
        urizen_char src[MAX_NAME_LEN] = { 0 };
        urizen_int  idx = -1;
        p++;
        read_name(&p, src, MAX_NAME_LEN);
        idx = find_variable(src);
        if (idx == -1) {
            assign_numeric_variable(name, 0.0);
            return;
        }
        switch (variables[idx].type) {
        case NUMERIC: assign_numeric_variable(name, variables[idx].value.num_value); break;
        case STRING:  assign_string_variable (name, variables[idx].value.str_value); break;
        case BOOLEAN: assign_boolean_variable(name, variables[idx].value.bool_value); break;
        case CHAR:    assign_char_variable   (name, variables[idx].value.char_value); break;
        }
        return;
    }

    /* Braced expression: `set x { 1 + 2 }`. */
    if (*p == LBRACE) {
        urizen_char expr_buf[MAX_EXPR_LEN] = { 0 };
        urizen_str  e = NULL;
        urizen_doub value = 0.0;
        extract_expr_block(&p, expr_buf, (urizen_int)sizeof(expr_buf));
        e = expr_buf;
        value = parse_expression(&e);
        assign_numeric_variable(name, value);
        return;
    }

    /* Otherwise treat the value as an arithmetic expression (e.g. `set x 3.14`). */
    {
        urizen_str  e = p;
        urizen_doub value = parse_expression(&e);
        assign_numeric_variable(name, value);
    }
}

/*
 *  handle_puts
 *  -----------
 *   puts <argument>
 *
 *   <argument> is one of
 *     "literal"           -> string (with $var interpolation)
 *     $name               -> variable value
 *     [expr { <arith> }]  -> arithmetic evaluation (printed as %.2lf)
 *     word                -> printed verbatim
 */
urizen_void handle_puts(urizen_str line) {
    urizen_str  p   = NULL;
    urizen_char out[MAX_EXPR_LEN] = { 0 };

    if (!line) return;

    /* Skip the `puts` keyword. */
    p = skip_ws(line);
    while (*p != EOS && !isspace((urizen_byte)*p)) p++;
    p = skip_ws(p);

    if (*p == QUOTES) {
        urizen_char raw[MAX_STR_LEN] = { 0 };
        urizen_int  i = 0;
        p++;
        while (*p != EOS && *p != QUOTES && i < MAX_STR_LEN - 1) {
            raw[i++] = *p++;
        }
        raw[i] = EOS;
        interpolate_string(raw, out, (urizen_int)sizeof(out));
    }
    else if (*p == DOLLAR) {
        urizen_char name[MAX_NAME_LEN] = { 0 };
        p++;
        read_name(&p, name, MAX_NAME_LEN);
        format_variable_by_name(name, out, (urizen_int)sizeof(out));
    }
    else if (*p == LBRACK) {
        p = skip_ws(p + 1);
        if (strncmp(p, KW_EXPR, 4) == 0
            && (isspace((urizen_byte)p[4]) || p[4] == LBRACE)) {
            urizen_char expr_buf[MAX_EXPR_LEN] = { 0 };
            urizen_str  e = NULL;
            urizen_doub value = 0.0;
            p = skip_ws(p + 4);
            if (*p == LBRACE) {
                extract_expr_block(&p, expr_buf, (urizen_int)sizeof(expr_buf));
            }
            e = expr_buf;
            value = parse_expression(&e);
            snprintf(out, sizeof(out), "%.2lf", value);
        }
    }
    else {
        /* Bare word: copy until whitespace / end. */
        urizen_int i = 0;
        while (*p != EOS && !isspace((urizen_byte)*p) && i < (urizen_int)sizeof(out) - 1) {
            out[i++] = *p++;
        }
        out[i] = EOS;
    }

    strcat_s(output_buffer, sizeof(output_buffer), out);
    strcat_s(output_buffer, sizeof(output_buffer), "\n");
}

/* handle_write kept as a spec-compatible alias for handle_puts. */
urizen_void handle_write(urizen_str expression) {
    handle_puts(expression);
}

/* ---------------------------------------------------------- */
/*  calculate - dispatch a single source line                 */
/* ---------------------------------------------------------- */
urizen_void calculate(urizen_str expression) {
    urizen_str p = NULL;

    if (!expression) return;

    p = skip_ws(expression);

    /* Blank line or comment. */
    if (*p == EOS)  return;
    if (*p == HASH) return;

    /* `set <name> <value>` */
    if (strncmp(p, KW_SET, 3) == 0 && isspace((urizen_byte)p[3])) {
        handle_set(p);
        return;
    }

    /* `puts <argument>` */
    if (strncmp(p, KW_PUTS, 4) == 0 && isspace((urizen_byte)p[4])) {
        handle_puts(p);
        return;
    }

    /* Anything else is silently ignored - the spec asks us to recognise
       only the minimum grammar.  We keep the line for diagnostic purposes. */
}

/* ---------------------------------------------------------- */
/*  Line splitter (used by process_content)                   */
/* ---------------------------------------------------------- */
static urizen_str* splitIntoLines(const urizen_str content, urizen_int* lineCount) {
    urizen_str* lines = NULL;
    const urizen_char* start = NULL;
    const urizen_char* end   = NULL;

    if (!content || !lineCount) return NULL;

    lines = (urizen_str*)malloc(MAX_LINES * sizeof(urizen_str));
    if (!lines) {
        perror("Error allocating memory for lines");
        return NULL;
    }

    *lineCount = 0;
    start = content;

    while ((end = strchr(start, NEWLINE)) != NULL) {
        urizen_int lineLength = 0;
        if (*lineCount >= MAX_LINES) {
            fprintf(stderr, "Exceeded maximum number of lines (%d).\n", MAX_LINES);
            break;
        }
        lineLength = (urizen_int)(end - start);
        /* Strip a trailing CR if the file uses CRLF line endings. */
        if (lineLength > 0 && start[lineLength - 1] == RETURN) lineLength--;

        lines[*lineCount] = (urizen_str)malloc((size_t)lineLength + 1);
        if (!lines[*lineCount]) {
            perror("Error allocating memory for line");
            break;
        }
        if (lineLength > 0) memcpy(lines[*lineCount], start, (size_t)lineLength);
        lines[*lineCount][lineLength] = EOS;

        (*lineCount)++;
        start = end + 1;
    }

    /* Trailing content without a final newline. */
    if (*start != EOS && *lineCount < MAX_LINES) {
        size_t len = strlen(start);
        lines[*lineCount] = (urizen_str)malloc(len + 1);
        if (lines[*lineCount]) {
            memcpy(lines[*lineCount], start, len);
            lines[*lineCount][len] = EOS;
            (*lineCount)++;
        }
    }

    return lines;
}

static urizen_void freeLines(urizen_str* lines, urizen_int lineCount) {
    urizen_int i = 0;
    if (!lines) return;
    for (i = 0; i < lineCount; i++) {
        if (lines[i]) free(lines[i]);
    }
    free(lines);
}

/* ---------------------------------------------------------- */
/*  Public driver functions                                   */
/* ---------------------------------------------------------- */

/* Print the accumulated outputs and the variable table. */
static urizen_void print_results(urizen_void) {
    urizen_int i = 0;

    printf("Outputs from file: \n");
    if (output_buffer[0] != EOS) printf("%s", output_buffer);

    printf("\nVariable values:\n");
    for (i = 0; i < var_count; i++) {
        switch (variables[i].type) {
        case NUMERIC:
            printf("%s = %.2lf\n", variables[i].name, variables[i].value.num_value);
            break;
        case STRING:
            printf("%s = \"%s\"\n", variables[i].name, variables[i].value.str_value);
            break;
        case BOOLEAN:
            printf("%s = %s\n", variables[i].name,
                variables[i].value.bool_value ? TRUE_STR : FALSE_STR);
            break;
        case CHAR:
            printf("%s = '%c'\n", variables[i].name, variables[i].value.char_value);
            break;
        }
    }
}

urizen_void process_file(const urizen_str filename) {
    FILE*       file = NULL;
    urizen_char line[MAX_EXPR_LEN] = { 0 };

    if (!filename) {
        printf("Error: process_file received a NULL filename.\n");
        return;
    }

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = EOS;
        if (line[0] == EOS) continue;
        calculate(line);
    }
    fclose(file);

    initial_phase = 0;
    print_results();
}

urizen_void process_content(urizen_str fileContent) {
    urizen_int  lineCount = 0;
    urizen_str* lines     = NULL;
    urizen_int  i         = 0;

    if (!fileContent) {
        printf("Error: process_content received NULL content.\n");
        return;
    }

    lines = splitIntoLines(fileContent, &lineCount);
    if (!lines) return;

    for (i = 0; i < lineCount; i++) {
        if (lines[i]) calculate(lines[i]);
    }

    initial_phase = 0;
    print_results();

    freeLines(lines, lineCount);
}
