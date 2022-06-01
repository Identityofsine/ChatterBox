#pragma once
#include<vector>
#include <string>

void print(const char* text);



std::vector<std::string> splitWords(std::string str);

std::string buildStringFromArgs(std::vector<std::string> args, int start, int end);