#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdio.h>
#include <stdlib.h>
#include "pitches.h"


//Bei einem passiven Buzzer geht es weniger um das pwm level und mehr um die eigentliche Frequenz die der Buzzer ausgibt
// Diese Frequenz muss mithilfe der Formel angepasst werden: 
// f =  (Systemtaktfrequenz)/
// (PWM-Wrap-Wert) × (Clock Divider)
// https://onlinesequencer.net/ für Songs

#define BUZZER_PIN 0
#define SWITCH_PIN 15

const uint WRAP_VAL =  4096;   // PWM-Auflösung (z.B. 8-Bit)
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
    printf("Berechnete Frequenz: %.2f Hz\n", SYS_CLK_FREQ / clock_divider*WRAP_VAL);

    pwm_set_clkdiv(slice_num, clock_divider);                // Setze den Clock Divider
    pwm_set_gpio_level(gpio, wrap_val / 2);                  // Setze den Duty Cycle auf 50% für maximalen Klang
}

void stop_tone(uint gpio) {
    pwm_set_gpio_level(gpio, 0); // Stoppe den Ton, indem der PWM-Level auf 0 gesetzt wird
}

int duration(int tempo, int t) {
    // Dauer einer ganzen Note in Millisekunden berechnen (60s/Tempo) * 4 Takte
    float wholenote = (60000.0f / tempo) * 4.0f;

    int noteDuration;
    if (t > 0) {
        noteDuration = static_cast<int>(wholenote / t);
    } else if (t < 0) {
        noteDuration = static_cast<int>(wholenote / abs(t));
        noteDuration = static_cast<int>(noteDuration * 1.5); // Punktierte Noten
    } else {
        noteDuration = 0; // Keine Dauer für "REST"
    }

    return noteDuration;
}
void play_melody(const int melody[][2], int num_notes, int tempo) {
    for (int i = 0; i < num_notes; ++i) {
        int note = melody[i][0];
        int duration_value = duration(tempo, melody[i][1]);

        if (note == REST) {
            stop_tone(BUZZER_PIN);
        } else {
            play_tone(BUZZER_PIN, note, WRAP_VAL);
        }

        sleep_ms(duration_value * 0.9); // 90% der Dauer spielen
        stop_tone(BUZZER_PIN);
        sleep_ms(duration_value * 0.1); // 10% Pause zwischen Noten
    }
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

    const int melody[][2] = {
    { NOTE_E5, 16 },
    { NOTE_E5, 8 },
    { NOTE_D5, 16 },
    { REST, 16 },
    { NOTE_CS5, -4 },
    { NOTE_E4, 8 },
    { NOTE_FS4, 16 },
    { NOTE_G4, 16 },
    { NOTE_A4, 16 },
    { NOTE_B4, -8 },
    { NOTE_E4, -8 },
    { NOTE_B4, 8 },
    { NOTE_A4, 16 },
    { NOTE_D5, -4 },
    { NOTE_E5, 16 },
    { NOTE_E5, 8 },
    { NOTE_D5, 16 },
    { REST, 16 },
    { NOTE_CS5, -4 },
    { NOTE_E4, 8 },
    { NOTE_FS4, 16 },
    { NOTE_G4, 16 },
    { NOTE_A4, 16 },
    { NOTE_B4, -8 },
    { NOTE_E4, -8 },
    { NOTE_B4, 8 },
    { NOTE_A4, 16 },
    { NOTE_D4, -4 },
    { REST, 8 },
    { NOTE_E5, 8 },
    { REST, 16 },
    { NOTE_B5, 16 },
    { REST, 8 },
    { NOTE_AS5, 16 },
    { NOTE_B5, 16 },
    { NOTE_AS5, 16 },
    { NOTE_G5, 16 },
    { REST, 4 },
    { NOTE_B5, 8 },
    { NOTE_B5, 16 },
    { NOTE_AS5, 16 },
    { REST, 16 },
    { NOTE_AS5, 16 },
    { NOTE_A5, 16 },
    { REST, 16 },
    { NOTE_B5, 16 },
    { NOTE_G5, 16 },
    { NOTE_B5, 16 },
    { NOTE_AS5, 16 },
    { REST, 16 },
    { NOTE_B5, 16 },
    { NOTE_A5, 16 },
    { NOTE_G5, 16 },
    { REST, 8 },
    { NOTE_E5, 8 },
    { REST, 16 },
    { NOTE_B5, 16 },
    { REST, 8 },
    { NOTE_AS5, 16 },
    { NOTE_B5, 16 },
    { NOTE_AS5, 16 },
    { NOTE_G5, 16 },
    { REST, 4 },
    { NOTE_B5, 8 },
    { NOTE_B5, 16 },
    { NOTE_AS5, 16 },
    { REST, 16 },
    { NOTE_AS5, 16 },
    { NOTE_A5, 16 },
    { REST, 16 },
    { NOTE_B5, 16 },
    { NOTE_G5, 16 },
    { NOTE_B5, 16 },
    { NOTE_AS5, 16 },
    { REST, 16 },
    { NOTE_B5, 16 },
    { NOTE_A5, 16 },
    { NOTE_G5, 16 },
    { NOTE_DS4, -8 },
    { NOTE_FS4, -8 },
    { NOTE_C5, 8 },
    { NOTE_B4, -8 },
    { NOTE_G4, -8 },
    { NOTE_E4, 8 },
    { NOTE_DS4, -8 },
    { NOTE_FS4, -8 },
    { NOTE_C5, 8 },
    { NOTE_B4, -8 },
    { NOTE_G4, -8 },
    { REST, 8 },
    { NOTE_DS4, -8 },
    { NOTE_FS4, -8 },
    { NOTE_C5, 8 },
    { NOTE_B4, -8 },
    { NOTE_G4, -8 },
    { NOTE_E4, 8 },
    { NOTE_DS4, -8 },
    { NOTE_FS4, -8 },
    { NOTE_C5, 8 },
    { NOTE_B4, -8 },
    { NOTE_CS5, -8 },
    { NOTE_DS5, 8 },
    { NOTE_E5, 16 },
    { NOTE_E5, 16 },
    { NOTE_E4, 16 },
    { NOTE_E4, -2 },
    { NOTE_C4, 8 },
    { NOTE_C4, 8 },
    { NOTE_E4, 16 },
    { NOTE_G4, -8 },
    { NOTE_D4, 8 },
    { NOTE_D4, 8 },
    { NOTE_FS4, 16 },
    { NOTE_A4, -8 },
    { NOTE_E5, 16 },
    { NOTE_E5, 16 },
    { NOTE_E4, 16 },
    { NOTE_E4, -2 },
    { NOTE_C4, 8 },
    { NOTE_C4, 8 },
    { NOTE_E4, 16 },
    { NOTE_G4, -8 },
    { NOTE_D4, 8 },
    { NOTE_D4, 8 },
    { NOTE_B3, 16 },
    { NOTE_D4, -8 },
    { NOTE_E5, 16 },
    { NOTE_E5, 8 },
    { NOTE_D5, 16 },
    { REST, 16 },
    { NOTE_CS5, -4 },
    { NOTE_E4, 8 },
    { NOTE_FS4, 16 },
    { NOTE_G4, 16 },
    { NOTE_A4, 16 },
    { NOTE_B4, -8 },
    { NOTE_E4, -8 },
    { NOTE_B4, 8 },
    { NOTE_A4, 16 },
    { NOTE_D5, -4 },
    { NOTE_E5, 16 },
    { NOTE_E5, 8 },
    { NOTE_D5, 16 },
    { REST, 16 },
    { NOTE_CS5, -4 },
    { NOTE_E4, 8 },
    { NOTE_FS4, 16 },
    { NOTE_G4, 16 },
    { NOTE_A4, 16 },
    { NOTE_B4, -8 },
    { NOTE_E4, -8 },
    { NOTE_B4, 8 },
    { NOTE_A4, 16 },
    { NOTE_D4, -4 },
    { REST, 8 },
    { NOTE_E5, 8 },
    { REST, 16 },
    { NOTE_B5, 16 },
    { REST, 8 },
    { NOTE_AS5, 16 },
    { NOTE_B5, 16 },
    { NOTE_AS5, 16 },
    { NOTE_G5, 16 },
    { REST, 4 },
    { NOTE_B5, 8 },
    { NOTE_B5, 16 },
    { NOTE_AS5, 16 },
    { REST, 16 },
    { NOTE_AS5, 16 },
    { NOTE_A5, 16 },
    { REST, 16 },
    { NOTE_B5, 16 },
    { NOTE_G5, 16 },
    { NOTE_B5, 16 },
    { NOTE_AS5, 16 },
    { REST, 16 },
    { NOTE_B5, 16 },
    { NOTE_A5, 16 },
    { NOTE_G5, 16 },
    { REST, 8 },
    { NOTE_E5, 8 },
    { REST, 16 },
    { NOTE_B5, 16 },
    { REST, 8 },
    { NOTE_AS5, 16 },
    { NOTE_B5, 16 },
    { NOTE_AS5, 16 },
    { NOTE_G5, 16 },
    { REST, 4 },
    { NOTE_B5, 8 },
    { NOTE_B5, 16 },
    { NOTE_AS5, 16 },
    { REST, 16 },
    { NOTE_AS5, 16 },
    { NOTE_A5, 16 },
    { REST, 16 },
    { NOTE_B5, 16 },
    { NOTE_G5, 16 },
    { NOTE_B5, 16 },
    { NOTE_AS5, 16 },
    { REST, 16 },
    { NOTE_B5, 16 },
    { NOTE_A5, 16 },
    { NOTE_G5, 16 },
    { NOTE_DS4, -8 },
    { NOTE_FS4, -8 },
    { NOTE_C5, 8 },
    { NOTE_B4, -8 },
    { NOTE_G4, -8 },
    { NOTE_E4, 8 },
    { NOTE_DS4, -8 },
    { NOTE_FS4, -8 },
    { NOTE_C5, 8 },
    { NOTE_B4, -8 },
    { NOTE_G4, -8 },
    { REST, 8 },
    { NOTE_DS4, -8 },
    { NOTE_FS4, -8 },
    { NOTE_C5, 8 },
    { NOTE_B4, -8 },
    { NOTE_G4, -8 },
    { NOTE_E4, 8 },
    { NOTE_DS4, -8 },
    { NOTE_FS4, -8 },
    { NOTE_C5, 8 },
    { NOTE_B4, -8 },
    { NOTE_CS5, -8 },
    { NOTE_DS5, 8 },
    { NOTE_E5, 16 },
    { NOTE_E5, 16 },
    { NOTE_E4, 16 },
    { NOTE_E4, -2 },
    { NOTE_C4, 8 },
    { NOTE_C4, 8 },
    { NOTE_E4, 16 },
    { NOTE_G4, -8 },
    { NOTE_D4, 8 },
    { NOTE_D4, 8 },
    { NOTE_FS4, 16 },
    { NOTE_A4, -8 },
    { NOTE_E5, 16 },
    { NOTE_E5, 16 },
    { NOTE_E4, 16 },
    { NOTE_E4, -2 },
    { NOTE_C4, 8 },
    { NOTE_C4, 8 },
    { NOTE_E4, 16 },
    { NOTE_G4, -8 },
    { NOTE_D4, 8 },
    { NOTE_D4, 8 },
    { NOTE_B3, 16 },
    { NOTE_D4, -8 },
};
    int tempo = 130;
    const int num_notes = sizeof(melody) / sizeof(melody[0]); // Beispielanzahl von Noten
     // BPM - Beats per Minute, Anpassen je nach Lied
    while (true) {
        if (button_pressed) {
            play_melody(melody, num_notes, tempo);  // Melodie abspielen
            button_pressed = false; // Zustand zurücksetzen
        }
        sleep_ms(10); // Kleine Verzögerung zur Entlastung der CPU
    }

    return 0;
}
