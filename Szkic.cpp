#include <iostream>
#include <memory> //Potrzebny do wzorców
#include <vector>
#include <deque>   //Kontener dla elementow weza
#include <random> //Dla generatora pseudo losowych liczb

enum class Directions {
    Up,
    Down,
    Right,
    Left
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
class Board;
class Fruit;
class Game_Rules;
class State_Game;
class Game_Snake;


//==========================================
//==========================================
//==========================================
/*
    Pomysl braku kolizji przy ścianie - ja go chyba nie chce
    virtual bool ResolveWall(const Board& board, int& nextX, int& nextY) const {
        if (!WrapWalls()) return board.Inside_Board(nextX, nextY);

        if (nextX < 0) nextX = board.Width() - 1;
        if (nextX >= board.Width()) nextX = 0;
        if (nextY < 0) nextY = board.Height() - 1;
        if (nextY >= board.Height()) nextY = 0;
        return true;
    }
*/

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
class State_Gra : public State_Game {
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