#include "present.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void generate_round_keys(ROUNDKEYS round_keys, byte *key, size_t size)
{
    for (size_t i = 0; i < ROUND_SIZE; ++i)
    {
        memcpy(round_keys[i], generate_round_key(key, i), size);
    }
}

byte *generate_round_key(byte *b, size_t i)
{
    if (!i)
    {
        return b;
    }
    return xor_with_round_counter(confuse(rotate_key(b)), i);
}

byte *rotate_key(byte *b)
{
    byte temp1;
    byte temp2;
    byte temp3;

    temp1.b0 = (b + 0)->b0;
    temp1.b1 = (b + 0)->b1;
    temp1.b2 = (b + 0)->b2;
    temp1.b3 = (b + 0)->b3;
    temp1.b4 = (b + 0)->b4;
    temp1.b5 = (b + 0)->b5;
    temp1.b6 = (b + 0)->b6;
    temp1.b7 = (b + 0)->b7;

    temp2.b0 = (b + 1)->b0;
    temp2.b1 = (b + 1)->b1;
    temp2.b2 = (b + 1)->b2;
    temp2.b3 = (b + 1)->b3;
    temp2.b4 = (b + 1)->b4;
    temp2.b5 = (b + 1)->b5;
    temp2.b6 = (b + 1)->b6;
    temp2.b7 = (b + 1)->b7;

    temp3.b0 = (b + 2)->b0;
    temp3.b1 = (b + 2)->b1;
    temp3.b2 = (b + 2)->b2;

    (b + 0)->b7 = (b + 3)->b2;
    (b + 0)->b6 = (b + 3)->b1;
    (b + 0)->b5 = (b + 3)->b0;
    (b + 0)->b4 = (b + 2)->b7;
    (b + 0)->b3 = (b + 2)->b6;
    (b + 0)->b2 = (b + 2)->b5;
    (b + 0)->b1 = (b + 2)->b4;
    (b + 0)->b0 = (b + 2)->b3;

    (b + 1)->b7 = (b + 4)->b2;
    (b + 1)->b6 = (b + 4)->b1;
    (b + 1)->b5 = (b + 4)->b0;
    (b + 1)->b4 = (b + 3)->b7;
    (b + 1)->b3 = (b + 3)->b6;
    (b + 1)->b2 = (b + 3)->b5;
    (b + 1)->b1 = (b + 3)->b4;
    (b + 1)->b0 = (b + 3)->b3;

    (b + 2)->b7 = (b + 5)->b2;
    (b + 2)->b6 = (b + 5)->b1;
    (b + 2)->b5 = (b + 5)->b0;
    (b + 2)->b4 = (b + 4)->b7;
    (b + 2)->b3 = (b + 4)->b6;
    (b + 2)->b2 = (b + 4)->b5;
    (b + 2)->b1 = (b + 4)->b4;
    (b + 2)->b0 = (b + 4)->b3;

    (b + 3)->b7 = (b + 6)->b2;
    (b + 3)->b6 = (b + 6)->b1;
    (b + 3)->b5 = (b + 6)->b0;
    (b + 3)->b4 = (b + 5)->b7;
    (b + 3)->b3 = (b + 5)->b6;
    (b + 3)->b2 = (b + 5)->b5;
    (b + 3)->b1 = (b + 5)->b4;
    (b + 3)->b0 = (b + 5)->b3;

    (b + 4)->b7 = (b + 7)->b2;
    (b + 4)->b6 = (b + 7)->b1;
    (b + 4)->b5 = (b + 7)->b0;
    (b + 4)->b4 = (b + 6)->b7;
    (b + 4)->b3 = (b + 6)->b6;
    (b + 4)->b2 = (b + 6)->b5;
    (b + 4)->b1 = (b + 6)->b4;
    (b + 4)->b0 = (b + 6)->b3;

    (b + 5)->b7 = (b + 8)->b2;
    (b + 5)->b6 = (b + 8)->b1;
    (b + 5)->b5 = (b + 8)->b0;
    (b + 5)->b4 = (b + 7)->b7;
    (b + 5)->b3 = (b + 7)->b6;
    (b + 5)->b2 = (b + 7)->b5;
    (b + 5)->b1 = (b + 7)->b4;
    (b + 5)->b0 = (b + 7)->b3;

    (b + 6)->b7 = (b + 9)->b2;
    (b + 6)->b6 = (b + 9)->b1;
    (b + 6)->b5 = (b + 9)->b0;
    (b + 6)->b4 = (b + 8)->b7;
    (b + 6)->b3 = (b + 8)->b6;
    (b + 6)->b2 = (b + 8)->b5;
    (b + 6)->b1 = (b + 8)->b4;
    (b + 6)->b0 = (b + 8)->b3;

    (b + 7)->b7 = temp1.b2;
    (b + 7)->b6 = temp1.b1;
    (b + 7)->b5 = temp1.b0;
    (b + 7)->b4 = (b + 9)->b7;
    (b + 7)->b3 = (b + 9)->b6;
    (b + 7)->b2 = (b + 9)->b5;
    (b + 7)->b1 = (b + 9)->b4;
    (b + 7)->b0 = (b + 9)->b3;

    (b + 8)->b7 = temp2.b2;
    (b + 8)->b6 = temp2.b1;
    (b + 8)->b5 = temp2.b0;
    (b + 8)->b4 = temp1.b7;
    (b + 8)->b3 = temp1.b6;
    (b + 8)->b2 = temp1.b5;
    (b + 8)->b1 = temp1.b4;
    (b + 8)->b0 = temp1.b3;

    (b + 9)->b7 = temp3.b2;
    (b + 9)->b6 = temp3.b1;
    (b + 9)->b5 = temp3.b0;
    (b + 9)->b4 = temp2.b7;
    (b + 9)->b3 = temp2.b6;
    (b + 9)->b2 = temp2.b5;
    (b + 9)->b1 = temp2.b4;
    (b + 9)->b0 = temp2.b3;

    return b;
}

byte *confuse(byte *b)
{
    (b + 9)->highNibble = sbox[(b + 9)->highNibble];

    return b;
}

byte *xor_with_round_counter(byte *b, size_t round_counter)
{
    byte temp;
    temp.val = round_counter;

    (b + 2)->b3 = (b + 2)->b3 ^ temp.b4;
    (b + 2)->b2 = (b + 2)->b2 ^ temp.b3;
    (b + 2)->b1 = (b + 2)->b1 ^ temp.b2;
    (b + 2)->b0 = (b + 2)->b0 ^ temp.b1;
    (b + 1)->b7 = (b + 1)->b7 ^ temp.b0;

    return b;
}

byte *encrypt(byte *state, ROUNDKEYS round_keys)
{
    byte *sub_key = round_keys[0];
    sub_key += 2;

    size_t i;

    for (i = 0; i < ROUND_SIZE - 1; ++i)
    {
        add_round_key(state, sub_key);

        sub_key = round_keys[i + 1];
        sub_key += 2;
        sbox_layer(state);
        p_layer(state);
    }
    sub_key = round_keys[i];
    sub_key += 2;
    add_round_key(state, sub_key);

    return state;
}

void add_round_key(byte *state, byte *sub_key)
{
    for (size_t i = 0; i < BLOCK_SIZE; ++i)
    {
        state[i].val = state[i].val ^ sub_key[i].val;
    }
}

void sbox_layer(byte *state)
{
    for (size_t i = 0; i < BLOCK_SIZE; ++i)
    {
        state[i].highNibble = sbox[state[i].highNibble];
        state[i].lowNibble = sbox[state[i].lowNibble];
    }
}

void p_layer(byte *state)
{
    byte ptemp[BLOCK_SIZE];

    ptemp[0].b0 = state[0].b0;
    ptemp[2].b0 = state[0].b1;
    ptemp[4].b0 = state[0].b2;
    ptemp[6].b0 = state[0].b3;
    ptemp[0].b1 = state[0].b4;
    ptemp[2].b1 = state[0].b5;
    ptemp[4].b1 = state[0].b6;
    ptemp[6].b1 = state[0].b7;

    ptemp[0].b2 = state[1].b0;
    ptemp[2].b2 = state[1].b1;
    ptemp[4].b2 = state[1].b2;
    ptemp[6].b2 = state[1].b3;
    ptemp[0].b3 = state[1].b4;
    ptemp[2].b3 = state[1].b5;
    ptemp[4].b3 = state[1].b6;
    ptemp[6].b3 = state[1].b7;

    ptemp[0].b4 = state[2].b0;
    ptemp[2].b4 = state[2].b1;
    ptemp[4].b4 = state[2].b2;
    ptemp[6].b4 = state[2].b3;
    ptemp[0].b5 = state[2].b4;
    ptemp[2].b5 = state[2].b5;
    ptemp[4].b5 = state[2].b6;
    ptemp[6].b5 = state[2].b7;

    ptemp[0].b6 = state[3].b0;
    ptemp[2].b6 = state[3].b1;
    ptemp[4].b6 = state[3].b2;
    ptemp[6].b6 = state[3].b3;
    ptemp[0].b7 = state[3].b4;
    ptemp[2].b7 = state[3].b5;
    ptemp[4].b7 = state[3].b6;
    ptemp[6].b7 = state[3].b7;

    ptemp[1].b0 = state[4].b0;
    ptemp[3].b0 = state[4].b1;
    ptemp[5].b0 = state[4].b2;
    ptemp[7].b0 = state[4].b3;
    ptemp[1].b1 = state[4].b4;
    ptemp[3].b1 = state[4].b5;
    ptemp[5].b1 = state[4].b6;
    ptemp[7].b1 = state[4].b7;

    ptemp[1].b2 = state[5].b0;
    ptemp[3].b2 = state[5].b1;
    ptemp[5].b2 = state[5].b2;
    ptemp[7].b2 = state[5].b3;
    ptemp[1].b3 = state[5].b4;
    ptemp[3].b3 = state[5].b5;
    ptemp[5].b3 = state[5].b6;
    ptemp[7].b3 = state[5].b7;

    ptemp[1].b4 = state[6].b0;
    ptemp[3].b4 = state[6].b1;
    ptemp[5].b4 = state[6].b2;
    ptemp[7].b4 = state[6].b3;
    ptemp[1].b5 = state[6].b4;
    ptemp[3].b5 = state[6].b5;
    ptemp[5].b5 = state[6].b6;
    ptemp[7].b5 = state[6].b7;

    ptemp[1].b6 = state[7].b0;
    ptemp[3].b6 = state[7].b1;
    ptemp[5].b6 = state[7].b2;
    ptemp[7].b6 = state[7].b3;
    ptemp[1].b7 = state[7].b4;
    ptemp[3].b7 = state[7].b5;
    ptemp[5].b7 = state[7].b6;
    ptemp[7].b7 = state[7].b7;

    memcpy(state, ptemp, BLOCK_SIZE);
}

byte *decrypt(byte *state, ROUNDKEYS round_keys)
{
    byte *sub_key = round_keys[ROUND_SIZE - 1];
    sub_key += 2;

    size_t i;

    for (i = ROUND_SIZE - 1; i > 0; --i)
    {
        add_round_key(state, sub_key);
        sub_key = round_keys[i - 1];
        sub_key += 2;
        inverse_p_layer(state);
        inverse_sbox_layer(state);
    }
    sub_key = round_keys[0];
    sub_key += 2;
    add_round_key(state, sub_key);

    return state;
}

void inverse_p_layer(byte *state)
{
    byte ptemp[BLOCK_SIZE];

    ptemp[0].b0 = state[0].b0;
    ptemp[0].b1 = state[2].b0;
    ptemp[0].b2 = state[4].b0;
    ptemp[0].b3 = state[6].b0;
    ptemp[0].b4 = state[0].b1;
    ptemp[0].b5 = state[2].b1;
    ptemp[0].b6 = state[4].b1;
    ptemp[0].b7 = state[6].b1;

    ptemp[1].b0 = state[0].b2;
    ptemp[1].b1 = state[2].b2;
    ptemp[1].b2 = state[4].b2;
    ptemp[1].b3 = state[6].b2;
    ptemp[1].b4 = state[0].b3;
    ptemp[1].b5 = state[2].b3;
    ptemp[1].b6 = state[4].b3;
    ptemp[1].b7 = state[6].b3;

    ptemp[2].b0 = state[0].b4;
    ptemp[2].b1 = state[2].b4;
    ptemp[2].b2 = state[4].b4;
    ptemp[2].b3 = state[6].b4;
    ptemp[2].b4 = state[0].b5;
    ptemp[2].b5 = state[2].b5;
    ptemp[2].b6 = state[4].b5;
    ptemp[2].b7 = state[6].b5;

    ptemp[3].b0 = state[0].b6;
    ptemp[3].b1 = state[2].b6;
    ptemp[3].b2 = state[4].b6;
    ptemp[3].b3 = state[6].b6;
    ptemp[3].b4 = state[0].b7;
    ptemp[3].b5 = state[2].b7;
    ptemp[3].b6 = state[4].b7;
    ptemp[3].b7 = state[6].b7;

    ptemp[4].b0 = state[1].b0;
    ptemp[4].b1 = state[3].b0;
    ptemp[4].b2 = state[5].b0;
    ptemp[4].b3 = state[7].b0;
    ptemp[4].b4 = state[1].b1;
    ptemp[4].b5 = state[3].b1;
    ptemp[4].b6 = state[5].b1;
    ptemp[4].b7 = state[7].b1;

    ptemp[5].b0 = state[1].b2;
    ptemp[5].b1 = state[3].b2;
    ptemp[5].b2 = state[5].b2;
    ptemp[5].b3 = state[7].b2;
    ptemp[5].b4 = state[1].b3;
    ptemp[5].b5 = state[3].b3;
    ptemp[5].b6 = state[5].b3;
    ptemp[5].b7 = state[7].b3;

    ptemp[6].b0 = state[1].b4;
    ptemp[6].b1 = state[3].b4;
    ptemp[6].b2 = state[5].b4;
    ptemp[6].b3 = state[7].b4;
    ptemp[6].b4 = state[1].b5;
    ptemp[6].b5 = state[3].b5;
    ptemp[6].b6 = state[5].b5;
    ptemp[6].b7 = state[7].b5;

    ptemp[7].b0 = state[1].b6;
    ptemp[7].b1 = state[3].b6;
    ptemp[7].b2 = state[5].b6;
    ptemp[7].b3 = state[7].b6;
    ptemp[7].b4 = state[1].b7;
    ptemp[7].b5 = state[3].b7;
    ptemp[7].b6 = state[5].b7;
    ptemp[7].b7 = state[7].b7;

    memcpy(state, ptemp, BLOCK_SIZE);
}

void inverse_sbox_layer(byte *state)
{
    for (size_t i = 0; i < BLOCK_SIZE; ++i)
    {
        state[i].highNibble = invere_sbox[state[i].highNibble];
        state[i].lowNibble = invere_sbox[state[i].lowNibble];
    }
}

void print_state(const byte *b, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        printf("%0X%0X", b->highNibble, b->lowNibble);
        b++;
    }
    printf("\n");
}

void print_round_keys(const ROUNDKEYS round_keys)
{
    for (size_t i = 0; i < ROUND_SIZE; ++i)
    {
        printf("%-2lu: ", i + 1);
        print_state(round_keys[i], BLOCK_SIZE);
    }
}

int is_key_valid(const char *keyStr)
{
    if (strlen(keyStr) != 2 * KEY_SIZE)
    {
        return 0;
    }

    for (int i = 0; i < 2 * KEY_SIZE; ++i)
    {
        if (!isxdigit(keyStr[i]))
        {
            return 0;
        }
    }

    return 1;
}

int is_plain_text_valid(const char *plainTextStr)
{
    if (strlen(plainTextStr) != 2 * BLOCK_SIZE)
    {
        return 0;
    }

    for (int i = 0; i < 2 * BLOCK_SIZE; ++i)
    {
        if (!isxdigit(plainTextStr[i]))
        {
            return 0;
        }
    }

    return 1;
}

void key_str_to_key(byte *key, const char *keyStr)
{
    for (int i = 2 * KEY_SIZE - 2; i >= 0; i -= 2)
    {
        key[i / 2].lowNibble = keyStr[i + 1] >= 65 ? keyStr[i + 1] - 55 : keyStr[i + 1] - 48;
        key[i / 2].highNibble = keyStr[i] >= 65 ? keyStr[i] - 55 : keyStr[i] - 48;
    }
}

void str_to_plain_text(byte *plainText, const char *plainTextStr)
{
    for (int i = 2 * BLOCK_SIZE - 2; i >= 0; i -= 2)
    {
        plainText[i / 2].lowNibble = plainTextStr[i + 1] >= 65 ? plainTextStr[i + 1] - 55 : plainTextStr[i + 1] - 48;
        plainText[i / 2].highNibble = plainTextStr[i] >= 65 ? plainTextStr[i] - 55 : plainTextStr[i] - 48;
    }
}