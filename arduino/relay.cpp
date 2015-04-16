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
#include "relay.h"

/* Initialize the relay pins */
void relay_init()
{
    /* The relay pins are outputs */
    pinMode(RELAY_LEFT_PIN, OUTPUT);
    pinMode(RELAY_RIGHT_PIN, OUTPUT);
    
    /* Make sure both valves are closed , or this whole project's kinda 
       pointless */
    digitalWrite(RELAY_LEFT_PIN, LOW);
    digitalWrite(RELAY_RIGHT_PIN, LOW);
}

/* Set the state of the specified relay */
void relay_set_state(uint8_t pin, relay_state_type state)
{
    switch (state)
    {
        case RELAY_OPEN:
            digitalWrite(pin, HIGH);
            break;
        
        case RELAY_CLOSED:
        default:
            digitalWrite(pin, LOW);
            break;
    }
}
