#pragma once
#include<sys/socket.h>//socket,listen,bind
#include<netinet/in.h>//sockaddr_in
#include<arpa/inet.h>//inet_htop
#include<unistd.h>//fork

#include <iostream>
using namespace std;

#define MAXN 10
class Server
{
public:
    Server(string addr = "127.0.0.1", size_t port = 9877) :_addr(addr), _port(port) {}
    ~Server() = default;
protected:
    using SA = sockaddr;
    string _addr;
    size_t _port;
    int _sockfd;
    int _connfd;
    sockaddr_in _servaddr;
public:
    int Init()
    {
        _sockfd = socket(AF_INET, SOCK_STREAM, 0);
        return 0;
    }

    int ServerMake()
    {
        Init();
        _servaddr.sin_family = AF_INET;
        _servaddr.sin_port = htons(_port);
        _servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(_sockfd, (SA*)&_servaddr, sizeof(_servaddr)) < 0)
        {
            cout << "bind error." << endl;
            return -1;
        }

        if (listen(_sockfd, 1024) < 0)
        {
            cout << "listen error." << endl;
            return -1;
        }

        Accept();
        return 0;
    }

    int ClientMake()
    {
        Init();
        int n;
        _servaddr.sin_family = AF_INET;
        _servaddr.sin_port = htons(_port);
        if ((n = inet_pton(AF_INET, _addr.c_str(), &_servaddr.sin_addr)) < 0)
        {
            cout << "inet_pton error. address: " << _addr.c_str() << endl;
            return -1;
        }
        else if (n == 0)
        {
            cout << "inet_pron wrong address: " << _addr.c_str() << endl;
            return -1;
        }
        if (connect(_sockfd, (SA*)&_servaddr, sizeof(_servaddr)) < 0)
        {
            cout << "connect error." << endl;
            return -1;
        }
        DoClientWork();
    }
    int Accept()
    {
        for (;;)
        {
            _connfd = accept(_sockfd, (SA*)NULL, NULL);
            if (fork() == 0)
            {
                close(_sockfd);
                DoServerWork();
                //close(_connfd);//可显示调用
                exit(0);
            }
            close(_connfd);
        }
        return 0;
    }

    int DoServerWork()
    {
        StrEcho();
        return 0;
    }
    int DoClientWork()
    {
        StrPush();
        return 0;
    }

    void StrPush()
    {
        for (;;)
        {
            string str;
            cin >> str;
            if (Writen(_sockfd, str.c_str(), str.size()) < 0)
            {
                cout << "client write error, str:" << str.c_str();
                return;
            }
            char recv[MAXN];
            Readn(_sockfd, recv, sizeof(recv));
            cout << "client " << getpid() << ": " << recv;
        }
    }
    void StrEcho()
    {
        int nread;
        char str[MAXN];
        for (;;)
        {
            if ((nread = Readn(_connfd, str, sizeof(str))) == 0)
            {
                return;
            }
            Writen(_connfd, str, sizeof(str));
            cout << "server " << str;
        }
    }

    ssize_t Readn(int sockfd, void* vptr, size_t n)
    {
        size_t nread;
        ssize_t nleft;
        char* ptr;

        ptr = (char*)vptr;
        nleft = n;
        while (nleft > 0)
        {
            if ((nread = read(sockfd, ptr, nleft)) < 0)
            {
                if (errno = EINTR)
                {
                    nread = 0;
                }
                else
                {
                    return -1;
                }
            }
            else if (nread == 0) {
                break;
            }
            nleft -= nread;
            ptr += nread;
        }
        return n - nleft;
    }

    ssize_t Writen(int sockfd, const void* vptr, size_t n)
    {
        size_t nleft;
        ssize_t nwrite;
        const char* ptr;//常量指针，可改指针不可改指向值;char* const ptr ,const修饰的是常量，指针指向不可以改，指针指向的值可以更改

        ptr = (char*)vptr;
        nleft = n;
        while (nleft > 0)
        {
            if ((nwrite = write(sockfd, ptr, nleft)) <= 0)
            {
                if (errno == EINTR && nwrite < 0)
                {
                    nwrite = 0;
                }
                else
                {
                    return -1;
                }
            }
            nleft -= nwrite;
            ptr += nwrite;
        }
        return n - nwrite;
    }
};