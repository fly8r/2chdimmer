/*
 * messages.h
 *
 * Created: 23.01.2019 10:44:16
 *  Author: fly8r
 */
#ifndef MESSAGES_H_
#define MESSAGES_H_

#include <stdio.h>
#include <util/atomic.h>

#define USE_BC_MESSAGES

// Messages queue deep
#define	MAX_MESSAGES				3
// Message definitions
enum MSG_ENUM
{
	MSG_SYSTEM=0,
	MSG_ENC_ROTATE,
	MSG_BTN_KEY_PRESSED,
};

#ifdef USE_BC_MESSAGES
#define MAX_BC_MESSAGES 		3

enum MSG_BC_ENUM
{
	MSG_BC_SYSTEM_STARTUP_RUN=0,
	MSG_BC_SYSTEM_STARTUP_COMPLETE,
	MSG_BC_PCF8574_INIT_CMD_SEND_COMPLETE,
};

#endif

#define MSG_STATE_INACTIVE			0
#define MSG_STATE_SET				1
#define MSG_STATE_ACTIVE			2


// Messages process function
void		InitMessages(void);
void		ProcessMessages(void);
void		SendMessageWParam(uint8_t Msg, void * pParams);
void		SendMessageWOParam(uint8_t Msg);
uint8_t		GetMessage(uint8_t Msg);
void		*GetMessageParam(uint8_t Msg);
#ifdef USE_BC_MESSAGES
	void	SendBCMessage(uint8_t Msg);
	uint8_t GetBCMessage(uint8_t Msg);
#endif


#endif /* MESSAGES_H_ */