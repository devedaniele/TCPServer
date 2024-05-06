#include <iostream>
#include "../server.hpp"
#include <string>

using namespace std;

int main(int argc, char const* argv[]){
   Server server;

   server.IP = "0.0.0.0";
   server.PORT = 8767;

   if (server.setupServer() != 0){
      return -1;
   }

   server.listenTo(2);
   int acc = 1;

   string serverMessage = "HTTP /1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
   string response = "<html><h1>Hello World</h1></html>";

   serverMessage.append(to_string(response.size()));
   serverMessage.append("\n\n");
   serverMessage.append(response);

   while (true){
      acc = server.acceptSocket();

      if (acc != -1) {
         //cout << "\nfrom client: ";
         server.readData();

         server.sendString(serverMessage);
      }

      server.closeClient();
   };

   return 0;  
}
