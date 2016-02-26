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
 */
 
 /*    
 *    This library simulates an temperature and humidity sensor
 *    using the Oregon Scientific protocol V2.1
 */

#ifndef _OS_V2_1_TEMP_HUM_SENSOR_H
#define _OS_V2_1_TEMP_HUM_SENSOR_H

#include "Arduino.h"

class OS_v2_1_temp_hum_sensor
{
  public:
    OS_v2_1_temp_hum_sensor(uint8_t transmittpin);

    /*
    *    Set radio channel to use
    *    1 to 15 is valid
    *    setChannel(5); Uses channel 5 
    */ 
    void setChannel(byte channel);
    
    /*
    *    Set sensor id number
    *    1 to 15 is valid
    *    setId(155); Sensor now has id number 155
    *    At the time of writing I don't know 
    *    the range of values allowed
    */
    void setId(byte ID);
    
    /*
    *    Set the battery status
    *    true = battery OK
    *    false = battery low
    *    setBatteryStatus(false); Sensor sends low battery
    */
    void setBatteryStatus(bool level);
    
    /*
    *    Sets the temperature in 0.1 deg Celsius divided by ten
    *    setTemperature(123); temperature set to 12.3 deg Celsius
    */
    void setTemperature(float temp);
    
    /*
    *    Sets the humidity in 1% steps
    *    setHumidity(40); temperature set to 40%
    */    
    void setHumidity(byte hum);
 
    /*
    *    sends a data packet with the parameters
    *    set above
    */    
    void buildAndSendPacket();
    
  private:
    byte _oregonMessageBuffer[9];//for 1A2D temp/hum
    uint8_t CalculateCRC8( uint8_t *addr, uint8_t len);
    const unsigned long _TIME = 512;
    const unsigned long _TWOTIME = _TIME * 2;
    uint8_t _transmittpin;


    inline void sendHigh(void);
    inline void sendLow(void);
    inline void sendZero(void);
    inline void sendOne(void);
    inline void sendQuarterMSB(const byte data);
    inline void sendQuarterLSB(const byte data);
    void sendData(byte *data, byte datasize);
    inline void sendPreamble(void);
    inline void sendPostamble(void);
    int Sum(byte count, const byte* data);
    void calculateAndSetChecksum(byte* data);
};
#endif
