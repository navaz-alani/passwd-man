#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "src/pman.h"
#include "src/io.h"
#include "src/crypto.h"

const char *welcome = "\npman: A Command Line Password manager.\n"
                      "Navaz Alani © 2019\n\n";
const char *options_prompt = "What would you like to do?\n";
const char *options = "- Store NEW credentials (n)\n"
                      "- Retrieve credentials (r)\n"
                      "- Edit credentials (e)\n"
                      "- Delete credentials (d)\n"
                      "- Print this menu again (m)\n"
                      "- Admin test (t)\n"
                      "- Quit program (q)\n\n";

int main(int argc, char **argv)
{
    printf("%s", welcome);

    char *mpass;

    if (access(MPASS_FILE, F_OK) != -1)
    {
        mpass = get_passwd("Enter master password: ");

        if (!verif_mpwd(mpass))
        {
            free_pwd(mpass);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if (get_bool("No master password recorded. Make a new record? "))
            mpass = get_passwd("Enter new master password: ");
        else
            exit(EXIT_SUCCESS);

        char *tmp = get_passwd("Re-enter password for confirmation: ");

        if (!strcmp(tmp, mpass))
        {
            if (!new_mpwd(mpass))
                goto exit_fail;
        }
        else
        {
            printf("ERROR [PWD-Verif]: Passwords do not match!\n");
        exit_fail:
            free_pwd(mpass);
            free_pwd(tmp);
            exit(EXIT_FAILURE);
        }

        free_pwd(tmp);
    }

    struct pman *p = mk_pman((const char *)mpass);
    free(mpass);
    printf("%s", options_prompt);

    char input = '\0';
    printf("%s", options);

    while (true)
    {
        printf("> ");

        if (scanf(" %c", &input) != 1)
        {
            printf("ERROR [INPUT]: Failed to read input. Please retry.\n");
            continue;
        }

        if (input == 'n')
        {
            fgetc(stdin);
            new_creds(p);
        }
        else if (input == 'r')
        {
            fgetc(stdin);
            char *acct_name = get_input("account name", "Enter account name: ");
            retrieve_creds(p, acct_name);
            free(acct_name);
        }
        else if (input == 'e')
        {
            fgetc(stdin);
            char *acct_name = get_input("account name", "Enter account name: ");
            edit_creds(p, acct_name);
            free(acct_name);
        }
        else if (input == 'd')
        {
            fgetc(stdin);
            char *acct_name = get_input("account name", "Enter account name: ");
            delete_creds(p, acct_name);
            free(acct_name);
        }
        else if (input == 'm')
            printf("%s", options);
        else if (input == 'q')
            break;
        else
            printf("ERROR [INPUT]: Invalid selection.\n");
    }

    free_pman(p);
}
