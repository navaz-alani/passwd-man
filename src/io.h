#pragma once

/**
 * Navaz Alani, 2019
 * PASSWORD MANAGER:
 *     IO Module
 * 
 * The following module provides functions for
 * dealing with file IO as well as reading
 * arbitrarily long input from the user.
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <termios.h>

/**
 * __read_input__() reads in user input from stdin
 * until the newline is encountered (user presses
 * enter).
 * Returns a pointer to a string containing the
 * user input or NULL if unsuccessful.
*/
char *__read_input__();

/**
 * get_input(value_name, prompt) prompts the user for
 * the value specified by value_name using the given
 * prompt. Prevents the user from submitting empty string.
 *
 * Returns pointer to user input
*/
char *get_input(const char *value_name, const char *prompt);

/**
 * get_bool(prompt) uses the given prompt to ask the user
 * for Yes/No input. Accepts Y,y,N,n.
 * Returns corresponding boolen value.
*/
bool get_bool(const char *prompt);

/**
 * get_passwd(prompt) asks the user for their password
 * using the given prompt. User input is masked using
 * the '*' character.
*/
char *get_passwd(const char *prompt);

/**
 * __get_output__(cmd) executes the given command and
 * captures the resulting output to stdout.
 * Returns a pointer to a string containing the output.
*/
char *__get_output__(char *cmd);

/**
 * readline(f) reads from file f until encountering a
 * newline character.
 * Returns pointer to string containing the line read. 
*/
char *readline(FILE *f);

#include "io.c"
