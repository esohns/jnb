
#ifndef __NET_H
#define __NET_H

/*#ifdef __cplusplus
extern "C" {
#endif*/

#ifdef USE_SDL
#include <SDL_net.h>
#endif /* USE_SDL */

#include "globals.h"

extern int server_said_bye;
extern int buggered_off;
extern int client_player_num;
extern int is_server;
extern int is_net;

extern TCPsocket sock;
extern SDLNet_SocketSet socketset;
struct NetInfo
{
	TCPsocket        sock;
	IPaddress        addr;
	SDLNet_SocketSet socketset;
};
extern struct NetInfo net_info[JNB_MAX_PLAYERS];

struct NetPacket
{
	unsigned long cmd;
	long          arg;
	long          arg2;
	long          arg3;
	long          arg4;
};

#define NETPKTBUFSIZE (4 + 4 + 4 + 4 + 4)

#define NETCMD_NACK       (0xF00DF00D + 0)
#define NETCMD_ACK        (0xF00DF00D + 1)
#define NETCMD_HELLO      (0xF00DF00D + 2)
#define NETCMD_GREENLIGHT (0xF00DF00D + 3)
#define NETCMD_MOVE       (0xF00DF00D + 4)
#define NETCMD_BYE        (0xF00DF00D + 5)
#define NETCMD_POSITION   (0xF00DF00D + 6)
#define NETCMD_ALIVE      (0xF00DF00D + 7)
#define NETCMD_KILL       (0xF00DF00D + 8)
#define NETCMD_ACK2       (0xF00DF00D + 9)

void tellServerPlayerMoved(int,  /* player id */
                           int,  /* movement type */
                           int); /* new value */
void bufToPacket(const char*,         /* buffer */
                 struct NetPacket*);  /* packet */
void packetToBuf(const struct NetPacket*, /* packet */
                 char*);                  /* buffer */
void sendPacketToSock(TCPsocket,          /* socket */
                      struct NetPacket*); /* packet */
void sendPacket(int,                /* player id */
                struct NetPacket*); /* packet */
void sendPacketToAll(struct NetPacket*); /* packet */
int grabPacket(TCPsocket,          /* socket */
               SDLNet_SocketSet,   /* socket */
               struct NetPacket*); /* packet */
int serverRecvPacket(struct NetPacket*); /* packet */
void wait_for_greenlight();
void tellServerGoodbye();
void processMovePacket(struct NetPacket*); /* packet */
void tellServerPlayerMoved(int,  /* player id */
                           int,  /* movement type */
                           int); /* newval */
void tellServerNewPosition();
void processKillPacket(struct NetPacket*); /* packet */
void processPositionPacket(struct NetPacket*); /* packet */
void processAlivePacket(struct NetPacket*); /* packet */
void serverTellEveryoneGoodbye();
int update_players_from_server();
void serverSendAlive(int); /* player id */
void serverSendKillPacket(int,  /* killer */
                          int,  /* victim */
													int); /* is_ack */
void update_players_from_clients();
void init_server(const char*); /* netarg */
void connect_to_server(char*); /* netarg */

/*#ifdef __cplusplus
}
#endif*/

#endif /* __NET_H */

