char* get_tx_data_buffer();
char* get_tx_data_buffer_end();
void  create_message(char* input);
int   receive_message(char* input);
char* get_rx_data_buffer();
char* get_rx_raw_data_buffer();
uint64_t get_and_clear_decoding_time();
uint64_t get_and_clear_encoding_time();
