#pragma once
#include <string>
#include <vector>
#include "player.h"
//45.131.3.105
//193.164.6.107
//213.142.151.12
class world {
   public:
    std::string name{};
    std::vector<player> players{};
    player local{};
    bool connected{};
};
