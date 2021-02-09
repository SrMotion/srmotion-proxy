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

int main() {

    SetConsoleCtrlHandler(HandlerRoutine, true);
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
    SetConsoleTitle("SrMotion Proxy ;)");
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
         Sleep(1);
       }
  }
   else
    print::set_text("Failed to start server or proxy.\n", LightGreen);
}
