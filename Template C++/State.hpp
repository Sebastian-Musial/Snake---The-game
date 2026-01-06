#include "Types.hpp"
#include "Game_Snake.hpp"

#pragma once

/*
Wzorzec OOP: STATE (interfejs)
Zmienna private:
- BRAK
Zmienna public:
- BRAK
Metody:
- virtual ~State_Game()
- virtual void Use_Input() = 0
- virtual void Update() = 0
*/
class State_Game { 
    public:
        virtual ~State_Game() = default;

        virtual void Use_Input(Game_Snake& Game, int Key) = 0; //Zgodnie w mysl ten sam klawisz ale inne mozliwe zachowanie wedlug state
        virtual void Update(Game_Snake& Game) = 0;
};


//Stany dla wzorca STATE
/*
Wzorzec OOP: STATE (konkretny stan: Menu)
Zmienna private:
- BRAK
Zmienna public:
- BRAK
Metody:
- void Use_Input(G) override
- void Update() override
*/
class State_Menu : public State_Game {
    public:
    /*Implementacja Update*/
    //Pokaz UI menu
    //Rysuj
    /*
    switch change options
    1-Play
    2-options
    3-Credits
    4-Quit
    */
    //Prototyp menu
    void Use_Input(Game_Snake& Game, int Key) override {
        if (Key == Key_Enter) Game.New_Game(); //+ game.Change_State(std::make_unique<State_Gra>());
        if (Key == Key_Esc) { } //Wyjscie z gry
    }

    void Update(Game_Snake&) override {

    }
};

/*
Wzorzec OOP: STATE (konkretny stan: Opcje)
Zmienna private:
- BRAK
Zmienna public:
- BRAK
Metody:
- (brak zdefiniowanych w szkicu)
*/
class State_Opcje : public State_Game {
    public:
    /*Implementacja Update*/
    /*
    Rozdzielczosc
    Audio
    Powrót do menu
    */
};

/*
Wzorzec OOP: STATE (konkretny stan: Credits)
Zmienna private:
- BRAK
Zmienna public:
- BRAK
Metody:
- (brak zdefiniowanych w szkicu)
*/
class State_Credits : public State_Game {
    public:
    /*Implementacja Update*/
    /*
    Autor
    Licencja Godot
    */
};

/*
Wzorzec OOP: STATE (konkretny stan: Error)
Zmienna private:
- BRAK
Zmienna public:
- BRAK
Metody:
- (brak zdefiniowanych w szkicu)
*/
class State_Error : public State_Game {
    public:
    //Error
};

/*
Wzorzec OOP: STATE (konkretny stan: Gra)
Zmienna private:
- BRAK
Zmienna public:
- BRAK
Metody:
- void Use_Input() override
- void Update() override
*/
class State_Game : public State_Game {
    public:
    /*Implementacja Update*/
    /*
    1 czy 2 snake
    Rysuj plansze
    <GRAMY>
    */
    //Sterowanie
    void Use_Input(Game_Snake& Game, int Key) override {
        if (Key == Key_W) Game.Input_Direction(Directions::Up);
        if (Key == Key_S) Game.Input_Direction(Directions::Down);
        if (Key == Key_A) Game.Input_Direction(Directions::Left);
        if (Key == Key_D) Game.Input_Direction(Directions::Right);
        if (Key == Key_Esc) {
            // game.Change_State(std::make_unique<State_Menu>());
        }  
    }  

    void Update(Game_Snake& Game) override {
        bool ok = Game.Turn();
        if (!ok) {
            // game.Change_State(std::make_unique<State_Error>());
        }
    }
};

/*
Wzorzec OOP: STATE (konkretny stan: Quit)
Zmienna private:
- BRAK
Zmienna public:
- BRAK
Metody:
- (brak zdefiniowanych w szkicu)
*/
class State_Quit : public State_Game {
    public:
    /*Implementacja Update*/
    //Wylacz
};