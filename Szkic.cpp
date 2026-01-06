#include <iostream>
#include <memory> //Potrzebny do wzorców
#include <vector>
#include <deque>   //Kontener dla elementow weza
#include <random> //Dla generatora pseudo losowych liczb

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

//Sterowanie
/*W ramach szkicu (GDscript może to zbierać inaczej) - sprawdzanie wciskanych klawiszy dla szkicu*/
enum Key {
    None,
    Key_W,
    Key_A,
    Key_S,
    Key_D,
    Key_Enter,
    Key_Esc
};

//Deklaracje klas
class Snake;
class Plansza;
class Owoc;
class Game_Rules;
class State_Game;
class Gra_Snake;


//==========================================
//==========================================
//==========================================
/*
    Pomysl braku kolizji przy ścianie - ja go chyba nie chce
    virtual bool ResolveWall(const Plansza& board, int& nextX, int& nextY) const {
        if (!WrapWalls()) return board.Inside_Board(nextX, nextY);

        if (nextX < 0) nextX = board.Width() - 1;
        if (nextX >= board.Width()) nextX = 0;
        if (nextY < 0) nextY = board.Height() - 1;
        if (nextY >= board.Height()) nextY = 0;
        return true;
    }
*/
//Idealny przyklad wzorca strategy - Zasady gry
class Game_Rules {
    public:
        virtual ~Game_Rules() = default;

        virtual Board_Size Get_Board_Size() const = 0;
        virtual int Turn_Time() const = 0; //Zamiast DT będziemy definiować czas na ture - ruch węża to np. 2 sekundy
};

//Na start 2 opcje z tylko 2 rozmiarami planszy - później można więcej według wzorca
class Rules_Normal : public Game_Rules {
    public:
        Board_Size Get_Board_Size() const override { return Small; }
        int Turn_Time() const override { return 1000; } //[1000 ms] = 1 sekunda
};

class Rules_Hard : public Game_Rules {
    public:
        Board_Size Get_Board_Size() const override { return Medium; }
        int Turn_Time() const override { return 500; }  //[500 ms] = 0.5 sekundy
};
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
        //Funkcja wymaga testu poniewaz uzyty nowo poznany generator losujacy
        void Spawn(const Plansza& board, const Snake& snake, std::mt19937& rng) {   //mt19937 wymaga referencji generator z algoryhtmem był jeden a nie kilka przy każdym wywołaniu 
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

        //Walidacja czy nie zmieniamy natychmiastowo kierunku na przeciwny
        static bool Is_Opposite(Kierunek A, Kierunek B) {
            return
                (A == Kierunek::Gora && B == Kierunek::Dol) ||
                (A == Kierunek::Dol && B == Kierunek::Gora) ||
                (A == Kierunek::Lewo && B == Kierunek::Prawo) ||
                (A == Kierunek::Prawo && B == Kierunek::Lewo);
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
        void Reset_Score() {Score = 0;}

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
        bool Grown_Snake = false;
    
    public:
        //Kontruktor
        Snake(int Start_X, int Start_Y, Kierunek patrze_w  = Kierunek::Gora) : Patrze_W(patrze_w) 
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
        void set_Grown(bool New_Grown) {Grown_Snake = New_Grown;}

        //Metody
        void Reset (int Start_X, int Start_Y, Kierunek patrze_w  = Kierunek::Gora) {
            Body.clear(); //Kasuje wszystkie elementy weza - czysci kontener
            Body.emplace_back(Start_X, Start_Y);  //Ustawienie weza w pozycji startowej - pierwszy element głowa pojawia się w kontenerze
            Patrze_W = patrze_w;    //Ustawia startowy kierunek weza - Gora
            Grown_Snake = false;
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
        bool Try_Set_Direction(Kierunek NewDir) {
                if (Game_Mechanic::Is_Opposite(Patrze_W, NewDir)) return false;
                Patrze_W = NewDir;
                return true;
            }

        void Grown() {set_Grown(true);}

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

        //Nowy krok czy się uda + dodaj punkt + respawn owocu - później mozna podzielic na kilka metod
        bool Step(const Plansza& board, Owoc& fruit, std::mt19937& RNG) {
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
            if (EatFruit) Grown();

            //Nowa głowa
            Body.emplace_front(nextX, nextY);

            //Test czy waz ma rosnac - czy waz sie powiekszy o element
            if (Grown_Snake) {
                Grown_Snake =false; //Rosnie i resetujemy czy ma ponownie urosnac w nastepnym ruchu
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

//==========================================
//==========================================
//==========================================

class Gra_Snake {
    private:
        std::unique_ptr<State_Game> Stan;
        //---
        std::unique_ptr<Game_Rules> Rules;
        std::unique_ptr<Plansza> Board;
        std::unique_ptr<Snake> Snake_Obj;
        std::unique_ptr<Owoc> Fruit;

        //Generator liczb pseudolosowych
        std::mt19937 RNG; //Random number generator
    
    public:
        Gra_Snake() //W konstruktorze wartość seed z random_device przekazywana jest do generatora liczb pseudolosowych
            : RNG(std::random_device{}())
        {}

        //Gettery
        Plansza* get_Board() { return Board.get(); }
        Snake* get_Snake() { return Snake_Obj.get(); }
        Owoc* get_Fruit() { return Fruit.get(); }
        Game_Rules* get_Rules() { return Rules.get(); }
    
        //Wzorzec State
        void Zmien_Stan(std::unique_ptr<State_Game> Nowy_stan) {
            Stan = std::move(Nowy_stan);
        }

        //Wzorzec strategy
        void Set_Rules(std::unique_ptr<Game_Rules> New_Rules) {
            Rules = std::move(New_Rules);
            Board = std::make_unique<Plansza>(Rules->Get_Board_Size());
        }

        void New_Game() {
            if (!Rules) Set_Rules(std::make_unique<Rules_Normal>());
            if (!Board) Board = std::make_unique<Plansza>(Rules->Get_Board_Size());
            
            int StartX = Board->Width() / 2;
            int StartY = Board->Height() / 2;

            Snake_Obj = std::make_unique<Snake>(StartX, StartY, Kierunek::Gora);
            Fruit = std::make_unique<Owoc>();
            Fruit->Spawn(*Board, *Snake_Obj, RNG);
        }

        bool Turn() {
            if (!Board || !Snake_Obj || !Fruit || !Rules) return false;
            int Next_X;
            int Next_Y;

            Game_Mechanic::Next_move(Snake_Obj->Head_X(), Snake_Obj->Head_Y(), Snake_Obj->get_Patrze_W(), Next_X, Next_Y);
            return Snake_Obj->Step(*Board, *Fruit, RNG);
        }

        void Input_Direction(Kierunek Dir) {if (Snake_Obj) Snake_Obj->Try_Set_Direction(Dir);}
};

//==========================================
//==========================================
//==========================================
//Wzorzec STATE
class State_Game { 
    public:
        virtual ~State_Game() = default;

        virtual void Use_Input(Gra_Snake& Game, int Key) = 0; //Zgodnie w mysl ten sam klawisz ale inne mozliwe zachowanie wedlug state
        virtual void Update(Gra_Snake& Game) = 0;
};


//Stany dla wzorca STATE
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
    void Use_Input(Gra_Snake& Game, int Key) override {
        if (Key == Key_Enter) Game.New_Game(); //+ game.Zmien_Stan(std::make_unique<State_Gra>());
        if (Key == Key_Esc) { } //Wyjscie z gry
    }

    void Update(Gra_Snake&) override {

    }
};
class State_Opcje : public State_Game {
    public:
    /*Implementacja Update*/
    /*
    Rozdzielczosc
    Audio
    Powrót do menu
    */
};

class State_Credits : public State_Game {
    public:
    /*Implementacja Update*/
    /*
    Autor
    Licencja Godot
    */
};

class State_Error : public State_Game {
    public:
    //Error
};
class State_Gra : public State_Game {
    public:
    /*Implementacja Update*/
    /*
    1 czy 2 snake
    Rysuj plansze
    <GRAMY>
    */
    //Sterowanie
    void Use_Input(Gra_Snake& Game, int Key) override {
        if (Key == Key_W) Game.Input_Direction(Kierunek::Gora);
        if (Key == Key_S) Game.Input_Direction(Kierunek::Dol);
        if (Key == Key_A) Game.Input_Direction(Kierunek::Lewo);
        if (Key == Key_D) Game.Input_Direction(Kierunek::Prawo);
        if (Key == Key_Esc) {
            // game.Zmien_Stan(std::make_unique<State_Menu>());
        }  
    }  

    void Update(Gra_Snake& Game) override {
        bool ok = Game.Turn();
        if (!ok) {
            // game.Zmien_Stan(std::make_unique<State_Error>());
        }
    }
};
class State_Quit : public State_Game {
    public:
    /*Implementacja Update*/
    //Wylacz
};