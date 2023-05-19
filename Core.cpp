#include <vector>
#include <string>
#include <iostream>

#include <SDL_net.h>

#include "Core.h"

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
	do req.push_back(0);
	while (SDLNet_TCP_Recv(client, &req.back(), 1) == 1);
	if (req.empty()) { SDLNet_TCP_DelSocket(set, client); SDLNet_TCP_Close(client); return; }

	std::vector<std::string> lines(1);
	for (const char& c : req)
		if (c == '\n') lines.push_back("");
		else if (c != '\r') lines.back().push_back(c);

	for (const std::string& line : lines)
		std::cout << line << '\n';

	SDLNet_TCP_DelSocket(set, client);
	SDLNet_TCP_Close(client);
}

void Core::quit()
{
	SDLNet_FreeSocketSet(set);
	SDLNet_Quit();
}