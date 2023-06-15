#include <iostream>

#include "../loguru.cpp"

int main(int argc, char* argv[])
{
    int size = 1;
    char name[8] = "log.txt";
    char* p = name;
    loguru::init(size, &p);
    
    LOG_F(INFO, "begin fuzz");

    for (int i = 0; i < argc; ++i) {
        const char* str = argv[i];
        LOG_F(INFO, str);
    }

    LOG_F(INFO, "end fuzz");
}
