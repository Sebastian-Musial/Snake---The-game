# Snake 
Klasyczna gra w węża stworzona w Godot engine na siatce TileMap.
Projekt gry zawiera 2 tryby dla gracza i 1 tryb komputera.

Tryby dla gracza:
- Normal
- Hard

## Dostępne funkcje
- Sterowanie wężem (WASD / strzałki)
- Zbieranie punktów i naliczanie punktów po zjedzeniu
- Kolizja ze ścianami i z samym sobą - ciałem węża
- Dwa tryby gry: Normal / Hard (różnica w tempie poruszania węża oraz tempie ścieżki dźwiękowej)
- Tryb gry komputera gdzie algorytm zachłanny (greedy algorithm) gra w grę - porusza wężem

## Wymagania do otworzenia projektu / samodzielnego importu projektu do pliku wykonywalnego
- Godot engine zalezana wersja 4.5.1.stable

## Sterowanie
- Ruch węża:
 - W / Strzałka w górę - Góra
 - A / Strzałka w lewo - Lewo
 - S / Strzałka w dół - Dół
 - D / Strzałka w prawo - prawo

- Menu / okna dialogowe:
 - Myszka - Poruszamy myszką aby poruszyć kursorem / Naciskamy lewy przycisk myszy w celu wybrania opcji

## Zasady gry
- Wąż porusza się co turę o 1 pole
 - Wąż nie może robić obrotów o 180 stopni np. jeżeli jest zwrócony w lewo to nie może natychmiastowo zmienić swój zwrot w prawo
- Zjedzenie owocu:
 - Zwiększa długość węża
 - Dodaje punkt
 - Generuje nowy owoc w losowym miejscu które jest wolne
- Przegrana:
 - Wąż uderzy w ściane
 - Wąż uderzy we własne ciało 
- Wygrana:
 - Wąż zajmuje wszystkie wolne pola planszy (Cała gra zajęta przez ciało węża i ściany)

## Tryby gry
Tryb gry wybierany jest po wybraniu opcji play w menu, przed startem gry

- Tryb normal: Prędkość gry to 1 sekunda
- Tryb Hard: Prędkość gry to 0.5 sekundy

Tryby gry zostały zaprojektowane zgodnie z zasadami wzorca STRATEGY, zgodnie z zasadami programowania obiektowego. 
Każda strategia ma swoją własną klase.

## Tryb AI
Tryb gry w której algorytm zachłanny (greedy algorithm) steruje ruchem węża i gracz nie ma możliwości sterowania wężem.
Myślenie algorytmu jest skierowanie na minimalizowanie odległości taksówkowej (Manhattan) czyli zmniejszania odległości z punktu A do punktu B.
Algorytm nie planuje następnych ruchów i podejmuje decyzje tu i teraz odnośnie tylko jednego - następnego ruchu.
Ze względu na brak planowania przyszłych ruchów komputer jest w stanie przegrać ponieważ może zablokować się na drodze bez wyjścia i wpaść w kolizję.

## Główna struktura projektu
- Board_Class.gd - Ciało i logika dla klasy planszy
- Fruit_Class.gd - Ciało, logika i metody dla klasy owocu
- Player_Class.gd - Logika dla klasy gracza
- Snake_Class.gd - Ciało, logika i metody dla węża, dziedzicząca klasę gracza
- Wall_class.gd - Ciało i logika dla ścian
- Game.gd - Klasa zajmująca się docelową grą, rysuje tilesmap, obsługuje przepływ gry, sprawdza warunki gry
- Game_Mechanic_Class.gd - Klasa pomocnicza posiadająca niektóre mechaniki gry
- Maim_menu.gd oraz inne pliki M_<nazwa>.gd - Klasy obsługujące menu, przejścia pomiędzy scenami oraz przekazujące dane odnośnie trybu gry na podstawie decyzji gracza
- Game_Rules_Class.gd - Interfejs STRATEGY dla zasad gry
- Rules_Hard_Class.gd / Rules_Normal_Class.gd - Konkretna strategia zasad gry
- SnakeAI.gd - Logika obsługująca zachowanie AI w grze i jej algorytmu
- Audio_menu_manager.gd / GameState.gd / Types_class.gd - Klasy pomocnicze do manipulacji scenami, do przekazywania trybów gry pomiędzy scenami oraz utworzenia typów własnych dla gry 

## Autor 
Autor: Sebastian Musiał