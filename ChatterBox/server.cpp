#include <iostream>

#include "server.h"
#include <vector>

using namespace std;

server startServer(int port) {
	server rf = server();
	rf.startServer(port);
	print("ran");
	return rf;
}

std::vector<std::string> splitWords(string str) {
	std::vector<std::string> splitwords;
	string word = "";
	for (auto x : str) {
		if (x == ' ') {
			splitwords.push_back(word);
			word = "";
		}
		else {
			word = word + x;
		}
	}
	if (!word.empty())
		splitwords.push_back(word);
	return splitwords;
}

std::string buildStringFromArgs(std::vector<string> args, int start = 0, int end = 0) {
	ostringstream ss;
	for (int i = start; i < end; i++) {
		ss << args.at(i) << " ";
	}
	return ss.str();
}