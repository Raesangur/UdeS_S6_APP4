#include "globaldef.h"
#include "gpio.h"
#include "message.h"


char* currentDataPointer = NULL;
char* endDataPointer     = NULL;
char  nextByte           = '\0';
char  previousByte       = '\0';

bool transmissionComplete = true;
char receptionByte = '\0';


void transmitBit()
{
    if(!currentDataPointer || !endDataPointer)
    {
        currentDataPointer   = get_tx_data_buffer();
        endDataPointer       = get_tx_data_buffer_end();
        printf("Writing message %s\n", currentDataPointer);
    }
    else
    {
        transmissionComplete = false;
        if(nextByte == '0')
        {
            set_tx_gpio();
            nextByte = '\0';
        }
        else if(nextByte == '1')
        {
            clear_tx_gpio();
            nextByte = '\0';
        }
        else
        {
            if(currentDataPointer == endDataPointer)
            {
                currentDataPointer   = NULL;
                endDataPointer       = NULL;
                transmissionComplete = true;
                printf("Transmission Complete!\n");
            }
            else
            {
                if(*currentDataPointer == '0')
                {
                   // printf("sending 0\n");
                    set_tx_gpio();
                    nextByte = '1';
                }
                else if(*currentDataPointer == '1')
                {
                    //printf("sending 1\n");
                    clear_tx_gpio();
                    nextByte = '0';
                }
                else
                {
                    nextByte = '\0';
                }
                currentDataPointer++;
            }
        }
    }
}


char get_and_clear_reception_byte()
{
    char byte = receptionByte;
    receptionByte = '\0';
    return byte;
}

void receiveBit()
{
    if (previousByte != '0' && previousByte != '1')
    {
        previousByte = read_rx_gpio() ? '1' : '0';
    }
    else
    {
        char currentByte = read_rx_gpio() ? '1' : '0';
        if (previousByte == '0' && currentByte == '1')
        {
            //printf("received 1\n");
            receptionByte = '1';
        }
        else if (previousByte == '1' && currentByte == '0')
        {
            //printf("received 0\n");
            receptionByte = '0';
        }

        previousByte = '\0';
    }
}
