#ifndef GPIO_H
#define GPIO_H

#include <string>
#include <fstream>
#include <fcntl.h>
#include <sstream>
#include <stdlib.h>
#include <poll.h>
#include <string.h>
#include <stdio.h>

#define RDBUF_LEN 5
using namespace std;
class GPIO
{
    string _gpioNum;                                    //zmienna przechowywujaca numer gpio

public:
    GPIO();
    GPIO(const unsigned int gpioNum);
    GPIO(const string& gpioNum):_gpioNum(gpioNum){}

    int Gexport();
    int Gdirection(const string &data);
    int GsetValue(unsigned int num);
    int GsetEdge(const string& mode);
    int GgetValue();
    int Gunexport();
    int Ginterupt();
    virtual ~GPIO();
private:
    int openFile(const string& path,unsigned int mode,const string& data);
    string toString(unsigned int num);
};

#endif // GPIO_H
