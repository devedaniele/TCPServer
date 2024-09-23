#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "TP.h"

#ifndef ds_server
    #define ds_server

    using namespace std;

    class Server{
        using Callback = function<void(const int&)>;
        
        public:
            int MAX_CLIENTS;
            
            Server(int domain){
                fd =  socket(domain,SOCK_STREAM,IPPROTO_TCP);

                if (fd == -1){
                    cout  << "socket creation failed." << endl;
                    exit(-1);
                }
                
                int opt = 1;

                if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(opt)) != 0){
                    cout << "setsockopt failed." << endl;
                    exit(-1);
                }

                serverAddress.sin_family = domain;
                serverAddress.sin_addr.s_addr = INADDR_ANY;
                
                MAX_CLIENTS = 5;
            }

            void listenTo(int port,Callback cb){
                serverAddress.sin_port = htons(port);

                if (bind(fd,(struct sockaddr*)&serverAddress,sizeof(serverAddress)) != 0){
                    cout << "socket binding failed." << endl;
                    exit(-1);
                }

                if (listen(fd, MAX_CLIENTS) != 0){
                    cout << "failed to listen." << endl;
                    exit(-1);
                }
                
                srvq.changeTPSize(MAX_CLIENTS);

                _listen(cb);
            }
            
            ~Server(){
                close(fd);
            }
        
        private:
            int fd;
            struct sockaddr_in serverAddress;
            
            TPQueue srvq;
            
            void _listen(Callback &cb){
                while(true){
                    struct sockaddr_in clientAddress;
                    socklen_t addressSize = sizeof(clientAddress);
                    
                    int cfd = accept(fd,(struct sockaddr*)&clientAddress,&addressSize);

                    if (cfd == -1)
                        continue;
                                        
                    srvq.enqueue([cb,cfd](){
                        cb(cfd);
                    });
                }
            }
    };

#endif
