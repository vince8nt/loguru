#include <iostream>

#include "../loguru.cpp"

#include <unistd.h>

#define BUFF_SIZE 64

char buff[BUFF_SIZE + 1];

int main(int argc, char* argv[])
{
    loguru::init(argc, argv);
    
    LOG_F(INFO, "begin fuzz");

    while (1) {
        ssize_t bytesRead = read(STDIN_FILENO, buff, BUFF_SIZE);
        if (bytesRead < 1) break;
        buff[bytesRead] = '\0';
        LOG_F(INFO, buff);
    }

    LOG_F(INFO, "end fuzz");
}
