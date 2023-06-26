#include "globaldef.h"
#include "manchester.h"
#include "message.h"
#include "serial_reception.h"

static bool addErrorToMessage = false;

void add_error_to_message()
{
    addErrorToMessage = true;
}

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
            printf("Sending: %s - %s\nTotal Encoding Time: %lld ticks\n",
                   serialData,
                   data,
                   get_and_clear_encoding_time());

            if(addErrorToMessage)
            {
                printf("Adding error to message\n");
                data[16] = data[16] == '0' ? '1' : '0';
            }


            ready_transmission();

            set_reception_flag();
            started_transmission = true;
        }
        if(is_transmission_complete() && started_transmission)
        {
            printf("Total Transmission Time: %lld ticks\n", get_and_clear_transmission_time());
            started_transmission = false;
        }
    }
}
