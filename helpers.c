#include "helpers.h"
#include "present.h"
#include <ctype.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

void generate_random_key(byte *key, size_t keySize)
{
    int urandom = open("/dev/urandom", O_RDONLY);
    if (urandom == EOF)
    {
        perror("Failed to open /dev/urandom");
        exit(EXIT_FAILURE);
    }

    ssize_t bytesRead = read(urandom, key, keySize);
    if (bytesRead == EOF)
    {
        perror("Failed to read from /dev/urandom");
        exit(EXIT_FAILURE);
    }

    close(urandom);
}

int is_file_exist(const char *filename)
{
    struct stat fileStat;

    if (!stat(filename, &fileStat))
    {
        return S_ISREG(fileStat.st_mode);
    }
    return 0;
}

int check_extension_length(const char *filename)
{
    const char *ext = strrchr(filename, '.');

    if (!ext) // File has no extension
    {
        return 1;
    }

    if (strlen(ext) > 12)
    {
        fprintf(stderr, "File extension has too many characters.");
        return 0;
    }

    return 1;
}

int prompt_user_for_continue(const char *filename)
{
    printf("After successful encrpytion \"%s\" will be deleted. Are you sure to continue? (yes/no): ", filename);

    char response[4];

    fgets(response, sizeof(response), stdin);

    response[strcspn(response, "\n")] = '\0';

    if (!strcmp(response, "yes") || !strcmp(response, "y"))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int prompt_user_for_deletion(const char *filename)
{
    printf("Decryption successful. Would you like to delete \"%s\"? (yes/no): ", filename);

    char response[4];

    fgets(response, sizeof(response), stdin);

    response[strcspn(response, "\n")] = '\0';

    if (!strcmp(response, "yes") || !strcmp(response, "y"))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

byte *generate_sig(byte *sig, const char *ext, size_t pad)
{
    size_t i = 0;
    size_t len = strlen(ext);
    sig[i].val = (unsigned char)len;
    while (i < len)
    {
        sig[i + 1].val = toupper(ext[i]);
        i++;
    }
    for (unsigned char j = 0; j < 2 * BLOCK_SIZE - len - 5; ++j, ++i)
    {
        sig[i + 1].val = 0;
    }

    sig[i + 1].val = (unsigned char)pad;
    sig[i + 2].val = 'A';
    sig[i + 3].val = 'O';
    sig[i + 4].val = 'G';

    return sig;
}

int extract(signature *temp, byte decoded_signature[2][BLOCK_SIZE])
{
    temp->len = decoded_signature[0][0].val;

    for (size_t i = 0; i < temp->len; i++)
    {
        temp->ext[i] = tolower(decoded_signature[0][i + 1].val);
    }
    temp->ext[temp->len] = '\0';

    temp->padding_bytes = decoded_signature[1][BLOCK_SIZE - 4].val;

    temp->sig[0] = tolower(decoded_signature[1][BLOCK_SIZE - 3].val);
    temp->sig[1] = tolower(decoded_signature[1][BLOCK_SIZE - 2].val);
    temp->sig[2] = tolower(decoded_signature[1][BLOCK_SIZE - 1].val);
    temp->sig[3] = '\0';

    if (strcmp(temp->sig, "aog"))
    {
        fprintf(stderr, "Wrong key decryption canceled\n");
        exit(EXIT_FAILURE);
    }
    return 1;
}

void encryption(const char *filename, byte *key)
{
    if (check_extension_length(filename) && prompt_user_for_continue(filename))
    {
        printf("Key       : ");
        print_state(key, KEY_SIZE);
        printf("Store this key. Otherwise you will not be able to recover encrypted file.\n");

        printf("Encrypting: %s...\n", filename);

        const char *extension = strrchr(filename, '.');

        char newFileName[NAME_MAX] = {0};

        if (!extension)
        {
            strcat(strcpy(newFileName, filename), ".enc");
        }
        else
        {
            strcat(strncpy(newFileName, filename, (extension - filename + 1)), "enc");
        }

        ROUNDKEYS roundKeys;
        generate_round_keys(roundKeys, key, KEY_SIZE);

        FILE *fs = fopen(filename, "rb+");

        FILE *fd = fopen(newFileName, "wb");

        fseek(fs, 0L, SEEK_END);
        long int file_size = ftell(fs);
        long int chunk_count = file_size / BLOCK_SIZE;
        size_t remaining_size = file_size % BLOCK_SIZE;
        size_t padding_bytes = BLOCK_SIZE - remaining_size;
        unsigned char x = 0;
        if (remaining_size)
        {
            for (size_t i = 0; i < padding_bytes; ++i)
            {
                fwrite(&x, sizeof(unsigned char), 1, fs);
            }
        }

        byte signature[2 * BLOCK_SIZE];

        if (!extension)
        {
            fwrite(generate_sig(signature, "NAN", padding_bytes % BLOCK_SIZE), 2 * BLOCK_SIZE, 1, fs);
        }
        else
        {
            fwrite(generate_sig(signature, extension + 1, padding_bytes % BLOCK_SIZE), 2 * BLOCK_SIZE, 1, fs);
        }

        fseek(fs, 0L, SEEK_SET);

        byte plainText[BLOCK_SIZE];

        clock_t start = clock();

        while (fread(plainText, sizeof(plainText), 1, fs))
        {
            fwrite(encrypt(plainText, roundKeys), sizeof(plainText), 1, fd);
        }

        clock_t end = clock();

        printf("Encryption took %f sec\n", (double)(end - start) / CLOCKS_PER_SEC);
        printf("Encrypted Filename: %s\n", newFileName);
        fclose(fs);
        fclose(fd);

        int r = remove(filename);
        if (!r)
        {
            printf("%s deleted\n", filename);
        }
    }
    else
    {
        fprintf(stderr, "Encryption canceled\n");
        exit(EXIT_FAILURE);
    }
}

void decryption(const char *filename, byte *key)
{
    printf("Decrypting: %s...\n", filename);

    FILE *fe = fopen(filename, "rb");
    fseek(fe, 0L, SEEK_END);
    long int file_size = ftell(fe);

    long int chunk_count = file_size / BLOCK_SIZE;

    fseek(fe, -16L, SEEK_END);
    byte decoded_signature[2][BLOCK_SIZE];
    fread(decoded_signature, BLOCK_SIZE, 2, fe);
    fseek(fe, 0L, SEEK_SET);

    ROUNDKEYS roundKeys;
    generate_round_keys(roundKeys, key, KEY_SIZE);

    decrypt(decoded_signature[0], roundKeys);
    decrypt(decoded_signature[1], roundKeys);

    signature temp;

    if (extract(&temp, decoded_signature))
    {
        char newFileName[NAME_MAX] = {0};

        if (!strcmp(temp.ext, "nan"))
        {
            strncpy(newFileName, filename, strlen(filename) - 4);
        }
        else
        {
            strcat(strncpy(newFileName, filename, (strrchr(filename, '.') - filename + 1)), temp.ext);
        }

        FILE *fd = fopen(newFileName, "wb");

        byte plainText[BLOCK_SIZE];

        clock_t start = clock();

        for (long int i = 0; i < (temp.padding_bytes ? chunk_count - 3 : chunk_count - 2); ++i)
        {
            fread(plainText, sizeof(plainText), 1, fe);
            fwrite(decrypt(plainText, roundKeys), sizeof(plainText), 1, fd);
        }

        if (temp.padding_bytes)
        {
            fread(plainText, sizeof(plainText), 1, fe);
            memcpy(plainText, decrypt(plainText, roundKeys), BLOCK_SIZE);
            for (size_t i = 0; i < BLOCK_SIZE - temp.padding_bytes; ++i)
            {
                fputc(plainText[i].val, fd);
            }
        }

        clock_t end = clock();

        printf("Decryption took %f sec\n", (double)(end - start) / CLOCKS_PER_SEC);
        printf("Decrypted Filename: %s\n", newFileName);

        fclose(fe);
        fclose(fd);

        if (prompt_user_for_deletion(filename))
        {
            int r = remove(filename);
            if (!r)
            {
                printf("%s deleted\n", filename);
            }
        }
    }
}