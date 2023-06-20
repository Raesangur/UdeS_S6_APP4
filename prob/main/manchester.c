#include "globaldef.h"
#include "gpio.h"
#include "message.h"


char* tx_currentDataPointer = NULL;
char* tx_endDataPointer     = NULL;
char  tx_nextByte           = '\0';
char  rx_previousByte       = '\0';

char* rx_currentDataPointer = NULL;

bool transmissionComplete = true;
bool firstRead = true;
char receptionByte = '\0';

uint64_t lastReceptionTime = 0;
uint64_t ticksBetweenData = 0;


void set_clock_us(uint64_t clock)
{
    ticksBetweenData = clock;
}

void transmitBit()
{
    if(!tx_currentDataPointer || !tx_endDataPointer)
    {
        tx_currentDataPointer   = get_tx_data_buffer();
        tx_endDataPointer       = get_tx_data_buffer_end();
        //printf("Writing message %s\n", tx_currentDataPointer);
    }
    else
    {
        transmissionComplete = false;
        if(tx_nextByte == '0')
        {
            set_tx_gpio();
            tx_nextByte = '\0';
        }
        else if(tx_nextByte == '1')
        {
            clear_tx_gpio();
            tx_nextByte = '\0';
        }
        else
        {
            if(tx_currentDataPointer == tx_endDataPointer)
            {
                tx_currentDataPointer   = NULL;
                tx_endDataPointer       = NULL;
                transmissionComplete = true;
                //printf("Transmission Complete!\n");
            }
            else
            {
                if(*tx_currentDataPointer == '0')
                {
                   // printf("sending 0\n");
                    set_tx_gpio();
                    tx_nextByte = '1';
                }
                else if(*tx_currentDataPointer == '1')
                {
                    //printf("sending 1\n");
                    clear_tx_gpio();
                    tx_nextByte = '0';
                }
                else
                {
                    tx_nextByte = '\0';
                }
                tx_currentDataPointer++;
            }
        }
    }
}


char* get_and_clear_reception_byte()
{
    return rx_currentDataPointer;
}

void clear_raw_data_buffer()
{
    rx_currentDataPointer = NULL;
}

void receiveBit()
{   
    if (receptionByte != '\0')
        return;

    if (!rx_currentDataPointer)
        rx_currentDataPointer = get_rx_raw_data_buffer();

    
    uint64_t currentTime = esp_timer_get_time();
    if (currentTime - lastReceptionTime < (ticksBetweenData * 1.5) && !firstRead)
    {
        rx_previousByte = '\0';
    }
    lastReceptionTime = currentTime;
    
    if (rx_previousByte != '0' && rx_previousByte != '1')
    {
        rx_previousByte = read_rx_gpio() ? '1' : '0';
        firstRead = true;
    }
    else
    {
        char currentByte = read_rx_gpio() ? '1' : '0';
        firstRead = false;
        if (rx_previousByte == '0' && currentByte == '1')
        {
            rx_previousByte = '1';
            *rx_currentDataPointer = rx_previousByte;
            rx_currentDataPointer++;
        }
        else if (rx_previousByte == '1' && currentByte == '0')
        {
            rx_previousByte = '0';
            *rx_currentDataPointer = rx_previousByte;
            rx_currentDataPointer++;
        }
    }
}
