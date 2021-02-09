#pragma once
#include <string>
#include "packet.h"

namespace gt {
    extern std::string version;
    extern std::string flag;
    extern std::string macaddr;

    extern bool resolving_uid2;
    extern bool connecting;
    extern bool aapbypass;
    extern bool resolving_uid;
    extern bool resolving_uid2;
    extern bool in_game;
    extern bool ghost;
    void resolve_uid_to_name(std::string text);
    void send_log(std::string text);
    void solve_captcha(std::string text);
}
