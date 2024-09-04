import re

def remove_comments_and_preserve(code):
    """
    Diese Funktion entfernt Python-Kommentare und konvertiert sie in C++-Kommentare.
    """
    # Finde und konvertiere alle Kommentare
    code_with_cpp_comments = re.sub(r'#(.*)', r'//\1', code)
    return code_with_cpp_comments

def parse_python_array(array_str):
    """
    Diese Funktion parst einen Array-String im Python-Format und gibt eine Liste von Tupeln zurück,
    wobei jedes Tupel eine Note und eine Dauer darstellt.
    """
    # Entfernen der eckigen Klammern und Umwandeln des Strings in eine Liste von Tupeln
    array_str = array_str.strip('[]').strip()
    items = re.findall(r"'([^']*)'|\d+", array_str)

    # Debug-Ausgabe der gefundenen Items
    print(f"Found items: {items}")

    if len(items) >= 2:
        description = items[0]
        # Entfernen von überflüssigen Leerzeichen und Umwandeln des Tempo-Werts
        tempo_str = items[1].strip()
        try:
            tempo = int(tempo_str)
        except ValueError:
            tempo = 0  # Falls der Tempo-Wert nicht konvertiert werden kann
        items = items[2:]
    else:
        description = ""
        tempo = 0

    # Debug-Ausgabe des extrahierten Tempos
    print(f"Extracted description: '{description}', tempo: {tempo}")

    # Umwandeln der verbleibenden Einträge in Tupel von (Wert, Dauer)
    result = []
    for i in range(0, len(items), 2):
        if i + 1 < len(items):
            note = items[i]
            try:
                duration = int(items[i + 1])
            except ValueError:
                duration = 0  # Falls die Dauer nicht konvertiert werden kann
            result.append((note, duration))
    
    return description, tempo, result

def convert_to_cpp_format(melody_data):
    """
    Diese Funktion konvertiert eine Liste von Melodien in das C++-Array-Format.
    """
    cpp_code = ""

    for description, tempo, melody in melody_data:
        cpp_code += f"// {description}\n"
        cpp_code += f"// Tempo: {tempo}\n"
        cpp_code += "const int melody[][2] = {\n"
        
        for note, duration in melody:
            cpp_code += f"    {{ {note}, {duration} }},\n"
        
        cpp_code += "};\n\n"

    return cpp_code

def main():
    # Lies die Python-Datei
    with open('melodies.py', 'r') as file:
        content = file.read()

    # Entferne die Zeilen melody.append(notes)
    content = re.sub(r"melody\s*=\s*\[\]", '', content)
    content = re.sub(r"melody\.append\(notes\)", '', content)
    
    # Entferne Python-Kommentare und konvertiere sie in C++-Kommentare
    content = remove_comments_and_preserve(content)

    # Finde alle Array-Daten in der Datei
    pattern = re.compile(r"notes = \[.*?\]", re.DOTALL)
    matches = pattern.findall(content)

    # Parsen und Konvertieren der Arrays
    melodies = [parse_python_array(match) for match in matches]

    cpp_code = convert_to_cpp_format(melodies)
    
    # Schreibe das Ergebnis in eine C++-Datei
    with open('output_notes.cpp', 'w') as file:
        file.write(cpp_code)
    
    print("C++ Code written to output_notes.cpp")

if __name__ == "__main__":
    main()
