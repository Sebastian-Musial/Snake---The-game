#include <iostream>
#include <memory> //Potrzebny do wzorców
#include <vector>
#include <deque>   //Kontener dla elementow weza
#include <random> //Dla generatora pseudo losowych liczb

#pragma once

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