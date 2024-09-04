#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define PWM_PIN 0
int main()
{
    stdio_init_all();
    gpio_init(PWM_PIN); 
    gpio_set_dir(PWM_PIN, GPIO_OUT);
    gpio_put(PWM_PIN, false);

    while (true) {
        for(int i = 0;i < 80; i++){
        gpio_put(PWM_PIN, true);
        sleep_ms(100);
        gpio_put(PWM_PIN, false);
        sleep_ms(100);
        }
    }
}
