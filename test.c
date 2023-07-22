#include "present.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: <plaintext/ciphertext> <key> <E/D>\n");
        exit(EXIT_FAILURE);
    }
    if (!is_plain_text_valid(argv[1]))
    {
        fprintf(stderr, "The plaintext you have provided is not valid\n");
        exit(EXIT_FAILURE);
    }

    if (!is_key_valid(argv[2]))
    {
        fprintf(stderr, "The key you have provided is not valid\n");
        exit(EXIT_FAILURE);
    }

    if (argv[3][0] == 'E')
    {
        printf("Encryption mode.");

        byte plainText[BLOCK_SIZE];

        str_to_plain_text(plainText, argv[1]);

        byte key[KEY_SIZE];

        key_str_to_key(key, argv[2]);

        printf("\nPlaintext : ");
        print_state(plainText, BLOCK_SIZE);

        printf("Key       : ");
        print_state(key, KEY_SIZE);

        ROUNDKEYS roundKeys;
        generate_round_keys(roundKeys, key, KEY_SIZE);

        byte cipherText[BLOCK_SIZE];
        memcpy(cipherText, encrypt(plainText, roundKeys), BLOCK_SIZE);
        printf("Ciphertext: ");
        print_state(cipherText, BLOCK_SIZE);
    }
    else if (argv[3][0] == 'D')
    {
        printf("Decryption mode.");

        byte cipherText[BLOCK_SIZE];

        str_to_plain_text(cipherText, argv[1]);

        byte key[KEY_SIZE];

        key_str_to_key(key, argv[2]);

        printf("\nCiphertext: ");
        print_state(cipherText, BLOCK_SIZE);

        printf("Key       : ");
        print_state(key, KEY_SIZE);

        ROUNDKEYS roundKeys;
        generate_round_keys(roundKeys, key, KEY_SIZE);

        byte plainText[BLOCK_SIZE];
        memcpy(plainText, decrypt(cipherText, roundKeys), BLOCK_SIZE);
        printf("Plaintext : ");
        print_state(plainText, BLOCK_SIZE);
    }
}