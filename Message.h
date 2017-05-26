#ifndef NPDFACE_MESSAGE_H
#define NPDFACE_MESSAGE_H
#include <string>
#include <stdio.h>
#include <time.h>
#include <iostream>
class Message{
public:
    void NowTime()
    {
        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        printf ( "|%02d:%02d:%02d|",timeinfo->tm_hour,
                 timeinfo->tm_min,timeinfo->tm_sec);

    }
    Message(const std::string functionName,const std::string message)
    {
        NowTime();
        std::cout<<"Position:"<< functionName<<"|"<<message <<"|"<<std::endl;
    }

};
#endif//NPDFACE_MESSAGE_HPP
