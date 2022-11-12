#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <fcntl.h>
#include <locale.h>
#include <unistd.h>
#include <time.h>

#include "server.h"
#include "client.h"
#include "../Game/struct.h"
#include "../Game/game.h"
#include "../Game/map.h"

#ifdef WIN32
#elif defined (linux)
#endif

static void init(void)
{
   #ifdef WIN32
      WSADATA wsa;
      int err = WSAStartup(MAKEWORD(2, 2), &wsa);
      if(err < 0)
      {
         puts("WSAStartup failed !");
         exit(EXIT_FAILURE);
      }
   #endif
}

static void end(void)
{
   #ifdef WIN32
      WSACleanup();
   #endif
}

static void app(void)
{  
   Game *game = initGame(1);

   int MAX_CLIENTS = game->nbMaxPlayer;
   // sock = socket serveur
   SOCKET sock = init_connection(MAX_CLIENTS);
   char buffer[BUF_SIZE];
   /* the index for the array */
   int actual = 0;
   int max = sock;
   /* an array for all clients */
   Client clients[MAX_CLIENTS];

   fd_set rdfs;

    srand(time(NULL));
    int randomfilePath = -1;
    int previousFilePath = -1;
    int startOfMap = 1;
   int continuer = 1;

   while(continuer)
   {
      int i = 0;
      FD_ZERO(&rdfs);

      /* add STDIN_FILENO */
      #ifdef linux
         FD_SET(STDIN_FILENO, &rdfs);
      #endif
      /* add the connection socket */
      FD_SET(sock, &rdfs);

      /* add socket of each client */
      for(i = 0; i < actual; i++)
      {
         FD_SET(clients[i].sock, &rdfs);
      }
      sleep(3);
      //Select permet d'attendre que rdfs soit modifié (données en lecture possibles), et lorsque c'est le cas, la suite du programme se déroule
      if(select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
/**/     sleep(3);
         perror("select()");
         exit(errno);
      }
/**/  sleep(3);
      /* something from standard input : i.e keyboard 
      si le fichier modifié est STDIN_FILENO, cela veut dire que l'on a ecrit quelque chose dans la console serveur, ce qui est notre condition d'arret du serveur */
      #ifdef linux
      if(FD_ISSET(STDIN_FILENO, &rdfs))
      {
         // stop process when type on keyboard
         break;
      }
      else
      #endif
      /* si sock est un fichier modifié, cela veut dire qu'il a reçu une demande de connexion*/
      if(FD_ISSET(sock, &rdfs))
      {
         /* new client */
         SOCKADDR_IN csin = { 0 };
         size_t sinsize = sizeof csin;
         if (actual == MAX_CLIENTS){
            int csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
            write_client(csock, "Server full.\n");
            close(csock);
         } else {
            int csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
            if(csock == SOCKET_ERROR)
            {
               perror("accept()");
               continue;
            }

            /* after connecting the client sends its name */
            if(read_client(csock, buffer) == -1)
            {
               /* disconnected */
               continue;
            }

            /* what is the new maximum fd ? */
            max = csock > max ? csock : max;
            // on rajoute le client aux descripteurs surveillés en lecture dans le select
            FD_SET(csock, &rdfs);

            Client c = { csock };
            strncpy(c.name, buffer, BUF_SIZE - 1);
            clients[actual] = c;
            actual++;
            printf("Waiting for %d player(s).\n",MAX_CLIENTS-actual);
         }
      /* le fichier modifié n'est ni STDIN_FILLO, ni le socket serveur, c'est donc un socket client qui nous a envoyé un message */
      } else { //pas encore assez de joueurs connectés au serveur
         int i = 0;
         for(i = 0; i < actual; i++)
         {
            /* On vérifie quel client parle */
            if(FD_ISSET(clients[i].sock, &rdfs))
            {
               Client client = clients[i];
               int c = read_client(clients[i].sock, buffer);
               /* client disconnected */
               if(c == 0)
               {
                  closesocket(clients[i].sock);
                  remove_client(clients, i, &actual);
                  strncpy(buffer, client.name, BUF_SIZE - 1);
                  strncat(buffer, " disconnected !", BUF_SIZE - strlen(buffer) - 1);
                  //send_message_to_all_clients(clients, client, actual, buffer, 1);
               }
               else
               {
                  write_client(clients[i].sock, "wait for other players\n");
               }
               break;
            }
         }
      }
      while ((actual == MAX_CLIENTS) && continuer) // tous les joueurs nécessaires sont connectés au serveur
      {
         if (startOfMap){
            if (game->nbOfMapsAvailable == 1){
               randomfilePath = 0;
            } else {
               do{
                  randomfilePath = rand()%game->nbOfMapsAvailable;

               }while(randomfilePath == previousFilePath);
               previousFilePath = randomfilePath;
            }
            char *filePath = game->filePathMapChoosen[randomfilePath];

            Map * map = initMap(filePath, multiplayer, 0);
            game->currentMap = map;
            startOfMap = 0;
            char buffer1[1024];

            for (int i = 0; i < MAX_CLIENTS; i++){
               char temp[60];
               map->playerList[i]->socket = clients[i].sock;
               clients[i].playerToken = map->playerList[i]->token;
               sprintf(temp,"Player %c is %s.\n",clients[i].playerToken, clients[i].name);
               strcat(buffer1,temp);
            }
            #ifdef linux
            sendAll(map->playerList,actual,buffer1);
            sleep(5);
            #endif
         }
         Map * map = game->currentMap;
         char action;
         #ifdef WIN32
         system("cls");
         #else
         system("clear");
         #endif
        int turn = 0;
         Player * currentPlayer;

        do{
            if (map->playerList[turn%map->nbPlayers]->status == isDead){
                turn++;
            } /* elsif player->isBot == true
                    botTurn() --Algo IA
                    turn++ */
            else {
               #ifdef WIN32
               system("cls");
               #else
               system("clear");
               #endif
               afficherMap(map);
               sendAll(map->playerList, MAX_CLIENTS,serializeMap(map));
               sleep(0.1);
               char buffer[1024];
                currentPlayer = map->playerList[turn%map->nbPlayers];
                for (int i = 0; i < MAX_CLIENTS;i++){
                  if (map->playerList[i] == currentPlayer){
                     sprintf(buffer,"\nPlayer %c status:\nMax bomb: %d / Fire Pwr: %d\nPasseBomb: %d / BombKick: %d\nInvincibility: %d turn(s)\nHeart Shield: %d / Life(s): %d\n", currentPlayer->token, currentPlayer->bombMax, currentPlayer->firePower, currentPlayer->passBombs, currentPlayer->bombKick, currentPlayer->invincibilityTime, currentPlayer->heart, currentPlayer->life);
                     write_client(currentPlayer->socket,buffer);
                  } else {
                     sprintf(buffer,"\nWaiting for player %c to play.\nPlayer %c status:\nMax bomb: %d / Fire Pwr: %d\nPasseBomb: %d / BombKick: %d\nInvincibility: %d turn(s)\nHeart Shield: %d / Life(s): %d\n", currentPlayer->token,currentPlayer->token, currentPlayer->bombMax, currentPlayer->firePower, currentPlayer->passBombs, currentPlayer->bombKick, currentPlayer->invincibilityTime, currentPlayer->heart, currentPlayer->life);
                     write_client(map->playerList[i]->socket,buffer);
                  }
                }
                sleep(1);
            sprintf(buffer,"Do something, Player %c:\n",currentPlayer->token);
            write_client(currentPlayer->socket,buffer);
            int currentPlayerhasPlayed = 0;
            while(currentPlayerhasPlayed == 0)
            {
               FD_ZERO(&rdfs);
               #ifdef linux
               FD_SET(STDIN_FILENO, &rdfs);
               #endif
               /* add socket of each client */
               for(int i = 0; i < actual; i++)
               {
                  FD_SET(clients[i].sock, &rdfs);
                  printf("client%d %s\n",i,clients[i].name);
               }

               if(select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
               {
                  perror("select()");
                  exit(errno);
               }
               
               #ifdef linux
               if(FD_ISSET(STDIN_FILENO, &rdfs))
               {
                  // stop process when type on keyboard
                  continuer = 0;
                  return ;
               }
               #endif

               for(int i = 0; i < actual; i++)
               {
                  /* On vérifie quel client parle */
                  if(FD_ISSET(clients[i].sock, &rdfs))
                  {
                     printf("client qui parle:%s\n",clients[i].name);
                     int c = read_client(clients[i].sock, buffer);
                     /* client disconnected */
                     if(c == 0)
                     {  
                        for (int i = 0; i < MAX_CLIENTS; i++){
                           if (map->playerList[i]->socket == clients[i].sock){
                              map->playerList[i]->status = isDead;
                           }
                        }
                        closesocket(clients[i].sock);
                        remove_client(clients, i, &actual);
                        strncpy(buffer, clients[i].name, BUF_SIZE - 1);
                        strncat(buffer, " disconnected !", BUF_SIZE - strlen(buffer) - 1);
                        //send_message_to_all_clients(clients, client, actual, buffer, 1);
                     }
                     else if (clients[i].sock != currentPlayer->socket)
                     {
                        write_client(clients[i].sock, "wait for your turn.\n");
                     }
                     else {
                        action = buffer[0];
                        if (keyHandler(action, map, currentPlayer) == 0){
                           currentPlayerhasPlayed = 1;
                        } else {
                           write_client(clients[i].sock, "Move impossible, please make another action.\n");
                        }
                     }
                     break;
                  }
               }
            }
            updateTimerBomb(map, currentPlayer);
            if (map->pause){
               sleep(3);
               map->pause = 0;
            }
            turn++;
            if (currentPlayer->invincibilityTime != 0){
                currentPlayer->invincibilityTime--;
            }
            }
        } while (map->nbPlayerAlive > 1);

        sendAll(map->playerList,map->nbPlayers, "Do you wish to continue ? y/n\n");
        printf("Continue send to all\n");
        int allAnswered = 0;
        int playersWhoHaveReplied = 0;
        int playerRepliedId[4];
        while(!allAnswered && continuer){
         FD_ZERO(&rdfs);
         printf("nbAnswer: %d\n",playersWhoHaveReplied);
                  /* add socket of each client */
                  for(int i = 0; i < actual; i++)
                  {
                     FD_SET(clients[i].sock, &rdfs);
                     printf("client%d %s\n",i,clients[i].name);
                  }

                  if(select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
                  {
                     perror("select()");
                     exit(errno);
                  }

                  for(int i = 0; i < actual; i++)
                  {
                     /* On vérifie quel client parle */
                     if(FD_ISSET(clients[i].sock, &rdfs))
                     {
                        int c = read_client(clients[i].sock, buffer);
                        /* client disconnected */
                        if(c == 0 || buffer[0] == 'n')
                        {
                           continuer = 0;
                           break;
                        }
                        if (buffer[0] == 'y'){
                           int j = 0;
                           for (j = 0; j < playersWhoHaveReplied; j++){
                              if (playerRepliedId[j] == i){
                                 break;
                              }
                           }
                           if (j == playersWhoHaveReplied){
                              playerRepliedId[playersWhoHaveReplied] = i;
                              playersWhoHaveReplied++;
                           }
                        }
                        if (playersWhoHaveReplied == MAX_CLIENTS){
                           allAnswered = 1;
                        }
                     }
            }
         }
         startOfMap = 1;
      }
   }

   if (actual == MAX_CLIENTS){
      sendAll(game->currentMap->playerList, actual, "Thank you for playing. See you soon !\n");
      sleep(3);
      for (int i = 0; i < game->currentMap->nbPlayers; i++){
            free(game->currentMap->playerList[i]);
        }
        free(game->currentMap->playerList);
        for (int i = 0; i < game->currentMap->nbBombsOnMap; i++){
            free(game->currentMap->bombList[i]);
        }
        free(game->currentMap->bombList);
        free(game->currentMap);
        free(game);
   }
   clear_clients(clients, actual);
   end_connection(sock);
}

static void clear_clients(Client *clients, int actual)
{
   int i = 0;
   for(i = 0; i < actual; i++)
   {
      closesocket(clients[i].sock);
   }
}

static void remove_client(Client *clients, int to_remove, int *actual)
{
   /* we remove the client in the array */
   memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(Client));
   /* number client - 1 */
   (*actual)--;
}

void sendAll(Player **clients, int actual, const char *buffer){
   //printf("%s",buffer);
   int i = 0;
   char message[BUF_SIZE];
   message[0] = 0;
   strncat(message, buffer, sizeof message - strlen(message) - 1);
   for (int i = 0; i < actual; i++){
      write_client(clients[i]->socket, message);
   }
}

static void send_message_to_all_clients(Client *clients, Client sender, int actual, const char *buffer, char from_server)
{
   int i = 0;
   char message[BUF_SIZE];
   message[0] = 0;
   for(i = 0; i < actual; i++)
   {
      /* we don't send message to the sender */
      if(sender.sock != clients[i].sock)
      {
         if(from_server == 0)
         {
            strncpy(message, sender.name, BUF_SIZE - 1);
            strncat(message, " : ", sizeof message - strlen(message) - 1);
         }
         strncat(message, buffer, sizeof message - strlen(message) - 1);
         write_client(clients[i].sock, message);
      }
   }
}

//créé la socket serveur + liste d'attente de connexion associée
static int init_connection(const int MAX_CLIENTS)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = { 0 };

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
   {
      perror("bind()");
      exit(errno);
   }

   // créé une liste d attente de taille MAX_CLIENT, souhaitant se connecter à sock (serveur)
   if(listen(sock, MAX_CLIENTS) == SOCKET_ERROR)
   {
      perror("listen()");
      exit(errno);
   }

   return sock;
}

static void end_connection(int sock)
{
   closesocket(sock);
}

static int read_client(SOCKET sock, char *buffer)
{
   int n = 0;

   if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      /* if recv error we disonnect the client */
      n = 0;
   }

   buffer[n] = 0;

   return n;
}

static void write_client(SOCKET sock, const char *buffer)
{
   if(send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}

int launchServer()
{
   init();
   app();
   end();
   sleep(3);

   return EXIT_SUCCESS;
}