#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum KeySize
{
    SIZE_16 = 16,
    SIZE_24 = 24,
    SIZE_32 = 32
};
// 1 MB = 1048576 bytes
// 2 MB = 2097152 bytes
#define INPUT_SIZE 2097152
#define NUMBER_OF_ROUNDS 10
#define KEY_SIZE SIZE_16

typedef unsigned char byte;

byte sbox[256] = {
    // 0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,  // 0
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,  // 1
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,  // 2
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,  // 3
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,  // 4
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,  // 5
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,  // 6
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,  // 7
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,  // 8
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,  // 9
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,  // A
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,  // B
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,  // C
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,  // D
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,  // E
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}; // F

byte rsbox[256] =
    {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};

void get_sbox_value(byte num, byte *value);
void get_inv_sbox_value(byte num, byte *value);

void rotate(byte *word);
byte rcon[255] = {
    //    0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a};

void get_rcon_value(byte num, byte *value);

void expand_main(byte *word, size_t iteration);

void key_expansion(byte *expanded_key, byte *key, enum KeySize, size_t expanded_key_size);

void sub_bytes(byte *state);
void shift_rows(byte *state);
void shift_row(byte *state, byte nbr);
void add_round_key(byte *state, byte *round_key);
void galois_mult(byte a, byte b, byte *result);
void mix_columns(byte *state);
void mix_column(byte *column);
void aes_round(byte *state, byte *round_key);
void create_round_key(byte *expanded_key, byte *round_key);
void aes_main(byte *state, byte *expanded_key, int number_rounds);
void aes_encrypt(byte *input, byte *output, byte *expand_key);

void inv_sub_bytes(byte *state);
void inv_shift_rows(byte *state);
void inv_shift_row(byte *state, byte nbr);
void inv_mix_columns(byte *state);
void inv_mix_column(byte *column);
void inv_aes_round(byte *state, byte *round_key);
void inv_aes_main(byte *state, byte *expanded_key, int number_rounds);
void aes_decrypt(byte *input, byte *output, byte *expanded_key);

int main(int argc, char *argv[])
{
    byte key[KEY_SIZE] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    int expanded_key_size = (NUMBER_OF_ROUNDS + 1) * 16;
    byte expanded_key[expanded_key_size];

    enum KeySize key_size = KEY_SIZE;
    byte plaintext[INPUT_SIZE] = "i'm a little teapot short and stout, here is my handle, here is my spout. when i get all steamed up, hear me shout, tip me over and pour me out!";

    byte ciphertext[INPUT_SIZE];

    byte decryptedtext[INPUT_SIZE];
    clock_t start, end;
    double cpu_time_used;

    printf("Implementation of AES encryption using C/Sequential\n");

    start = clock();
    key_expansion(expanded_key, key, key_size, expanded_key_size);
    for (size_t i = 0; i < INPUT_SIZE; i += SIZE_16)
    {
        aes_encrypt(&plaintext[i], &ciphertext[i], expanded_key);
    }
    for (size_t i = 0; i < INPUT_SIZE; i += SIZE_16)
    {
        aes_decrypt(&ciphertext[i], &decryptedtext[i], expanded_key);
    }
    end = clock();
    printf("%s \n", decryptedtext);

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time used in CPU/NORMAL mode: %f\n\n", cpu_time_used);

    return 0;
}

void get_sbox_value(byte num, byte *value)
{
    *value = sbox[num];
}

void get_inv_sbox_value(byte num, byte *value)
{
    *value = rsbox[num];
}

void rotate(byte *word)
{
    byte c;
    c = word[0];
    for (size_t i = 0; i < 3; i++)
        word[i] = word[i + 1];
    word[3] = c;
}

void get_rcon_value(byte num, byte *value)
{
    *value = rcon[num];
}

void expand_main(byte *word, size_t iteration)
{
    rotate(word);
    byte rcon_value = 0;

    for (size_t i = 0; i < 4; ++i)
    {
        get_sbox_value(word[i], &word[i]);
    }
    get_rcon_value(iteration, &rcon_value);
    word[0] = word[0] ^ rcon_value;
}

void key_expansion(byte *expanded_key, byte *key, enum KeySize key_size, size_t expanded_key_size)
{
    int current_size = 0;
    int rcon_it = 1;

    byte tmp[4] = {0};
    for (size_t i = 0; i < key_size; i++)
        expanded_key[i] = key[i];
    current_size += key_size;

    while (current_size < expanded_key_size)
    {
        for (size_t i = 0; i < 4; i++)
        {
            tmp[i] = expanded_key[(current_size - 4) + i];
        }

        if (current_size % key_size == 0)
        {
            expand_main(tmp, rcon_it++);
        }

        if (key_size == SIZE_32 && ((current_size % key_size) == 16))
        {
            for (size_t i = 0; i < 4; i++)
                get_sbox_value(tmp[i], &tmp[i]);
        }

        for (size_t i = 0; i < 4; i++)
        {
            expanded_key[current_size] = expanded_key[current_size - key_size] ^ tmp[i];
            current_size++;
        }
    }
}

void sub_bytes(byte *state)
{
    for (size_t i = 0; i < 16; i++)
        get_sbox_value(state[i], &state[i]);
}

void shift_rows(byte *state)
{
    for (size_t i = 0; i < 4; i++)
        shift_row(state + i * 4, i);
}

void shift_row(byte *state, byte nbr)
{

    byte tmp;
    for (size_t i = 0; i < nbr; i++)
    {
        tmp = state[0];
        for (size_t j = 0; j < 3; j++)
            state[j] = state[j + 1];
        state[3] = tmp;
    }
}

void add_round_key(byte *state, byte *round_key)
{
    for (size_t i = 0; i < 16; i++)
        state[i] = state[i] ^ round_key[i];
}

void galois_mult(byte a, byte b, byte *result)
{
    byte tmp_result = 0;
    byte leftmost_bit;
    for (byte counter = 0; counter < 8; counter++)
    {
        if ((b & 1) == 1)
            tmp_result ^= a;
        leftmost_bit = (a & 0x80);
        a <<= 1;
        if (leftmost_bit == 0x80)
            a ^= 0x1b;
        b >>= 1;
    }
    *result = tmp_result;
}

void mix_columns(byte *state)
{
    byte column[4];
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            column[j] = state[(j * 4) + i];
        }

        mix_column(column);
        for (size_t j = 0; j < 4; j++)
        {
            state[(j * 4) + i] = column[j];
        }
    }
}

void mix_column(byte *column)
{
    byte tmp[4];
    byte result[4];
    for (size_t i = 0; i < 4; i++)
    {
        tmp[i] = column[i];
    }
    galois_mult(tmp[0], 2, &result[0]);
    galois_mult(tmp[1], 3, &result[1]);
    galois_mult(tmp[2], 1, &result[2]);
    galois_mult(tmp[3], 1, &result[3]);

    column[0] = result[0] ^ result[1] ^ result[2] ^ result[3];

    galois_mult(tmp[0], 1, &result[0]);
    galois_mult(tmp[1], 2, &result[1]);
    galois_mult(tmp[2], 3, &result[2]);

    column[1] = result[0] ^ result[1] ^ result[2] ^ result[3];

    galois_mult(tmp[1], 1, &result[1]);
    galois_mult(tmp[2], 2, &result[2]);
    galois_mult(tmp[3], 3, &result[3]);

    column[2] = result[0] ^ result[1] ^ result[2] ^ result[3];

    galois_mult(tmp[0], 3, &result[0]);
    galois_mult(tmp[2], 1, &result[2]);
    galois_mult(tmp[3], 2, &result[3]);

    column[3] = result[0] ^ result[1] ^ result[2] ^ result[3];
}

void aes_round(byte *state, byte *round_key)
{
    sub_bytes(state);
    shift_rows(state);
    mix_columns(state);
    add_round_key(state, round_key);
}

void create_round_key(byte *expanded_key, byte *round_key)
{
    size_t i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
            round_key[(i + (j * 4))] = expanded_key[(i * 4) + j];
    }
}

void aes_main(byte *state, byte *expanded_key, int number_rounds)
{
    size_t i = 0;

    byte round_key[16];

    create_round_key(expanded_key, round_key);
    add_round_key(state, round_key);

    for (i = 1; i < number_rounds; i++)
    {
        create_round_key(expanded_key + 16 * i, round_key);
        aes_round(state, round_key);
    }

    create_round_key(expanded_key + 16 * number_rounds, round_key);
    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, round_key);
}

void aes_encrypt(byte *input,
                 byte *output,
                 byte *expanded_key)
{
    int number_rounds = NUMBER_OF_ROUNDS;
    byte block[16];

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
            block[(i + (j * 4))] = input[(i * 4) + j];
    }
    aes_main(block, expanded_key, number_rounds);
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
            output[(i * 4) + j] = block[(i + (j * 4))];
    }
}

void inv_sub_bytes(byte *state)
{
    for (size_t i = 0; i < 16; i++)
        get_inv_sbox_value(state[i], &state[i]);
}

void inv_shift_rows(byte *state)
{
    for (size_t i = 0; i < 4; i++)
        inv_shift_row(state + i * 4, i);
}

void inv_shift_row(byte *state, byte nbr)
{
    byte tmp;
    for (size_t i = 0; i < nbr; i++)
    {
        tmp = state[3];
        for (size_t j = 3; j > 0; j--)
            state[j] = state[j - 1];
        state[0] = tmp;
    }
}

void inv_mix_columns(byte *state)
{
    byte column[4];
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            column[j] = state[(j * 4) + i];
        }
        inv_mix_column(column);
        for (size_t j = 0; j < 4; j++)
        {
            state[(j * 4) + i] = column[j];
        }
    }
}

void inv_mix_column(byte *column)
{
    byte tmp[4];
    byte result[4];
    for (size_t i = 0; i < 4; i++)
    {
        tmp[i] = column[i];
    }
    galois_mult(tmp[0], 14, &result[0]);
    galois_mult(tmp[3], 9, &result[1]);
    galois_mult(tmp[2], 13, &result[2]);
    galois_mult(tmp[1], 11, &result[3]);

    column[0] = result[0] ^ result[1] ^ result[2] ^ result[3];

    galois_mult(tmp[1], 14, &result[0]);
    galois_mult(tmp[0], 9, &result[1]);
    galois_mult(tmp[3], 13, &result[2]);
    galois_mult(tmp[2], 11, &result[3]);

    column[1] = result[0] ^ result[1] ^ result[2] ^ result[3];

    galois_mult(tmp[2], 14, &result[0]);
    galois_mult(tmp[1], 9, &result[1]);
    galois_mult(tmp[0], 13, &result[2]);
    galois_mult(tmp[3], 11, &result[3]);

    column[2] = result[0] ^ result[1] ^ result[2] ^ result[3];

    galois_mult(tmp[3], 14, &result[0]);
    galois_mult(tmp[2], 9, &result[1]);
    galois_mult(tmp[1], 13, &result[2]);
    galois_mult(tmp[0], 11, &result[3]);

    column[3] = result[0] ^ result[1] ^ result[2] ^ result[3];
}

void inv_aes_round(byte *state, byte *round_key)
{

    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_round_key(state, round_key);
    inv_mix_columns(state);
}

void inv_aes_main(byte *state, byte *expanded_key, int number_rounds)
{

    byte round_key[16];

    create_round_key(expanded_key + 16 * number_rounds, round_key);
    add_round_key(state, round_key);

    for (size_t i = number_rounds - 1; i > 0; i--)
    {
        create_round_key(expanded_key + 16 * i, round_key);
        inv_aes_round(state, round_key);
    }

    create_round_key(expanded_key, round_key);
    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_round_key(state, round_key);
}

void aes_decrypt(byte *input,
                 byte *output,
                 byte *expanded_key)
{

    int number_rounds = NUMBER_OF_ROUNDS;
    byte block[16];
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
            block[(i + (j * 4))] = input[(i * 4) + j];
    }
    inv_aes_main(block, expanded_key, number_rounds);
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
            output[(i * 4) + j] = block[(i + (j * 4))];
    }
}
