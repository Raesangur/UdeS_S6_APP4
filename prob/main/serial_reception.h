void set_reception_flag();
void clear_reception_flag();

bool  has_serial_reception();
char* get_serial_buffer();

void reception_task(void* pvParameter);
