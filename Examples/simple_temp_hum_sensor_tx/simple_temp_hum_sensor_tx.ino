/*
 * connectingStuff, Oregon Scientific v2.1 Emitter
 * http://www.connectingstuff.net/blog/encodage-protocoles-oregon-scientific-sur-arduino/
 *
 * Copyright (C) 2013 olivier.lebrun@gmail.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
/*
 * Made into an arduino library by deberman 2016-02-24
 * 
 */

#include "os_v2_1_temp_hum_sensor.h"

OS_v2_1_temp_hum_sensor Sensor(7);


void setup()
{
  Serial.begin(115200);

  Sensor.setChannel(0x20);
  
  Sensor.setId(0xBB);
}

void loop()
{
  // Get Temperature, humidity and battery level from sensors
  Sensor.setBatteryStatus(true); // 0 : low, 1 : high
  
  Sensor.setTemperature(1.5);
  
  Sensor.setHumidity(5);  // Set Humidity

  // Send the Message over RF
  Sensor.buildAndSendPacket();
 
  delay(1000);
}

