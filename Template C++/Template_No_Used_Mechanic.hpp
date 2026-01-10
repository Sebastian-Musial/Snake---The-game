#pragma once

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