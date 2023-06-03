#pragma once

#include "game/tile.h"

class TileFloating : public Tile {

private:
    Coordf visLocation;

public:

    TileFloating();

    void update(float delta, const Coordf &locationGroup, const Coordi &indexGroup);

    Coordf getVisLocation(const Coordf &locationGroup, const Coordi &indexGroup) override;

};
