#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <map>

#define MAXLINE 1024  
#define PORT 8001
using namespace std;

int main()
{

  int socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
  int number;
  socklen_t addressLength;
  char message[MAXLINE];
  map<string,string> table;
  // string message;

  struct sockaddr_in  serverAddress,clientAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr=INADDR_ANY;
  serverAddress.sin_port=htons(PORT);

  bind(socketDescriptor,(struct sockaddr*)&serverAddress,sizeof(serverAddress));

  std :: cout << "\nServer Started ...\n";

  while(1){
    std :: cout << "\n";
    addressLength = sizeof(clientAddress);

    number = recvfrom(socketDescriptor,message,MAXLINE,0,(struct sockaddr*)&clientAddress,&addressLength);

    std :: cout << "\n New Connection from client " << inet_ntoa(clientAddress.sin_addr) << ":"<< ntohs(clientAddress.sin_port) <<"\n "; 
    
    string username, cipher, msg;
    int i;
    for(i=0; message[i]!='_'; i++)
      username += message[i];
    i++;
    for(; message[i]!='_' || message[i]!='_'; i++)
      cipher += message[i];
    // cout << "Username : " << username << "\n";
    // cout << "Cipher Password : " << cipher << "\n";
    char choice = message[i+1];

    int f = 0;
    for(auto it : table){
      if(it.first == username)
      f = 1;
    }

    if(choice == '1') {
      if(f == 0) {
        table[username] = cipher;
        msg = "Account created\n";
        cout << "Username saved : " << username << "\n";
        cout << "Password ciphertext saved : " << cipher << "\n";
      }
      else {
        msg = "Username already exists. Please enter another Username!\n";
        cout << "Username already present : " << username << "\n";
      }
    }
    else {
      if(f == 0) {
        msg = "Username does not exist. Please Signing Up!\n";
        cout << "No such Username found!\n ";
      }
      else {
        if(table[username] == cipher) {
          msg = "Authentication Successful\n";
          cout << "Username found : " << username << "\n";
          cout << "Password ciphertext matched : " << cipher << "\n";
        }
        else {
          msg = "Authentication Failed\n";
          cout << "Username found : " << username << "\n";
          cout << "But the password ciphertext didn't match with the saved one : " << cipher << "\n";
        }
      }
    }

    for(i=0; i<msg.length(); i++)
      message[i] = msg[i];
    message[i] = '\0'; 

    if(number<1)
      perror("send error");

    sendto(socketDescriptor,message,number,0,(struct sockaddr*)&clientAddress,addressLength);
  }
  return 0;
}
