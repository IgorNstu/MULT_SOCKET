#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/poll.h>
#include <sys/epoll.h>
#include <sys/time.h>


int main(void)
{
    int id_soc[2],accept_id;
    int r,i;
    struct sockaddr_in server_address[2],user_address;
    int port[2];
    port[0]=6000;
    port[1]=6001;
    /*Задаём различные ответные сообщения для TCP и UDP сокета*/
    char msg1[]="Hi, i'm TCP-server";
    char msg2[]="Hi, i'm UDP-server";
    char msg3[1024];
    int msg1_len=sizeof(msg1);
    int msg2_len=sizeof(msg2);
    int msg3_len=sizeof(msg3);
    socklen_t user_len,server_len;
    user_len=sizeof(user_address);
    server_len=sizeof(server_address[0]);
    int optval =1;
    int optlen = sizeof(optval);
    id_soc[0]=socket(AF_INET,SOCK_STREAM,0);
    if (id_soc[0]==-1){
        perror("Создание сокета");
        return 0;
    }

    id_soc[1]=socket(AF_INET,SOCK_DGRAM,0);
    if (id_soc[1]==-1){
        perror("Создание сокета");
        return 0;
    }
    /*Создаём два сокета, заполняем структуры адреса для каждого, устанавливаем параметр и именнуем*/
    for (i=0;i<2;i++){
        server_address[i].sin_family = AF_INET;
        server_address[i].sin_port = htons(port[i]);
        server_address[i].sin_addr.s_addr=  htonl(INADDR_LOOPBACK);
    }

    for (i=0;i<2;i++){
        r=setsockopt(id_soc[i],SOL_SOCKET,SO_REUSEADDR,&optval,optlen);
        if (r==-1){
            perror("Установка параметра сокета ");
            close(id_soc[i]);
            return 0;
        }
    }

    for (i=0;i<2;i++){
        r = bind(id_soc[i], (struct sockaddr *)&server_address[i], server_len);
        if(r==-1){
            perror("Порт занят\n");
            close(id_soc[i]);
            return 0;
        }
    }
    listen(id_soc[0],10);
    listen(id_soc[1],10);
    printf("Ожидаю пользователей \n");
    struct pollfd p[2];
    p[0].fd=id_soc[0];
    p[1].fd=id_soc[1];
    p[0].events=POLLIN;
    p[1].events=POLLIN;
    /*Если пришёл сигнал для TCP сокета,то после отрабатывания poll
     * вызываем accept,приём и обратную отправку сообщения.Если для UDP,то приём и отправку.
     * После возвращаемся к ожиданию новых сигналов*/
    while(1){
        poll(p,2,-1);
        if(p[0].revents&POLLIN)
            {
            accept_id=accept(id_soc[0],0,0);
            recv(accept_id,msg3,msg3_len,0);
            printf("Я TCP,получил такое сообщение от клиента: %s\n",msg3);
            send(accept_id,msg1,msg1_len,0);
            close(accept_id);
        }
        if(p[1].revents&POLLIN)
            {
            recvfrom(id_soc[1],msg3,msg3_len,0,(struct sockaddr*)&user_address,&user_len);
            printf("Я UDP,получил такое сообщение от клиента: %s\n",msg3);
            sendto(id_soc[1],msg2,msg2_len,0,(struct sockaddr*)&user_address,user_len);

        }
    }
    return 0;
}

