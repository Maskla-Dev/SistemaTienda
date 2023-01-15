//
// Created by maskla on 26/11/22.
//

#include "Messages.h"
#include <sys/ipc.h>
#include <string.h>
#include "Utils.h"

MSGID getMsgid(char *name, int id) {
    if (strlen(name) == 0)
        return UNDEFINED_MSG;
    key_t msg_key = getKey(name, id);
    return msgget(msg_key, IPC_CREAT | MSG_PERMS);
}

int sendMsg(MSGID msg_id, char *message, long id) {
    MBUF buffer;
    memset(buffer.mText, 0, MAX_BUFFER_LENGTH);
    if (strlen(message) == 0 && msg_id == 0)
        return UNDEFINED_MSG;
    strcpy(buffer.mText, message);
    buffer.mType = id;
    return msgsnd(msg_id, &buffer, MAX_BUFFER_LENGTH, 0);
}

ssize_t readMsg(MSGID msg_id, MBUF *message_container, long expected_id) {
    if (msg_id == UNDEFINED_MSG)
        return 0;
    return msgrcv(msg_id, message_container, MAX_BUFFER_LENGTH, expected_id, 0);
}

int destroyMsgPipe(MSGID msg_id) {
    return msgctl(msg_id, IPC_RMID, NULL);
}