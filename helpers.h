#ifndef HELPERS_H
#define HELPERS_H

#include "present.h"
#include <stddef.h>

typedef struct signature
{
    unsigned char len;
    char ext[11];
    unsigned char padding_bytes;
    char sig[4];
} signature;


// Generates 80-bit random key using /dev/urandom which is special file that serve as cryptographically secure pseudorandom number generator.
void generate_random_key(byte *key, size_t keySize);

// Checks if a file exists by using the stat function to retrieve information about the file and then checking if it is a regular
// file.
int is_file_exist(const char *filename);

// Checks if a file has an extension and if the length of the extension is within a certain limit
int check_extension_length(const char *filename);

// Promts user if they would like to continue for encryption process which means the original file will be deleted.
int prompt_user_for_continue(const char *filename);

// Promts user if they would like to delete encrypted file after successful decryption.
int prompt_user_for_deletion(const char *filename);

// This function generates a signature by populating a byte array (sig) based on the length of a given string (ext), a padding size
// (pad), and some predefined characters ("AOG"). Purpose of this function is to later check if the given key is corret at the
// decryption process. Also to store the count of the padding bytes added to the original file.
byte *generate_sig(byte *sig, const char *ext, size_t pad);

// This function extracts and assigns values from the "decoded_signature" array to the corresponding members of the "temp"
// structure. It also performs some checks and error handling. The purpose of this function is to check last two bytes of decrypted
// file has correct signature. Also get the original files extension and how many padding byteas are added for encryption.
int extract(signature *temp, byte decoded_signature[2][BLOCK_SIZE]);

// Encryipton function that uses present block cipher to encrypt a given file and key.
void encryption(const char *filename, byte *key);

// Decryipton function that uses present block cipher to decrypt a given file and key.
void decryption(const char *filename, byte *key);

#endif