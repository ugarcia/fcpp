#ifndef FC_SOCKETCLIENT_H
#define FC_SOCKETCLIENT_H

#include <string>
#include <functional>
#include <map>
#include <thread>

#define FC_SOCKET_MAX_LINE (1024)

using namespace std;

namespace fcpp
{
    namespace sockets
    {
        class SocketClient
        {
            private:
                int fd;
                string host;
                string ip;
                short port;
                map<int, function<void(SocketClient*, void*)>> receiveCallbacks;
                thread receiveThread;

                int hostnameToIP(string hostname , string *ip);
                int connectSocket();
                int releaseSocket();
                void socketReceiveThread();

            public:
                SocketClient(string host, short port);
                bool isConnected();
                int sendMessage(string message);
                int addReceiveListener(function<void(SocketClient*, void*)> callback);
                int removeReceiveListener(int idx);
                int start();
        };
    }
}

#endif // FC_SOCKETCLIENT_H
