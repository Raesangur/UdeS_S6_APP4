#include "globaldef.h"
#include "serial_reception.h"
#include "message.h"
#include "manchester.h"

void transmission_task(void* pvParameter)
{
    bool started_transmission = false;
    
    while(true)
    {
        if(has_serial_reception())
        {
            char* serialData = get_serial_buffer();
            create_message(serialData);
            char* data = get_tx_data_buffer();
            printf("Sending: %s - %s\nTotal Encoding Time: %lld ticks\n", serialData, data, get_and_clear_encoding_time());

            /*
            dataLength = strlen(data);
            receptionData = get_rx_raw_data_buffer();
            end = receptionData + dataLength;
            */

            ready_transmission();

            set_reception_flag();
            started_transmission = true;
        }
        if (is_transmission_complete() && started_transmission)
        {
            printf("Total Transmission Time: %lld ticks\n", get_and_clear_transmission_time());
            started_transmission = false;
        }
    }
}
