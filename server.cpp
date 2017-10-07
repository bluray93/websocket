#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
using namespace std;

static int connFd;

void *task1 (void *dummyPt){
    cout << "Thread No: " << pthread_self() << endl;
    char test[300];
    bzero(test, 301);
    while(true) {
        bzero(test, 301);
        read(connFd, test, 300);
        string tester (test);
        cout << tester << endl;
        if(tester == "exit") break;
    }
    cout << "\nClosing thread and conn" << endl;
    close(connFd);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    int pId, portNo, listenFd;
    socklen_t len; //store size of the address
    struct sockaddr_in svrAdd, clntAdd;
    pthread_t threadA;
    if (argc < 2){
        cerr << "Syntam : ./server <port>" << endl;
        return 0;
    }
    portNo = atoi(argv[1]);
    if((portNo > 65535) || (portNo < 2000)){
        cerr << "Please enter a port number between 2000 - 65535" << endl;
        return 0;
    }
    //create socket
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    int enable = 1;
    if (setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
      cerr << "setsockopt" << endl;
    }
    if(listenFd < 0){
      cerr << "Cannot open socket" << endl;
      return 0;
    }

    bzero((char*) &svrAdd, sizeof(svrAdd));
    svrAdd.sin_family = AF_INET;
    svrAdd.sin_addr.s_addr = INADDR_ANY;
    svrAdd.sin_port = htons(portNo);

    //bind socket
    if(bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0){
      cerr << "Cannot bind" << endl;
      return 0;
    }

    if (listen(listenFd, 5)<0){
      perror("listen");
      exit(EXIT_FAILURE);
    }
    len = sizeof(clntAdd);
    cout << "Listening" << endl;
    //this is where client connects. svr will hang in this mode until client conn
    if((connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len))<0){
      perror("accept");
      exit(EXIT_FAILURE);
    }
    if (connFd < 0){
        cerr << "Cannot accept connection" << endl;
        return 0;
    }
    else{
        cout << "Connection successful" << endl;
    }

    pthread_create(&threadA, NULL, task1, NULL);
    pthread_join(threadA, NULL);
    return 0;
}
