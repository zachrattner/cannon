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

#include "msgq.h"

/* Initialize a message queue */
void msgq_init(msgq_type* msgq)
{
	uint8_t idx = 0;
	msgq->head  = 0;
	msgq->size  = 0;
	
	for (idx = 0; idx < MSGQ_LEN; idx++)
	{
		msgq->msgs[idx] = 0;
        msgq->cbs[idx]  = NULL;
	}
}

/* Push a message onto a given queue */
msgq_rsp_type msgq_push(msgq_type* msgq, uint16_t msg)
{
	return msgq_push_cb(msgq, msg, NULL);
}

/* Push a message onto the queue with a callback function to call once the 
   message is popped. */
msgq_rsp_type msgq_push_cb(msgq_type* msgq, uint16_t msg, msgq_cb_type cb)
{
	uint8_t idx;
	if (msgq->size >= MSGQ_LEN)
	{
		return MSGQ_S_FULL;
	}
	
	idx = (msgq->head + msgq->size) % MSGQ_LEN;
	msgq->msgs[idx] = msg;
	msgq->cbs[idx]  = cb;
	msgq->size++;
	return MSGQ_S_SUCCESS;
}

/* Pop a message off of a given queue. If the message has a callback, call 
   msgq_pop_cb instead. */
uint16_t msgq_pop(msgq_type* msgq, msgq_rsp_type* status)
{
	uint16_t msg;
	if (msgq->size <= 0)
	{
        if (status != NULL)
        {
    		*status = MSGQ_S_EMPTY;
	    }

    	return 0;
	}
	
    if (status != NULL)
    {
	    *status = MSGQ_S_SUCCESS;
	}

	msg = msgq->msgs[msgq->head];
	msgq->head = (msgq->head + 1) & (MSGQ_LEN - 1);
	msgq->size--;
	
	return msg;
}

/* Pop a message off a given queue and populate the callback field. */
uint16_t msgq_pop_cb(msgq_type* msgq, msgq_rsp_type* status, msgq_cb_type cb)
{
	uint16_t msg;
	if (msgq->size <= 0)
	{
        if (status != NULL)
        {
    		*status = MSGQ_S_EMPTY;
	    }

    	return 0;
	}
	
    if (status != NULL)
    {
	    *status = MSGQ_S_SUCCESS;
	}

	msg = msgq->msgs[msgq->head];
	cb  = msgq->cbs[msgq->head];
	msgq->head = (msgq->head + 1) % MSGQ_LEN;
	msgq->size--;
	
	return msg;
}

/* Wait for a message to be added to the queue */
void msgq_wait(msgq_type* msgq)
{
	while(!msgq->size);
}
