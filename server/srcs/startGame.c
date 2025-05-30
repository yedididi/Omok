#include "../incs/define.h"

int startGame(int sfd_server, t_info **info, int *playerNum, int *gameStatus, int *sfd_clients)
{
    int sfd_client;
    struct sockaddr_in addr_client;
    socklen_t addr_client_len;

    printf("[%d] waiting for client ...\n", getpid());
    addr_client_len = sizeof(addr_client);
    sfd_client = accept(sfd_server, (struct sockaddr *)&addr_client, &addr_client_len);
    if(sfd_client == -1) {
        printf("[%d] error: %s (%d)\n", getpid(), strerror(errno), __LINE__);
        return EXIT_FAILURE;
    }
    *sfd_clients = sfd_client;
    
    printf("[%d] connected\n", getpid());

    printf("sfd_client:%d\n", sfd_client);
    if (fillInfo(&info[sfd_client], playerNum, sfd_client, gameStatus) == EXIT_FAILURE)
        return (EXIT_FAILURE);
    if (info[sfd_client]->gameInfo == NULL)
        printf("info[sfd_client]->gameInfo is NULL\n");
    else
        printf("right after fillInfo, gameInfo malloc ok\n");

    pthread_mutex_lock(&(info[sfd_client]->playerNumMuxtex));
    (*playerNum)++;
    printf("player Num increased to %d\n", *playerNum);
    pthread_mutex_unlock(&(info[sfd_client]->playerNumMuxtex));

    if (*playerNum == 1)
        info[sfd_client]->whichClient = C1;
    else if (*playerNum == 2)
        info[sfd_client]->whichClient = C2;


    printf("[%d] creating thread\n", getpid());
    int ret = pthread_create(&((info[sfd_client])->thread_id), NULL, &startThread, info[sfd_client]);
    if(ret != 0) 
    {
        printf("[%d] error: %d (%d)\n", getpid(), ret, __LINE__);
        return EXIT_FAILURE;
    }

    return (EXIT_SUCCESS);

}