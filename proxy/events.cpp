#include "events.h"
#include <iostream>
#include "gt.hpp"
#include "proton/hash.hpp"
#include "proton/rtparam.hpp"
#include "proton/variant.hpp"
#include "server.h"
#include "utils.h"
#include "print.h"
#include <fstream>
#include <chrono>
#include <thread>
#include <future>
#include "dialog.h"
#include "math.h"
#include "HTTPRequest.hpp"

using namespace events::out;
using namespace std;

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

bool events::out::variantlist(gameupdatepacket_t* packet) {
    variantlist_t varlist{};
    varlist.serialize_from_mem(utils::get_extended(packet));
    PRINTS("varlist: %s\n", varlist.print().c_str());
    return false;
}
bool events::out::pingreply(gameupdatepacket_t* packet) {
    //since this is a pointer we do not need to copy memory manually again
    packet->m_vec2_x = 1000.f;  //gravity
    packet->m_vec2_y = 250.f;   //move speed
    packet->m_vec_x = 64.f;     //punch range
    packet->m_vec_y = 64.f;     //build range
    packet->m_jump_amount = 0;  //for example unlim jumps set it to high which causes ban
    packet->m_player_flags = 0; //effect flags. good to have as 0 if using mod noclip, or etc.
    return false;
}
bool find_command(std::string chat, std::string name) {
    bool found = chat.find("/" + name) == 0;
    if (found)
        gt::send_log("`6" + chat);
    return found;
}
bool events::out::worldoptions(std::string option)
{
    std::string username = "all";
    for (auto& player : g_server->m_world.players) {
        auto name_2 = player.name.substr(2); //remove color
        std::transform(name_2.begin(), name_2.end(), name_2.begin(), ::tolower);
        if (name_2.find(username)) {
            auto& bruh = g_server->m_world.local;
            if (option == "pull")
            {
                string plyr = player.name.substr(2).substr(0, player.name.length() - 4);
                if (plyr != bruh.name.substr(2).substr(0, player.name.length() - 4))
                {
                    g_server->send(false, "action|input\n|text|/pull " + plyr);
                }
            }
            if (option == "kick")
            {
                string plyr = player.name.substr(2).substr(0, player.name.length() - 4);
                if (plyr != bruh.name.substr(2).substr(0, player.name.length() - 4))
                {
                    g_server->send(false, "action|input\n|text|/kick " + plyr);

                }
            }
            if (option == "ban")
            {
                string plyr = player.name.substr(2).substr(0, player.name.length() - 4);
                if (plyr != bruh.name.substr(2).substr(0, player.name.length() - 4))
                {
                    g_server->send(false, "action|input\n|text|/ban " + plyr);

                }
            }
        }
    }

    return true;
}
bool events::out::generictext(std::string packet) {

    PRINTS("Generic text: %s\n", packet.c_str());
    auto& world = g_server->m_world;
    rtvar var = rtvar::parse(packet);
    if (!var.valid())
        return false;
    if (packet.find("roulette2|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("te2|") + 4, packet.size());
            std::string number = aaa.c_str();
            autopull = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("fastdrop|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("rop|") + 4, packet.size());
            std::string number = aaa.c_str();
            fastdrop = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("fasttrash|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("ash|") + 4, packet.size());
            std::string number = aaa.c_str();
            fasttrash = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");
        }
    }
    if (packet.find("visualspin|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("pin|") + 4, packet.size());
            std::string number = aaa.c_str();
            visualspin = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");
        }
    }
    if (packet.find("fastmode|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("ode|") + 4, packet.size());
            std::string number = aaa.c_str();
            fastmmode = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");
        }
    }

    if (packet.find("autotax|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("tax|") + 4, packet.size());
            std::string number = aaa.c_str();
            taxsystem = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("wltroll1|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("ll1|") + 4, packet.size());
            std::string number = aaa.c_str();
            wltroll = stoi(number);
            if (wltroll == true)
            {
                gt::send_log("`9worldlock`` trolling mode is now `2on");
            }
            else
            {
                gt::send_log("`9worldlock`` trolling mode is now `4off");
            }
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }

    if (packet.find("taxamount|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("unt|") + 4, packet.size());
            std::string number = aaa.c_str();
            yuzde = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("roulette5") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("te5|") + 4, packet.size());
            std::string number = aaa.c_str();
            while (!number.empty() && isspace(number[number.size() - 1]))
                number.erase(number.end() - (76 - 0x4B));
            ruletsayi = std::stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("dicespeed|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("eed|") + 4, packet.size());
            std::string number = aaa.c_str();
            events::out::dicespeed = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");
        }
    }
    if (packet.find("worldbanmod|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("mod|") + 4, packet.size());
            std::string number = aaa.c_str();
            worldbanjoinmod = stoi(number);
            if (worldbanjoinmod == true)
            {
                gt::send_log("`9Successfuly started checking entered for `#@Moderators `9and `#@Guardians`9, please wait...");
            }
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("wrenchpull|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("ull|") + 4, packet.size());
            std::string number = aaa.c_str();
            while (!number.empty() && isspace(number[number.size() - 1]))
                number.erase(number.end() - (76 - 0x4B));
            wrenchpull = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("autohosts|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("sts|") + 4, packet.size());
            std::string number = aaa.c_str();
            events::out::autohosts = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("countryzz|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("yzz|") + 4, packet.size());
            while (!aaa.empty() && isspace(aaa[aaa.size() - 1]))
                aaa.erase(aaa.end() - (76 - 0x4B));
            variantlist_t varlist{ "OnTextOverlay" };
            if (packet.find("buttonClicked|countrylist") != -1)
            {
                std::string paket;
                paket =
                    "\nadd_label_with_icon|big|Country List|left|3394|"
                    "\nadd_spacer|small"
                    "\nadd_textbox|`otr: Turkey|left|2480|"
                    "\nadd_textbox|`oen: USA|left|2480|"
                    "\nadd_textbox|`okr: Korean|left|2480|"
                    "\nadd_textbox|`oid: Indonesia|left|2480|"
                    "\nadd_textbox|`oaf: Afghanistan|left|2480|"
                    "\nadd_textbox|`oal: Albania|left|2480|"
                    "\nadd_textbox|`odz: Algeria|left|2480|"
                    "\nadd_textbox|`oas: American Samoa|left|2480|"
                    "\nadd_textbox|`oad: Andorra|left|2480|"
                    "\nadd_textbox|`oao: Angola|left|2480|"
                    "\nadd_textbox|`oai: Anguilla|left|2480|"
                    "\nadd_textbox|`oaq: Antarctica|left|2480|"
                    "\nadd_textbox|`oag: Antigua and Barbuda|left|2480|"
                    "\nadd_textbox|`oar: Argentina|left|2480|"
                    "\nadd_textbox|`oam: Armenia|left|2480|"
                    "\nadd_textbox|`oth: Thailand|left|2480|"
                    "\nadd_textbox|`oes: Spain|left|2480|"
                    "\nadd_textbox|`oso: Somalia|left|2480|"
                    "\nadd_textbox|`ose: Sweden|left|2480|"
                    "\nadd_quick_exit|"
                    "\nend_dialog|end|Cancel|Okay|";
                variantlist_t liste{ "OnDialogRequest" };
                liste[1] = paket;
                g_server->send(true, liste);
            }
            else
            {
                country = aaa.c_str();

                varlist[1] = "Your country Successfuly changed!, Disconnecting";
                g_server->send(true, varlist);
                gt::flag = country;
                gt::send_log("Disconnected?! Will attempt to reconnect..");//lolbruh
                g_server->disconnectsr(true);
            }
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }

    if (packet.find("buttonClicked|optionzzz") != -1)
    {
        try
        {
            Dialog a;
            a.addLabelWithIcon("Options Page", 758, LABEL_BIG);
            a.addSpacer(SPACER_SMALL);
            a.addCheckbox("roulette2", "Enable Auto Pull", autopull);
            a.addCheckbox("wrenchpull", "Enable Wrench " + mode, wrenchpull);
            a.addCheckbox("fastdrop", "Enable Fast Drop", fastdrop);
            a.addCheckbox("fasttrash", "Enable Fast Trash", fasttrash);
            a.addCheckbox("dicespeed", "Enable Dice Speed", events::out::dicespeed);
            a.addCheckbox("worldbanmod", "Exit world when mod joins", worldbanjoinmod);
            a.addCheckbox("visualspin", "Enable Visual Spin", visualspin);
            a.addCheckbox("autohosts", "Enable Show X,Y Position", autohosts);
            a.addCheckbox("wltroll1", "Enable World Lock Troll", wltroll);
            a.addInputBox("roulette5", "Number:", to_string(ruletsayi), 2);
            a.addInputBox("saveworld", "Save World:", saveworld, 12);
            a.endDialog("end","Okey","Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("xvault111|") != -1) {
        std::string aaa = packet.substr(packet.find("111|") + 4, packet.size());
        try
        {
            posx = std::stoi(aaa.c_str());
            gt::send_log("posx: " + std::to_string(posx));
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `9Please enter numbers only.");

        }

    }
    if (packet.find("saveworld|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("rld|") + 4, packet.size());
            while (!aaa.empty() && isspace(aaa[aaa.size() - 1]))
                aaa.erase(aaa.end() - (76 - 0x4B));
            events::out::saveworld = aaa;
        }
        catch (exception a)
        {

        }
    }
    if (packet.find("2vault222|") != -1) {
        std::string aaa = packet.substr(packet.find("222|") + 4, packet.size());
        while (!aaa.empty() && isspace(aaa[aaa.size() - 1]))
            aaa.erase(aaa.end() - (76 - 0x4B));
        try
        {
            posy = std::stoi(aaa.c_str());
            gt::send_log("posy: " + std::to_string(posy));
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `9Please enter numbers only.");
        }

    }
    if (packet.find("buttonClicked|iso11pul1") != -1) {
        mode = "`5Pull";//bannj

    }
    if (packet.find("buttonClicked|bannj") != -1) {

        mode = "`4Ban";

    }
    if (packet.find("buttonClicked|kicj") != -1) {

        mode = "`4Kick";

    }
    if (packet.find("iditemzz1|") != -1) {
        std::string aaa = packet.substr(packet.find("zz1|") + 4, packet.size());
        while (!aaa.empty() && isspace(aaa[aaa.size() - 1]))
            aaa.erase(aaa.end() - (76 - 0x4B));
        std::string number = aaa.c_str();
        try
        {
            iditemm = std::stoi(aaa.c_str());

        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `9Please enter numbers only.");

        }
    }
    if (packet.find("buttonClicked|killall") != -1)
    {
        worldoptions("kick");

    }
    if (packet.find("buttonClicked|banall") != -1)
    {

        worldoptions("ban");

    }
    if (packet.find("buttonClicked|pullall") != -1)
    {
        worldoptions("pull");
    }
    if (packet.find("buttonClicked|ubaworld") != -1)
    {
        g_server->send(false, "action|input\n|text|/uba");

    }
    if (wrenchpull == true) {
        if (packet.find("action|wrench") != -1) {
            g_server->send(false, packet);

            std::string str = packet.substr(packet.find("netid|") + 6, packet.length() - packet.find("netid|") - 1);
            std::string gta5 = str.substr(0, str.find("|"));
            if (mode == "`5Pull")
            {
                g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + gta5 + "|\nnetID|" + gta5 + "|\nbuttonClicked|pull");

            }
            if (mode == "`4Kick")
            {
                g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + gta5 + "|\nnetID|" + gta5 + "|\nbuttonClicked|kick");

            }
            if (mode == "`4Ban")
            {
                g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + gta5 + "|\nnetID|" + gta5 + "|\nbuttonClicked|worldban");

            }
            variantlist_t varlist{ "OnTextOverlay" };
            varlist[1] = "Successfuly " + mode + "`` netID: " + gta5;
            g_server->send(true, varlist);
            return true;
        }
    }
    if (var.get(0).m_key == "action" && var.get(0).m_value == "input") {
        if (var.size() < 2)
            return false;
        if (var.get(1).m_values.size() < 2)
            return false;

        if (!world.connected)
            return false;

        auto chat = var.get(1).m_values[1];

        if (find_command(chat, "name ")) { //ghetto solution, but too lazy to make a framework for commands.
            std::string name = "``" + chat.substr(6) + "``";
            variantlist_t va{ "OnNameChanged" };
            va[1] = name;
            g_server->send(true, va, world.local.netid, -1);
            gt::send_log("name set to: " + name);
            return true;
        }
        else if (find_command(chat, "flag ")) {
            int flag = atoi(chat.substr(6).c_str());
            variantlist_t va{ "OnGuildDataChanged" };
            va[1] = 1;
            va[2] = 2;
            va[3] = flag;
            va[4] = 3;
            g_server->send(true, va, world.local.netid, -1);
            gt::send_log("flag set to item id: " + std::to_string(flag));
            return true;
        }
        else if (find_command(chat, "ghost")) {
            gt::ghost = !gt::ghost;
            if (gt::ghost)
                gt::send_log("Ghost is now enabled.");
            else
                gt::send_log("Ghost is now disabled.");
            return true;
        }
        else if (find_command(chat, "casino")) {
            Dialog casino;
            casino.addLabelWithIcon("Auto Tax System", 758, LABEL_BIG);
            casino.addSpacer(SPACER_SMALL);
            if (taxsystem == true)
            {
                casino.addCheckbox("autotax", "Enable Auto Tax", CHECKBOX_SELECTED);
            }
            else
            {
                casino.addCheckbox("autotax", "Enable Auto Tax", CHECKBOX_NOT_SELECTED);

            }
            if (fastmmode == true)
            {
                casino.addCheckbox("fastmode", "Fast Drop Mode", CHECKBOX_SELECTED);
            }
            else
            {
                casino.addCheckbox("fastmode", "Fast Drop Mode", CHECKBOX_NOT_SELECTED);

            }
            casino.addInputBox("taxamount", "Tax Amount: %", std::to_string(yuzde), 2);
            casino.addQuickExit();
            casino.endDialog("end", "OK", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = casino.finishDialog();
            g_server->send(true, liste);
            return true;
        }
        else if (find_command(chat, "options")) {
            Dialog a;
            a.addLabelWithIcon("Options Page", 758, LABEL_BIG);
            a.addSpacer(SPACER_SMALL);
            a.addCheckbox("roulette2", "Enable Auto Pull", autopull);
            a.addCheckbox("wrenchpull", "Enable Wrench " + mode, wrenchpull);
            a.addCheckbox("fastdrop", "Enable Fast Drop", fastdrop);
            a.addCheckbox("fasttrash", "Enable Fast Trash", fasttrash);
            a.addCheckbox("dicespeed", "Enable Dice Speed", events::out::dicespeed);
            a.addCheckbox("worldbanmod", "Exit world when mod joins", worldbanjoinmod);
            a.addCheckbox("visualspin", "Enable Visual Spin", visualspin);
            a.addCheckbox("autohosts", "Enable Show X,Y Position", autohosts);
            a.addCheckbox("wltroll1", "Enable World Lock Troll", wltroll);
            a.addInputBox("roulette5", "Number:", to_string(ruletsayi), 2);
            a.addInputBox("saveworld", "Save World:", saveworld, 12);
            a.endDialog("end", "Okey", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);

            return true;
        }
        else if (find_command(chat, "account")) {
            std::string paket;
            paket =
                "add_label_with_icon|big|Account Page|left|2480| \nadd_spacer|small\n"
                "add_textbox|`oAccount Expire Time:"
                " Never|left|\n"
                "add_textbox|`oAccount Type:"
                " " + type + "|left|\n"
                "add_textbox|`oLastest Logged IP:"
                " " + ip + "|left|\n"
                "add_spacer|small|\n"
                "\nadd_button|manageacc|`wManage Account``|noflags|0|0|"
                "\nend_dialog|end|Cancel|Okay|";
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = paket;
            g_server->send(true, liste);
            return true;
        }
        else if (find_command(chat, "wrench")) {
            std::string paket;
            paket =
                "add_label_with_icon|big|Wrench Mode: " + mode + "|left|2480| \nadd_spacer|small\n"
                "\nadd_button|kicj|`4Kick``|noflags|0|0|"
                "\nadd_button|bannj|`4Ban``|noflags|0|0|"
                "\nadd_button|iso11pul1|`5Pull``|noflags|0|0|"
                "\nend_dialog|end|Cancel|Okay|";
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = paket;
            g_server->send(true, liste);
            return true;
        }
        if (autopull == true) {
            if (packet.find("/options") != -1) {
            }
            else {
                if (packet.find("text|") != -1) {
                    std::string str = packet.substr(packet.find("text|") + 5, packet.length() - packet.find("text|") - 1);
                    g_server->send(false, "action|input\n|text|/pull " + str);
                    return true;
                }
            }
            return true;
        }
        else if (find_command(chat, "country")) {
            std::string paket;
            paket =
                "add_label_with_icon|big|Country|left|3394| \nadd_spacer|small\n"
                "\nadd_text_input|countryzz|CountryID|" +
                country +
                "|2|"
                "\nadd_button|countrylist|`wCountry List``|noflags|0|0|"
                "\nadd_quick_exit|"
                "\nend_dialog|end|Cancel|Okay|";
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = paket;
            g_server->send(true, liste);

            return true;
        }
        else if (find_command(chat, "server")) {
            std::string paket;
            paket =
                "add_label_with_icon|big|Server Settings|left|32|\nadd_spacer|small\n"
                "\nadd_text_input|serverip|Server IP|" +
                g_server->m_server +
                "|15|"
                "\nadd_text_input|serverport|Server Port|" +
                std::to_string(g_server->m_port) +
                "|5|"
                "\nend_dialog|end|Cancel|Okay|";
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = paket;
            g_server->send(true, liste);

            return true;
        }
        else if (find_command(chat, "vault")) {

            std::string paket;
            paket =
                "add_label_with_icon|big|Safe Vault Bypass|left|8878| \nadd_spacer|small\n"
                "\nadd_text_input|xvault111|PosX|" + std::to_string(posx) + "|3|"
                "\nadd_text_input|2vault222|PosY|" + std::to_string(posy) + "|3|"
                "\nadd_text_input|iditemzz1|ItemID|" + std::to_string(iditemm) + "|6|"
                "\nadd_spacer|small"
                "\nend_dialog|end|Cancel|Set|";
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = paket;
            g_server->send(true, liste);
            return true;
        }

        else if (find_command(chat, "bypassvault")) {
            g_server->send(false, "action|dialog_return\ndialog_name|storageboxxtreme\ntilex|" + std::to_string(posx) + "|\ntiley|" + std::to_string(posy) + "|\nitemid|" + std::to_string(iditemm) + "|\nbuttonClicked|do_add\nitemcount|1");
            return true;
        }
        else if (find_command(chat, "setuid ")) {
            string chateyazilan = chat.substr(7);
            if (chateyazilan.find("owner") != -1)
            {
                uidz = stoi(owneruid);
                gt::send_log("`9Target UID Successfuly set to owner uid `#" + to_string(uidz));
                return true;
            }
            if (chateyazilan.find("current") != -1 || chateyazilan.find("Current") != -1 || chateyazilan.find("CURRENT") != -1)
            {
                uidz = stoi(currentuid);
                gt::send_log("`9Target UID Successfuly set to current uid `#" + to_string(uidz));
            }
            else
            {
                try
                {
                    uidz = stoi(chat.substr(7));
                    gt::send_log("`9Target UID Successfuly set to `#" + to_string(uidz));
                }
                catch (exception a)
                {
                    gt::send_log("`4Critical Error: `9Please enter numbers only.");
                }


            }
            return true;
        }
        else if (find_command(chat, "wlt")) {
            if (wltroll == false)
            {
                wltroll = true;
                gt::send_log("`9worldlock`` trolling mode is now `2on");
            }
            else
            {
                gt::send_log("`9worldlock`` trolling mode is now `4off");
                wltroll = false;
            }
            return true;
        }
        else if (find_command(chat, "dicemode")) {
            if (dicemod == false)
            {
                dicemod = true;
                gt::send_log("`9AntiSpeed`` mode is now `2on");
            }
            else
            {
                gt::send_log("`9AntiSpeed`` mode is now `4off");
                dicemod = false;
            }
            return true;
        }
        else if (find_command(chat, "crash")) {
            if (events::out::type2 > 2)
            {
                if (world.name.find("ACE") != -1)
                {
                    gt::send_log("`9You cant do in this world sorry :( if you have a problem pm " + dc);
                    return true;
                }
                else
                {
                    gt::send_log("`9Please use VPN For doesn't get `#SHADOW `4BAN!");
                    g_server->send(false, "action|input\n|text|`eCrashed by " + dc);
                    //for (auto& player : g_server->m_world.players) {
                    //    gameupdatepacket_t packet{ 0 };
                    //    packet.m_type = PACKET_SET_ICON_STATE;
                    //    packet.m_packet_flags = 8;
                    //    packet.m_int_data = utils::random(0, 2);
                    //    packet.m_vec2_x = utils::random(0, 2);
                    //    packet.m_vec_x = utils::random(0, 2);
                    //    packet.m_netid = player.netid;
                    //    for (int i = 0; i < 999; i++) {
                    //        g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                    //    }
                    //   
                    //}
                }
            }
            else
            {
                gt::send_log("`9You dont have permissions. You need `4Buyer+`` Role.");
                return true;

            }
        }
        else if (find_command(chat, "whitelist")) {
            Dialog whitelist;
            whitelist.addLabelWithIcon("Whitelist", 3394, LABEL_BIG);
            whitelist.addTextBox("Whitelisted Player List");
            whitelist.addQuickExit();
            whitelist.endDialog("end", "Okay", "Close");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = whitelist.finishDialog();
            g_server->send(true, liste);
            return true;
        }
        else if (find_command(chat, "tp ")) {
            std::string name = chat.substr(4);
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            for (auto& player : g_server->m_world.players) {
                auto name_2 = player.name.substr(2); //remove color
                std::transform(name_2.begin(), name_2.end(), name_2.begin(), ::tolower);
                if (name_2.find(name) == 0) {
                    gt::send_log("Teleporting to " + player.name);
                    variantlist_t varlist{ "OnSetPos" };
                    varlist[1] = player.pos;
                    g_server->m_world.local.pos = player.pos;
                    g_server->send(true, varlist, g_server->m_world.local.netid, -1);
                    break;
                }
            }
            return true;
        }
        else if (find_command(chat, "logs")) {
            Dialog Logs;
            Logs.addLabelWithIcon("Logs Page", 5016, LABEL_BIG);
            Logs.addSpacer(SPACER_SMALL);
            Logs.addLabelWithIcon("Hacks", 32, LABEL_SMALL);
            if (gt::ghost == true)
            {
                Logs.addTextBox("Ghost Mode``: `2ON");

            }
            else
            {
                Logs.addTextBox("Ghost Mode``: `4OFF");

            }
            if (events::out::dicespeed == true)
            {
                Logs.addTextBox("Dice Speed``: `2ON");

            }
            else
            {
                Logs.addTextBox("Dice Speed``: `4OFF");

            }
            Logs.addLabelWithIcon("Door Bypass", 12, LABEL_SMALL);
            Logs.addTextBox("The current `2tileX`` position is `2" + std::to_string(setx));
            Logs.addTextBox("The current `2tileY`` position is `2" + std::to_string(sety));
            Logs.addTextBox("The current `2DoorID`` is `2" + std::to_string(id));
            Logs.addLabelWithIcon("Safe Vault", 8878, LABEL_SMALL);
            Logs.addTextBox("The current `2tileX`` position is `2" + std::to_string(posx));
            Logs.addTextBox("The current `2tileY`` position is `2" + std::to_string(posy));
            Logs.addTextBox("The current `2ItemID`` is `2" + std::to_string(iditemm));
            Logs.addLabelWithIcon("Wrench Logs", 32, LABEL_SMALL);
            if (wrenchpull == true)
            {
                Logs.addTextBox("The `5Wrench Opinions`` is `2ON");
            }
            else
            {
                Logs.addTextBox("The `5Wrench Opinions`` is `4OFF");

            }
            Logs.addTextBox("The Current `2Wrench Mode ``is " + mode + "``");
            Logs.addLabelWithIcon("Tax Mode Options", 758, LABEL_SMALL);
            if (taxsystem == true)
            {
                Logs.addTextBox("The `2Tax System`` is `2ON");
                Logs.addTextBox("The current tax is `2%" + std::to_string(yuzde));
            }
            else
            {
                Logs.addTextBox("The `2Tax System`` is `2OFF");
                Logs.addTextBox("The current tax is `2%" + std::to_string(yuzde));
            }
            Logs.addLabelWithIcon("Casino Options", 758, LABEL_SMALL);
            if (worldbanjoinmod == true)
            {
                Logs.addTextBox("The `2Exit world when mod joins `2ON");

            }
            else
            {
                Logs.addTextBox("The `2Exit world when mod joins `4OFF");

            }
            if (visualspin == true)
            {
                Logs.addTextBox("The `2Visual Spin`` is `2ON");
                Logs.addTextBox("The `2Visual Spin`` Number is `4" + std::to_string(ruletsayi));

            }
            else
            {
                Logs.addTextBox("The `2Visual Spin`` is `4OFF");
                if (ruletsayi == 0) { Logs.addTextBox("The `2Visual Spin`` Number is `2" + std::to_string(ruletsayi)); }
                else
                    Logs.addTextBox("The `2Visual Spin`` Number is `4" + std::to_string(ruletsayi));

            }
            Logs.addLabelWithIcon("Skip Dialogs", 18, LABEL_SMALL);
            if (fasttrash == true)
            {
                Logs.addTextBox("The `2Fast Trash`` is `2ON");
            }
            else
            {
                Logs.addTextBox("The `2Fast Trash`` is `4OFF");

            }
            if (fastdrop == true)
            {
                Logs.addTextBox("The `2Fast Drop`` is `2ON");
            }
            else
            {
                Logs.addTextBox("The `2Fast Drop`` is `4OFF");

            }
            if (autopull == true)
            {
                Logs.addTextBox("The `2Auto Pull`` is `2ON");
            }
            else
            {
                Logs.addTextBox("The `2Auto Pull`` is `4OFF");

            }
            Logs.addLabelWithIcon("Visuals", 3394, LABEL_SMALL);
            Logs.addTextBox("The current `2Country`` is `4" + country);

            Logs.endDialog("end", "OK", "");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = Logs.finishDialog();
            g_server->send(true, liste);
            return true;
        }
        else if (find_command(chat, "setx ")) {
            if (events::out::type2 > 2)
            {
                try
                {
                    setx = std::stoi(chat.substr(6));
                    gt::send_log("Set tileX to `5" + std::to_string(setx) + "`` successfuly");
                }
                catch (exception a)
                {
                    gt::send_log("`4Critical Error: `9Please enter numbers only.");
                }
            }
            return true;
        }
        else if (find_command(chat, "sety ")) {
            if (events::out::type2 > 2)
            {
                try
                {
                    sety = std::stoi(chat.substr(6));
                    gt::send_log("Set tileY to `5" + std::to_string(sety) + "`` successfuly");
                }
                catch (exception a)
                {
                    gt::send_log("`4Critical Error: `9Please enter numbers only.");

                }
            }

            return true;
        }
        else if (find_command(chat, "setid ")) {
            if (events::out::type2 > 2)
            {
                try
                {
                    id = std::stoi(chat.substr(6));
                    gt::send_log("Set DoorID to `5" + std::to_string(id) + "`` successfuly");
                }
                catch (exception a)
                {
                    gt::send_log("`4Critical Error: `9Please enter numbers only.");

                }
            }
            return true;
        }
        else if (find_command(chat, "bypassdoor")) {
            if (events::out::type2 > 2)
            {
                g_server->send(false, "action|dialog_return\ndialog_name|door_edit\ntilex|" + std::to_string(setx) + "|\ntiley|" + std::to_string(sety) + "|\ndoor_name|SrMotion\ndoor_target|SrMotion:123\ndoor_id|" + std::to_string(id) + "\ncheckbox_locked|1");
                gt::send_log("Hacking the door of ID of tileX `5" + std::to_string(setx) + " ``and tileY `5" + std::to_string(sety));
                gt::send_log("Please do /logs to check your `2ID");
            }

            return true;
        }
        else if (find_command(chat, "bypasspath")) {
            if (events::out::type2 > 2)
            {
                g_server->send(false, "action|dialog_return\ndialog_name|sign_edit\ntilex|" + std::to_string(setx) + "|\ntiley|" + std::to_string(sety) + "|\nsign_text|" + std::to_string(id));
                gt::send_log("Hacking the door of ID of tileX `5" + std::to_string(setx) + " ``and tileY `5" + std::to_string(sety));
                gt::send_log("Please do /logs to check your `2ID");
            }
            return true;
        }
        else if (find_command(chat, "visdice"))
        {
            if (events::out::visdice == false)
            {
                events::out::visdice = true;
                gt::send_log("`9Dice Visual Mode `#Enabled");
            }
            else
            {
                events::out::visdice = false;
                gt::send_log("`9Dice Visual Mode `4Disabled");

            }
            return true;
        }
        else if (find_command(chat, "dset "))
        {
            try
            {
                events::out::sayi = std::stoi(chat.substr(5)) - 1;
                gt::send_log("`9Number set to `#" + chat.substr(5));
            }
            catch (std::exception)
            {
                gt::send_log("Critical error: use number");
            }
            return true;
        }
        else if (find_command(chat, "save")) {

            g_server->send(false, "action|join_request\nname|" + saveworld, 3);
            return true;
        }
        else if (find_command(chat, "warp ")) {
            g_server->send(false, "action|join_request\nname|" + chat.substr(5), 3);
            gt::send_log("`9Warping to `#" + chat.substr(5));
            return true;
        }
        else if (find_command(chat, "skin ")) {

            int skin = atoi(chat.substr(5).c_str());
            variantlist_t va{ "OnChangeSkin" };
            va[1] = skin;
            g_server->send(true, va, world.local.netid, -1);
            return true;
        }

        else if (find_command(chat, "uid ")) {
            std::string name = chat.substr(5);
            gt::send_log("resolving uid for " + name);
            g_server->send(false, "action|input\n|text|/ignore /" + name);
            g_server->send(false, "action|friends");
            gt::resolving_uid2 = true;
            return true;
        }
        else if (find_command(chat, "dropentire"))
        {
        //deleted for private reasons ;)
            return true;
        }
        else if (find_command(chat, "world")) {

            std::string paket;
            paket =
                "\nadd_label_with_icon|big|World Commands|left|32|"
                "\nadd_spacer|small"
                "\nadd_button|killall|`4Kill All``|noflags|0|0|"
                "\nadd_button|banall|`4Ban All``|noflags|0|0|"
                "\nadd_button|pullall|`5Pull All``|noflags|0|0|"
                "\nadd_button|ubaworld|`5Unban World``|noflags|0|0|"
                "\nadd_textbox|`4IMPORTANT: `oDiscord: SrMotion#0069.|left|2480|"
                "\nadd_quick_exit|"
                "\nend_dialog|end|Cancel|Okay|";
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = paket;
            g_server->send(true, liste);
            return true;
        }
        else if (find_command(chat, "proxy")) {
            std::string paket;
            paket =
                "\nadd_label_with_icon|big|Proxy Commands|left|32|"
                "\nadd_spacer|small"
                "\nadd_textbox|`9/proxy `#(shows commands)|left|2480|"
                "\nadd_textbox|`9/world `#(shows commands)|left|2480|"
                "\nadd_textbox|`9/bypassvault `#(bypassing safe vault)|left|2480|"
                "\nadd_textbox|`9/bypassdoor `#(bypassing the doorid /setid (set door id))|left|2480|"
                "\nadd_textbox|`9/bypasspath `#(bypassing the path marker)|left|2480|"
                "\nadd_textbox|`9/tp [name] `#(teleports to a player in the world) |left|2480|"
                "\nadd_textbox|`9/ghost `#(toggles ghost, you wont move for others when its enabled)|left|2480|"
                "\nadd_textbox|`9/uid [name] `#(resolves name to uid)|left|2480|"
                "\nadd_textbox|`9/warp [world] `#(warp's the world)|left|2480|"
                "\nadd_textbox|`9/flag [id] `#(sets flag to item id)|left|2480|"
                "\nadd_textbox|`9/skin [id] `#(sets your skin)|left|2480|"
                "\nadd_textbox|`9/name [name] `#(sets name to name)|left|2480|"
                "\nadd_textbox|`9/options `#(features)|left|2480|"
                "\nadd_textbox|`9/country `#(Changes your country flag)|left|2480|"
                "\nadd_textbox|`9/whitelist `#(Show's Whitelisted Players)|left|2480|"
                "\nadd_textbox|`9/blacklist `#(Show's Blacklisted Players)|left|2480|"
                "\nadd_textbox|`9/server `#(You Can Change Server Informations)|left|2480|"
                "\nadd_textbox|`9/account `#(See your account credentials)|left|2480|"
                "\nadd_textbox|`9/setuid `#(set uid for world lock troll)|left|2480|"
                "\nadd_textbox|`9/wlt `#(open/close worldlock troll)|left|2480|"
                "\nadd_textbox|`9/crash `#(You can Crash World)|left|2480|"
                "\nadd_textbox|`9/vault `#(Safe Vault Bypass with /bypassvault)|left|2480|"
                "\nadd_textbox|`9/visdice `#(Open Visual Dice Mode and /dset [number] to set number)|left|2480|"
                "\nadd_textbox|`9/antidice `#(Open Anti Dice Speed Mode)|left|2480|"
                "\nadd_textbox|`9/token `#(token exploit you need mentorship ticket you can buy with /buym and set netid to /setnid)|left|2480|"
                "\nadd_textbox|`9/name [name] `#(Change's your gt name visual)|left|2480|"
                "\nadd_quick_exit|"
                "\nend_dialog|end|Cancel|Okay|";
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = paket;
            g_server->send(true, liste);
            return true;
        }
        return false;
    }

    if (packet.find("game_version|") != -1) {
        if (events::out::loggedin == 1)
        {
            rtvar var = rtvar::parse(packet);
            auto mac = utils::generate_mac();
            auto hash_str = mac + "RT";
            auto hash2 = utils::hash((uint8_t*)hash_str.c_str(), hash_str.length());
            var.set("mac", mac);
            var.set("wk", utils::generate_rid());
            var.set("rid", utils::generate_rid());
            var.set("fz", std::to_string(utils::random(INT_MIN, INT_MAX)));
            var.set("zf", std::to_string(utils::random(INT_MIN, INT_MAX)));
            var.set("hash", std::to_string(utils::random(INT_MIN, INT_MAX)));
            var.set("hash2", std::to_string(hash2));
            //var.set("meta", "TLddQ2jYAo"); // no need to spoof meta
            //var.set("game_version", gt::version); removed due version bypass got patched.
            var.set("country", gt::flag);
            var.set("player_age", "20");
            if (var.find("tankIDName")) {
                name = var.find("tankIDName")->m_values[0];
            }
            packet = var.serialize();
            gt::in_game = false;
            print::set_color(LightGreen);
            PRINTS("Spoofing login info\n");
            g_server->send(false, packet);
            return true;
        }
    }

    return false;
}

bool events::out::gamemessage(std::string packet) {
    PRINTS("Game message: %s\n", packet.c_str());
    if (packet == "action|quit") {
        g_server->quit();
        return true;
    }

    return false;
}
int sayi = 1;
bool events::out::state(gameupdatepacket_t* packet) {
    if (!g_server->m_world.connected)
        return false;
    auto& bruh = g_server->m_world.local;
    if (events::out::autohosts == true)
    {
        int playerx = bruh.pos.m_x / 32;
        int playery = bruh.pos.m_y / 32;
        variantlist_t va{ "OnNameChanged" };
        va[1] = bruh.name + " `4[" + std::to_string(playerx) + "," + std::to_string(playery) + "]" + " `4[" + std::to_string(bruh.netid) + "]``" + " `#[" + to_string(bruh.userid) + "]``";
        g_server->send(true, va, bruh.netid, -1);
    }
    g_server->m_world.local.pos = vector2_t{ packet->m_vec_x, packet->m_vec_y };
    PRINTS("local pos: %.0f %.0f\n", packet->m_vec_x, packet->m_vec_y);
    if (gt::ghost)
        return true;
    return false;
}
std::string paket = "add_label_with_icon|big|`5The Proxy Gazette``|left|5016|"
//"\nadd_image_button|banner|interface/large/wtr_lvl11_xualqu.rttex|bannerlayout|||"
"\nadd_spacer|small|\n\nadd_url_button||`1Join Discord Server``|NOFLAGS|https://discord.gg/KD4a37S2an|Open link?|0|0|"
"\nadd_textbox|`wDecember 26th: `5Don't Forget Join Discord Server https://discord.gg/KD4a37S2an |left|"
"\nadd_spacer|small|"
"\nadd_textbox|`wDecember 26th: `5Proxy made by SrMotion#0001 |left|"
"\nadd_spacer|small|"
"\nadd_textbox|All Commands: |left|"
"\nadd_spacer|small|"
"\nadd_textbox|`9/proxy `#(shows commands)|left|2480|"
"\nadd_textbox|`9/world `#(shows commands)|left|2480|"
"\nadd_textbox|`9/bypassvault `#(bypassing safe vault)|left|2480|"
"\nadd_textbox|`9/bypassdoor `#(bypassing the doorid /setid (set door id))|left|2480|"
"\nadd_textbox|`9/bypasspath `#(bypassing the path marker)|left|2480|"
"\nadd_textbox|`9/tp [name] `#(teleports to a player in the world) |left|2480|"
"\nadd_textbox|`9/ghost `#(toggles ghost, you wont move for others when its enabled)|left|2480|"
"\nadd_textbox|`9/uid [name] `#(resolves name to uid)|left|2480|"
"\nadd_textbox|`9/flag [id] `#(sets flag to item id)|left|2480|"
"\nadd_textbox|`9/warp [world] `#(warp's the world)|left|2480|"
"\nadd_textbox|`9/skin [id] `#(sets your skin)|left|2480|"
"\nadd_textbox|`9/name [name] `#(sets name to name)|left|2480|"
"\nadd_textbox|`9/options `#(features)|left|2480|"
"\nadd_textbox|`9/country `#(Changes your country flag)|left|2480|"
"\nadd_textbox|`9/whitelist `#(Show's Whitelisted Players)|left|2480|"
"\nadd_textbox|`9/blacklist `#(Show's Blacklisted Players)|left|2480|"
"\nadd_textbox|`9/server `#(You Can Change Server Informations)|left|2480|"
"\nadd_textbox|`9/account `#(See your account credentials)|left|2480|"
"\nadd_textbox|`9/setuid `#(set uid for world lock troll)|left|2480|"
"\nadd_textbox|`9/wlt `#(open/close worldlock troll)|left|2480|"
"\nadd_textbox|`9/crash `#(You can Crash World)|left|2480|"
"\nadd_textbox|`9/vault `#(Safe Vault Bypass)|left|2480|"
"\nadd_textbox|`9/token `#(token exploit you need mentorship ticket you can buy with /buym and set netid to /setnid)|left|2480|"
"\nadd_textbox|`9/name [name] `#(Change's your gt name visual)|left|2480|"
"\nadd_spacer|small|"
"\nadd_layout_spacer|3imageslayout|\nadd_layout_spacer|3imageslayout|\nadd_spacer|small|"
"\nadd_textbox|Don't forget to join our `2Official Proxy Discord Server`` on `2https://discord.gg/KD4a37S2an `` or click the link below!|left|"
"\nadd_textbox|Visit our Social Media pages for more Content!|left|"
"\nadd_spacer|small|"
"\nadd_layout_spacer|2imageslayout|"
"\nadd_image_button|iotm_layout_4|interface/large/gazette/gazette_5columns_social_btn04.rttex|5imageslayout|https://www.youtube.com/channel/UCLZOpt3XfT1Pp9U4JUC6SzQ"
"UCLZOpt3XfT1Pp9U4JUC6SzQ|Would you like to open this in Youtube?|"
"\nadd_image_button|iotm_layout_4|interface/large/gazette/gazette_5columns_social_btn06.rttex|5imageslayout|https://discord.gg/KD4a37S2an|Would you like to join our "
"discord?|"
"\nadd_layout_spacer|2imageslayout|"
"\nadd_textbox||\nadd_spacer|small|"
"\nadd_textbox||\nadd_spacer|small|"
"\nadd_textbox||\nadd_spacer|small|"
"\nadd_quick_exit|"
"\nset_survey_enabled|1"
"\nend_dialog|gazette||OK|";
bool events::in::variantlist(gameupdatepacket_t* packet) {
    variantlist_t varlist{};
    auto extended = utils::get_extended(packet);
    extended += 4; //since it casts to data size not data but too lazy to fix this
    varlist.serialize_from_mem(extended);
    PRINTC("varlist: %s\n", varlist.print().c_str());
    auto func = varlist[0].get_string();

    //probably subject to change, so not including in switch statement.
    if (func.find("OnSuperMainStartAcceptLogon") != -1)
        gt::in_game = true;

    switch (hs::hash32(func.c_str())) {
        //solve captcha
        case fnv32("onShowCaptcha"): {
          auto menu = varlist[1].get_string();
              if (menu.find("`wAre you Human?``") != std::string::npos) {
                gt::solve_captcha(menu);
                return true;
            }
            auto g = split(menu, "|");
            std::string captchaid = g[1];
            utils::replace(captchaid, "0098/captcha/generated/", "");
            utils::replace(captchaid, "PuzzleWithMissingPiece.rttex", "");
            captchaid = captchaid.substr(0, captchaid.size() - 1);

            http::Request request{ "http://api.surferstealer.com/captcha/index?CaptchaID=" + captchaid };//lazy to add my own captcha solver, I put my friend's api, it's very accomplished
            const auto response = request.send("GET");
            std::string output = std::string{ response.body.begin(), response.body.end() };
            if (output.find("Answer|Failed") != std::string::npos) 
                return false;//failed
            else if (output.find("Answer|") != std::string::npos) {
                utils::replace(output, "Answer|", "");
                gt::send_log("Solved Captcha As "+output);
                g_server->send(false, "action|dialog_return\ndialog_name|puzzle_captcha_submit\ncaptcha_answer|" + output + "|CaptchaID|" + g[4]);
                return true;//success
            }
            return false;//failed
        } break;

    case fnv32("OnRequestWorldSelectMenu"): {
        auto& world = g_server->m_world;
        world.players.clear();
        world.local = {};
        world.connected = false;
        world.name = "EXIT";
    } break;

    case fnv32("OnSendToServer"): g_server->redirect_server(varlist); return true;

    case fnv32("OnConsoleMessage"): {
        auto wry = varlist[1].get_string();
        if (worldbanjoinmod == true)
        {
            if (wry.find("Removed your access from all locks.") != -1)
            {
                gt::send_log("`$Leaving the world due to having Mod bypass on and due to having a `#mod `$in the world!");
                g_server->send(false, "action|join_request\nname|exit", 3);
            }
        }
        if (visualspin == true) {
            if (wry.find("`7[```w" + name + "`` spun the wheel and got") != -1) {
                if (std::to_string(ruletsayi) == "0") {
                    varlist[1] = "`7[```w" + name + "`` spun the wheel and got `2" + std::to_string(ruletsayi) + "``!`7]``";
                }
                else {
                    varlist[1] = "`7[```w" + name + "`` spun the wheel and got `4" + std::to_string(ruletsayi) + "``!`7]``";
                }
                g_server->send(true, varlist, -1, 1900);
                return true;
            }
            else {
                if (wry.find("`7[```2" + name + "`` spun the wheel and got") != -1) {
                    if (std::to_string(ruletsayi) == "0") {
                        varlist[1] = "`7[```2" + name + "`` spun the wheel and got `2" + std::to_string(ruletsayi) + "``!`7]``";
                    }
                    else {
                        varlist[1] = "`7[```2" + name + "`` spun the wheel and got `4" + std::to_string(ruletsayi) + "``!`7]``";
                    }
                    g_server->send(true, varlist, -1, 1900);
                    return true;
                }
                else
                {
                    varlist[1] = packets + varlist[1].get_string();
                    g_server->send(true, varlist);
                    return true;
                }
            }
        }
        else {
            varlist[1] = packets + varlist[1].get_string();
            g_server->send(true, varlist);
            return true;
        }
    } break;
    case fnv32("OnTalkBubble"): {
        auto wry = varlist[2].get_string();        
        if (visualspin == true) {
            if (wry.find("`7[```w" + name + "`` spun the wheel and got") != -1) {
                if (std::to_string(ruletsayi) == "0") {
                    varlist[2] = "`7[```w" + name + "`` spun the wheel and got `2" + std::to_string(ruletsayi) + "``!`7]``";
                }
                else {
                    varlist[2] = "`7[```w" + name + "`` spun the wheel and got `4" + std::to_string(ruletsayi) + "``!`7]``";
                }
                g_server->send(true, varlist, -1, 1900);
                return true;
            }
            else
            {
                if (wry.find("`7[```2" + name + "`` spun the wheel and got") != -1) {
                    if (std::to_string(ruletsayi) == "0") {
                        varlist[2] = "`7[```2" + name + "`` spun the wheel and got `2" + std::to_string(ruletsayi) + "``!`7]``";
                    }
                    else {
                        varlist[2] = "`7[```2" + name + "`` spun the wheel and got `4" + std::to_string(ruletsayi) + "``!`7]``";
                    }
                    g_server->send(true, varlist, -1, 1900);
                    return true;
                }
            }
        }
        else {
            g_server->send(true, varlist);
            return true;
        }
    } break;
    case fnv32("OnDialogRequest"): {

        auto content = varlist[1].get_string();
        if (gt::resolving_uid2 && (content.find("friend_all|Show offline") != -1 || content.find("Social Portal") != -1) ||
            content.find("Are you sure you wish to stop ignoring") != -1) {
            return true;
        }
        else if (content.find("add_label_with_icon|small|Remove Your Access From World|left|242|") != -1) {
            if (worldbanjoinmod == true)
            {
                g_server->send(false, "action|dialog_return\ndialog_name|unaccess");
                return true;
            }
        }
        else if (gt::resolving_uid2 && content.find("Ok, you will now be able to see chat") != -1) {
            gt::resolving_uid2 = false;
            return true;
        }
        else if (gt::resolving_uid2 && content.find("`4Stop ignoring") != -1) {
            int pos = content.rfind("|`4Stop ignoring");
            auto ignore_substring = content.substr(0, pos);
            auto uid = ignore_substring.substr(ignore_substring.rfind("add_button|") + 11);
            auto uid_int = atoi(uid.c_str());
            if (uid_int == 0) {
                gt::resolving_uid2 = false;
                gt::send_log("name resolving seems to have failed.");
            }
            else {
                gt::send_log("`9Target UID: `#" + uid);
                gt::send_log("`9worldlock troll UID set to: `#" + uid);
                uidz = stoi(uid);
                g_server->send(false, "action|dialog_return\ndialog_name|friends\nbuttonClicked|" + uid);
                g_server->send(false, "action|dialog_return\ndialog_name|friends_remove\nfriendID|" + uid + "|\nbuttonClicked|remove");
            }
            return true;
        }
        if (wltroll == true)
        {
            if (content.find("add_label_with_icon|big|`wEdit World Lock``|left|242|") != -1)
            {
                int x = std::stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                int y = std::stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
                g_server->send(false, "action|dialog_return\ndialog_name|lock_edit\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\ncheckbox_" + to_string(uidz) + "|0\ncheckbox_public|0\ncheckbox_disable_music|0\ntempo|100\ncheckbox_disable_music_render|0\ncheckbox_set_as_home_world|0\nminimum_entry_level|1");
                return true;
            }
        }
        if (content.find("add_label_with_icon|big|`wEdit World Lock``|left|") != -1)
        {
            gt::send_log("find world lock x,y path.");
            wlx = std::stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
            wly = std::stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
        }
        if (dicemod == true)
        {
            if (content.find("add_label_with_icon|big|`wEdit Dice Block``|left|456|") != -1)
            {
                int x = std::stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                int y = std::stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
                if (content.find("add_checkbox|checkbox_public|Usable by public|0") != -1)
                {
                    gt::send_log("`4 (X:" + to_string(x) + " Y:" + to_string(y) + ")`9 Dice is `2Public");
                    g_server->send(false, "action|dialog_return\ndialog_name|boombox_edit\ntilex|" + to_string(x) + "|\ntiley|" + to_string(y) + "|\ncheckbox_public|1\ncheckbox_silence|1");
                    return true;

                }
                else
                {
                    gt::send_log("`4 (X:" + to_string(x) + " Y:" + to_string(y) + ")`9 Dice is `4Not Public");
                    g_server->send(false, "action|dialog_return\ndialog_name|boombox_edit\ntilex|" + to_string(x) + "|\ntiley|" + to_string(y) + "|\ncheckbox_public|0\ncheckbox_silence|1");
                    return true;

                }
                return true;
            }
        }
        if (content.find("set_default_color|`o") != -1)
        {
            if (content.find("add_label_with_icon|big|`wAre you Human?``|left|206|") != -1)
            {
                gt::solve_captcha(content);
                return true;
            }
        }
        if (content.find("add_label_with_icon|big|`wThe Growtopia Gazette``|left|5016|") != -1)
        {
            varlist[1] = paket;
            g_server->send(true, varlist);
            return true;
        }
        if (taxsystem == true)
        {
            if (content.find("Drop") != -1) {
                if (content.find("embed_data|itemID|") != -1) {
                    std::string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
                    //gt::send_log(itemid);
                    if (itemid.find("242") != -1) {
                        std::string count = content.substr(content.find("count||") + 7, content.length() - content.find("count||") - 1);
                        int bruh = stoi(count) % 10;
                        int bruh2 = stoi(count);
                        if (bruh == 1) {
                            bruh2 = bruh2 - 1;
                        }
                        if (bruh == 2) {
                            bruh2 = bruh2 - 2;
                        }
                        if (bruh == 3) {
                            bruh2 = bruh2 - 3;
                        }
                        if (bruh == 4) {
                            bruh2 = bruh2 - 4;
                        }
                        if (bruh == 5) {
                            bruh2 = bruh2 + 5;
                        }
                        if (bruh == 6) {
                            bruh2 = bruh2 + 4;
                        }
                        if (bruh == 7) {
                            bruh2 = bruh2 + 3;
                        }
                        if (bruh == 8) {
                            bruh2 = bruh2 + 2;
                        }
                        if (bruh == 9) {
                            bruh2 = bruh2 + 1;
                        }
                        int bruh3 = bruh2 * yuzde / 100;
                        int bruh4 = stoi(count) - bruh3;
                        if (fastmmode == true)
                        {
                            g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itemid + "|\ncount|" + std::to_string(bruh4));
                            return true;

                        }
                        else
                        {
                            varlist[1] = "set_default_color|`o\nadd_label_with_icon|big|`wDrop World Lock``|left|242|\nadd_textbox|How many to drop?|left|\nadd_text_input|count||" + to_string(bruh4) + "|5|\nembed_data|itemID|242\nend_dialog|drop_item|Cancel|OK|";
                            g_server->send(true, varlist);
                            return true;
                        }
                    }
                }
            }

        }
        if (fastdrop == true) {
            std::string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
            std::string count = content.substr(content.find("count||") + 7, content.length() - content.find("count||") - 1);
            if (content.find("embed_data|itemID|") != -1) {
                if (content.find("Drop") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itemid + "|\ncount|" + count);
                    return true;
                }
            }
        }
        if (taxsystem == true)
        {
            if (fastmmode == true) {
                std::string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
                std::string count = content.substr(content.find("count||") + 7, content.length() - content.find("count||") - 1);
                if (content.find("embed_data|itemID|") != -1) {
                    if (content.find("Drop") != -1) {
                        if (itemid.find("242") != -1) {
                            return true;
                        }
                    }
                }
            }

        }
        if (fasttrash == true) {
            std::string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
            std::string count = content.substr(content.find("you have ") + 9, content.length() - content.find("you have ") - 1);
            std::string s = count;
            std::string delimiter = ")";
            std::string token = s.substr(0, s.find(delimiter));
            if (content.find("embed_data|itemID|") != -1) {
                if (content.find("Trash") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|trash_item\nitemID|" + itemid + "|\ncount|" + token);
                    return true;
                }
            }
        }
        if (wrenchpull == true) {
            std::string netidyarragi = content.substr(content.find("embed_data|netID") + 17, content.length() - content.find("embed_data|netID") - 1);
            if (content.find("set_default_color|`o") != -1) { // trash bug fix
                if (content.find("embed_data|netID") != -1) {
                    return true;
                }
            }
        }
        else if (content.find("add_button|report_player|`wReport Player``|noflags|0|0|") != -1) {
            content = content.insert(content.find("add_button|report_player|`wReport Player``|noflags|0|0|"), "\nadd_button|whitelist|`2Add to Whitelist``|noflags|0|0|\nadd_button|blacklist|`4Add to Blacklist``|noflags|0|0|\n");
            varlist[1] = content;
            g_server->send(true, varlist, -1, -1);
            return true;
        }
        else if (content.find("add_button|my_worlds|`$My Worlds``|noflags|0|0|") != -1)
        {
            content = content.insert(content.find("add_button|my_worlds|`$My Worlds``|noflags|0|0|"), "\nadd_button|whitelisted_players|`2Whitelisted Players``|noflags|0|0|\n\nadd_button|blacklisted_players|`4Blacklisted Players``|noflags|0|0|\nadd_button|optionzzz|`5Options``|noflags|0|0|\n");

            varlist[1] = content;
            g_server->send(true, varlist, -1, -1);
            return true;
        }
    } break;
    case fnv32("OnRemove"): {
        auto text = varlist.get(1).get_string();
        if (text.find("netID|") == 0) {
            auto netid = atoi(text.substr(6).c_str());

            if (netid == g_server->m_world.local.netid)
                g_server->m_world.local = {};

            auto& players = g_server->m_world.players;
            for (size_t i = 0; i < players.size(); i++) {
                auto& player = players[i];
                if (player.netid == netid) {
                    players.erase(std::remove(players.begin(), players.end(), player), players.end());
                    break;
                }
            }
        }
    } break;
    case fnv32("OnSpawn"): {
        std::string meme = varlist.get(1).get_string();
        rtvar var = rtvar::parse(meme);
        auto name = var.find("name");

        auto netid = var.find("netID");
        auto onlineid = var.find("onlineID");
        auto userid = var.find("userID");
        if (name->m_value.find("`^") != -1)
        {
            std::string unaccess = userid->m_value;
            if (unaccesspaketi.find("\ncheckbox_" + userid->m_value + "|0") != -1)
            {
            }
            else
            {
                unaccesspaketi += "\ncheckbox_" + userid->m_value + "|0";

            }

        }
        if (name && netid && onlineid) {
            player ply{};
            if (var.find("invis")->m_value != "1") {
                ply.name = name->m_value;
                ply.country = var.get("country");
                if (events::out::autohosts == true)
                {
                    name->m_values[0] += " `4[" + netid->m_value + "]``" + " `#[" + userid->m_value + "]``";

                }
                auto pos = var.find("posXY");
                if (pos && pos->m_values.size() >= 2) {
                    auto x = atoi(pos->m_values[0].c_str());
                    auto y = atoi(pos->m_values[1].c_str());
                    ply.pos = vector2_t{ float(x), float(y) };
                }
            }
            else {
                gt::send_log("Moderator entered the world. 1/2");
                ply.mod = true;
                ply.invis = true;
            }
            if (var.get("mstate") == "1" || var.get("smstate") == "1")
                ply.mod = true;
            ply.userid = var.get_int("userID");
            ply.netid = var.get_int("netID");
            if (meme.find("type|local") != -1) {
                var.find("mstate")->m_values[0] = "1";
                g_server->m_world.local = ply;
            }
            g_server->m_world.players.push_back(ply);
            auto str = var.serialize();
            utils::replace(str, "onlineID", "onlineID|");
            varlist[1] = str;
            PRINTC("new: %s\n", varlist.print().c_str());
            g_server->send(true, varlist, -1, -1);
            return true;
        }
    } break;
    }
    return false;
}
bool events::in::generictext(std::string packet) {
    PRINTC("Generic text: %s\n", packet.c_str());

    return false;
}

bool events::in::gamemessage(std::string packet) {
    PRINTC("Game message: %s\n", packet.c_str());

    if (gt::resolving_uid2) {
        if (packet.find("PERSON IGNORED") != -1) {
            g_server->send(false, "action|dialog_return\ndialog_name|friends_guilds\nbuttonClicked|showfriend");
            g_server->send(false, "action|dialog_return\ndialog_name|friends\nbuttonClicked|friend_all");
        }
        else if (packet.find("Nobody is currently online with the name") != -1) {
            gt::resolving_uid2 = false;
            gt::send_log("Target is offline, cant find uid.");
        }
        else if (packet.find("Clever perhaps") != -1) {
            gt::resolving_uid2 = false;
            gt::send_log("Target is a moderator, can't ignore them.");
        }
    }
    return false;
}

bool events::in::sendmapdata(gameupdatepacket_t* packet) {
    g_server->m_world = {};
    auto extended = utils::get_extended(packet);
    extended += 4;
    auto data = extended + 6;
    auto name_length = *(short*)data;

    char* name = new char[name_length + 1];
    memcpy(name, data + sizeof(short), name_length);
    char none = '\0';
    memcpy(name + name_length, &none, 1);

    g_server->m_world.name = std::string(name);
    g_server->m_world.connected = true;
    delete[] name;
    PRINTC("world name is %s\n", g_server->m_world.name.c_str());

    return false;
}

bool events::in::state(gameupdatepacket_t* packet) {
    if (!g_server->m_world.connected)
        return false;
    if (packet->m_player_flags == -1)
        return false;

    auto& players = g_server->m_world.players;
    for (auto& player : players) {
        if (player.netid == packet->m_player_flags) {
            player.pos = vector2_t{ packet->m_vec_x, packet->m_vec_y };
            PRINTC("player %s position is %.0f %.0f\n", player.name.c_str(), player.pos.m_x, player.pos.m_y);
            break;
        }
    }
    return false;
}

bool events::in::tracking(std::string packet) {
    PRINTC("Tracking packet: %s\n", packet.c_str());
    if (packet.find("eventName|102_PLAYER.AUTHENTICATION") != -1)
    {
        string wlbalance = packet.substr(packet.find("Worldlock_balance|") + 18, packet.length() - packet.find("Worldlock_balance|") - 1);

        if (wlbalance.find("PLAYER.") != -1)
        {
            gt::send_log("`9World Lock Balance: `#0");
        }
        else
        {
            gt::send_log("`9World Lock Balance: `#" + wlbalance);

        }
        if (packet.find("Authenticated|1") != -1)
        {
            gt::send_log("`9Player Authentication `2Successfuly.");
        }
        else
        {
            gt::send_log("`9Player Authentication `4Failed.");
        }

    }
    if (packet.find("eventName|100_MOBILE.START") != -1)
    {
        string gem = packet.substr(packet.find("Gems_balance|") + 13, packet.length() - packet.find("Gems_balance|") - 1);
        string level = packet.substr(packet.find("Level|") + 6, packet.length() - packet.find("Level|") - 1);
        string uidd = packet.substr(packet.find("GrowId|") + 7, packet.length() - packet.find("GrowId|") - 1);
        gt::send_log("`9Gems Balance: `#" + gem);
        gt::send_log("`9Account Level: `#" + level);
        gt::send_log("`9Your Current UID: `#" + uidd);
        currentuid = uidd;
    }
    if (packet.find("eventName|300_WORLD_VISIT") != -1)
    {
        if (packet.find("Locked|0") != -1)
        {
            gt::send_log("`4This world is not locked by a world lock.");
        }
        else
        {
            gt::send_log("`2This world is locked by a world lock.");

            if (packet.find("World_owner|") != -1)
            {
                string uidd = packet.substr(packet.find("World_owner|") + 12, packet.length() - packet.find("World_owner|") - 1);
                gt::send_log("`9World Owner UID: `#" + uidd);
                owneruid = uidd;
            }
        }
    }

    return true;
}
