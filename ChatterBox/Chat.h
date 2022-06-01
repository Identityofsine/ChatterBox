#pragma once



class Chat
{
public:
	bool running = false;
	Chat runChat();
	bool closeChat();
};
struct ChatConstructer {// put somewhere else, two seperate pieces of code responsible for connecting and creating
	bool connect();
	bool createServer();

};
