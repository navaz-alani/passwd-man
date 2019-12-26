#pragma once

/* 
 * Navaz Alani, 2019 
 * PASSWORD MANAGER:
 *     Cryptography module
 * 
 * The following module is used to implement
 * string digests and encryption in the context
 * of the password manager.
 * 
 * It makes use of the OpenSSL command line tool
 * in order to do so.
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "io.h"

#define MPASS_FILE "src/creds/master.pwd.enc"

/* 
 * free_pwd(pwd) wipes the string at pwd
 * and frees the memory.
 * This is to safely discard memory with
 * sensitive information.
 */
void free_pwd(char *pwd);

/*
 * __salt__() generates a random hexadecimal
 * number and returns a pointer to its string
 * representation.
 */
char *__salt__();

/*
 * __hash__(txt, salt) computes the hash of
 * provided txt while salting with given salt.
 * Returns a pointer to the string representation
 * of the base64 hash.
 */
char *__hash__(char *txt, char *salt);

/*
 * new_mpwd(new_pwd) changes the program's master
 * password to the new specified one.
 * 
 * If successful, returns true, else false.
 */
bool new_mpwd(char *new_pwd);

/*
 * verf_mpwd(pwd) verifies whether provided pwd
 * matches the current program master password.
 * 
 * If successful, returns true, else false.
 */
bool verif_mpwd(char *pwd);

/*
 * __filename__(acct_name, mpwd) generates the filename
 * for the account called acct_name using the provided
 * master password; mpwd.
 * 
 * Returns a pointer to a string containing the filename.
 * If unsuccessful, NULL is returned.
 */
char *__filename__(char *acct_name, const char *mpwd);

/*
 * __enc__(txt, mpwd) encrypts the provided txt using
 * the master password mpwd.
 * 
 * Returns string containing the base64 representation
 * of the encrypted value.
 * If unsuccessful, returns NULL
*/
char *__enc__(char *txt, const char *mpwd);

/*
 * __dec__(txt, mpwd) decrypts the provided txt using
 * the master password mpwd.
 * 
 * Returns string containing the original text, before
 * encryption.
 * If unsuccessful, returns NULL
*/
char *__dec__(char *txt, const char *mpwd);

#include "crypto.c"