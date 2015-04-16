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

#ifndef RELAY_H
#define RELAY_H

/* Left relay digital output pin */
#define RELAY_LEFT_PIN  6

/* Right relay digital output pin */
#define RELAY_RIGHT_PIN 7

/* Relay states. The valve state matches the relay state. So, an open relay 
   means air can flow out of the pressurized canister. */
typedef enum
{
    /* Open relay: circuit is open */
    RELAY_OPEN,
    
    /* Closed relay: circuit is closed */
    RELAY_CLOSED
} relay_state_type;

/* Initialize the relay pins */
void relay_init();

/* Set the state of the specified relay */
void relay_set_state(uint8_t pin, relay_state_type state);

#endif /* !RELAY_H */