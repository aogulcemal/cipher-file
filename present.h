// https://en.wikipedia.org/wiki/PRESENT

#ifndef PRESENT_H
#define PRESENT_H

#include <stddef.h>
#include <stdint.h>

#define ROUND_SIZE 32
#define KEY_SIZE 10
#define BLOCK_SIZE 8

static const unsigned char sbox[16] = {0XC, 0X5, 0X6, 0XB, 0X9, 0X0, 0XA, 0XD, 0X3, 0XE, 0XF, 0X8, 0X4, 0X7, 0X1, 0X2};

static const unsigned char invere_sbox[16] = {0X5, 0XE, 0XF, 0X8, 0XC, 0X1, 0X2, 0XD, 0XB, 0X4, 0X6, 0X3, 0X0, 0X7, 0X9, 0XA};

typedef union
{
    unsigned char val;
    struct
    {
        unsigned char lowNibble : 4;
        unsigned char highNibble : 4;
    };

    struct
    {
        unsigned char b0 : 1;
        unsigned char b1 : 1;
        unsigned char b2 : 1;
        unsigned char b3 : 1;
        unsigned char b4 : 1;
        unsigned char b5 : 1;
        unsigned char b6 : 1;
        unsigned char b7 : 1;
    };
} byte;

typedef byte ROUNDKEYS[ROUND_SIZE][KEY_SIZE];

// Key Schedule: 80-bit Key
void generate_round_keys(ROUNDKEYS round_keys, byte *key, size_t size);
byte *generate_round_key(byte *b, size_t i);

// 1. Step: Rotation
// Rotate 80-bit Master key to 19 to the right.
// [k79k78...k1k0] = [k18k17..k20k19]
byte *rotate_key(byte *b);

// 2. Step: Confusion
// Leftmost 4bit goes throug an SBOX
// [k79k78k77k76] = sbox[k79k78k77k76]
byte *confuse(byte *b);

// 3. Step: xor'ing with round_counter
// [k19k18k17k16k15] = [k19k18k17k16k15] ⊕ round_counter
byte *xor_with_round_counter(byte *b, size_t round_counter);

// This function takes 8-byte chunk and round_keys that are generated for the given 10-byte key and encrypts it using present block
// cipher algorithm.
byte *encrypt(byte *state, ROUNDKEYS round_keys);

void add_round_key(byte *state, byte *subKey);

void sbox_layer(byte *state);

void p_layer(byte *state);

byte *decrypt(byte *state, ROUNDKEYS round_keys);

void inverse_p_layer(byte *state);

void inverse_sbox_layer(byte *state);

void print_round_keys(const ROUNDKEYS round_keys);
void print_state(const byte *b, size_t size);

int is_key_valid(const char *keyStr);
int is_plain_text_valid(const char *plainTextStr);
void key_str_to_key(byte *key, const char *keyStr);
void str_to_plain_text(byte *plainText, const char *plainTextStr);

#endif