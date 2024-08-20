#include <iostream>
#include "../server.hpp"
#include <string>

using namespace std;

int main(int argc, char const* argv[]){
   Server server;

   server.PORT = 8767;

   string serverMessage = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
   string response = "<html><h1>Hello World</h1></html>";

   serverMessage.append(to_string(response.size()));
   serverMessage.append("\n\n");
   serverMessage.append(response);

   cout << "starting server..." << endl;

   server.listenTo(3,[serverMessage](Server& srv){
      srv.readData();
      srv.sendString(serverMessage);

      srv.closeClient();
   });

   return 0;  
}