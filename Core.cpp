#include <vector>
#include <string>
#include <iostream>

#include <SDL_net.h>

#include "Core.h"

std::vector<std::string> split(std::string line, char delim, const std::vector<char>& exclude = {})
{
	std::vector<std::string> words{};
	for (const char& c : line)
		if (c == delim) words.push_back("");
		else if (std::find(exclude.begin(), exclude.end(), c) != exclude.end())
			words.back().push_back(c);
	return words;
}

void printIP(IPaddress* ip)
{
	unsigned int ipData{ SDL_Swap32(ip->host) };
	std::cout << ((ipData >> 24) & 0xff) << '.';
	std::cout << ((ipData >> 16) & 0xff) << '.';
	std::cout << ((ipData >> 8) & 0xff) << '.';
	std::cout << (ipData & 0xff) << '\n';
}

void Core::init()
{
	SDLNet_Init();

	SDLNet_ResolveHost(&ip, NULL, 80);
	server = SDLNet_TCP_Open(&ip);
	set = SDLNet_AllocSocketSet(1);

	std::cout << "Server started\n";
}

void Core::loop()
{
	TCPsocket client{ SDLNet_TCP_Accept(server) };
	if (!client) return;

	SDLNet_TCP_AddSocket(set, client);
	IPaddress* clientIP{ SDLNet_TCP_GetPeerAddress(client) };
	std::cout << "Client connected with IP "; printIP(clientIP);

	std::vector<char> req{};
	int bytes{};
	do
	{
		req.push_back(0);
		if (SDLNet_CheckSockets(set, 0) == 1)
			bytes = SDLNet_TCP_Recv(client, &req.back(), 1);
		else bytes = 0;
	} while (bytes);

	if (req.empty()) { SDLNet_TCP_DelSocket(set, client); SDLNet_TCP_Close(client); return; }

	std::vector<std::string> lines{ split(std::string{ req.data() }, '\n', { '\r' }) };
	std::vector<std::string> statusLine{ split(lines[0], ' ') };

	SDLNet_TCP_DelSocket(set, client);
	SDLNet_TCP_Close(client);
}

void Core::quit()
{
	SDLNet_FreeSocketSet(set);
	SDLNet_Quit();
}