#include "globaldef.h"

#define MAX_CHARACTERS 80

static char receptionBuffer[MAX_CHARACTERS + 1] = {'\0'};
static bool receptionFlag = true;
static bool fullReception = false;

void set_reception_flag()
{
    receptionFlag = true;
}
void clear_reception_flag()
{
    receptionFlag = false;
}

bool has_serial_reception()
{
    bool reception = fullReception;
    fullReception = false;
    return reception;
}

char* get_serial_buffer()
{
    return receptionBuffer;
}

static void getLineInput()
{
    fpurge(stdin); //clears any junk in stdin
    char *bufp = &receptionBuffer[0];
    while(true)
    {
        vTaskDelay(100/portTICK_PERIOD_MS);
        *bufp = getchar();
        if(*bufp != '\0' && *bufp != 0xFF && *bufp != '\r') //ignores null input, 0xFF, CR in CRLF
        {
            //printf("%c", *bufp);
            //'enter' (EOL) handler 
            if(*bufp == '\n'){
                //*bufp = '\0';
                //printf("Received message! %s", buf);
                return;
            } //backspace handler
            else if (*bufp == '\b'){
                if(bufp-&receptionBuffer[0] >= 1)
                    bufp--;
            }
            else{
                //pointer to next character
                bufp++;
            }
        }
        
        //only accept len-1 characters, (len) character being null terminator.
        if(bufp- &receptionBuffer[0] > (MAX_CHARACTERS)-1){
            bufp = &receptionBuffer[0] + (MAX_CHARACTERS);
            *bufp = '\0';
            return;
        }
    } 
}


void reception_task(void *pvParameter)
{
	while(true)
	{
	    if (receptionFlag)
	    {
	        getLineInput();
	        fullReception = true;
	        clear_reception_flag();   
	    }
	    else
	    {
	        vTaskDelay(100);
	    }
	}
}
