#include "server.h"

int main(int argc, char** argv)
{
    cout << "giao" << endl;
    if (argc > 1)
    {
        Server* serv = new Server(argv[1]);
        serv->ClientMake();
    }
    else
    {
        Server* serv = new Server();
        serv->ServerMake();
    }

}