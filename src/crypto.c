void free_pwd(char *pwd)
{
    if (!pwd)
        return;
    memset(pwd, '\0', strlen(pwd));
    free(pwd);
}

char *__salt__()
{
    return __get_output__("openssl rand -hex 50");
}

char *__hash__(char *txt, char *salt)
{
    const char *__HASH_MALLOC_ERR__ = "ERROR [HASH-alloc]: Failed to hash!\n";
    char *buff = NULL;

    if (salt != NULL)
    {
        char *hash_cmd = "echo %s | openssl passwd -6 -stdin -salt %s";
        int buff_len = strlen(hash_cmd) + strlen(txt) + strlen(salt);
        buff = malloc((buff_len + 1) * sizeof(char));

        if (!buff)
        {
            printf("%s", __HASH_MALLOC_ERR__);
            return NULL;
        }

        sprintf(buff, hash_cmd, txt, salt);
    }
    else
    {

        char *hash_cmd = "echo %s | openssl passwd -6 -stdin";
        int buff_len = strlen(hash_cmd) + strlen(txt);
        buff = malloc((buff_len + 1) * sizeof(char));

        if (!buff)
        {
            printf("%s", __HASH_MALLOC_ERR__);
            return NULL;
        }

        sprintf(buff, hash_cmd, txt);
    }

    char *ret = __get_output__(buff);
    free(buff);
    return ret;
}

bool new_mpwd(char *mpwd)
{
    char *salt = __salt__();
    printf("new salt: %s\n", salt);
    char *mp_hash = __hash__(mpwd, salt);
    printf("new hash: %s\n", mp_hash);

    FILE *pwd_file = fopen(MPASS_FILE, "w");

    bool ret = false;

    if (!pwd_file)
    {
        printf("ERROR [MPWD-init]: Failed to create new master password!\n");
        goto exit_seq;
    }

    fputs(salt, pwd_file);
    fputs(mp_hash, pwd_file);

    if (fclose(pwd_file))
        printf("ERROR [FILE-cl]: Failed to close file!\n");

    ret = true;
    printf("INFO [MPWD-init]: Successfully created new master password.\n");

exit_seq:
    free_pwd(salt);
    free_pwd(mp_hash);
    return ret;
}

bool verif_mpwd(char *pwd)
{
    FILE *pwd_file = fopen(MPASS_FILE, "r");

    if (!pwd_file)
    {
        printf("ERROR [AUTH-verif]: Failed to verify password!\n");
        return false;
    }

    char *salt = readline(pwd_file);
    char *hash = readline(pwd_file);

    if (fclose(pwd_file))
        printf("ERROR [FILE-cl]: Failed to close file!\n");

    char *pwd_hash = __hash__(pwd, salt);
    // compensate for newline in terminal output
    pwd_hash[strlen(pwd_hash) - 1] = '\0';

    if (strcmp(hash, pwd_hash))
    {
        printf("ERROR [MPASS-verif]: Incorrect password!\n");
        return false;
    }

    printf("INFO [MPASS-verif]: Master password verification successful.\n");
    return true;
}

char *__filename__(char *acct_name, const char *mpwd)
{
    const char *__FNAME_GEN_ERR__ = "ERROR [ACCT-new]: Failed to generate filename!\n";
    FILE *pwd_file = fopen(MPASS_FILE, "r");

    if (!pwd_file)
    {
        printf("%s", __FNAME_GEN_ERR__);
        return false;
    }

    char *salt = readline(pwd_file);

    if (fclose(pwd_file))
        printf("ERROR [FILE-cl]: Failed to close file!\n");

    char *fname_cmd = "printf \"src/creds/%%s.cred.enc\" $(echo %s | openssl passwd -6 -stdin -salt %s | xxd -p -c 256 | cut -c 50-75)";
    int cmd_len = strlen(fname_cmd) + strlen(acct_name) + strlen(salt);
    char *cmd = malloc((cmd_len + 1) * sizeof(char));

    sprintf(cmd, fname_cmd, acct_name, salt);

    if (!cmd)
    {
        printf("%s", __FNAME_GEN_ERR__);
        return false;
    }

    char *fname = __get_output__(cmd);

    free(salt);
    return fname;
}

char *__enc__(char *txt, const char *mpwd)
{
    char *enc_cmd = "echo '%s' | openssl enc -aes-256-cbc -e -iter 1000 -salt -pbkdf2 -a -k '%s'";
    int cmd_len = strlen(enc_cmd) + strlen(txt) + strlen(mpwd);
    char *cmd = malloc((cmd_len + 1) * sizeof(char));
    if (!cmd)
    {
        printf("ERROR [ENC-alloc]: Encryption failed!\n");
        return NULL;
    }

    sprintf(cmd, enc_cmd, txt, mpwd);

    char *ret = __get_output__(cmd);
    free(cmd);
    return ret;
}

char *__dec__(char *txt, const char *mpwd)
{
    char *dec_cmd = "echo %s | openssl enc -aes-256-cbc -d -iter 1000 -salt -pbkdf2 -a -k %s";
    int cmd_len = strlen(dec_cmd) + strlen(txt) + strlen(mpwd);
    char *cmd = malloc((cmd_len + 1) * sizeof(char));
    if (!cmd)
    {
        printf("ERROR [DEC-alloc]: Encryption failed!\n");
        return NULL;
    }

    sprintf(cmd, dec_cmd, txt, mpwd);

    char *ret = __get_output__(cmd);
    free(cmd);
    return ret;
}
