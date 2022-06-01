#pragma once
#include "Chat.h"
#include "BasicFunction.h"
#include <WS2tcpip.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#pragma comment (lib, "ws2_32.lib")
using namespace std;

struct Client {
	SOCKET sock;
	string name;
	Client(SOCKET s, string n) {
		cout << n << " has joined the server" << endl;
		sock = s;
		name = n;
	}
	~Client() {

	}
	bool operator==(const SOCKET& rhs) const
	{
		if (rhs == INVALID_SOCKET)
			return false;
		return rhs == sock;
	}

};


struct server {
	thread thread_;
	WSADATA wsData; // holds socket data.
	WORD ver;
	int wsOk;
	fd_set master;
	SOCKET listening;
	bool isRunning = false;
	std::vector<Client> currentClients;
	void startServer(int port) {
		//init winsock
		ver = MAKEWORD(2, 2);
		wsOk =WSAStartup(ver, &wsData);
		if (wsOk != 0)
		{
			cerr << "ERROR: Can't Initialize winsock... Quitting!";
			return;
		}
		//create a socket, a socket is just an endpoint
		listening = socket(AF_INET, SOCK_STREAM, 0);
		if (listening == INVALID_SOCKET)
		{
			cerr << "ERROR: Can't create a socket! Quitting!" << endl;
			return;
		}
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		hint.sin_addr.S_un.S_addr = INADDR_ANY; //could use inet_pton
		// bind to the socket
		bind(listening, (sockaddr*)&hint, sizeof(hint));
		//Tell winsock the socket is for listening.
		listen(listening, SOMAXCONN);

		FD_ZERO(&master);
		FD_SET(listening, &master);

		isRunning = true;

		thread_ = thread(&server::threadUsers, this);
		runConsole();
		//cleanup
		WSACleanup();
	}
	void runConsole() {
		while (isRunning) {
			string message;
			getline(cin, message);
			vector<string> args = splitWords(message);
			if (args.size() > 0) {
				if (args.at(0) == "quit")
					exit(0);
				if (args.at(0) == "kickall") {
					string kickmessage = "";
					if (args.size() > 1) {
						kickmessage = buildStringFromArgs(args, 1, args.size());
						cout << kickmessage << endl;
					}
					kickAll(kickmessage);
				}
			}
		}
	}
	void threadUsers() {
		while (true) {
			fd_set copy = master;
			int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
			for (int i = 0; i < socketCount; i++) {
				SOCKET sock = copy.fd_array[i];
				if (sock == listening)
				{
					//accept a new connection
					SOCKET client = accept(listening, nullptr, nullptr);
					currentClients.push_back(Client(client, "client"));
					// Add the new connection to the list of connected clients
					FD_SET(client, &master);
					// Send a welcome message to the connect clients
					string wMessage = "Welcome to a ChatterBox Server!\n\r";
					send(client, wMessage.c_str(), wMessage.size() + 1, 0);
					// TODO: Broadcast client join.
				}
				else {
					char buf[4096];
					ZeroMemory(buf, 4096);
					int bR = recv(sock, buf, 4096, 0);
					if (bR <= 0) {
						//drop the client!
						currentClients.erase(remove(currentClients.begin(), currentClients.end(), sock), currentClients.end());
						closesocket(sock);
						FD_CLR(sock, &master);
					}
					else
					{
						for (int i = 0; i < master.fd_count; i++) {
							{
								SOCKET outSock = master.fd_array[i];
								std::vector<Client>::iterator it;
								it = find(currentClients.begin(), currentClients.end(), outSock);
								if (it != currentClients.end()) {
									if (outSock != listening && outSock != sock) {
										ostringstream ss;
										ss << "SOCKET #" << currentClients.at(it - currentClients.begin()).sock << ": " << buf << "\r\n";
										string strOut = ss.str();
										send(outSock, strOut.c_str(), strOut.size() + 1, 0);
									}
								}
							}
						}
						//send message to other clients
					}
					//accept a new message
				}
			}
		}
	}

	void stopServer();
	void kickAll(string message = "") {
		//kick all the users
		int size = currentClients.size();
		auto copy = currentClients;
			for (int i = 0; i < size; i++) {
				SOCKET sock = copy.at(i).sock;
				send(sock, message.c_str(), message.size() + 1, 0);
				std::this_thread::sleep_for(std::chrono::seconds(1));
				closesocket(sock);
				FD_CLR(sock, &master);
			}
			currentClients.clear();
	}
};

server startServer(int port);