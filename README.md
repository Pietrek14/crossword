# crossword

A console-based crossword app made for school. It is written in C++ and requires support for ANSI escape codes.

## Input

The program reads input from the `crossword.cwd` file. The format is:
1. A rectangle of characters representing the filled in crossword, where `#` is a black square. The max size is 255x255.
2. An empty line.
3. A list of crossword clues, starting with the position of the first letter (first column, then row, separated by space indexing starts at 0), the direction (either **pion** - vertical, or **poziom** - horizontal), ` - `, and the clue itself, e.g. `0 0 pion - first clue`.

---

Program wczytuje wejście z pliku `crossword.cwd`. Format jest następujący:
1. Prostokąt znaków reprezentujący wypełnioną krzyżówkę, gdzie `#` oznacza czarny kwadrat. Maksymalny rozmiar to 255x255.
2. Pusta linia.
3. Lista wskazówek do krzyżówki, zaczynająca się od pozycji pierwszej litery (najpierw kolumna, potem wiersz, oddzielone spacją, indeksowanie zaczyna się od 0), kierunku (**pion** lub **poziom**), ` - `, i wskazówki, np. `0 0 pion - pierwsza wskazówka`.
