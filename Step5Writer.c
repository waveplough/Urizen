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
# ECHO "[CODER SCRIPT ..........................]"
# ECHO "                                         "
*/

/*
***********************************************************
* File name: Writer.c
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
#define _CRT_SECURE_NO_WARNINGS
#ifndef CODER_H_
#include "Step5Writer.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Global variables */
Variable variables[MAX_VARS];
urizen_int var_count = 0;
urizen_int initial_phase = 1; // Flag to track the initial phase
urizen_char output_buffer[MAX_EXPR_LEN * 10] = { 0 }; // Buffer to store write output

/* Finds variables */
urizen_int find_variable(const urizen_str name) {
    urizen_int i = 0;
    for (i = 0; i < var_count; ++i) {
        if (strcmp(variables[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

/* Assign numerical variable */
urizen_void assign_numeric_variable(const urizen_str name, urizen_doub value) {
    urizen_int idx = find_variable(name);
    if (idx == -1) {
        idx = var_count++;
        strcpy_s(variables[idx].name, sizeof(variables[idx].name), name);
    }
    variables[idx].type = NUMERIC;
    variables[idx].value.num_value = value;
}

/* Assign string variable */
urizen_void assign_string_variable(const urizen_str name, const urizen_str value) {
    urizen_int idx = find_variable(name);
    if (idx == -1) {
        idx = var_count++;
        strcpy_s(variables[idx].name, sizeof(variables[idx].name), name);
    }
    variables[idx].type = STRING;
    strncpy_s(variables[idx].value.str_value, sizeof(variables[idx].value.str_value), value, sizeof(variables[idx].value.str_value) - 1);
    variables[idx].value.str_value[sizeof(variables[idx].value.str_value) - 1] = EOS;
}

/* Assign boolean variable */
urizen_void assign_boolean_variable(const urizen_str name, urizen_int value) {
    urizen_int idx = find_variable(name);
    if (idx == -1) {
        idx = var_count++;
        strcpy_s(variables[idx].name, sizeof(variables[idx].name), name);
    }
    variables[idx].type = BOOLEAN;
    variables[idx].value.bool_value = value;
}

urizen_int get_boolean_value(const urizen_str name) {
    urizen_int idx = find_variable(name);
    if (idx != -1 && variables[idx].type == BOOLEAN) {
        return variables[idx].value.bool_value;
    }
    return 0; // Default to false if variable not found
}

/* Assign char variable */
urizen_void assign_char_variable(const urizen_str name, urizen_char value) {
    urizen_int idx = find_variable(name);
    if (idx == -1) {
        idx = var_count++;
        strcpy_s(variables[idx].name, sizeof(variables[idx].name), name);
    }
    variables[idx].type = CHAR;
    variables[idx].value.char_value = value;
}

/* Gets char variable */
urizen_char get_char_value(const urizen_str name) {
    urizen_int idx = find_variable(name);
    if (idx != -1 && variables[idx].type == CHAR) {
        return variables[idx].value.char_value;
    }
    return EOS; // Default to null character if variable not found
}

/* Gets numerical value */
urizen_doub get_numeric_value(const urizen_str name) {
    int idx = find_variable(name);
    if (idx != -1 && variables[idx].type == NUMERIC) {
        return variables[idx].value.num_value;
    }
    return ZERO;
}

/* Gets string variable */
const urizen_str get_string_value(const urizen_str name) {
    urizen_int idx = find_variable(name);
    if (idx != -1 && variables[idx].type == STRING) {
        return variables[idx].value.str_value;
    }
    return STREMPTY;
}

/* Parsing for term */
urizen_doub parse_term(urizen_str* expr) {
    urizen_doub value = 0.0;
    while (isspace(**expr)) (*expr)++;
    if (**expr == LPAR) {
        (*expr)++;
        value = parse_expression(expr);
        if (**expr == RPAR) {
            (*expr)++;
        }
    }
    else if (isalpha(**expr)) {
        urizen_char var_name[32] = { 0 };
        urizen_int i = 0;
        while (isalnum(**expr)) {
            var_name[i++] = *(*expr)++;
        } // Finishes with alpha and number
        var_name[i++] = *(*expr)++; // Includes the suffix
        value = get_numeric_value(var_name);
    }
    else {
        value = strtod(*expr, expr);
    }
    while (isspace(**expr)) (*expr)++;
    while (**expr == MULT || **expr == DIV) {
        urizen_char op = *(*expr)++;
        urizen_doub next_value = parse_term(expr);
        if (op == MULT) {
            value *= next_value;
        }
        else {
            if (next_value != 0)
                value /= next_value;
            else {
                printf("Error: division by zero! ");
                value = 0;
            }
        }
    }
    return value;
}

/* Parsing for expression */
urizen_doub parse_expression(urizen_str* expr) {
    urizen_doub value = parse_term(expr);
    while (isspace(**expr)) (*expr)++;
    while (**expr == PLUS || **expr == MINUS) {
        urizen_char op = *(*expr)++;
        urizen_doub next_value = parse_term(expr);
        if (op == PLUS) {
            value += next_value;
        }
        else {
            value -= next_value;
        }
    }
    return value;
}

/* Write output */
urizen_void handle_write(urizen_str expression) {
    urizen_char buffer[MAX_EXPR_LEN] = { 0 };
    urizen_str start = strchr(expression, LPAR) + 1;
    urizen_str end = strrchr(expression, RPAR);
    if (start != NULL && end != NULL && start < end) {
        *end = EOS;
        while (*start != EOS) {
            if (*start == QUOTES) {
                start++;
                while (*start != QUOTES && *start != EOS) {
                    strncat_s(buffer, sizeof(buffer), start, 1);
                    start++;
                }
                start++;
            }
            else if (isalpha(*start)) {
                urizen_char var_name[32] = { 0 };
                urizen_int i = 0;
                while (isalnum(*start)) {
                    var_name[i++] = *start++;
                } // Finishes with alpha and number
                var_name[i++] = *start++; // Includes the suffix
                urizen_int var_idx = find_variable(var_name);
                if (var_idx != -1) {
                    if (variables[var_idx].type == STRING) {
                        strcat_s(buffer, sizeof(buffer), get_string_value(var_name));
                    }
                    else if (variables[var_idx].type == NUMERIC) {
                        urizen_char num_str[256];
                        snprintf(num_str, sizeof(num_str), "%.2lf", get_numeric_value(var_name));
                        strcat_s(buffer, sizeof(buffer), num_str);
                    }
                    else if (variables[var_idx].type == BOOLEAN) {
                        strcat_s(buffer, sizeof(buffer), get_boolean_value(var_name) ? TRUE : FALSE);
                    }
                    else if (variables[var_idx].type == CHAR) {
                        urizen_char char_str[2];
                        char_str[0] = get_char_value(var_name);
                        char_str[1] = EOS;
                        strcat_s(buffer, sizeof(buffer), char_str);
                    }
                }
            }
            else if (isspace(*start)) {
                strncat_s(buffer, sizeof(buffer), start, 1);
                start++;
            }
            else {
                start++;
            }
        }
    }
    if (initial_phase) {
        strcat_s(output_buffer, sizeof(output_buffer), buffer);
        char replacement[] = "[Undefined]";
        if (strlen(buffer) == 0) {
            strcpy(buffer, replacement);
            strcat_s(output_buffer, sizeof(output_buffer), buffer);
        }
        strcat_s(output_buffer, sizeof(output_buffer), "\n");
    }
    else {
        printf("%s\n", buffer);
    }
}

/* Calculate expression */
urizen_void calculate(urizen_str expression) {
    urizen_char var_name[32] = { 0 };
    if (strchr(expression, EQUALS)) {
        sscanf_s(expression, "%31s =", var_name, (unsigned)_countof(var_name));
        urizen_str expr = strchr(expression, EQUALS) + 1;
        while (isspace(*expr)) expr++;
        if (*expr == QUOTES) {
            expr++;
            urizen_char str_value[256] = { 0 };
            urizen_int i = 0;
            while (*expr != QUOTES && *expr != EOS) {
                str_value[i++] = *expr++;
            }
            assign_string_variable(var_name, str_value);
            if (!initial_phase) {
                printf("%s = \"%s\"\n", var_name, str_value);
            }
        }
        else if (strncmp(expr, TRUE, 4) == 0 || strncmp(expr, FALSE, 5) == 0) {
            urizen_int bool_value = strncmp(expr, TRUE, 4) == 0 ? 1 : 0;
            assign_boolean_variable(var_name, bool_value);
            if (!initial_phase) {
                printf("%s = %s\n", var_name, bool_value ? TRUE : FALSE);
            }
        }
        else if (*expr == QUOTE) {
            expr++;
            urizen_char char_value = *expr;
            assign_char_variable(var_name, char_value);
            if (!initial_phase) {
                printf("%s = '%c'\n", var_name, char_value);
            }
        }
        else {
            urizen_doub value = parse_expression(&expr);
            assign_numeric_variable(var_name, value);
            if (!initial_phase) { ///xxx
                printf("%s = %.2lf\n", var_name, value);
            }
        }
    }
    else if (strstr(expression, WRITE)) {
        handle_write(expression);
    }
    else {
        urizen_str expr = expression;
        urizen_doub result = parse_expression(&expr);
        if (!initial_phase) {
            printf("Result: %.2lf\n", result);
        }
    }
}

/* Process input file */
urizen_void process_file(const urizen_str filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    urizen_char line[MAX_EXPR_LEN];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == NEWLINE || line[0] == RETURN) {
            continue; // Skip empty lines
        }
        line[strcspn(line, "\n")] = 0; // Remove newline character
        calculate(line);
    }
    fclose(file);
    initial_phase = 0; // End of initial phase
    printf("%s", output_buffer); // Print the buffered write output
    printf("\nVariable values:\n");
	urizen_int i = 0;
    for (i = 0; i < var_count; i++) {
        if (variables[i].type == STRING) {
            printf("%s = \"%s\"\n", variables[i].name, variables[i].value.str_value);
        }
        else if (variables[i].type == NUMERIC) {
            printf("%s = %.2lf\n", variables[i].name, variables[i].value.num_value);
        }
        else if (variables[i].type == BOOLEAN) {
            printf("%s = %s\n", variables[i].name, variables[i].value.bool_value ? "true" : "false");
        }
        else if (variables[i].type == CHAR) {
            printf("%s = '%c'\n", variables[i].name, variables[i].value.char_value);
        }
    }
}

/* Split lines into components */
urizen_str* splitIntoLines(const urizen_str content, urizen_int* lineCount) {
    urizen_str* lines = malloc(MAX_LINES * sizeof(char*));
    if (!lines) {
        perror("Error allocating memory");
        return NULL;
    }
    *lineCount = 0;
    const urizen_char* start = content;
    const urizen_char* end;
    while ((end = strchr(start, NEWLINE)) != NULL) {
        if (*lineCount >= MAX_LINES) {
            fprintf(stderr, "Exceeded maximum number of lines\n");
            break;
        }
        urizen_int lineLength = (urizen_int) (end - start);
        lines[*lineCount] = malloc(lineLength + 1);
        if (!lines[*lineCount]) {
            perror("Error allocating memory for line");
            break;
        }
        // Use strncpy_s for safer copying
        if (strncpy_s(lines[*lineCount], lineLength + 1, start, lineLength) != 0) {
            perror("Error copying string using strncpy_s");
            free(lines[*lineCount]);
            break;
        }
        lines[*lineCount][lineLength] = EOS; // Null-terminate the string
        (*lineCount)++;
        start = end + 1;
    }
    // Handle the last line if it doesn't end with a newline
    if (*start != EOS && *lineCount < MAX_LINES) {
        size_t lineLength = strlen(start);
        lines[*lineCount] = malloc(lineLength + 1);
        if (!lines[*lineCount]) {
            perror("Error allocating memory for line");
        }
        else {
            // Use _strdup to copy the remaining content
            lines[*lineCount] = _strdup(start);
            if (!lines[*lineCount]) {
                perror("Error copying string using _strdup");
            }
            else {
                (*lineCount)++;
            }
        }
    }
    return lines;
}

/* Free lines */
urizen_void freeLines(urizen_str* lines, urizen_int lineCount) {
    urizen_int i = 0;
    for (i = 0; i < lineCount; i++) {
        free(lines[i]);
    }
    free(lines);
}

/* Process content */
urizen_void process_content(urizen_str fileContent) {
    urizen_int lineCount = 0;
    urizen_str* lines = splitIntoLines(fileContent, &lineCount);
    urizen_str line = malloc(MAX_EXPR_LEN);
    if (!lines || !line) {
        return;
    }
    printf("Lines from content:\n");
    urizen_int i = 0;
	for (i = 0; i < lineCount; i++) {
		line = lines[i];
		calculate(line);
	}
    initial_phase = 0; // End of initial phase
    printf("%s", output_buffer); // Print the buffered write output
    printf("\nVariable values:\n");
    for (i = 0; i < var_count; i++) {
        if (variables[i].type == STRING) {
            printf("%s = \"%s\"\n", variables[i].name, variables[i].value.str_value);
        }
        else if (variables[i].type == NUMERIC) {
            printf("%s = %.2lf\n", variables[i].name, variables[i].value.num_value);
        }
        else if (variables[i].type == BOOLEAN) {
            printf("%s = %s\n", variables[i].name, variables[i].value.bool_value ? "true" : "false");
        }
        else if (variables[i].type == CHAR) {
            printf("%s = '%c'\n", variables[i].name, variables[i].value.char_value);
        }
    }
}
