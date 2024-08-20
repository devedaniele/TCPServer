#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <vector>

#ifndef ds_server
    #define ds_server

    using namespace std;

    class Server{
        public:
            int PORT;
            string IP;
            //vector<char> data;

            Server(){
                (*this).fd =  socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

                if ((*this).fd == -1){
                    cout  << "socket creation failed." << endl;
                    exit(-1);
                }

                (*this).serverAddress.sin_family = AF_INET;
            }

            Server(int domain,int type,int protocol){
                (*this).fd =  socket(domain,type,protocol);

                if ((*this).fd == -1){
                    cout  << "socket creation failed." << endl;
                    exit(-1);
                }

                (*this).serverAddress.sin_family = domain;
            }

            void listenTo(int n,function<void(Server&)> callback){
                int opt = 1;

                if (setsockopt((*this).fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(opt)) != 0){
                    cout << "setsockopt failed." << endl;
                    exit(-1);
                }

                (*this).serverAddress.sin_addr.s_addr = INADDR_ANY;
                (*this).serverAddress.sin_port = htons((*this).PORT);

                if (bind((*this).fd,(struct sockaddr*)&(*this).serverAddress,sizeof((*this).serverAddress)) != 0){
                    cout << "socket binding failed." << endl;
                    exit(-1);
                }

                (*this).addressSize = sizeof((*this).serverAddress);

                if (listen((*this).fd, n) != 0){
                    cout << "failed to listen." << endl;
                    exit(-1);
                }
                
                while(true){
                    (*this).c_fd = accept((*this).fd,(struct sockaddr*)&(*this).clientAddress,&(*this).addressSize);

                    if ((*this).c_fd == -1){
                        cout << "socket acceptation failed." << endl;
                        continue;
                    }
                    
                    callback((*this));
                }
            }

            int readData(){
                ssize_t bytesRead = read((*this).c_fd,buffer,1024);

                if (bytesRead == -1){
                    cout << "read error" << endl;

                    return -1;
                }

                cout << buffer << endl;

                return 0;
            }
            int sendString(string message){
                int byteSent = 0;
                int totalByteSent = 0;

                while(totalByteSent < message.size()){
                    byteSent = write((*this).c_fd,message.c_str(),message.size());

                    if (byteSent == -1){
                        return -1;
                    }

                    totalByteSent += byteSent;
                }

                return 0;
            }

            int closeClient(){
                return close((*this).c_fd);
            }
            int closeServer(){
                close((*this).c_fd);
                return close((*this).fd);
            }

            ~Server(){
                closeServer();
            }
        
        private:
            int fd;
            int c_fd;
            struct sockaddr_in serverAddress;
            struct sockaddr_in clientAddress;
            socklen_t addressSize;
            char buffer[1024];
    };

#endif
