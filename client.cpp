#include "util.h"
using namespace std;

static int socketFd;

void *rcvThread(void *dummyPt){
    cout << "Thread No: " << pthread_self() << endl;
    char test[MSG_SIZE];
    //bzero(test, 301);
    while(true) {
        bzero(test, MSG_SIZE+1);
        if(recv_msg(socketFd, test, MSG_SIZE)<0) error("Cannot read");
        string tester (test);
        cout << tester << endl;
        if(tester == "exit") break;
        cout << "client" << endl;
    }
    cout << "\nClosing thread and conn" << endl;
    pthread_exit(NULL);
}

int main (int argc, char* argv[]){
    int portNo;
    struct sockaddr_in svrAdd;
    struct hostent *server;
    pthread_t threadA;
    if(argc < 3) error("Syntax : ./client <host name> <port>");
    portNo = atoi(argv[2]);
    if((portNo > 65535) || (portNo < 2000)) error("Please enter port number between 2000 - 65535");
    //create client skt
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFd < 0) error("Cannot open socket");
    server = gethostbyname(argv[1]);
    if(server == NULL) error("Host does not exist");
    bzero((char *) &svrAdd, sizeof(svrAdd));
    svrAdd.sin_family = AF_INET;
    bcopy((char *) server -> h_addr, (char *) &svrAdd.sin_addr.s_addr, server -> h_length);
    svrAdd.sin_port = htons(portNo);
    if(connect(socketFd,(struct sockaddr *) &svrAdd, sizeof(svrAdd))<0) error("Cannot connect!");

    pthread_create(&threadA, NULL, rcvThread, NULL);

    //send stuff to server
    while(true){
        char s[MSG_SIZE];
        //cin.clear();
        //cin.ignore(256, '\n');
        cout << "Enter stuff: ";
        bzero(s, MSG_SIZE+1);
        cin.getline(s, MSG_SIZE);
        if(send_msg(socketFd, s)<0) error("Cannot write");
        string tester (s);
        if(tester == "exit") break;
    }

    pthread_join(threadA, NULL);
    return 0;
}
