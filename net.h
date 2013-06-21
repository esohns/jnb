#include "globals.h"

#include "SDL_net.h"

static int server_said_bye = 0;
static int buggered_off = 0;
static int client_player_num = -1;
static int is_server = 1;
static int is_net = 0;

static TCPsocket sock = NULL;
static SDLNet_SocketSet socketset = NULL;

struct NetInfo
{
	TCPsocket        sock;
	IPaddress        addr;
	SDLNet_SocketSet socketset;
};
struct NetInfo net_info[JNB_MAX_PLAYERS];

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

void tellServerPlayerMoved(int,  // player id
                           int,  // movement type
                           int); // new value
void bufToPacket(const char*,         // buffer
                 struct NetPacket*);  // packet
void packetToBuf(const struct NetPacket*, // packet
                 char*);                  // buffer
void sendPacketToSock(TCPsocket,          // socket
                      struct NetPacket*); // packet
void sendPacket(int,                // player id
                struct NetPacket*); // packet
void sendPacketToAll(struct NetPacket*); // packet
int grabPacket(TCPsocket,          // socket
               SDLNet_SocketSet,   // socket
               struct NetPacket*); // packet
int serverRecvPacket(struct NetPacket*); // packet
void wait_for_greenlight();
void tellServerGoodbye();
void processMovePacket(struct NetPacket*); // packet
void tellServerPlayerMoved(int,  // player id
                           int,  // movement type
                           int); // newval
void tellServerNewPosition();
void processKillPacket(struct NetPacket*); // packet
void processPositionPacket(struct NetPacket*); // packet
void processAlivePacket(struct NetPacket*); // packet
void serverTellEveryoneGoodbye();
int update_players_from_server();
void serverSendAlive(int); // player id
void serverSendKillPacket(int,  // killer
                          int); // victim
void update_players_from_clients();
void init_server(const char*); // netarg
void connect_to_server(char*); // netarg
