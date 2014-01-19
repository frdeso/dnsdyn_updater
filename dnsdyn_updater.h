#ifndef _DNSDYN_UPDATER_H_
#define _DNSDYN_UPDATER_H_
#include <stdio.h>
#include <string.h>
#include <netdb.h>

#include "dnsdyn_updater.h"

typedef struct info_t{
  char hostname[100];
  char credential[100];
  char ip[20];
} dnsdyn_updater_info;

void dnsdyn_updater_init_info(dnsdyn_updater_info* i, char* host, char *cred, char *ip);

int dnsdyn_updater_set_ip(dnsdyn_updater_info i);


//////////////////////////////////////////////////////////////////
//
//	IMPLEMENTATION SECTION
//
//////////////////////////////////////////////////////////////////

void dnsdyn_updater_init_info(dnsdyn_updater_info* i, char* host, char *cred, char *ip){
  strcpy(i->hostname,host);
  strcpy(i->credential,cred);
  strcpy(i->ip,ip);
}

int dnsdyn_updater_set_ip(dnsdyn_updater_info i){

  char host[100],addrstr[100], message[500] , server_reply[500];
  strcpy(host,"www.dnsdynamic.org");
  struct addrinfo hints, *res;
  int err, sock;
  void *ptr;
  struct sockaddr_in server;


  memset (&hints, 0, sizeof (hints));
  hints.ai_family = PF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags |= AI_CANONNAME;

  err = getaddrinfo (host, NULL, &hints, &res);
  if (err != 0) {
    fprintf(stderr,"getaddrinfo failed. Unable to get host IP address.");
    return -1;
  }

  inet_ntop (res->ai_family, res->ai_addr->sa_data, addrstr, 100);

  ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;


  inet_ntop (res->ai_family, ptr, addrstr, 100);

  sock = socket(AF_INET , SOCK_STREAM , 0);
  if (sock == -1)
  {
    printf("Could not create socket");
  }

  server.sin_addr.s_addr = inet_addr(addrstr);
  server.sin_family = AF_INET;
  server.sin_port = htons( 80 );

  if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
  {
    fprintf(stderr,"connect failed. Error");
    return -1;
  }


  sprintf(message,"GET /api/?hostname=%s&myip=%s HTTP/1.1\r\n",i.hostname,i.ip);
  sprintf(message+strlen(message),"User-Agent: DNSDyn_Updater 1.0\r\n");
  sprintf(message+strlen(message),"Accept: */*\r\n");
  sprintf(message+strlen(message),"Host: %s\r\n", host);
  sprintf(message+strlen(message),"Connection: Keep-Alive\r\n");
  sprintf(message+strlen(message),"Authorization: Basic %s\r\n\r\n",i.credential);
 //ipntf("%s",message);

  if( send(sock , message , strlen(message) , 0) < 0) {
    fprintf(stderr,"Send failed.");
    return -1;
  }

  if( recv(sock , server_reply , 500 , 0) < 0) {
    fprintf(stderr,"Receive failed.");
    return -1;
  }

  close(sock);
  return 0;
}
#endif // _DNSDYN_UPDATER_H_