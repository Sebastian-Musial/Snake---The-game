#include "Types.hpp"
#include "Fruit.hpp"

#pragma once

/*
Wzorzec OOP: BRAK (klasa bazowa gracza)
Zmienna private:
- int Score = 0
Zmienna public:
- BRAK
Metody:
- int get_Score()
- int set_Score()
- void Add_One_Point()
- void Reset_Score()
*/
class Player {
    private:        
        int Score = 0;
    public:
        //Kontruktor
        Player(int Score = 0) : Score(Score) {}

        //Get
        int get_Score() const {return Score;}

        //Set
        void set_Score(int I) {Score = I;}

        //Metody
        void Add_One_Point() {set_Score(Score + 1);}
        void Reset_Score() {Score = 0;}

};

/*
Wzorzec OOP: VALUE OBJECT (segment węża)
Zmienna private:
- int Pos_X
- int Pos_Y
Zmienna public:
- BRAK
Metody:
- int get_X()
- int get_Y()
- void set_XY()
*/
class Snake_Element {
    private:
        int Pos_X;
        int Pos_Y;

    public:
        //Kontruktor
        Snake_Element(int X, int Y) : Pos_X(X), Pos_Y(Y) {}

        //Get
        int get_X() const {return Pos_X;}
        int get_Y() const {return Pos_Y;}
        
        //Set - Na raz X i Y 
        void set_XY(int N_Pos_X, int N_Pos_Y) {
            Pos_X = N_Pos_X;
            Pos_Y = N_Pos_Y;
        }
};

/*
Wzorzec OOP: dziedziczenie (Snake ← Player)
Zmienna private:
- Directions My_Direction
- deque<Snake_Element> Body
- bool Grows_Snake = false
Zmienna public:
- BRAK
Metody:
- Snake()
- int Head_X() const
- int Head_Y() const
- const std::deque<Snake_Element>& get_Body() const
- int get_X() const
- int get_Y() const
- Directions get_My_Direction() const
- void set_X()
- void set_Y()
- void set_My_Direction()
- void set_Grows()
- void Reset()
- void Eat()
- bool Occupies() const
- bool Try_Set_Direction()
- void Grows()
- bool Collision_Wall()
- bool Collision_Self()
- bool Collision_Fruit()
- bool Step()
*/
class Snake : public Player {
    private:
        Directions My_Direction;
        //std::vector <Snake_Element> Body;
        std::deque<Snake_Element> Body;
        bool Grows_Snake = false;
    
    public:
        //Kontruktor
        Snake(int Start_X, int Start_Y, Directions Start_Direction  = Directions::Up) : My_Direction(Start_Direction) 
        {
            Body.emplace_back(Start_X, Start_Y); //Długość węża 1
        }

        //Współrzędne głowy
        int Head_X() const {return Body.empty() ? 0 : Body[0].get_X();}
        int Head_Y() const {return Body.empty() ? 0 : Body[0].get_Y();}

        const std::deque<Snake_Element>& get_Body() const {return Body;}

        //Get
        Directions get_My_Direction() const {return My_Direction;}
        //const std::vector<Snake_Element>& get_Body() const {return Body;}

        //Set
        void set_My_Direction(Directions New_My_Direction) { My_Direction = New_My_Direction;}
        void set_Grows(bool New_Grows) {Grows_Snake = New_Grows;}

        //Metody
        void Reset (int Start_X, int Start_Y, Directions Start_Direction  = Directions::Up) {
            Body.clear(); //Kasuje wszystkie elementy weza - czysci kontener
            Body.emplace_back(Start_X, Start_Y);  //Ustawienie weza w pozycji startowej - pierwszy element głowa pojawia się w kontenerze
            My_Direction = Start_Direction;    //Ustawia startowy Directions weza - Up
            Grows_Snake = false;
            Reset_Score();
        }
        
        void Eat() {
            if (!Body.empty()) {
                const Snake_Element& Last_Element = Body.back();
                Body.push_back(Last_Element);
            }
        }

        //Test sprawdzania które pola są zajęte przez węża
        bool Occupies(int X, int Y) const {
            for (const Snake_Element& seg : Body) if (seg.get_X() == X && seg.get_Y() == Y) return true;   //Dla przypomnienia zakresowe for przechodzi po wszystkich elementach obiektu np. kontenera
            return false;
        }

        //Zmiana z kierunku z brakiem możliwości zmiany na przeciwny 
        bool Try_Set_Direction(Directions NewDir) {
                if (Game_Mechanic::Is_Opposite(My_Direction, NewDir)) return false;
                My_Direction = NewDir;
                return true;
            }

        void Grows() {set_Grows(true);}

        //Kolicje: Ze ścianą, z samym sobą, z Fruitem
        //Ze ścianą
        bool Collision_Wall(const Board& p, int x, int y) {
            return !p.Inside_Board(x, y);
        }

        //Z samym zobą
        bool Collision_Self(const Snake& s, int x, int y) {
            const std::deque<Snake_Element>& C_body = s.get_Body();
            for (int i = 0; i < C_body.size(); i++) {   //Możliwe że i = 1 ponieważ indeks 0 to głowa - Wymaga testu. Nie jawna konwersja i z int na size_t
                if (C_body[i].get_X() == x && C_body[i].get_Y() == y) {
                    return true;
                }
            }
            return false;
        }

        //Z owocem
        bool Collision_Fruit(int HeadX, int HeadY, const Fruit& o) {
            return (HeadX == o.get_X() && HeadY == o.get_Y());
        }

        //Nowy krok czy się uda + dodaj punkt + respawn owocu - później mozna podzielic na kilka metod
        bool Step(const Board& board, Fruit& fruit, std::mt19937& RNG) {
            if (Body.empty()) return false;

            //Liczymy nową pozycje głowy węża
            int nextX; 
            int nextY;
            Game_Mechanic::Next_move(Head_X(), Head_Y(), My_Direction, nextX, nextY);

            //Test kolizji
            if (Collision_Wall(board, nextX, nextY)) {
                return false;   //Przegrana - Przez ściane
            }

            for (const auto& seg : Body) {
                if (seg.get_X() == nextX && seg.get_Y() == nextY) {
                    return false; //Przegrana - przez samegosiebie
                }
            }    

            //Test zjedzenia owocu
            bool EatFruit = (nextX == fruit.get_X() && nextY == fruit.get_Y());
            if (EatFruit) Grows();

            //Nowa głowa
            Body.emplace_front(nextX, nextY);

            //Test czy waz ma rosnac - czy waz sie powiekszy o element
            if (Grows_Snake) {
                Grows_Snake =false; //Rosnie i resetujemy czy ma ponownie urosnac w nastepnym ruchu
            }
            else {
                Body.pop_back();    //Nie rośnie wiec usuwamy ostatni element z kontenera przez .pop.back()
            }

            //Dodaj punkt i respawn nowego owocu
            if (EatFruit) {
                Add_One_Point();
                fruit.Spawn(board, *this, RNG);
            }

            return true;
        }
};