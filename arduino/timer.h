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

#ifndef TIMER_H
#define TIMER_H

#include <WProgram.h>

/* Get the TimerOne library from Google Code: 
       http://code.google.com/p/arduino-timerone/downloads/list 
   Documentation:
       http://arduino.cc/playground/Code/Timer1 */
#include <TimerOne.h>

#include "com_inc.h"

/* Timer operating mode */
typedef enum
{
    /* Single shot: use to schedule an event at a fixed point in the future */
    TIMER_MODE_SINGLE_SHOT,
    
    /* Periodic: use the schedule an event to run at a periodic rate */
    TIMER_MODE_PERIODIC
} timer_mode_type;

/* Timer callback function type */
typedef void(*timer_cb_type)(void);

/* Initialize the scheduler timer, in microseconds */
void timer_init(unsigned long period);

/* Timer interrupt service routine */
void timer_service();

/* Set the timer data report callback function */
void timer_set_cb(timer_cb_type cb);

/* Set the mode to either single-shot or periodic */
void timer_set_mode(timer_mode_type mode);

/* Set the scheduler timer's data period, in microseconds */
void timer_set_period(unsigned long period);

#endif /* !TIMER_H */