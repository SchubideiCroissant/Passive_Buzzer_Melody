# Passive Buzzer Melody - Rasperri Pico W Projekt

Dieses Projekt verwendet einen passiven Buzzer, um verschiedene Melodien abzuspielen. Die Melodien werden über eine Arduino-Bibliothek bereitgestellt. Der Benutzer kann per Knopfdruck einen Song abspielen lassen.

## Funktionsweise

- Ein **passiver Buzzer** wird verwendet, um Töne zu erzeugen.
- Über einen **Button** kann eine Melodie abgespielt werden.
- Die Daten für die Melodien (Noten und Dauer) sowie das Tempo der Songs sind in einer **cpp_melody_arrays.txt** hinterlegt.
- Zum Abspielen eines neuen Songs müssen der **`tempo`**-Wert und der **`melody`**-Array in der Datei `Passive_Buzzer_Melody.cpp` entsprechend der Angaben in der Textdatei aktualisiert werden (zu finden in der main() ).

## Struktur der Textdatei

Die Textdatei enthält Informationen zu mehreren Songs:

1. **Tempo**: Das Tempo der Melodie in Beats per Minute (BPM).
2. **Array**: Enthält Informationen zu den gespielten Noten und deren Dauer. Jede Note wird durch eine Zahl dargestellt und die Dauer durch die Array-Position.

## Frequenzen für den passiven Buzzer mit dem Raspberry Pi Pico

Der passive Buzzer benötigt bestimmte Frequenzen, um die korrekten Töne zu erzeugen. Diese Frequenzen sind in der Datei **`pitches.h`** als Konstanten definiert. Für jede Note (z.B. C4, G3 usw.) ist eine bestimmte Frequenz in Hertz (Hz) angegeben.

Um die richtige PWM-Frequenz zum passiven Buzzer zu senden, nutzt man folgende Formel:

f =  (Systemtaktfrequenz)/
( (PWM-Wrap-Wert) × (Clock Divider) )

f - Gewünschte Frequenz
Systemtaktfrequenz - 125 MHz
PWM-Wrap Wert - 1024

Da f bekannt ist muss die Formel nach Clock Divider umgestellt werden, den man dann mithilfe einer Funktion setzen kann.
