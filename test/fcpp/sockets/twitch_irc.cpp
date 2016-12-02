#include <iostream>
#include <string>

#include "fcpp/sockets/SocketClient.hpp"

using namespace std;
using namespace fcpp::sockets;

/* Main */
int main (int argc, char *argv[])
{
  int err;
  string host = "irc.chat.twitch.tv";
  short port = 6667;
  string user;
  string token;
  
  // Ask for user
  cout << "Enter Username: ";
  getline(cin, user);

  // Ask for token
  cout << "Enter oAuth Token: ";
  getline(cin, token);

  // Create and connect the socket
  SocketClient sc(host, port);

  // Check if no created/connected
  if (!sc.isConnected()) {
    cerr << "Could not create and/or connect socket" << endl;
    exit(1);
  }

  // Add listener
  sc.addReceiveListener([user](SocketClient *s, void *buf) {
    string sin((char*)buf);
    cout << sin << endl;

    // TODO: Handle here the incoming messages and reply consequently
    // TODO: Every 5 min we get a PING, that should be responded with a PONG

    // If connected, join a channel
    if (sin.find("376 " + user + " :>") != string::npos) {
      s->sendMessage("JOIN #" + user);
    }
  });

  // Send Password (oAuth)
  if ((err = sc.sendMessage("PASS oauth:" + token)) == -1) {
    cerr << "Could not send password" << endl;
    exit(1);
  }

  // Send User (Nick)
  if ((err = sc.sendMessage("NICK " + user)) == -1) {
    cerr << "Could not send user" << endl;
    exit(1);
  }

  // Start socket client (this is blocking!)
  return sc.start();
}
