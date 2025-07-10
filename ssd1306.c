#include "ssd1306.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <string.h>

static i2c_inst_t *oled_i2c;
static uint8_t oled_address;

#define SSD1306_BUFFER_SIZE (SSD1306_WIDTH * SSD1306_HEIGHT / 8)
static uint8_t buffer[SSD1306_BUFFER_SIZE];

static const uint8_t font5x7[][5] = {
    {0x00,0x00,0x00,0x00,0x00}, {0x00,0x00,0x5F,0x00,0x00},
    {0x00,0x07,0x00,0x07,0x00}, {0x14,0x7F,0x14,0x7F,0x14}
};

static void ssd1306_command(uint8_t cmd) {
    uint8_t buf[2] = {0x00, cmd};
    i2c_write_blocking(oled_i2c, oled_address, buf, 2, false);
}

void ssd1306_init(i2c_inst_t *i2c, uint8_t address) {
    oled_i2c = i2c;
    oled_address = address;

    sleep_ms(100);
    ssd1306_command(0xAE);
    ssd1306_command(0x20); ssd1306_command(0x00);
    ssd1306_command(0xB0); ssd1306_command(0xC8);
    ssd1306_command(0x00); ssd1306_command(0x10);
    ssd1306_command(0x40); ssd1306_command(0x81); ssd1306_command(0xFF);
    ssd1306_command(0xA1); ssd1306_command(0xA6);
    ssd1306_command(0xA8); ssd1306_command(0x3F);
    ssd1306_command(0xA4); ssd1306_command(0xD3); ssd1306_command(0x00);
    ssd1306_command(0xD5); ssd1306_command(0xF0);
    ssd1306_command(0xD9); ssd1306_command(0x22);
    ssd1306_command(0xDA); ssd1306_command(0x12);
    ssd1306_command(0xDB); ssd1306_command(0x20);
    ssd1306_command(0x8D); ssd1306_command(0x14);
    ssd1306_command(0xAF);
    ssd1306_clear();
    ssd1306_show();
}

void ssd1306_clear(void) {
    memset(buffer, 0x00, SSD1306_BUFFER_SIZE);
}

void ssd1306_show(void) {
    for (uint8_t page = 0; page < 8; page++) {
        ssd1306_command(0xB0 + page);
        ssd1306_command(0x00);
        ssd1306_command(0x10);

        uint8_t data[129];
        data[0] = 0x40;
        memcpy(&data[1], &buffer[SSD1306_WIDTH * page], SSD1306_WIDTH);
        i2c_write_blocking(oled_i2c, oled_address, data, 129, false);
    }
}

void ssd1306_draw_text(uint8_t x, uint8_t y, const char *text) {
    uint16_t index = y * SSD1306_WIDTH + x;
    while (*text && index + 5 < SSD1306_BUFFER_SIZE) {
        char c = *text++;
        if (c < 32 || c > 127) c = '?';
        for (int i = 0; i < 5; i++) {
            buffer[index++] = font5x7[(c - 32) % 4][i];
        }
        buffer[index++] = 0x00;
    }
}