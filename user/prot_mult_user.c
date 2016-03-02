#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>

int create_tcp_inet(void){
    int id_soc;

    char msg[] = "Hello,Server!!!";
    int msg_len=sizeof(msg);
    char msg2[1024];
    int msg2_len= sizeof(msg2);
    struct sockaddr_in server_address;
    int server_len;
    id_soc=socket(AF_INET,SOCK_STREAM,0);

    if (id_soc==-1){
        perror("Создание сокета\n");
        close(id_soc);
        return 1;
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(6000);
    server_address.sin_addr.s_addr= htonl(INADDR_LOOPBACK);
    server_len = sizeof(server_address);
    int t = connect(id_soc,(struct sockaddr*)&server_address,server_len);
    if (t==-1){
            perror("Коннект\n");
            close(id_soc);
            return 1;
        }

     send(id_soc,msg,msg_len,0);
     recv(id_soc,msg2,msg2_len,0);
     printf("Я клиент,получил эту строку:%s\n",msg2);
     close(id_soc);
     return 0;

}


int create_udp_inet(void){
    int id_soc,r;

    char msg[] = "Hello,Server!!!";
    int msg_len=sizeof(msg);
    char msg2[1024];
    int msg2_len= sizeof(msg2);
    struct sockaddr_in server_address;
    int server_len;
    id_soc=socket(AF_INET,SOCK_DGRAM,0);

    if (id_soc==-1){
        perror("Создание сокета\n");
        close(id_soc);
        return 1;
    }

    /*Заполняем структуру с адрессом сервера*/
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(6001);
    server_address.sin_addr.s_addr= htonl(INADDR_LOOPBACK);
    server_len = sizeof(server_address);

    r=sendto(id_soc,msg,msg_len,0,(struct sockaddr*)&server_address,(socklen_t)server_len);
    if (r<0) {
        perror("Отправка сообщения");
        close(id_soc);
        return 1;
    }

    printf("Я клиент,отправил эту строку серверу:%s\n",msg);
    recvfrom(id_soc,msg2,msg2_len,0,0,0);
    printf("Я клиент,получил эту строку:%s\n",msg2);
    close(id_soc);
    return 0;
}
