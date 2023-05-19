#pragma once

#include <array>

using namespace std;

namespace Display {

    void initialize();
    void preUpdate();
    void postUpdate();

    const array<int, 2>& getSize();

}
