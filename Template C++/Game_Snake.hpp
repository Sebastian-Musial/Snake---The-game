#include "Types.hpp"

#include "Board.hpp"
#include "Player_Snake.hpp"
#include "Fruit.hpp"
#include "Rules.hpp"
#include "State.hpp"
#include "Game_Mechanic.hpp"

#pragma once

/*
Wzorzec OOP: CONTEXT (łączy STATE + STRATEGY)
Zmienna private:
- unique_ptr<State_Game> Stan
- unique_ptr<Game_Rules> Rules
- unique_ptr<Board> Board
- unique_ptr<Snake> Snake_Obj
- unique_ptr<Fruit> Fruit
- mt19937 RNG
Zmienna public:
- BRAK
Metody:
- Board* get_Board()
- Snake* get_Snake()
- Fruit* get_Fruit()
- Game_Rules* get_Rules()
- void Change_State(std::unique_ptr<State_Game> Nowy_stan)
- void Set_Rules(std::unique_ptr<Game_Rules> New_Rules)
- void New_Game()
- bool Turn()
- void Input_Direction(Directions Dir)
*/
class Game_Snake {
    private:
        std::unique_ptr<State_Game> Stan;
        //---
        std::unique_ptr<Game_Rules> Rules;
        std::unique_ptr<Board> Board_Obj;
        std::unique_ptr<Snake> Snake_Obj;
        std::unique_ptr<Fruit> Fruit_Obj;

        //Generator liczb pseudolosowych
        std::mt19937 RNG; //Random number generator
    
    public:
        Game_Snake() //W konstruktorze wartość seed z random_device przekazywana jest do generatora liczb pseudolosowych
            : RNG(std::random_device{}())
        {}

        //Gettery
        Board* get_Board() { return Board_Obj.get(); }
        Snake* get_Snake() { return Snake_Obj.get(); }
        Fruit* get_Fruit() { return Fruit_Obj.get(); }
        Game_Rules* get_Rules() { return Rules.get(); }
    
        //Wzorzec State
        void Change_State(std::unique_ptr<State_Game> Nowy_stan) {
            Stan = std::move(Nowy_stan);
        }

        //Wzorzec strategy
        void Set_Rules(std::unique_ptr<Game_Rules> New_Rules) {
            Rules = std::move(New_Rules);
            Board_Obj = std::make_unique<Board>(Rules->Get_Board_Size());
        }

        void New_Game() {
            if (!Rules) Set_Rules(std::make_unique<Rules_Normal>());
            if (!Board_Obj) Board_Obj = std::make_unique<Board>(Rules->Get_Board_Size());
            
            int StartX = Board_Obj->Width() / 2;
            int StartY = Board_Obj->Height() / 2;

            Snake_Obj = std::make_unique<Snake>(StartX, StartY, Directions::Up);
            Fruit_Obj = std::make_unique<Fruit>();
            Fruit_Obj->Spawn(*Board_Obj, *Snake_Obj, RNG);
        }

        bool Turn() {
            if (!Board_Obj || !Snake_Obj || !Fruit_Obj || !Rules) return false;
            int Next_X;
            int Next_Y;

            Game_Mechanic::Next_move(Snake_Obj->Head_X(), Snake_Obj->Head_Y(), Snake_Obj->get_My_Direction(), Next_X, Next_Y);
            return Snake_Obj->Step(*Board_Obj, *Fruit_Obj, RNG);
        }

        void Input_Direction(Directions Dir) {if (Snake_Obj) Snake_Obj->Try_Set_Direction(Dir);}
};