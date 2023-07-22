#include "helpers.h"
#include "present.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc == 1 || argc > 3)
    {
        fprintf(stderr, "Usage: <filename> <key>\n");
        exit(EXIT_FAILURE);
    }

    byte key[KEY_SIZE];

    if (argc == 2)
    {
        generate_random_key(key, KEY_SIZE);
    }

    const char *filename;
    const char *keyStr;

    if (argc == 3)
    {
        keyStr = argv[2];
        if (!is_key_valid(keyStr))
        {
            fprintf(stderr, "The key you have provided is not valid\n");
            exit(EXIT_FAILURE);
        }
        key_str_to_key(key, keyStr);
    }

    filename = argv[1];

    if (!is_file_exist(filename))
    {
        fprintf(stderr, "File [%s] does not exist or could not be opened or is not a file.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (!(strrchr(filename, '.')) || strcmp(strrchr(filename, '.') + 1, "enc"))
    {
        encryption(filename, key);
    }
    else
    {
        decryption(filename, key);
    }
}
