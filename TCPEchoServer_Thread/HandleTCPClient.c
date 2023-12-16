#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <netinet/in.h>  // for ntohl()
#include "../common/ccLemon.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */

void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    ssize_t recvMsgSize;                    /* Size of received message */
    int receivedInt;
    player p;

    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, &p, sizeof(player), 0)) < 0)
        DieWithError("recv() failed");

    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0)      /* zero indicates end of transmission */
    {
        printf("Received: %d\n", p.cmd);
        p.status = 200;
        
        /* Echo message back to client */
        if (send(clntSocket, &p, recvMsgSize, 0) != recvMsgSize)
            DieWithError("send() failed");

        /* See if there is more data to receive */
        if ((recvMsgSize = recv(clntSocket, &p, sizeof(player), 0)) < 0)
            DieWithError("recv() failed");
    }

    close(clntSocket);    /* Close client socket */
}
