# Passive Buzzer Melody - Rasperri Pico W Projekt

Dieses Projekt verwendet einen passiven Buzzer, um verschiedene Melodien abzuspielen. Die Melodien werden über eine Arduino-Bibliothek bereitgestellt. Der Benutzer kann per Knopfdruck einen Song abspielen lassen.

## Funktionsweise

- Ein **passiver Buzzer** wird verwendet, um Töne zu erzeugen.
- Über einen **Button** kann eine Melodie abgespielt werden.
- Die Daten für die Melodien (Noten und Dauer) sowie das Tempo der Songs sind in einer **Textdatei** hinterlegt.
- Zum Abspielen eines neuen Songs müssen der **`tempo`**-Wert und der **`melody`**-Array in der Datei `Passive_Buzzer_Melody.cpp` entsprechend der Angaben in der Textdatei aktualisiert werden (zu finden in der main() ).

## Struktur der Textdatei

Die Textdatei enthält Informationen zu mehreren Songs:

1. **Tempo**: Das Tempo der Melodie in Beats per Minute (BPM).
2. **Array**: Enthält Informationen zu den gespielten Noten und deren Dauer. Jede Note wird durch eine Zahl dargestellt und die Dauer durch die Array-Position.
