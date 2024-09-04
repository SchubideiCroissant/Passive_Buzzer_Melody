#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdio.h>
#include "pitches.h"


//Bei einem passiven Buzzer geht es weniger um das pwm level und mehr um die eigentliche Frequenz die der Buzzer ausgibt
// Diese Frequenz muss mithilfe der Formel angepasst werden: 
// f=   (Systemtaktfrequenz)
// (PWM-Wrap-Wert) × (Clock Divider)

#define BUZZER_PIN 0
#define SWITCH_PIN 15

const uint WRAP_VAL = 255;   // PWM-Auflösung (z.B. 8-Bit)
const float SYS_CLK_FREQ = 125000000.0f; // Systemtaktfrequenz in Hz (125 MHz)
volatile bool button_pressed = false;

void setup_pwm(uint gpio, uint wrap_val) {
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    gpio_set_function(gpio, GPIO_FUNC_PWM); // Konfiguriere den Pin für PWM
    pwm_set_wrap(slice_num, wrap_val);      // Setze den Wrap-Wert (PWM-Periode)
    pwm_set_enabled(slice_num, true);       // Aktiviere den PWM-Slice
}

void play_tone(uint gpio, uint freq, uint wrap_val) {
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    float clock_divider = SYS_CLK_FREQ / (freq * wrap_val);  // Berechne den Clock Divider
    pwm_set_clkdiv(slice_num, clock_divider);                // Setze den Clock Divider
    pwm_set_gpio_level(gpio, wrap_val / 2);                  // Setze den Duty Cycle auf 50% für maximalen Klang
}

void stop_tone(uint gpio) {
    pwm_set_gpio_level(gpio, 0); // Stoppe den Ton, indem der PWM-Level auf 0 gesetzt wird
}

void play_melody() {
    printf("Melodie....");
    play_tone(BUZZER_PIN, NOTE_A4, WRAP_VAL); // Spiele den Ton A4 (440 Hz)
    sleep_ms(1000); // Ton für 1 Sekunde halten
    stop_tone(BUZZER_PIN); // Stoppe den Ton
    sleep_ms(500); // Pause für 0.5 Sekunden

    play_tone(BUZZER_PIN, NOTE_C5, WRAP_VAL); // Spiele den Ton C5 (523 Hz)
    sleep_ms(1000); // Ton für 1 Sekunde halten
    stop_tone(BUZZER_PIN); // Stoppe den Ton
    sleep_ms(500); // Pause für 0.5 Sekunden
    printf("Melodie Ende....");
    // Weitere Töne können hier hinzugefügt werden
}

void switch_isr(uint gpio, uint32_t event_mask) {
    button_pressed = true; // Setze den Zustand, um die Melodie in der Hauptschleife abzuspielen
}

int main() {
    stdio_init_all(); // Initialisiere Standard-I/O
    setup_pwm(BUZZER_PIN, WRAP_VAL); // Konfiguriere PWM auf dem Buzzer-Pin

    // Button-Pin als Eingang mit Pull-Up-Widerstand konfigurieren
    gpio_init(SWITCH_PIN);
    gpio_set_dir(SWITCH_PIN, GPIO_IN);
    gpio_pull_up(SWITCH_PIN);

    // Interrupt für den Button konfigurieren
    gpio_set_irq_enabled_with_callback(SWITCH_PIN, GPIO_IRQ_EDGE_FALL, true, &switch_isr);

    while (true) {
        if (button_pressed) {
            play_melody();  // Melodie abspielen
            button_pressed = false; // Zustand zurücksetzen
        }
        sleep_ms(10); // Kleine Verzögerung zur Entlastung der CPU
    }

    return 0;
}
