#include <iostream>
#include <memory> //Potrzebny do wzorców
#include <vector>
#include <deque>   //Kontener dla elementow weza

enum class Kierunek {
    Gora,
    Dol,
    Prawo,
    Lewo
};

struct Board_Size {int Width, Height;};

constexpr Board_Size Small{20,20};
constexpr Board_Size Medium{40,40};
constexpr Board_Size Large{60,60};

//==========================================
//==========================================
//==========================================

class Plansza {
    private:
        Board_Size Size;
    
    public:
        //Kontruktor
        Plansza(Board_Size S) : Size(S) {} //Zmienne dobrze by było jak by były zależne od rozdzielczości 

        //Get
        int Width() const  {return Size.Width;}
        int Height() const {return Size.Height;}

        void Rysuj_Plansze() {
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
//==========================================
//==========================================
//==========================================

class Owoc {
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

};

//==========================================
//==========================================
//==========================================

class Game_Mechanic {
    public:
        //Metody
        static void Next_move(int Actual_X, int Actual_Y, Kierunek Dir, int& nextX, int& nextY) {
            nextX = Actual_X;
            nextY = Actual_Y;
                
            //Mechanika na przód gdzie patrze
            switch (Dir) {
                case Kierunek::Gora:
                    nextY--;
                    break;
                case Kierunek::Dol:
                    nextY++;
                    break;
                case Kierunek::Prawo:
                    nextX++;
                    break;
                case Kierunek::Lewo:
                    nextX--;
                    break;
            }
        }

};

//==========================================
//==========================================
//==========================================

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

};

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

class Snake : public Player {
    private:
        int Pos_X;
        int Pos_Y;
        Kierunek Patrze_W;
        //std::vector <Snake_Element> Body;
        std::deque<Snake_Element> Body;
    
    public:
        //Kontruktor
        Snake(int Start_X, int Start_Y, Kierunek patrze_w  = Kierunek::Gora) : Patrze_W(patrze_w ) 
        {
            Body.emplace_back(Start_X, Start_Y); //Długość węża 1
        }

        //Współrzędne głowy
        int Head_X() const {return Body.empty() ? 0 : Body[0].get_X();}
        int Head_Y() const {return Body.empty() ? 0 : Body[0].get_Y();}

        const std::deque<Snake_Element>& get_Body() const {return Body;}

        //Get
        int get_X() const {return Pos_X;}
        int get_Y() const {return Pos_Y;}
        Kierunek get_Patrze_W() const {return Patrze_W;}
        //const std::vector<Snake_Element>& get_Body() const {return Body;}

        //Set
        void set_X(int New_X) {Pos_X = New_X;}
        void set_Y(int New_Y) {Pos_Y = New_Y;}
        void set_Patrze_W(Kierunek New_Patrze_W) { Patrze_W = New_Patrze_W;}

        //Metody
        void Eat() {
            if (!Body.empty()) {
                const Snake_Element& Last_Element = Body.back();
                Body.push_back(Last_Element);
            }
        }

        //Kolicje: Ze ścianą, z samym sobą, z owocem
        //Ze ścianą
        bool Kolizja_Sciana(const Plansza& p, int x, int y) {
            return !p.Inside_Board(x, y);
        }

        //Z samym zobą
        bool Kolizja_Ze_Soba(const Snake& s, int x, int y) {
            const std::deque<Snake_Element>& C_body = s.get_Body();
            for (int i = 0; i < C_body.size(); i++) {   //Możliwe że i = 1 ponieważ indeks 0 to głowa - Wymaga testu 
                if (C_body[i].get_X() == x && C_body[i].get_Y() == y) {
                    return true;
                }
            }
            return false;
        }

        //Z owocem
        bool Kolizja_Owoc(int HeadX, int HeadY, const Owoc& o) {
            return (HeadX == o.get_X() && HeadY == o.get_Y());
        }

        //Nowy krok czy się uda
        bool Step(const Plansza& board, Owoc& fruit) {
            if (Body.empty()) return false;


            //Liczymy nową pozycje głowy węża
            int nextX; 
            int nextY;
            Game_Mechanic::Next_move(Head_X(), Head_Y(), Patrze_W, nextX, nextY);

            //Test kolizji
            if (Kolizja_Sciana(board, nextX, nextY)) {
                return false;   //Przegrana - Przez ściane
            }

            for (const auto& seg : Body) {
                if (seg.get_X() == nextX && seg.get_Y() == nextY) {
                    return false; //Przegrana - przez samegosiebie
                }
            }    

            //Test zjedzenia owocu
            bool EatFruit = (nextX == fruit.get_X() && nextY == fruit.get_Y());

            //Nowa głowa
            Body.emplace_front(nextX, nextY);

            if (!EatFruit) {
            Body.pop_back();
            } else {
                //Respawn owocu + score+1
            }

            return true;
        }



};

//==========================================
//==========================================
//==========================================

class Gra_Snake; //Wcześniejsza deklaracja w celu użycia w state_game


class State_Game {  //Interfej stanu
    public:
        virtual ~State_Game() = default;
};

class Gra_Snake {
    private:
        std::unique_ptr<State_Game> stan;
    
    public:
        void Zmien_Stan(std::unique_ptr<State_Game> Nowy_stan) {
            stan = std::move(Nowy_stan);
        }
};

//Stany dla wzorca STATE
class State_Menu : public State_Game {
    public:
    //Pokaz menu
    //Rysuj
    /*
    switch change options
    1-Play
    2-options
    3-Credits
    4-Quit
    */
};
class State_Opcje : public State_Game {
    public:
    /*
    Rozdzielczosc
    Audio
    Powrót do menu
    */
};
class State_Credits : public State_Game {
    public:
    /*
    Autor
    Licencja Godot
    */
};
class State_Error : public State_Game {
    public:
    //Komunikat z błędem 
};
class State_Gra : public State_Game {
    public:
    /*
    1 czy 2 snake
    Rysuj plansze
    <GRAMY>
    */
};
class State_Quit : public State_Game {
    public:
    //Wylacz
};

//==========================================
//==========================================
//==========================================