#include <iostream>

#include "../loguru.cpp"

#include <unistd.h>
#include <fcntl.h>

#define BUFF_SIZE 1024

int main(int argc, char* argv[]) {
    char buff[BUFF_SIZE + 1];
    ssize_t bytesWritten = 0;
    ssize_t bytesRead = read(STDIN_FILENO, buff, BUFF_SIZE);
    if (bytesRead == -1)
        return 0; // exit success so not counted as defect
    buff[bytesRead] = '\0';

    loguru::init(argc, argv);
    LOG_F(INFO, "begin fuzz");

    while (1) {
        if (bytesWritten >= bytesRead)
            break;
        LOG_F(INFO, buff + bytesWritten);
        ++bytesWritten;
        while (bytesWritten < bytesRead and buff[bytesWritten] != '\0')
            ++bytesWritten;
    }
    
    LOG_F(INFO, "end fuzz");
}
