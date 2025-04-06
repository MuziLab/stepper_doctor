#include "./BSP/TMC2209/tmc2209.h"
#include "./BSP/UART/uart.h"


// CRC校验函数（TMC2209 CRC算法）
uint8_t tmc_crc(uint8_t *data, uint8_t len)
{
    uint8_t crc = 0;
    for (uint8_t i = 0; i < len; i++)
    {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x07;
            else
                crc <<= 1;
        }
    }
    return crc;
}
/**
 * @note rx_buff长度为4，输出数据位，如果crc不对，则输出为0
 */

void tmc2209_read(uint8_t tmc_addr, uint8_t register_addr, uint8_t *rx_buff)
{
    uint8_t tx[4];
    tx[0] = 0x07;          // 读命令
    tx[1] = tmc_addr;      // 芯片地址
    tx[2] = register_addr; // GCONF 寄存器地址
    tx[3] = tmc_crc(tx, 3);

    half_duplex_uart_transmit(tx, 4, 100);

    // 等待驱动返回：8字节数据包（写命令 + 数据 + CRC）
    uint8_t rx[8];
    half_duplex_uart_receive(rx, 8, 100);
    uint8_t crc_back = tmc_crc(rx, 7);
    if (crc_back == rx[7])
    {
        // 提取数据（4字节）
        for (size_t i = 0; i < 4; i++)
        {
            rx_buff[i] = rx[i + 3];
        }
    }
    else
    {
        *rx_buff = 0;
    }
}
/**
 * @attention tx_data长度为四
 */
void tmc2209_write(uint8_t tmc_addr, uint8_t register_addr, uint8_t *tx_data)
{
    uint8_t buffer[8];
    buffer[0] = 0x05;       // 写命令
    buffer[1] = 0x00;       // 芯片地址（默认）
    buffer[2] = 0x10;       // IHOLD_IRUN 寄存器地址
    buffer[3] = tx_data[0]; // IHOLD
    buffer[4] = tx_data[1]; // IRUN
    buffer[5] = tx_data[2];
    buffer[6] = tx_data[3];

    buffer[7] = tmc_crc(buffer, 7); // 计算CRC

    half_duplex_uart_transmit(buffer, 8, 100);
}
