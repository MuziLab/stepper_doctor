#include "./BSP/UART/uart.h"
/*这里写tmc的驱动函数*/
extern uint8_t dir_left_data[4];
extern uint8_t dir_right_data[4];



// CRC校验函数（TMC2209 CRC算法）
void official_tmc_CRC(uint8_t *datagram, uint8_t datagramLength);

//半双工读写数据
void tmc2209_read(uint8_t tmc_addr,uint8_t register_addr,uint8_t* rx_buff) ;
void tmc2209_write(uint8_t tmc_addr,uint8_t register_addr,uint8_t* tx_data);

//全双工的单工读写数据
void tmc2209_full_read(uint8_t tmc_addr, uint8_t register_addr, uint8_t *rx_buff);
void tmc2209_full_write(uint8_t tmc_addr, uint8_t register_addr, uint8_t *tx_data);
