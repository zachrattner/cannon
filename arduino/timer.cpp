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

/* Get the TimerOne library from Google Code: 
       http://code.google.com/p/arduino-timerone/downloads/list 
   Documentation:
       http://arduino.cc/playground/Code/Timer1 */
#include <TimerOne.h>

#include "timer.h"

/* Timer period, in microseconds */
static uint16_t s_timer_per;

/* Timer expiry callback function */
static timer_cb_type s_timer_cb;

/* Timer mode */
static timer_mode_type s_mode;

/* Initialize the timer, but don't start it */
void timer_init(unsigned long period)
{
    s_timer_cb = NULL;
    timer_set_mode(TIMER_MODE_SINGLE_SHOT);
    timer_set_period(period); 
}

/* Set the operating mode */
void timer_set_mode(timer_mode_type mode)
{
    s_mode = mode;
}

/* Set the expiry function callback */
void timer_set_cb(timer_cb_type cb)
{
    s_timer_cb = cb;
}

/* Timer ISR */
void timer_service()
{
    if (s_timer_cb)
    {
        s_timer_cb();
    }
    
    if (s_mode == TIMER_MODE_SINGLE_SHOT)
    {
        Timer1.stop();
    }
}

/* Set the timer period and start ticking */
void timer_set_period(unsigned long period)
{
    s_timer_per = period;
    Timer1.stop();
    Timer1.initialize(period);
    Timer1.attachInterrupt(timer_service, period);
    Timer1.start();
}
