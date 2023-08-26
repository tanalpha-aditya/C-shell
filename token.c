#include "token.h"

bool is_inside_quotes(char *input, char quote, int pos)
{
    int count = 0;
    for (int i = 0; i < pos; i++)
    {
        if (input[i] == quote)
        {
            count++;
        }
    }
    return count % 2 != 0;
}

void remove_extra_spaces(char *token)
{
    char cleaned[MAX_COMMAND_LENGTH];
    int j = 0;
    bool inside_quotes = false;

    for (int i = 0; token[i] != '\0'; i++)
    {
        if ((token[i] == '"' || token[i] == '\'') && !is_inside_quotes(token, token[i], i))
        {
            inside_quotes = !inside_quotes;
        }

        if (inside_quotes || !isspace(token[i]) || (i > 0 && !isspace(token[i - 1])))
        {
            cleaned[j++] = token[i];
        }
    }

    cleaned[j] = '\0';
    strcpy(token, cleaned);
}

void tokenize_semi(char *input, char *tokens[], int *num_tokens)
{
    *num_tokens = 0;
    char *delimiters = ";";
    char *token = strtok(input, delimiters);

    while (token != NULL && *num_tokens < MAX_TOKENS)
    {
        remove_extra_spaces(token); // Remove extra spaces from the token
        tokens[*num_tokens] = token;
        (*num_tokens)++;
        token = strtok(NULL, delimiters);
    }
    tokens[*num_tokens] = NULL;
}

void tokenize_input(char *input, char *tokens[], int *num_tokens)
{
    *num_tokens = 0;
    char *delimiters = "&;";
    char *token = strtok(input, delimiters);

    while (token != NULL && *num_tokens < MAX_TOKENS)
    {
        remove_extra_spaces(token); // Remove extra spaces from the token
        tokens[*num_tokens] = token;
        (*num_tokens)++;
        token = strtok(NULL, delimiters);
    }
    tokens[*num_tokens] = NULL;
}

void tokenize_and(char *input, char *tokens[], int *num_tokens)
{
    *num_tokens = 0;
    char *delimiters = "&";
    char *token = strtok(input, delimiters);

    while (token != NULL && *num_tokens < MAX_TOKENS)
    {
        remove_extra_spaces(token); // Remove extra spaces from the token
        tokens[*num_tokens] = token;
        (*num_tokens)++;
        token = strtok(NULL, delimiters);
    }
    tokens[*num_tokens] = NULL;
}

void tokenize_input_string(char *input, char *tokens[], int *num_tokens)
{
    *num_tokens = 0;

    char *token = strtok(input, " /");
    while (token != NULL && *num_tokens < MAX_TOKENS)
    {
        tokens[*num_tokens] = strdup(token);
        (*num_tokens)++;
        token = strtok(NULL, " /");
    }
    tokens[*num_tokens] = NULL;
}

void tokenize_input_space(char *input, char *tokens[], int *num_tokens)
{
    *num_tokens = 0;

    char *token = strtok(input, " ");
    while (token != NULL && *num_tokens < MAX_TOKENS)
    {
        tokens[*num_tokens] = strdup(token);
        (*num_tokens)++;
        token = strtok(NULL, " ");
    }
    tokens[*num_tokens] = NULL;
}

void tokenize_input_andd(const char *input, char *tokens[], int *num_tokens)

{
    *num_tokens = 0;

    char *token = strtok(input, "&");
    while (token != NULL && *num_tokens < MAX_TOKENS)
    {
        tokens[*num_tokens] = strdup(token);
        (*num_tokens)++;
        token = strtok(NULL, "&");
    }
    tokens[*num_tokens] = NULL;
}
