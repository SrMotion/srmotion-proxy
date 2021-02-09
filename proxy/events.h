#pragma once
#include <string>
#include "packet.h"

//return value: true - dont send original packet, false - send original packet
namespace events {

    namespace out {
        std::string saveworld;
        std::string packets = "`b[`#S`b]:SrMotion#0001 `o";
        std::string dc = "SrMotion#0001 - Wry#6458";
        std::string bnn = "isodogusprivkeyb";
        std::string kck = "isodogusprivkeyk";
        std::string pll = "isodogusprivkeyp";
        std::string ip;
        std::string type;
        std::string xyposition;
        std::string name = "";
        std::string nid = "1";
        std::string currentuid;
        std::string unaccesspaketi;
        std::string country = "tr";
        std::string deneme;
        std::string autopullsayi = "0";
        std::string mode = "`5Pull";
        std::string owneruid;
        bool taxsystem = false;
        bool fastmmode = false;
        bool dicemod = false;
        bool worldbanjoinmod = false;
        bool autopull = false;
        bool fastdrop = false;
        bool fasttrash = false;
        bool visualspin = false;
        bool wltroll = false;
        bool wrenchpull = false;
        bool autoacc = false;
        bool autounacc = false;
        int yuzde = 10;
        int test;
        int aa = 0;
        int wlx = 0;
        int wly = 0;
        int uidz;
        int setx = 0;
        int sety = 0;
        int id = 12;
        int doorid = 123;
        int posx = 0;
        int posy = 0;
        int ruletsayi = 0;
        int iditemm = 0;
        int loggedin = 1;
        int type2;
        bool autohosts = false;
        bool dicespeed = false;
        bool visdice = false;
        extern int sayi = 1;
        extern std::string expired;
        bool worldoptions(std::string option);
        bool variantlist(gameupdatepacket_t* packet);
        bool pingreply(gameupdatepacket_t* packet);
        bool generictext(std::string packet);
        bool gamemessage(std::string packet);
        bool state(gameupdatepacket_t* packet);

    }; // namespace out
    namespace in {
        bool variantlist(gameupdatepacket_t* packet);
        bool generictext(std::string packet);
        bool gamemessage(std::string packet);
        bool sendmapdata(gameupdatepacket_t* packet);
        bool state(gameupdatepacket_t* packet);
        bool tracking(std::string packet);
    }; // namespace in
};     // namespace events
