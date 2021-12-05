#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "rectangle.h"

#define MAXLINE 1024
#define PORT 8001
using namespace std;

int main()
{
  int serverDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
  int addressLength;
  
  char sendMessage[MAXLINE],recvMessage[MAXLINE];
  // string sendMessage,recvMessage;
  char choice;
  string msg, pswd;
  std :: cout<<"\n Press 1 to Create New User-Account or Press 2 to Login to Existing Account:";
  cin >> choice;

  if(choice == '1') {
    std :: cout<<"\n Enter New Username :";
    cin >> msg;
    std :: cout<<"\n Create a New Password :";
    cin >> pswd;
  }
  else {
    std :: cout<<"\n Enter Username :";
    cin >> msg;
    std :: cout<<"\n Enter Password :";
    cin >> pswd;
  }

  string cipher = encryptstr(pswd);
  int i;
  for(i=0; i<msg.length(); i++)
    sendMessage[i] = msg[i];
  sendMessage[i] = '_';

  i++;
  int ii = i;
  for(i=0; i<cipher.length(); i++)
    sendMessage[ii + i] = cipher[i];
  sendMessage[ii + i] = '_';
  sendMessage[ii + i + 1] = choice;
  sendMessage[ii + i + 2] = '\0';
  
  struct sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(PORT); 
 
  addressLength = sizeof(serverAddress);
 
  sendto(serverDescriptor,sendMessage,MAXLINE,0,(struct sockaddr*)&serverAddress,addressLength);
  recvfrom(serverDescriptor,recvMessage,MAXLINE,0,NULL,NULL);
 
  std :: cout<<"\nServer's Echo : "<< recvMessage << "\n";

  return 0;
}
