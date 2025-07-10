#include "aht10.h"
#include "pico/stdlib.h"

bool aht10_init(i2c_inst_t *i2c) {
    uint8_t cmd[] = {0xE1, 0x08, 0x00};  // Inicialização normal
    return i2c_write_blocking(i2c, AHT10_ADDR, cmd, 3, false) >= 0;
}

bool aht10_read(i2c_inst_t *i2c, float *temperature, float *humidity) {
    uint8_t cmd[] = {0xAC, 0x33, 0x00};
    uint8_t data[6];

    if (i2c_write_blocking(i2c, AHT10_ADDR, cmd, 3, false) < 0) return false;
    sleep_ms(80);  // tempo para conversão

    if (i2c_read_blocking(i2c, AHT10_ADDR, data, 6, false) < 0) return false;

    if (data[0] & 0x80) return false; // bit ocupado

    uint32_t raw_humidity = ((data[1] << 12) | (data[2] << 4) | (data[3] >> 4));
    uint32_t raw_temp     = (((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5]);

    *humidity = (raw_humidity * 100.0) / 1048576.0;
    *temperature = ((raw_temp * 200.0) / 1048576.0) - 50.0;

    return true;
}
