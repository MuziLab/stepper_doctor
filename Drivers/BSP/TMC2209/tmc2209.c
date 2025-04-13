#include "./BSP/TMC2209/tmc2209.h"
#include "./BSP/UART/uart.h"

uint8_t dir_right_data[4] = {0x00,0x00,0x00,0x09};
uint8_t dir_left_data[4] = {0x00,0x00,0x00,0x01};

void official_tmc_CRC(uint8_t *datagram, uint8_t datagramLength)
{
    int i, j;
    uint8_t *crc = datagram + (datagramLength - 1); // CRC 位于数据包最后一个字节
    uint8_t currentByte;

    *crc = 0; // 初始 CRC 值设为 0

    // 遍历数据包中除最后一个字节外的所有数据
    for (i = 0; i < (datagramLength - 1); i++)
    {
        currentByte = datagram[i];

        // 对每个字节的每一位进行处理，共 8 位
        for (j = 0; j < 8; j++)
        {
            if (((*crc >> 7) ^ (currentByte & 0x01)) ) // 异或判断
            {
                *crc = (*crc << 1) ^ 0x07; // 多项式为 0x07
            }
            else
            {
                *crc = (*crc << 1);
            }
            currentByte = currentByte >> 1; // 处理下一位
        }
    }
}



// 全双工实现单工读
void tmc2209_full_read(uint8_t tmc_addr, uint8_t register_addr, uint8_t *rx_buff)
{
    uint8_t tx[4] = {0};
    tx[0] = 0x05;          
    tx[1] = tmc_addr;      // 芯片地址
    tx[2] = register_addr; 
    official_tmc_CRC(tx, 4);
    full_duplex_uart_transmit(tx, 4, 100);

    // 等待驱动返回：8字节数据包（写命令 + 数据 + CRC）
    uint8_t rx[8];
    
    if (full_duplex_uart_receive(rx, 8, 1000)!= HAL_OK)
    {   
        uint8_t err_message[3] = {'e', 'r','r'};
        uart_transmit(err_message,3,100);
    }
    
    ;

    // 提取数据（4字节）
    for (size_t i = 0; i < 4; i++)
    {
        rx_buff[i] = rx[i + 3];
    }
}

/**
 * @brief 全双工实现单工写
 * @attention 高位在前
 *   */ 

void tmc2209_full_write(uint8_t tmc_addr, uint8_t register_addr, uint8_t *tx_data)
{

    uint8_t buffer[8];
    buffer[0] = 0x05;       
    buffer[1] = 0x00;       // 芯片地址（默认）
    buffer[2] = register_addr|0x80;       // IHOLD_IRUN 寄存器地址
    buffer[3] = tx_data[0]; // IHOLD
    buffer[4] = tx_data[1]; // IRUN
    buffer[5] = tx_data[2];
    buffer[6] = tx_data[3];
    official_tmc_CRC(buffer,8);
    full_duplex_uart_transmit(buffer, 8, 100);
}
