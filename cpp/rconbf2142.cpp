#include "rconbf2142.h"

namespace rconbf2142
{
	bool connect(std::string addr, std::string port, SOCKET& sock)
	{
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return false;

		struct addrinfo * result = NULL,
						* ptr = NULL,
						hints;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		if (getaddrinfo(addr.c_str(), port.c_str(), &hints, &result) == 0)
		{
			ptr = result;

			SOCKET _sock = INVALID_SOCKET;
			_sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

			if (_sock != INVALID_SOCKET)
			{

				if (connect(_sock, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR)
				{
					closesocket(_sock);
					_sock = INVALID_SOCKET;
				}

				freeaddrinfo(result);

				if (_sock != INVALID_SOCKET)
				{
					sock = _sock;
					return true;
				}
			}
		}

		WSACleanup();
		return false;
	}

	bool login(SOCKET sock, std::string password)
	{
		std::string data = "";
		char buffer[BUFFLEN];
		int res;

		while (data.find("\n\n") == std::string::npos)
		{
			memset(buffer, 0, BUFFLEN);

			res = recv(sock, buffer, BUFFLEN, 0);
			if (res > 0)
			{
				data += buffer;
			}
			else
			{
				closesocket(sock);
				WSACleanup();
				return false;
			}
		}

		int index = data.find("### Digest seed: ");
		if (index != std::string::npos)
		{
			std::string pwhash = md5(data.substr(index + 17, 16) + password);
	
			std::string result;
			if (invoke(sock, "login " + pwhash, result))
			{
				if (result.find("Authentication success") != std::string::npos)
				{
					return true;
				}
			}
		}

		closesocket(sock);
		WSACleanup();
		return false;
	}

	bool invoke(SOCKET sock, std::string message, std::string& result)
	{
		if (!_send(sock, "\x02" + message + "\n")) return false;
		if (!_recv(sock, result)) return false;
		return true;
	}

	bool _send(SOCKET sock, std::string message)
	{
		int temp,
			sent = 0,
			msglen = message.length();

		while (sent < msglen)
		{
			temp = send(sock, message.c_str(), msglen - sent, 0);
			if (temp == SOCKET_ERROR)
			{
				closesocket(sock);
				WSACleanup();
				return false;
			}

			sent += temp;
		}

		return true;
	}

	bool _recv(SOCKET sock, std::string& result)
	{
		int res;
		char buffer[BUFFLEN];

		result = "";

		while (result.find("\x04") == std::string::npos)
		{
			memset(buffer, 0, BUFFLEN);

			res = recv(sock, buffer, BUFFLEN, 0);
			if (res > 0)
			{
				result += buffer;
			}
			else
			{
				closesocket(sock);
				WSACleanup();
				return false;
			}
		}

		result.erase(result.length() - 1);
		return true;
	}
}
