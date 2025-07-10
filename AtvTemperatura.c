#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "aht10.h"

#define I2C_PORT i2c0
#define SDA_PIN 0
#define SCL_PIN 1

int main() {
    stdio_init_all();
    sleep_ms(2000);
    printf("Iniciando leitura do sensor AHT10...\n");

    // Inicializa I2C
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    if (!aht10_init(I2C_PORT)) {
        printf("Erro ao inicializar o sensor AHT10!\n");
        return 1;
    }

    while (true) {
        float temperatura = 0.0f, umidade = 0.0f;
        if (aht10_read(I2C_PORT, &temperatura, &umidade)) {
            printf("Temperatura: %.2f °C  |  Umidade: %.2f %%\n", temperatura, umidade);
        } else {
            printf("Erro na leitura do sensor!\n");
        }
        sleep_ms(1000);
    }
}
