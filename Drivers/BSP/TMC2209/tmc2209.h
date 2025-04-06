#include "./BSP/UART/uart.h"
/*这里写tmc的驱动函数*/

// CRC校验函数（TMC2209 CRC算法）
uint8_t tmc_crc(uint8_t *data, uint8_t len);

//读数据
void tmc2209_read(uint8_t tmc_addr,uint8_t register_addr,uint8_t* rx_buff) ;
void tmc2209_write(uint8_t tmc_addr,uint8_t register_addr,uint8_t* tx_data);
