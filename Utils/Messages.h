//
// Created by maskla on 26/11/22.
//

#ifndef P2_P2_MESSAGES_H
#define P2_P2_MESSAGES_H

#include <sys/types.h>
#include <sys/msg.h>

#define UNDEFINED_MSG (-1)
#define MSG_PERMS 0666
#define MAX_BUFFER_LENGTH 2048
#define CANAL_MENSAJES_CLIENTE_ID "MensajeriaClientes\0"
#define CANAL_MENSAJES_TIENDA_ID "MensajeriaTienda\0"

typedef int MSGID;

typedef struct mbuf {
    long mType;
    char mText[MAX_BUFFER_LENGTH];
} MBUF;

MSGID getMsgid(char *name, int id);

int sendMsg(MSGID msg_id, char *message, long id);

ssize_t readMsg(MSGID msg_id, MBUF *message_container, long id);

int destroyMsgPipe(MSGID msg_id);

#endif //P2_P2_MESSAGES_H

