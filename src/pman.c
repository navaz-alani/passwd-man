// Password manager
struct pman
{
    const char *master_pwd;
};

struct pman *mk_pman(const char *master_pwd)
{
    struct pman *new = malloc(sizeof(struct pman));
    if (!new)
        return NULL;

    new->master_pwd = malloc((strlen(master_pwd) + 1) * sizeof(char));
    if (!new->master_pwd)
    {
        free(new);
        return NULL;
    }
    strcpy((char *)new->master_pwd, master_pwd);

    return new;
}

void free_pman(struct pman *p)
{
    if (!p)
        return;

    free_pwd((char *)p->master_pwd);
    free(p);
}

bool copy_to_clipboard(const char *txt)
{
    char *cp_cmd = "python3 -c 'import pyperclip as pc; pc.copy(\"%s\")'";
    int cmd_len = strlen(cp_cmd);
    int txt_len = strlen(txt);
    int buff_size = cmd_len + txt_len;

    char *buff = malloc((buff_size + 1) * sizeof(char));
    if (!buff)
    {
        printf("ERROR [CP]: Failed to copy!\n");
        return false;
    }

    sprintf(buff, cp_cmd, txt);

    if (get_bool("Copy password to clipboard? "))
    {
        system(buff);
        printf("INFO [CP]: Copied to clipboard.\n\n");
    }

    free(buff);
    return false;
}

void new_creds(struct pman *p)
{
    if (!p)
        return;

    char *acct_name = get_input("name", "Enter account name: ");
    char *uid = get_input("user-id", "Enter username: ");
    char *pwd = get_passwd("Enter password: ");

    char *fname = __filename__(acct_name, p->master_pwd);
    free(acct_name);

    if (access(fname, F_OK) == 0)
    {
        printf("ERROR [ACCT-new]: Account already exists!\n");
        goto panic;
    }

    FILE *new_acct = fopen(fname, "w");

    if (!new_acct)
    {
        printf("ERROR [ACCT-new]: Failed to create account file!\n");
        goto panic;
    }

    char *uid_enc = __enc__(uid, p->master_pwd);
    fputs(uid_enc, new_acct);
    free(uid_enc);
    free(uid);

    char *pwd_enc = __enc__(pwd, p->master_pwd);
    fputs(pwd_enc, new_acct);
    free(pwd_enc);
    free(pwd);

    if (fclose(new_acct))
        printf("ERROR [FILE-cl]: Failed to close account file!\n");
    free(fname);
    return;

panic:
    free(fname);
    free(uid);
    free(pwd);
}

void retrieve_creds(struct pman *p, char *acct_name)
{
    if (!(p && acct_name))
        return;

    char *fname = __filename__(acct_name, p->master_pwd);

    if (access(fname, F_OK) != 0)
    {
        printf("ERROR [ACCT-new]: Account does not exist!\n");
        free(fname);
        return;
    }

    FILE *new_acct = fopen(fname, "r");

    if (!new_acct)
    {
        printf("ERROR [ACCT-new]: Failed to create account file!\n");
        free(fname);
        return;
    }

    char *uid_enc = readline(new_acct);
    char *uid = __dec__(uid_enc, p->master_pwd);
    free(uid_enc);

    char *pwd_enc = readline(new_acct);
    char *pwd = __dec__(pwd_enc, p->master_pwd);
    // make up for newline
    pwd[strlen(pwd) - 1] = '\0';
    free(pwd_enc);

    if (fclose(new_acct))
        printf("ERROR [FILE-cl]: Failed to close account file!\n");
    free(fname);

    printf("\t[%s] UserID: %s\n", acct_name, uid);

    if (copy_to_clipboard(pwd))
        printf("\tPassword copied to clipboard!\n");

    free(uid);
    free(pwd);
}

void edit_creds(struct pman *p, char *acct_name)
{
    if (!(p && acct_name))
        return;

    char *fname = __filename__(acct_name, p->master_pwd);

    if (access(fname, F_OK) != 0)
    {
        printf("ERROR [ACCT-new]: Account does not exist!\n");
        free(fname);
        return;
    }

    FILE *acct = fopen(fname, "r");

    if (!acct)
    {
        printf("ERROR [ACCT-new]: Failed to edit account file!\n");
        free(fname);
        return;
    }

    char *uid_enc = readline(acct);
    char *uid = __dec__(uid_enc, p->master_pwd);
    free(uid_enc);

    char *pwd_enc = readline(acct);
    char *pwd = __dec__(pwd_enc, p->master_pwd);
    free(pwd_enc);

    if (fclose(acct))
        printf("ERROR [FILE-cl]: Failed to close account file!\n");

    char *new_acct_name = acct_name;
    char *new_uid = uid;
    char *new_pwd = pwd;

    if (get_bool("Edit account name? "))
    {
        char *tmp = get_input("New account name", "Enter new account name: ");
        if (!tmp)
            printf("ERROR [ACCT_EDIT-read-acct_name]: Failed to read new account name. No changes made.");
        else
            new_acct_name = tmp;
    }
    if (get_bool("Edit username? "))
    {
        char *tmp = get_input("New user ID", "Enter new user ID: ");
        if (!tmp)
            printf("ERROR [ACCT_EDIT-read_uid]: Failed to read new user ID. No changes made.\n");
        else
            new_uid = tmp;
    }
    if (get_bool("Edit password? "))
    {
        char *tmp = get_passwd("Enter new password: ");
        if (!tmp)
            printf("ERROR [ACCT_PWD-read_pwd]: Failed to read new password. No changes made.\n");
        else
            new_pwd = tmp;
    }

    FILE *updated_acct = fopen(fname, "w");

    if (!updated_acct)
    {
        printf("ERROR [ACCT-new]: Failed to create account file!\n");
        // make panic routine
    }

    uid_enc = __enc__(new_uid, p->master_pwd);
    fputs(uid_enc, updated_acct);
    free(uid_enc);
    printf("INFO [ACCT_EDIT-uid]: User ID for '%s' changed to '%s'\n",
           new_acct_name, new_uid);

    if (new_uid == uid)
        free(new_uid);
    else
    {
        free(new_uid);
        free(uid);
    }

    pwd_enc = __enc__(new_pwd, p->master_pwd);
    fputs(pwd_enc, updated_acct);
    free(pwd_enc);
    printf("INFO [ACCT_EDIT-pwd]: Password for '%s' changed\n", new_acct_name);

    if (new_pwd == pwd)
        free(new_pwd);
    else
    {
        free(new_pwd);
        free(pwd);
    }

    if (fclose(updated_acct))
        printf("ERROR [FILE-cl]: Failed to close account file!\n");
    free(fname);

    if (new_acct_name != acct_name)
    {
        char *new_fname = __filename__(acct_name, p->master_pwd);
        char *mv_cmd = "mv %s %s";
        int cmd_len = strlen(mv_cmd) + strlen(acct_name) + strlen(new_acct_name);
        char *cmd = malloc((cmd_len + 1) * sizeof(char));
        if (!cmd)
        {
            printf("ERROR [ACCT_EDIT-acct_name]: Failed to change account name!\n");
            free(new_acct_name);
            free(acct_name);
            free(new_fname);
            return;
        }
        sprintf(cmd, mv_cmd, fname, new_fname);
        system(cmd);
        printf("INFO [ACCT_EDIT-acct_name]: Account name for '%s' changed to '%s'\n",
               acct_name, new_acct_name);
    }
    else
    {
        free(new_acct_name);
    }
}

void delete_creds(struct pman *p, char *acct_name)
{
    if (!p)
        return;

    // run rm cmd
}
