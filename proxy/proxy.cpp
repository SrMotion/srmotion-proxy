#pragma once
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include "enet/include/enet.h"
#include "http.h"
#include "server.h"
#include "print.h"
#include <conio.h>
#include <fstream>
#include "events.h"
#include "lw_http.hpp"
#include "utils.h"
#include <regex>
#include "proton/rtparam.hpp"
#include "HTTPRequest.hpp"
server* g_server = new server();
using namespace std;
BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
    print::set_text("\nFixing Hosts File!", LightGreen);

    try
    {
        std::ofstream dosyaYaz("C:\\Windows\\System32\\drivers\\etc\\hosts");

        if (dosyaYaz.is_open()) {
            dosyaYaz << "";
            dosyaYaz.close();
        }
    }
    catch (std::exception)
    {
    }
    return FALSE;
}
vector<string> split(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}
void setgtserver() {
    try
    {
        std::ofstream dosyaYaz("C:\\Windows\\System32\\drivers\\etc\\hosts");

        if (dosyaYaz.is_open()) {
            dosyaYaz << "";
            dosyaYaz.close();
        }
    }
    catch (std::exception)
    {
    }

    try
    {
        http::Request request{ "http://growtopia2.com/growtopia/server_data.php" };
        const auto response = request.send("POST", "version=1&protocol=158", { "Content-Type: application/x-www-form-urlencoded" });
        rtvar var = rtvar::parse({ response.body.begin(), response.body.end() });
        var.serialize();
        if (var.find("server")) {
            g_server->m_port = std::stoi(var.get("port"));
            g_server->portz = std::stoi(var.get("port"));

        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Request failed, error: " << e.what() << '\n';
    }
    try
    {
        std::ofstream dosyaYaz("C:\\Windows\\System32\\drivers\\etc\\hosts");

        if (dosyaYaz.is_open()) {
            dosyaYaz << "127.0.0.1 growtopia1.com\n127.0.0.1 growtopia2.com";
            dosyaYaz.close();
        }
    }
    catch (std::exception)
    {
    }

}
int main() {
    cout << "NOTE: NO MORE UPDATES & HELPS TO SRMOTION PROXY, WE'RE WORKING ON FLEXFARM DO NOT FORGET JOIN OUR DISCORD(SrMotion#1337)" << endl;
    system("start https://discord.gg/CAjMzKNeJX");
    SetConsoleTitleA("SrMotion Proxy ;)");

    printf("Parsing the server_data.php\n");
    SetConsoleCtrlHandler(HandlerRoutine, true);
    setgtserver();

    system("Color a");
    printf("Based on enet by ama.\n");
    events::out::type2 = 2;
    g_server->ipserver = "127.0.0.1";
    g_server->create = "0.0.0.0";
    std::thread http(http::run, g_server->ipserver, "17191");
    http.detach();
    print::set_color(LightGreen);
    enet_initialize();
    if (g_server->start()) {
       print::set_text("Server & client proxy is running.\n", LightGreen);
       while (true) {
       g_server->poll();
         std::this_thread::sleep_for(std::chrono::milliseconds(1));
       }
  }
   else
    print::set_text("Failed to start server or proxy.\n", LightGreen);
}
