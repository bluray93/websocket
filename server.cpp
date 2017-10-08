#include "util.h"
using namespace std;

static int connFd;

void *rcvThread(void *dummyPt){
    cout << "Thread No: " << pthread_self() << endl;
    char test[MSG_SIZE];
    //bzero(test, MSG_SIZE+1);
    while(true) {
        bzero(test, MSG_SIZE+1);
        if(recv_msg(connFd, test, MSG_SIZE)<0) error("Cannot read");
        string tester (test);
        cout << tester << endl;
        if(tester == "exit") break;
        cout << "server" << endl;
    }
    cout << "\nClosing thread and conn" << endl;
    close(connFd);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    int pId, portNo, socketFd;
    socklen_t len; //store size of the address
    struct sockaddr_in svrAdd, clntAdd;
    pthread_t threadA;
    if (argc < 2) error("Syntam : ./server <port>");
    portNo = atoi(argv[1]);
    if((portNo > 65535) || (portNo < 2000)) error("Please enter a port number between 2000 - 65535");
    //create socket
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    int enable = 1;
    if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) error("setsockopt");
    if(socketFd < 0) error("Cannot open socket");

    bzero((char*) &svrAdd, sizeof(svrAdd));
    svrAdd.sin_family = AF_INET;
    svrAdd.sin_addr.s_addr = INADDR_ANY;
    svrAdd.sin_port = htons(portNo);

    //bind socket
    if(bind(socketFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0) error("Cannot bind");
    if (listen(socketFd, 5)<0) error("Cannot listen");
    len = sizeof(clntAdd);
    cout << "Listening" << endl;
    //this is where client connects. svr will hang in this mode until client conn
    if((connFd = accept(socketFd, (struct sockaddr *)&clntAdd, &len))<0) error("Cannot accept connection");
    else cout << "Connection successful" << endl;

    pthread_create(&threadA, NULL, rcvThread, NULL);

    while(true){
        char s[MSG_SIZE];
        //cin.clear();
        //cin.ignore(256, '\n');
        cout << "Enter stuff: ";
        bzero(s, MSG_SIZE+1);
        cin.getline(s, 300);
        if(send_msg(connFd, s)<0) error("Cannot write");
        cout << "fatto" << endl;
        string tester (s);
        if(tester == "exit") break;
    }
    pthread_join(threadA, NULL);
    return 0;
}
