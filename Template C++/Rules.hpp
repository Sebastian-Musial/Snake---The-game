#include "Types.hpp"

#pragma once

/*
Wzorzec OOP: STRATEGY (interfejs zasad gry)
Zmienna private:
- BRAK
Zmienna public:
- BRAK
Metody:
- virtual ~Game_Rules()
- virtual Board_Size Get_Board_Size() const = 0
- virtual int Turn_Time() const = 0
*/
class Game_Rules {
    public:
        virtual ~Game_Rules() = default;

        virtual Board_Size Get_Board_Size() const = 0;
        virtual int Turn_Time() const = 0; //Zamiast DT będziemy definiować czas na ture - ruch węża to np. 2 sekundy
};

//Na start 2 opcje z tylko 2 rozmiarami planszy - później można więcej według wzorca

/*
Wzorzec OOP: STRATEGY (konkretna strategia – tryb normalny)
Zmienna private:
- BRAK
Zmienna public:
- BRAK
Metody:
- Board_Size Get_Board_Size() const override
- int Turn_Time() const override
*/
class Rules_Normal : public Game_Rules {
    public:
        Board_Size Get_Board_Size() const override { return Small; }
        int Turn_Time() const override { return 1000; } //[1000 ms] = 1 sekunda
};

/*
Wzorzec OOP: STRATEGY (konkretna strategia – tryb trudny)
Zmienna private:
- BRAK
Zmienna public:
- BRAK
Metody:
- Board_Size Get_Board_Size() const override
- int Turn_Time() const override
*/
class Rules_Hard : public Game_Rules {
    public:
        Board_Size Get_Board_Size() const override { return Medium; }
        int Turn_Time() const override { return 500; }  //[500 ms] = 0.5 sekundy
};