#define IO_MOD

const char ENTER = '\n';
const char DELETE = 127;

char *__read_input__()
{
    int buff_max = 1;
    char *buff = malloc((buff_max + 1) * sizeof(char));
    if (!buff)
        return NULL;
    int written = 0;

    char c = '\0';

    while (scanf("%c", &c) == 1 && c != '\n')
    {
        if (written == buff_max)
        {
            buff_max *= 2;
            char *tmp = realloc(buff, (buff_max + 1) * sizeof(char));
            if (!tmp)
                exit(EXIT_FAILURE);
            buff = tmp;
        }

        buff[written++] = c;
    }

    buff[written] = '\0';
    return buff;
}

char *get_input(const char *value_name, const char *prompt)
{
    char *input = NULL;

    while (true)
    {
        printf("%s", prompt);
        printf("> ");
        input = __read_input__();

        if (strlen(input) != 0)
            return input;

        printf("ERROR [INPUT]: %s cannot be empty!\n", value_name);
    }
}

bool get_bool(const char *prompt)
{
    const char *err_msg = "ERROR [INPUT-yes/no]: Input must be one of y,n,Y,N\n";

    while (true)
    {
        char *input = get_input("a yes/no response", prompt);

        if (strlen(input) != 1)
            printf("%s", err_msg);
        else if (*input == 'y' || *input == 'Y')
            return true;
        else if (*input == 'n' || *input == 'N')
            return false;
        else
            printf("%s", err_msg);
    }
}

char *get_passwd(const char *prompt)
{
    // Disable input echoing
    struct termios old_kbd;
    struct termios new_kbd;

    if (tcgetattr(0, &old_kbd))
    {
        printf("ERROR [INPUT-Passwd]: tcgetattr failed.\n");
        return NULL;
    }
    memcpy(&new_kbd, &old_kbd, sizeof(struct termios));

    new_kbd.c_lflag &= ~(ICANON | ECHO);
    new_kbd.c_cc[VTIME] = 0;
    new_kbd.c_cc[VMIN] = 1;

    if (tcsetattr(0, TCSANOW, &new_kbd))
    {
        printf("ERROR [INPUT-Passwd]: tcsetattr failed.\n");
        return NULL;
    }

    // Setup input buffer
    int buff_max = 1;
    char *buff = malloc((buff_max + 1) * sizeof(char));
    if (!buff)
        return NULL;
    int written = 0;

    printf("%s", prompt);
    char c = '\0';

    while (true)
    {
        if (written == buff_max)
        {
            buff_max *= 2;
            char *tmp = realloc(buff, (buff_max + 1) * sizeof(char));
            if (!tmp)
            {
                free(buff);
                return NULL;
            }
            buff = tmp;
        }

        c = fgetc(stdin);

        if (c == ENTER)
        {
            if (written == 0)
            {
                printf("ERROR [PASSWD]: Password cannot be empty!\n");
                printf("Enter password: >");
                continue;
            }
            buff[written] = '\0';
            break;
        }
        else if (c == DELETE)
        {
            if (written > 0)
            {
                --written;
                printf("\b \b");
            }
        }
        else if (' ' <= c && c <= '~')
        {
            printf("*");
            buff[written++] = c;
        }
        else
        {
            printf("ERROR [INPUT-Passwd]: Unexpected character!\n");
            free(buff);
            break;
        }
    }
    printf("\n");

    // Re-enable input echoing
    if (tcsetattr(0, TCSANOW, &old_kbd))
    {
        printf("ERROR [INPUT-Passwd]: tcsetattr failed.\n");
        if (buff)
            free(buff);
        return NULL;
    }

    return buff;
}

char *__get_output__(char *cmd)
{
    int out_size = 1;
    char *out_buff = malloc((out_size + 1) * sizeof(char));
    if (!out_buff)
        return NULL;

    FILE *out_stream = popen(cmd, "r");

    if (!out_stream)
    {
        printf("ERROR [PIPE-o]: Failed to open external bridge!\n");
        return NULL;
    }

    for (int i = 0;;)
    {
        if (i == out_size)
        {
            out_size *= 2;
            char *tmp = realloc(out_buff, (out_size + 1) * sizeof(char));
            if (!tmp)
            {
                free(out_buff);
                return NULL;
            }
            out_buff = tmp;
        }

        char c = fgetc(out_stream);

        if (c == EOF)
        {
            out_buff[i] = '\0';
            return out_buff;
        }
        out_buff[i++] = c;
    }

    if (pclose(out_stream))
        printf("ERROR [PIPE-c]: Command not found or exited with error.\n");

    return out_buff;
}

char *readline(FILE *f)
{
    int line_len = 1;
    char *line = malloc((line_len + 1) * sizeof(char));
    if (!line)
    {
        printf("ERROR [FILE-r_alloc]: Failed to read file!\n");
        return NULL;
    }
    int written = 0;

    char c = fgetc(f);

    while (c && c != '\n')
    {
        if (written == line_len)
        {
            line_len *= 2;
            char *tmp = realloc(line, (line_len + 1) * sizeof(char));
            if (!tmp)
            {
                printf("ERROR [FILE-r_realloc]: Failed to read file!\n");
                free(line);
                return NULL;
            }
            line = tmp;
        }

        line[written++] = c;
        c = fgetc(f);
    }

    line[written] = '\0';
    return line;
}
