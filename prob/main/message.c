#include "globaldef.h"

static char message[696];
char*       message_begin;
char*       message_end;
bool        ready;

#define CRC16 0x8005

static uint16_t gen_crc16(const char* data, uint16_t size)
{
    uint16_t out       = 0;
    int      bits_read = 0, bit_flag;

    /* Sanity check: */
    if(data == NULL)
        return 0;

    while(size > 0)
    {
        bit_flag = out >> 15;

        /* Get next bit: */
        out <<= 1;
        out |= (*data >> (7 - bits_read)) & 1;

        /* Increment bit counter: */
        bits_read++;
        if(bits_read > 7)
        {
            bits_read = 0;
            data++;
            size--;
        }

        /* Cycle check: */
        if(bit_flag)
            out ^= CRC16;
    }

    return out;
}

void create_message(char* input)
{
    int     k         = 0;
    uint8_t data_size = strlen(input);
    char    data[640];
    for(int i = 0; i < 80; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            data[i * 8 + j] = ((input[i] & (1 << j)) >> j) ? '1' : '0';
        }
    }


    // Preamble
    while(k < 8)
    {
        message[k] = (k % 2 == 0) ? '0' : '1';
        k++;
    }

    // Start byte
    message[k++] = '0';
    while(k < 15)
    {
        message[k++] = '1';
    }
    message[k++] = '0';

    // Type + flags
    while(k < 24)
    {
        message[k++] = '0';
    }

    // Data size
    k += 8;
    for(int i = 0; i < 8; i++)
    {
        message[--k] = ((data_size & (1 << i)) >> i) ? '1' : '0';
    }
    k += 8;

    // Data
    k += data_size * 8;
    for(int i = 0; i < data_size * 8; i++)
    {
        message[--k] = data[i];
    }
    k += data_size * 8;

    // CRC
    uint16_t crc = gen_crc16(input, data_size);
    for(int i = 0; i < 16; i++)
    {
        message[k++] = ((crc & (1 << i)) >> i) ? '1' : '0';
    }

    // End byte
    message[k++] = '0';
    for(int i = 0; i < 6; i++)
    {
        message[k++] = '1';
    }
    message[k++] = '0';

    message[k] = '\0';


    message_begin = message;
    message_end   = message + k;
    ready         = true;
}

char* get_tx_data_buffer()
{
    if(ready)
    {
        return message_begin;
    }
    else
    {
        return NULL;
    }
}

char* get_tx_data_buffer_end()
{
    if(ready)
    {
        return message_end;
    }
    else
    {
        return NULL;
    }
}
