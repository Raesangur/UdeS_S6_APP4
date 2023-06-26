void transmitBit();
void receiveBit();

void ready_transmission();
bool is_transmission_complete();

char* get_and_clear_reception_byte();
void clear_raw_data_buffer();
void ready_raw_data_buffer();

void set_clock_us(uint64_t);
uint64_t get_and_clear_reception_time();
uint64_t get_and_clear_transmission_time();
