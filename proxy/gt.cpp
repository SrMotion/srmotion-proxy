#include "gt.hpp"
#include "packet.h"
#include "server.h"
#include "utils.h"

std::string gt::version = "3.64";
std::string gt::flag = "tr";
bool gt::resolving_uid2 = false;
bool gt::connecting = false;
bool gt::in_game = false;
bool gt::ghost = false;
bool gt::resolving_uid = false;
bool gt::aapbypass = false;
std::string gt::macaddr = "16:69:f9:t6:ga:6b" ;
void gt::send_log(std::string text) {
    g_server->send(true, "action|log\nmsg|" + text, NET_MESSAGE_GAME_MESSAGE);
}

void gt::solve_captcha(std::string text) {
    //Get the sum :D
    utils::replace(text,
        "set_default_color|`o\nadd_label_with_icon|big|`wAre you Human?``|left|206|\nadd_spacer|small|\nadd_textbox|What will be the sum of the following "
        "numbers|left|\nadd_textbox|",
        "");
    utils::replace(text, "|left|\nadd_text_input|captcha_answer|Answer:||32|\nend_dialog|captcha_submit||Submit|", "");
    auto number1 = text.substr(0, text.find(" +"));
    auto number2 = text.substr(number1.length() + 3, text.length());
    int result = atoi(number1.c_str()) + atoi(number2.c_str());
    send_log("Solved captcha as `2" + std::to_string(result) + "``");
    g_server->send(false, "action|dialog_return\ndialog_name|captcha_submit\ncaptcha_answer|" + std::to_string(result));
}
void gt::resolve_uid_to_name(std::string uid) {
    g_server->send(false, "action|friends");
    g_server->send(false, "action|dialog_return\ndialog_name|friends_guilds\nbuttonClicked|show_apprentices");
    g_server->send(false, "action|dialog_return\ndialog_name|show_mentees\nbuttonClicked|" + uid);
    resolving_uid = true;
}
