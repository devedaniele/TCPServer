#include <iostream>
#include "../server.h"
#include <string>

using namespace std;

int main(int argc, char const* argv[]){
   Server server(AF_INET);

   string serverMessage = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
   string response = "<html><h1>Hello World</h1></html>";

   serverMessage.append(to_string(response.size()));
   serverMessage.append("\n\n");
   serverMessage.append(response);

   cout << "starting server..." << endl;

   server.listenTo(8767,[serverMessage](const int &cfd){
      char buffer[1024];
      if (read(cfd,buffer,1024) != -1)
         cout << buffer << endl;
      
      int totalByteSent = 0;

      while(totalByteSent < serverMessage.size()){
         int byteSent = write(cfd,serverMessage.c_str(),serverMessage.size());

         if (byteSent == -1){
            cout << "writing failed." << endl;
            return;
         }
         
         totalByteSent += byteSent;
      }

      close(cfd);
   });

   cout << "after listen " << endl;

   return 0;  
}