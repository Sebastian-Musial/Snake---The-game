#include "Types.hpp"

#pragma once

/*
Wzorzec OOP: BRAK (klasa pomocnicza "utility class" – logika ruchu)
Zmienna private:
- BRAK
Zmienna public:
- BRAK
Metody:
- static void Next_move()
- static bool Is_Opposite()
*/
class Game_Mechanic {
    public:
        //Metody
        static void Next_move(int Actual_X, int Actual_Y, Directions Dir, int& nextX, int& nextY) {
            nextX = Actual_X;
            nextY = Actual_Y;
                
            //Mechanika na przód gdzie patrze
            switch (Dir) {
                case Directions::Up:
                    nextY--;
                    break;
                case Directions::Down:
                    nextY++;
                    break;
                case Directions::Right:
                    nextX++;
                    break;
                case Directions::Left:
                    nextX--;
                    break;
            }
        }

        //Walidacja czy nie zmieniamy natychmiastowo kierunku na przeciwny
        static bool Is_Opposite(Directions A, Directions B) {
            return
                (A == Directions::Up && B == Directions::Down) ||
                (A == Directions::Down && B == Directions::Up) ||
                (A == Directions::Left && B == Directions::Right) ||
                (A == Directions::Right && B == Directions::Left);
            }
};