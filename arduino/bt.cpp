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
#include "bt.h"

/* The serial port to use for the Bluetooth connection. On the Arduino Uno, 
   there is only one choice. Arduino Megas have the choice of Serial1 through 
   Serial4, if you're interested. */
#define s_bt_serial Serial

/* Input queue: external device to the Arduino */
static  msgq_type*  s_bt_in_msgq  = NULL;

/* Output queue: Arduino to external device */
static  msgq_type* s_bt_out_msgq = NULL;

/* Initialize the Bluetooth module */
void bt_init(msgq_type* bt_in_msgq, msgq_type* bt_out_msgq)
{
    s_bt_in_msgq  = bt_in_msgq;
    s_bt_out_msgq = bt_out_msgq;
    
    /* Either queue may be null. Check before initializing, lest ye blow yer 
       microprocessor into oblivion. */
    if (s_bt_in_msgq)
    {
        msgq_init(s_bt_in_msgq);
    }
    
    if (s_bt_out_msgq)
    {
        msgq_init(s_bt_out_msgq);
    }
    
    /* Initialize serial communication over the port */
    s_bt_serial.begin(BT_BAUD_RATE);
    pinMode(BT_RX_PIN, OUTPUT);
    pinMode(BT_TX_PIN, INPUT);
}

/* Waste energy until a Bluetooth message has been placed in to the queue. If 
   you really wanted to be cool, you'd put the microprocessor to sleep here and 
   have the incoming serial data interrupt wake it up. */
bool bt_wait_for_msgs()
{
    while (!s_bt_serial.available());
    return bt_check_for_msgs();
}

/* Add a message to the incoming Bluetooth queue, if one is available. 
   Returns true if a message was added. */
bool bt_check_for_msgs()
{    
    while (s_bt_serial.available())
    {
        uint8_t msg = 0;
        msgq_rsp_type rsp;
        
        msg = s_bt_serial.read();
        if (msg == BT_MSG_INVALID)
        {
            return false;
        }
        
        rsp = msgq_push(s_bt_in_msgq, msg);
        if (rsp != MSGQ_S_SUCCESS)
        {
            return false;
        }
    }
    
    return true;
}
