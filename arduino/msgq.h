/******************************************************************************
* This file is part of Cannon, a Bluetooth-enabled pneumatic cannon developed 
* by Qualcomm engineers representing Virginia Tech.
*
* The Cannon team:
*     Doug DeCarme:   Android application developer
*     Shaver Deyerle: Hardware developer
*     Zach Rattner:   Arduino developer
*
* Cannon was released in July 2011.
*
* Cannon is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Cannon is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Cannon.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef MSGQ_H
#define MSGQ_H

#include "com_inc.h"

/* Maximum number of elements in a message queue */
#define MSGQ_LEN 128

/* Message queue callback function type */
typedef void(*msgq_cb_type)(void);

/* Message queue type */
typedef struct
{
	/* Index of the first element of the queue */
	uint8_t head;
	
	/* Size of the queue */
	uint8_t size;
	
	/* Messages */
	uint16_t msgs[MSGQ_LEN];

	/* Callbacks to be executed after the message is processed */
	msgq_cb_type cbs[MSGQ_LEN];
} msgq_type;

/* Status indicators that can be returned from a push/pope operation */
typedef enum
{
	/* Indicates a successful operation */
	MSGQ_S_SUCCESS,
	
	/* Indicates there was a problem reading */
	MSGQ_S_READ_ERROR,
	
	/* Indicates there was a problem writing */
	MSGQ_S_WRITE_ERROR,
	
	/* Indicates a full queue, write ignored */
	MSGQ_S_FULL,
	
	// Indicates an empty queue, read ignored */
	MSGQ_S_EMPTY
} msgq_rsp_type;

/* Initialize a message queue to default values */
void msgq_init(msgq_type* msgq);

/* Push a message onto a given queue. The return value indicates success or 
   failure. */
msgq_rsp_type msgq_push(msgq_type* msgq, uint16_t msg);

/* Push a message onto a given queue with a callback. Return value indicates 
   success or failure. */
msgq_rsp_type msgq_push_cb(msgq_type* msgq, uint16_t msg, msgq_cb_type cb);

/* Pop a message off of a given queue. Status parameter indicates success or 
   failure. If status is not MSGQ_S_SUCCESS, then the returned message is 0. */
uint16_t msgq_pop(msgq_type* msgq, msgq_rsp_type* status);

/* Same as msgq_pop, but the cb parameter is populated with the callback 
   function, which may be null */
uint16_t msgq_pop_cb(msgq_type* msgq, msgq_rsp_type* status, msgq_cb_type cb);

/* Wait for a message to be added to the queue */
void msgq_wait(msgq_type* msgq);

#endif /* !MSGQ_H */
