#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>

using namespace std;

class Server{
    public:
        int PORT;
        string IP;

        Server(){
            PORT = 8080;
            IP = "127.0.0.1";
        }

        int createSocket(){
            server =  socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

            if (server == -1){
                cout  << "socket creation failed." << endl;

                return -1;
            }

            return 0;
        }

        int bindSocket(){
			if (bind(server,(struct sockaddr*)&serverAddress,sizeof(serverAddress)) != 0){
				cout << "socket binding failed." << endl;

				return -1;
			}

            addressSize = sizeof(serverAddress);

			return 0;
		}

        int listenTo(int n){
			if (listen(server, n) != 0){
				cout << "failed to listen." << endl;

				return -1;
			}

			return 0;
		}

        int acceptSocket(){
			client = accept(server,(struct sockaddr*)&serverAddress,&addressSize);

			if (client == -1){
				cout << "socket acceptation failed." << endl;

				return -1;
			}

			return 0;
		}

        int setupServer(){
            if (createSocket() == -1){				
				return -1;
			}

            int opt = 1;

            if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(opt)) != 0){
				cout << "setsockopt failed." << endl;

				return -1;
			}

            serverAddress.sin_family = AF_INET;
			serverAddress.sin_addr.s_addr = inet_addr(IP.c_str());
			serverAddress.sin_port = htons(PORT);

            return bindSocket();;
        }

        int readData(){
			ssize_t bytesRead = read(client,buffer,1024);

            if (bytesRead == -1){
                return -1;
            }

            cout << buffer << endl;

            return 0;
		}

        int sendString(string message){
			int byteSent = 0;
            int totalByteSent = 0;

            while(totalByteSent < message.size()){
                byteSent = write(client,message.c_str(),message.size());

                if (byteSent == -1){
                    return -1;
                }

                totalByteSent += byteSent;
            }

            return 0;
		}

        int closeClient(){
            return close(client);
        }

        void closeServer(){
			close(client);
			close(server);
		}

        ~Server(){
			closeServer();
		}
    
    private:
        int server;
        int client;
        struct sockaddr_in serverAddress;
        socklen_t addressSize;
        char buffer[1024];
};