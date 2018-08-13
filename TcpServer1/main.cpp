/*
 * client               server
 * ---------------------------------
 *  socket()             socket()
 *                        bind()
 *                       listen()
 *                       accept()
 * --------------------------------
 *  connect()   ------>
 *  send()      ------>    recv()
 *  recv()      ------>    send()
 * --------------------------------
 *  close()     ------>    recv()
 *
 * eidt: 2018-08-12 22:41
 *
 * history:
 * 2018-08-13 20:33 pull and push to remote github.com
 *
*/

#include <stdio.h>
#include <iostream>

/* -----  includes ----- */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>        // bzero()
#include <string.h>         // memcpy()
#include <stdlib.h>         // exit()
#include <unistd.h>

#define SVR_PORT 2345       // port

using namespace std;

int main(int argc, char *argv[])
{
    cout << "argc = " << argc << endl;
    cout << "argv[0] = " << argv[0] << endl;

    // [step-1]: socket
    int sockfd, cnsockfd;
    struct sockaddr_in addr;
    int addrLen = sizeof(struct sockaddr_in);
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket created error!");
        exit(1);
    }
    else
    {
        printf("socket created successfully!\n");
        printf("socket id:%d\n", sockfd);
    }

    // [step-2]: bind
    bzero(&addr, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SVR_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sockfd, (struct sockaddr*)(&addr), sizeof(struct sockaddr)) < 0)
    {
        perror("bind error!");
        exit(1);
    }
    else
    {
        printf("bind port successfully!\n");
        printf("server local port:%d\n", SVR_PORT);
    }

    // [step-3]: listen
    if(listen(sockfd, 5) < 0)           // max 5 client accept, blocking...
    {
        perror("listen error!");
        exit(1);
    }
    else
    {
        printf("listenning ... \n");
    }


    // [step-4]: accept a remote connection request.
    if((cnsockfd = accept(sockfd, (struct sockaddr*)(&addr), (socklen_t*)&addrLen)) < 0)
    {
        perror("accept error!");
    }
    else
    {
        printf("accept a new connection.\n");
        printf("new socket id:%d\n", cnsockfd);
    }

    // [step-5]: recv and send
    char rcvbuf[200] = {0};
    char sndbuf[200] = {0};
    while(1)
    {
        int iResult = recv(cnsockfd, rcvbuf, sizeof(rcvbuf), 0);
        if(iResult > 0)
        {
            printf("recv %d bytes:%s\necho back...\n", iResult, rcvbuf);
            memcpy(sndbuf, rcvbuf, iResult);
            int iSnd = send(cnsockfd, sndbuf, iResult, 0);
            if(iSnd > 0)
            {
                printf("echo back successfully!\n");
            }
        }
        else if(iResult == 0)
        {
            printf("connecction closed!\n");
            exit(0);
        }
        else
        {
            printf("recv failed!\n");
        }
    }

    return 0;
}























