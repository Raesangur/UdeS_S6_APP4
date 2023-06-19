#include "globaldef.h"

char  tx_message[696];
char* tx_message_begin;
char* tx_message_end;
bool  tx_ready;

char  rx_message[80];
char* rx_message_begin;
char* rx_message_end;
bool  rx_ready;


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
        tx_message[k] = (k % 2 == 0) ? '0' : '1';
        k++;
    }

    // Start byte
    tx_message[k++] = '0';
    while(k < 15)
    {
        tx_message[k++] = '1';
    }
    tx_message[k++] = '0';

    // Type + flags
    while(k < 24)
    {
        tx_message[k++] = '0';
    }

    // Data size
    k += 8;
    for(int i = 0; i < 8; i++)
    {
        tx_message[--k] = ((data_size & (1 << i)) >> i) ? '1' : '0';
    }
    k += 8;

    // Data
    k -= 8;
    for(int i = 0; i < data_size * 8; i++)
    {
        if(i % 8 == 0)
            k += 16;
        tx_message[--k] = data[i];
    }
    k += 8;

    // CRC
    uint16_t crc = gen_crc16(input, data_size);
    printf("%s - %d\n", input, crc);
    k += 16;
    for(int i = 0; i < 16; i++)
    {
        tx_message[--k] = ((crc & (1 << i)) >> i) ? '1' : '0';
    }
    k += 16;

    // End byte
    tx_message[k++] = '0';
    for(int i = 0; i < 6; i++)
    {
        tx_message[k++] = '1';
    }
    tx_message[k++]  = '0';
    tx_message[k]    = '\0';
    tx_message_begin = tx_message;
    tx_message_end   = tx_message + k;
    tx_ready         = true;
}

int receive_message(char* input)
{
    int     k            = 0;
    uint8_t message_size = strlen(input);
    while(k < message_size)
    {
        // Preambule
        char temp[9] = {'\0'};
        for(int i = 0; i < 8; i++)
        {
            temp[i] = input[k + i];
        }
        temp[8] = '\0';
        if(strcmp(temp, "01010101"))
        {
            return -1;
        }


        k += 8;
        for(int i = 0; i < 8; i++)
        {
            temp[i] = input[k + i];
        }
        if(strcmp(temp, "01111110"))
        {
            return -1;
        }
        k += 8;
        for(int i = 0; i < 8; i++)
        {
            temp[i] = input[k + i];
        }
        if(strcmp(temp, "00000000"))
        {
            return -1;
        }
        k += 8;
        uint8_t data_size = 0;
        for(int i = 0; i < 8; i++)
        {
            if(input[k + i] == '0')
            {
            }
            else if(input[k + i] == '1')
            {
                data_size |= 1 << (7 - i);
            }
            else
            {
                return -1;
            }
        }
        k += 8;
        for(int i = 0; i < data_size; i++)
        {
            char unit = 0;
            for(int j = 0; j < 8; j++)
            {
                if(input[k + (i * 8) + j] == '0')
                {
                }
                else if(input[k + (i * 8) + j] == '1')
                {
                    unit |= 1 << (7 - j);
                }
                else
                {
                    return -1;
                }
            }

            rx_message[i] = unit;
        }
        k += data_size * 8;

        uint16_t crc = gen_crc16(rx_message, data_size);
        char crc_sequence[17] = {'\0'};
        for(int i = 0; i < 16; i++)
        {
            crc_sequence[15 - i] = ((crc & (1 << i)) >> i) ? '1' : '0';
        }

        for(int i = 0; i < 16; i++)
        {
            printf("%c\n", input[i + k]);
            if(input[i + k] != crc_sequence[i])
            {
                return -1;
            }
        }
        k += 16;
        for(int i = 0; i < 8; i++)
        {
            temp[i] = input[k + i];
        }
        if(strcmp(temp, "01111110"))
        {
            return -1;
        }


        rx_message_begin = rx_message;
        rx_message_end   = rx_message + data_size;

        rx_ready = 1;
        return 0;
    }
    return -1;
}

char* get_tx_data_buffer()
{
    if(tx_ready)
    {
        return tx_message_begin;
    }
    else
    {
        return NULL;
    }
}

char* get_tx_data_buffer_end()
{
    if(tx_ready)
    {
        return tx_message_end;
    }
    else
    {
        return NULL;
    }
}
