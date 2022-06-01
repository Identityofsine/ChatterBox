// ChatterBox.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define portNumber 56000

#include <iostream>
#include "Chat.h"
#include "BasicFunction.h"
#include "server.h"


void print(const char* text) {
    std::cout << text << std::endl;
}


int main()
{
    int c = 0;
    while (c != -1) {  
        print("1:CONNECT\n2:START SERVER");
        std::cin >> c;
        switch (c) {
        case 1:
            //connect to server
            print("connecting...");
            c = -1;
            break;
        case 2:
            print("starting Server...");
            startServer(portNumber);
            c = -1;
            break;
        default:
            break;
        }
    }
}

