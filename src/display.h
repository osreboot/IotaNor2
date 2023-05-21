#pragma once

#include <utility>

using namespace std;

namespace display {

    void initialize();
    void preUpdate();
    void postUpdate();
    void close();

    bool isExiting();

    const pair<int, int>& getSize();

}
