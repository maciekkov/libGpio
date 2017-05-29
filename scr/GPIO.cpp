#include "GPIO.h"
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
using namespace std;
enum mode                                       //Opcje jako SWITCH w metodzie otwieraj¹cej plik
{
    WRITE   = 1,
    READ    = 2
};
GPIO::GPIO(const unsigned int gpioNum)
{
    _gpioNum = toString(gpioNum);
}
int GPIO::Ginterupt()
{
    bool interrupt = false;
    string  path   = "/sys/class/gpio/gpio"+_gpioNum+"/value";
    char bufor [RDBUF_LEN];
    int fd,ret;
    struct pollfd pfd[2];                       //structura przechowuj¹ca kofiguracjê przerwañ

    if(fd = open(path.c_str(),O_RDONLY)<0)      //file descriptor do pliku który bedzie monitorowany
    {
        perror("open()");
        return -1;
    }
    pfd[0].fd = STDIN_FILENO;                   // Dwa rodzaje obs³ugiowanych przerwañ jedno z klawiatury drugie z GPIO
    pfd[1].fd = fd;                             //Ale GPIO niechce dzia³aæ, mimo, ¿e sprawdza³em podczas dzia³ania programu i wartoœæ
    pfd[0].events = POLLIN;                     //w pliku value ulega zmianie.
    pfd[1].events = POLLPRI;

    while(!interrupt)
    {
        cout<<"valueMAIN:"<< endl;
        memset(bufor,0,RDBUF_LEN);

        if(ret = poll(pfd,2,-1)<0)                 //oczekiwanie na przerwanie na naszym pliku value,-1 znaczy,¿e czeka w nieskoñczonosæ
        {
            perror("poll()");
            close(fd);
            return-1;
        }
        if(ret == 0)
        {
            cout << "timeout"<<endl;
        }
        cout << "timeout"<<endl;                     //obs³uga przerwañ
        if (pfd[1].revents & POLLPRI)
        {
            lseek(pfd[1].fd, 0, SEEK_SET);
            ret = read(pfd[1].fd, bufor, RDBUF_LEN);
            cout << "Gpio interupt occurred" << endl;
        }
        if (pfd[0].revents & POLLIN)
        {
            (void)read(pfd[0].fd, bufor, 1);
            cout << "STDIN interupt occurred" << endl;
        }
        interrupt = true;
    }
    return 1;
}
int GPIO::openFile(const string& path,unsigned int modee,const string& data="")
{
    fstream fd;
    string value;
    string bufor = "/sys/class/gpio"+path;
    fd.open(bufor.c_str());
    if(fd.fail())
    {
        perror("couldn't open file");
        return -1;
    }
    switch(modee)
    {
    case WRITE:
        fd << data;
        fd.close();
        return 1;
    case READ:
        fd >> value;
        fd.close();
        return  atoi(value.c_str());
    }
}
int GPIO::Gexport()
{
    string path = "/export";
    if(openFile(path,WRITE,_gpioNum)<0)
    {
        perror("error export");
        return -1;
    }
    return 1;
}
int GPIO::Gdirection(const string &data)
{
    string path ="/gpio"+_gpioNum+"/direction";
    if(openFile(path,WRITE,data)<0)
    {
        perror("error direction");
        return -1;
    }
    return 1;

}
int GPIO::GsetValue(unsigned int num)
{
    string path ="/gpio"+_gpioNum+"/value";
    if(openFile(path,WRITE,toString(num))<0)
    {
        perror("error value");
        return -1;
    }
    return 1;
}
int GPIO::GsetEdge(const string& type)
{
    string path ="/gpio"+_gpioNum+"/edge";
    if(openFile(path,WRITE,type)<0)
    {
        perror("error value");
        return -1;
    }
    return 1;

}
int GPIO::GgetValue()
{
    string path ="/gpio"+_gpioNum+"/value";

    int value=0;
    value = openFile(path,READ);
    if(value <0)
    {
        perror("error value");
        return -1;
    }
    //  cout<<"value:"<<value<<endl;
    return value;
}
int GPIO::Gunexport()
{
    string path ="/unexport";
    if(openFile(path,WRITE,_gpioNum)<0)
    {
        perror("error unexport");
        return -1;
    }
}
string GPIO::toString(unsigned int num)
{
    string bufor;
    stringstream ss;
    ss<<num;
    bufor = ss.str();
    return bufor;
}
GPIO::~GPIO()
{
    //dtor
}
