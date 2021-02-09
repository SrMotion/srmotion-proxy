#pragma once
#include "sandbird/sandbird.h"
#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "shlwapi.lib")
#else
#include <unistd.h>
#endif // _WIN32
#include <stdint.h>
#include <stdio.h>
#include <string>


namespace http {
	int handler(sb_Event* evt);
	void start();
	void run(std::string dest, std::string port);
}