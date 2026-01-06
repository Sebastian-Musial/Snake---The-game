#include "Types.hpp"
#include "Board.hpp"
#include "Player_Snake.hpp"

#pragma once

/*
Wzorzec OOP: ENTITY (owoc gry / encja)
Zmienna private:
- int F_X
- int F_Y
Zmienna public:
- BRAK
Metody:
- int get_X() const
- int get_Y() const
- void set_XY()
- void Spawn()
*/
class Fruit {
    private:
        int F_X = 0;
        int F_Y = 0;
    public:
        //Get
        int get_X() const { return F_X; }
        int get_Y() const { return F_Y; }

        //Set
        void set_XY(int x, int y) { 
            F_X = x; 
            F_Y = y; 
        }
        //Funkcja wymaga testu poniewaz uzyty nowo poznany generator losujacy
        void Spawn(const Board& board, const Snake& snake, std::mt19937& rng) {   //mt19937 wymaga referencji generator z algoryhtmem był jeden a nie kilka przy każdym wywołaniu 
            std::uniform_int_distribution<int> dx(0, board.Width() - 1);
            std::uniform_int_distribution<int> dy(0, board.Height() - 1);

            //Petla losujaca miejsce az trafi na puste pole
            for (int try_free_space = 0; try_free_space < 5000; try_free_space++) {
                int x = dx(rng);
                int y = dy(rng);
                if (!snake.Occupies(x, y)) {
                    set_XY(x, y);
                return;
                }
            }   //Brak wolnego miejsca = funkcja nic nie zrobi
        }

        
        /*Dokumentacja dla generowania liczb pseudolosowych: https://cppreference.com/w/cpp/numeric/random.html
        
        std::mt19937& rng - Generator liczb pseudolosowych z biblioteki std - algorytm Mersenne Twister
            Przykład działania generatora:

            // 1. Losuje ziarno (seed)
            std::random_device rd;

            // 2. Generator liczb pseudolosowych
            std::mt19937 rng(rd());

            // 3. Zakres liczb w losowaniu 
            std::uniform_int_distribution<int> dist(1, 100);

            // 4. Losowanie liczb
            for (int i = 0; i < 10; ++i) {
                int liczba = dist(rng);
                std::cout << liczba << " ";
            }
            
            //Przykład innego generatora z algorytmem
            #include <cstdlib>
            #include <ctime>

                srand( time( NULL ) );
                std::cout << "Wylosowanie pierwsze: " <<(( std::rand() % 50 ) + 7 ) << std::endl;
                int liczba =( std::rand() % 50 ) + 7;
                std::cout << "Wylosowanie drugie: " << liczba << std::endl;
                liczba =( std::rand() % 50 ) + 7;
                std::cout << "Wylosowanie trzecie: " << liczba << std::endl;
                return 0;
        */
};