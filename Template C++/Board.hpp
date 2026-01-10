#include "Types.hpp"

#pragma once

/*
Wzorzec OOP: BRAK (enkapsulacja planszy)
Zmienna private:
- Board_Size Size
Zmienna public:
- BRAK
Metody:
- int Width() const
- int Height() const
- void Draw_Boards()
- bool Inside_Board() const
*/
class Board {
    private:
        Board_Size Size;
    
    public:
        //Kontruktor
        Board(Board_Size S) : Size(S) {} //Zmienne dobrze by było jak by były zależne od rozdzielczości 

        //Get
        int Width() const  {return Size.Width;}
        int Height() const {return Size.Height;}

        void Draw_Boards() {
            for (int iY = 0; iY < Size.Height; iY++)
                {
                    for (int iX = 0; iX < Size.Width; iX++) {
                        //Tutaj funkcja rysująca - W GDscript TileMap
                    }
                }
        }

        //Test czy znajdujemy się jeszcze na planszy - indeksy od 0
        bool Inside_Board(int X, int Y) const {
            return ( (X >= 0 && X < Size.Width) && (Y >= 0 && Y < Size.Height));
        }
};