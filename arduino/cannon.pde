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

/*
* Key Parts (although I'm sure you'll be able to find them elsewhere too):
* BlueSmirf Silver Bluetooth Modem: http://www.sparkfun.com/products/10269
* Arduino Uno: http://www.sparkfun.com/products/9950
* Sprinkler Valve: http://goo.gl/UXjS9
*/

#include <WProgram.h>
#include <TimerOne.h>
#include "evt_loop.h"

/* The only real reason the whole "event loop" thing exists is because I don't
   like Arduino's setup and loop functions. I also like to think it makes the 
   code more portable, but you're free to disagree. */
   
void setup()
{
    evt_loop_init();
}

void loop()
{
    evt_loop_proc();
}