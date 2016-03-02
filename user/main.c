#include <stdio.h>
#include "ad.h"
#include <stdlib.h>
#include <unistd.h>

#define MY_DELAY 1
int main(void)
{
   int c;
   while (1) {
       /*Случайным образом создаём клиентов(TCP либо UDP) и соединяемся с сооветсвующими
        * сокетами на принимающей стороне*/
       c=rand()%2;
       if (c==1){
           create_udp_inet();
       }
       if (c==0){
           create_tcp_inet();
       }
       sleep(MY_DELAY);
   }
   return 0;
}

