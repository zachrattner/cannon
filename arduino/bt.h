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

#ifndef BT_H
#define BT_H

#include "com_inc.h"
#include "msgq.h"
 
/* Invalid message read from serial port. This approach possible because the 
   leftmost bit of a valid ASCII byte is always zero. */
#define BT_MSG_INVALID -1

/* Number of bytes in a valid bluetooth message */
#define BT_MSG_LEN 2

/* Bluetooth transfer pin -> Use the Arduino Uno's hardware serial I/O port. */
#define BT_TX_PIN 0 

/* Bluetooth receive pin */
#define BT_RX_PIN 1 
 
/* Bluetooth serial baud rate. This is the default on the BlueSmirf Silver. */
#define BT_BAUD_RATE 115200

/* Initialize the Bluetooth module */
void bt_init(msgq_type* bt_in_msgq, msgq_type* bt_out_msgq);

/* Waste energy until a Bluetooth message has been placed in to the queue. If 
   you really wanted to be cool, you'd put the microprocessor to sleep here and 
   have the incoming serial data interrupt wake it up. */
bool bt_wait_for_msgs();

/* Add a message to the incoming Bluetooth queue, if one is available. 
   Returns true if a message was added. */
bool bt_check_for_msgs();

#endif /* !BT_H */
