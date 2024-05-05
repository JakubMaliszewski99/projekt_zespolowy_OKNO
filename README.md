# projekt_zespolowy_OKNO 2024 lato

## Pierwszoosobowa gra fps 2.5D (strzelanka) w stylu Doom93 lub Wolfenstein 3D wykorzystujca raycasting w języku C++ z biblioteką SFML.

## Założenia i funkcjonalność

1. Generowanie mapy z tabeli (minimapa poziomu w rogu ekranu),
2. Renderowanie sceny za pomocą raycastingu,
3. Użycie tekstur na ścianach,
4. Shading zależny od odległości od gracza,
5. Podstawowe ruchy gracza (ruch prawo, lewo, przód, tył, oraz obrót prawo-lewo wokół osi z), gracz posiada również broń, którą może strzelać
6. W grze są wrogowie, którzy atakują gracza i przemieszczają się w jego kierunku.

## Wymagania

1. Biblioteka SFML w wersji 2.5.1. (ścieżka C:\SFML-2.5.1) download: https://www.sfml-dev.org/files/SFML-2.5.1-windows-gcc-7.3.0-mingw-32-bit.zip
2. Zalecany kompilator g++ wersja 7.6.0 (ścieżka C:\mingw32\bin) download: https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/7.3.0/threads-posix/dwarf/i686-7.3.0-release-posix-dwarf-rt_v5-rev0.7z/download

## Instalacja

1. Klonowanie repozytorium przez url: `https://github.com/JakubMaliszewski99/projekt_zespolowy_OKNO.git`
2. Instalacja kompilatora gcc 7.6.0,
3. Instalacja biblioteki SFML 2.5.1,
4. Zbudowanie projektu możliwe poprzez komendę make, lub przez otwarcie folderu w VS code i zbudowanie aplikacji,
5. Odpalenie gry plikiem wykonywalnym .exe w folderze bin.

## TODO:
1. Dodac do loggera zapis do pliku.
2. Przerobic InputManager na emit-subscribe?
3. Ogarnac wszystkie komentarze z TODO.

Autorzy:
Jakub Maliszewski
Mateusz Lewczak