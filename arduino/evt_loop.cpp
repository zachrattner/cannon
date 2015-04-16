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

#include <WProgram.h>
#include "com_inc.h"
#include "evt_loop.h"
#include "bt.h"
#include "msgq.h"
#include "relay.h"

/* Timer period, in microseconds */
#define TIMER_DEFAULT_PERIOD 1000000

/* Bluetooth message queue */
static msgq_type bt_q; 

/* Initialize the main program's event loop */
void evt_loop_init()
{
    relay_init();
    bt_init(&bt_q, NULL);    
}

/* Process the data continuously */
void evt_loop_proc()
{
    uint16_t msg;
    msgq_rsp_type rsp;
    
    /* Sit here until a message is received from the Bluetooth queue */
    bt_wait_for_msgs();    
    msg = msgq_pop(&bt_q, &rsp);
    
    /* Reset the queue if there was a problem */
    if (rsp != MSGQ_S_SUCCESS)
    {
        bt_init(&bt_q, NULL);
        return;
    }
    
    /* Open the appropriate relay, and start the timer to close it 
       automatically one second later. */
    if (msg == '1')
    {
        relay_set_state(RELAY_LEFT_PIN, RELAY_OPEN);       
        timer_init(TIMER_DEFAULT_PERIOD);
        timer_set_cb(&evt_loop_handle_timer);
    }
    else if (msg == '2')
    {
        relay_set_state(RELAY_RIGHT_PIN, RELAY_OPEN);       
        timer_init(TIMER_DEFAULT_PERIOD);
        timer_set_cb(&evt_loop_handle_timer);   
    }
}

/* Handle the timer interrupt to close the relay automatically */
void evt_loop_handle_timer()
{
    relay_set_state(RELAY_LEFT_PIN,  RELAY_CLOSED);
    relay_set_state(RELAY_RIGHT_PIN, RELAY_CLOSED);
}
