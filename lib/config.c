#include "config.h"
//-----------------------------------------INMP441-------------------------------------------------//
i2s_chan_handle_t rx_channel; 
int16_t buffer16[DMA_BUFFER_SIZE / sizeof(int32_t) * 3 / 2] = {0}; //288 samples (576 bytes) de luu duoc 3 bytes sau khi dich cua buffer32, VD: 3 byte cần 2 mẫu, ví dụ: 192 mẫu 3 bytes = 288 mẫu 2 bytes
int32_t buffer32[DMA_BUFFER_SIZE / sizeof(int32_t)] = {0}; //192 samples (768 bytes) , mỗi mẫu 32-bit (4 bytes)

//-----------------------------------------MAX30102-------------------------------------------------//
TaskHandle_t readMAXTask_handle = NULL;