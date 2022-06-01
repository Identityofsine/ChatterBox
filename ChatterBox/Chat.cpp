#include "Chat.h"
#include "BasicFunction.h"

Chat Chat::runChat() {
	//check if all settings are good, return an active Chat Client
	if (this->running)
		return Chat();
	this->running = true;

}

bool Chat::closeChat() {
	running = false;
	return running;
}