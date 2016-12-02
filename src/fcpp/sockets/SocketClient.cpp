#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <thread>
#include <functional>
#include <map>

#include "fcpp/sockets/SocketClient.hpp"

using namespace std;
using namespace fcpp::sockets;

/* Constructor */
SocketClient::SocketClient(string host, short port)
{
    // Save the remote info
    this->host = host;
    this->port = port;
    
    // Create the socket
    this->fd = socket(AF_INET, SOCK_STREAM, 0);

    // Check proper file descriptor, otherwise show error message
    if (this->fd == -1) {
        cerr << "Could not initiate socket descriptor" << endl;
        this->releaseSocket();
    
    // Proper descriptor, connect it
    } else {

        // Errors connecting. show them and release socket
        if (this->connectSocket() == -1) {
            cerr << "Could not connect socket" << endl;
            this->releaseSocket();

        // Successfully connected    
        } else {
            cout << "Socket connected" << endl;
        }
    }
}

/* Returns if the socket is connected or not */
// TODO: Improve this check?
bool SocketClient::isConnected()
{
    return this->fd != -1;
}

/* Get ip from domain name */
int SocketClient::hostnameToIP(string hostname , string *ip)
{
    int sockfd;  
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_in *h;
    int rv;
 
    memset(&hints, 0, sizeof hints);
    // Use AF_INET6 to force IPv6
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
 
    // Try to get the server info from Hostname
    if ((rv = getaddrinfo(hostname.c_str(), "http", &hints, &servinfo)) != 0) {
        cerr << "Could not resolve hostname: " << hostname << endl;
        cerr << "getaddrinfo: " << gai_strerror(rv) << endl;
        return -1;
    }
 
    // Loop through all the results and get the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        h = (struct sockaddr_in*)p->ai_addr;
        *ip = inet_ntoa(h->sin_addr);
        // strcpy(ip, inet_ntoa(h->sin_addr));
    }
     
    // Cleanup and exit
    freeaddrinfo(servinfo);
    cout << "Hostname " << hostname << " resolved to IP " << *ip << endl;
    return 0;
}

/* Connects a socket */
int SocketClient::connectSocket()
{
    int err;
    struct sockaddr_in sa;

    // Try to get IP from DNS
    err = this->hostnameToIP(this->host , &this->ip);

    // Check if error, we skip if any
    if (err == -1) { return -1; }

    // Build the sever_addr struct
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(this->ip.c_str());
    sa.sin_port = htons(port);

    // Try to connect
    err = connect(this->fd, (struct sockaddr*) &sa, sizeof(sa));

    // TODO: Release 'sa' struct here? Or afterwards?

    // Check if error, we skip if any
    if (err == -1) { return -1; }

    // Spawn receive thread
    this->receiveThread = thread(&SocketClient::socketReceiveThread, this);

    // TODO: Check if errors in thread stuff
    return 0;
}

/* Starts receive loop */
int SocketClient::start()
{
    // Check it's open, otherwise fail
    if (!this->isConnected()) {
        return -1;
    }

    // Join receive thread
    this->receiveThread.join();

    // Done!!
    return 0;
}

/* Releases a socket */
int SocketClient::releaseSocket()
{
    int err;

    // Try to close the socket if it's opened
    if (this->isConnected()) {
        err = close(this->fd);
    }
    
    // If  no errors, make this -1
    this->fd = -1;

    // Return result of chained operations feedbacks
    return err;
}

/* Sends a message thru the socket */
int SocketClient::sendMessage(string message)
{
    // Check it's open, otherwise fail
    if (!this->isConnected()) {
        return -1;
    }

    // Fill line end in message
    string sin = message + "\r\n";

    // Try to send it, return result
    return send(this->fd, sin.c_str(), sin.size(), 0);
}

/* Receive method for separate thread */
void SocketClient::socketReceiveThread()
{
    int err;
    char buf[FC_SOCKET_MAX_LINE];

    // Receive loop, until we get an error
    do {

        // Blocking read
        // err = SSL_read((SSL*)fd, buf, MAX_LINE - 1);
        err = recv(this->fd, buf, FC_SOCKET_MAX_LINE - 1, 0);

        // Proceed only if no error
        if (err > -1) {

            // Terminate buffer
            buf[err] = '\0';

            // Loop over defined receive callbacks, call all of them
            // ACHTUNG: They are called same order they were defined
            for (auto c : this->receiveCallbacks) {
                c.second(this, buf);
            }
        }
    } while (err > -1);

    // At this point we received an error, so log it and release the socket
    cerr << "Received error from server socket" << endl;
    this->releaseSocket();
}

/* Adds a listener to receive thread */
int SocketClient::addReceiveListener(function<void(SocketClient*, void*)> callback)
{
    // Get the current size of the callbacks map
    int s = this->receiveCallbacks.size();

    // Add the new one
    // TODO: Add try/catch here?
    this->receiveCallbacks.insert(pair<int,function<void(SocketClient*, void*)>>(s, callback));

    // Return the key index
    return s;
}

/* Removes a listener from receive thread */
int SocketClient::removeReceiveListener(int idx)
{
    // Remove it and return '1' if removed, '0' or '-1' otherwise?
    // TODO: Add try/catch here?
    return this->receiveCallbacks.erase(idx);
}
