#ifndef RCONBF2142
#define RCONBF2142

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define BUFFLEN 1024

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include "md5.h"
#include <iostream>
#include <string>

#endif

namespace rconbf2142
{
	bool connect(std::string addr, std::string port, SOCKET& sock);
	bool login(SOCKET sock, std::string password);
	bool invoke(SOCKET sock, std::string message, std::string& result);
	bool _send(SOCKET sock, std::string message);
	bool _recv(SOCKET sock, std::string& result);
}

