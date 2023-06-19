#include "globaldef.h"
#include "gpio.h"
#include "message.h"


char* currentDataPointer = NULL;
char* endDataPointer = NULL;
char nextByte = '\0';

bool transmissionComplete = true;

void transmitBit()
{
    if (!currentDataPointer || endDataPointer)
    {
        currentDataPointer = get_tx_data_buffer();
        endDataPointer = get_tx_data_buffer_end();
        transmissionComplete = true;
    }
    else
    {
        transmissionComplete = false;
        if (nextByte == '0')
        {
            set_tx_gpio();
            nextByte = '\0';
        }
        else if (nextByte == '1')
        {
            clear_tx_gpio();
            nextByte = '\0';
        }
        else
        {
            if (currentDataPointer == endDataPointer)
            {
                currentDataPointer = NULL;
                endDataPointer = NULL;
                transmissionComplete = true;
            }
            else
            {
                if (*currentDataPointer == '0')
                {
                    set_tx_gpio();
                    nextByte = '0';
                }
                else if (*currentDataPointer == '1')
                {
                    clear_tx_gpio();
                    nextByte = '1';
                }
                else
                {
                    nextByte = '\0';
                }
            }
        }
    }
}
