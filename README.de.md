<p align="center">
    <img src="./assets/fux-material-icon.svg" width=25% />
</p>

![total lines](https://aschey.tech/tokei/github/fuechs/fux?labelColor=151515&color=fcaa68&style=for-the-badge)
![top language](https://img.shields.io/github/languages/top/fuechs/fux?labelColor=151515&color=fcaa68&style=for-the-badge)
![version](https://img.shields.io/badge/version-alpha-fcaa68?colorA=151515&style=for-the-badge)
[![license](https://img.shields.io/badge/license-Apache%20License%20v2.0-fcaa68?labelColor=151515&style=for-the-badge)](./LICENSE)

> __Warning__ \
> Fux ist noch in früher Entwicklung. \
> Momentan werden nur Unix-basierte Systeme unterstützt.

###### Flawless Unambiguous Programming

## Die Bessere Alternative Für C++

```rust
main(): i64 {
    puts "Hello World!\n";
    return 0;
}
```

<p align="center">
    <a href="#beispiele"><img src="https://img.shields.io/badge/-Beispiele-fcaa68?style=for-the-badge"/></a>
    <a href="#status"><img src="https://img.shields.io/badge/-Status-fcaa68?style=for-the-badge" /></a>
    <a href="./docs/README.md"><img src="https://img.shields.io/badge/-Dokumentation-fcaa68?style=for-the-badge" /></a>
</p>

## Warum Fux?

### Flexibel, Portabel, Schnell

- Mindestens genauso leistungsfähig wie C++[^1]
- Verwendung von LLVM zum Kompilieren und Ausführen auf verschiedenen Plattformen

### Einfach zu lernen

- Bereits bekannte Umgebung für Entwickler mit Erfahrung in C++
- Pythonische Eigenschaften: automatische Typisierung, eingebauter Stringtyp, einfache Arrays, sichere Speicherverwaltung

### Große Standard-Bibliothek

- Fux kommt mit einer umfangreichen Standardbibliothek, die viele nützliche Funktionen enthält
- Die "Core"-Bibiliothek umfasst unter anderem[^3]
    - Konsolen-I/O
    - Datei-I/O
    - Mathematik
    - Algorithmen

### Besser lesbarer Code

- Variablen- und Funktionstypen werden nach dem Namen deklariert, was das Lesen des Codes erleichtert
- Die Pipe-Operatoren (`<<`, `>>`) sind ein Kernmerkmal von Fux und machen es einfach, die Verschachtelung von Funktionen zu vermeiden[^3]
- Variablen, Funktionen und Klassen können ohne die Verwendung eines einzigen Schlüsselworts definiert werden[^2]

[^1]: Solange der Compiler nicht auf die Garbage Collection zurückgreifen muss und viel Arbeit abnimmt.

[^2]: Solange die automatische Typisierung verwendet wird.

[^3]: Mehr Informationen [hier](./docs/README.md).

## Status

### Plan

- [&check;] `Error System`
- [&check;] `Lexer`
- [...] `Parser`
- [&cross;] `AST Analyse`
- [...] `IR Generation (LLVM)`
- [&cross;] `Optimisierung (LLVM)`
- [&cross;] `Kompilation (LLVM)`
- [...] `Runtime`
- [...] `Syntax`
- [...] `Standard-Bibliothek`

Fux befindet sich derzeit noch in einer frühen Entwicklungsphase. Es gibt noch keinen funktionierenden Compiler für diese Sprache. 

Im Moment konzentriere ich mich auf die Verfeinerung der Syntax von Fux, da einige Aspekte noch nicht klar sind.
Ich habe mich von [C++](https://isocpp.org/), [D](https://dlang.org/), [Python](https://python.org/), [Odin](https://odin-lang.org/) und [Elixir](https://elixir-lang.org/) inspirieren lassen.

## Beispiele

### Arrays

```rust
array: i32[]; // {}
array[] << 1; // {1}
array[] << 3; // {1, 3}
```

### Dateien auslesen

```rust
get core.file;

using file;
...
filePath := "path/to/file.txt";
file: File(filePath, 'r');
contents := file.read();
```

### Formatierte Ausgabe

```rust
get core.io;
...
res := 1;
io.putf("Result: %i", res);
```

### Fibonacci

```rust
get core.io;

fib(num: i32): i32 {
    if (num < 2)
    	return num;    
    return fib(num-1) + fib(num-2);
}

main(): i64 {
    io.getint() >> fib() >> io.putln();
    return 0;
}
```