#pragma once

/**
 * Navaz Alani, 2019
 * PASSWORD MANAGER:
 *     Interface and Operations Module
 * 
 * The following module builds around the Cryptography
 * module and implements the password manager.
 * It also uses the IO module to implement the CLI.
*/

#include <stdlib.h>
#include <stdio.h>

#include "io.h"
#include "crypto.h"

/**
 * struct pman packages the program's master password.
*/
struct pman;

/**
 * mk_pman(master_password) packages the provided master
 * password so that it can be freed.
 * Returns a pointer to a struct pman or NULL if
 * unsuccessful.
*/
struct pman *mk_pman(const char *master_pwd);

/**
 * free_pman(p) frees the provided struct pman and
 * other related memory allocations.
*/
void free_pman(struct pman *p);

/**
 * copy_to_clipboard(txt) copies the provided text
 * to the client machine's clipboard.
 * 
 * Returns true if successful, false otherwise.
*/
bool copy_to_clipboard(const char *txt);

/**
 * new_creds(p) uses the master password represented by
 * p and interactively sets up a new account to be stored.
*/
void new_creds(struct pman *p);

/**
 * retrieve_creds(p, acct_name) retrieves the credentials
 * for account called acct_name.
 * Displays username to terminal and asks user if he/she
 * wants to copy this account's password to the clipboard.
*/
void retrieve_creds(struct pman *p, char *acct_name);

/**
 * edit_creds(p, acct_name) interactively edits the account
 * called acct_name (if it exists).
 * User can change account name, username and password.
*/
void edit_creds(struct pman *p, char *acct_name);

/**
 * delete_creds(p, acct_name) deletes the account called
 * acct_name.
 * This includes the username and password corresponding to
 * acct_name.
*/
void delete_creds(struct pman *p, char *acct_name);

#include "pman.c"
