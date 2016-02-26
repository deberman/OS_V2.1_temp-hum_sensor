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


OS_v2_1_temp_hum_sensor::OS_v2_1_temp_hum_sensor(uint8_t transmittpin)
{
  pinMode(transmittpin, OUTPUT);
  _transmittpin = transmittpin;
  sendLow();
}


inline void OS_v2_1_temp_hum_sensor::sendHigh()
{
  digitalWrite(_transmittpin, HIGH);
}

inline void OS_v2_1_temp_hum_sensor::sendLow()
{
  digitalWrite(_transmittpin, LOW);
}

inline void OS_v2_1_temp_hum_sensor::sendZero(void)
{
  sendHigh();
  delayMicroseconds(_TIME);
  sendLow();
  delayMicroseconds(_TWOTIME);
  sendHigh();
  delayMicroseconds(_TIME);
}

inline void OS_v2_1_temp_hum_sensor::sendOne(void)
{
  sendLow();
  delayMicroseconds(_TIME);
  sendHigh();
  delayMicroseconds(_TWOTIME);
  sendLow();
  delayMicroseconds(_TIME);
}

inline void OS_v2_1_temp_hum_sensor::sendQuarterMSB(const byte data)
{
  (bitRead(data, 4)) ? sendOne() : sendZero();
  (bitRead(data, 5)) ? sendOne() : sendZero();
  (bitRead(data, 6)) ? sendOne() : sendZero();
  (bitRead(data, 7)) ? sendOne() : sendZero();
  }

inline void OS_v2_1_temp_hum_sensor::sendQuarterLSB(const byte data)
{
  (bitRead(data, 0)) ? sendOne() : sendZero();
  (bitRead(data, 1)) ? sendOne() : sendZero();
  (bitRead(data, 2)) ? sendOne() : sendZero();
  (bitRead(data, 3)) ? sendOne() : sendZero();
}

void OS_v2_1_temp_hum_sensor::sendData(byte *data, byte datasize)
{
  for (byte i = 0; i < datasize; ++i)
  {
    sendQuarterLSB(data[i]);
    sendQuarterMSB(data[i]);
  }
}

void OS_v2_1_temp_hum_sensor::buildAndSendPacket()
{
  _oregonMessageBuffer[0] = 0x1A;
  _oregonMessageBuffer[1] = 0X2D;
  calculateAndSetChecksum(_oregonMessageBuffer);
  sendPreamble();
  sendData(_oregonMessageBuffer, sizeof(_oregonMessageBuffer));
  sendPostamble();
}

inline void OS_v2_1_temp_hum_sensor::sendPreamble(void)
{
  byte PREAMBLE[] = {0xFF, 0xFF};
  sendData(PREAMBLE, 2);
}

inline void OS_v2_1_temp_hum_sensor::sendPostamble(void)
{
#ifdef THN132N
  sendQuarterLSB(0x00);
#else
  byte POSTAMBLE[] = {0x00};
  sendData(POSTAMBLE, 1);
#endif
}

void OS_v2_1_temp_hum_sensor::setChannel(byte channel)
{
  _oregonMessageBuffer[2] = channel;
}

void OS_v2_1_temp_hum_sensor::setId(byte ID)
{
  _oregonMessageBuffer[3] = ID;
}

void OS_v2_1_temp_hum_sensor::setBatteryStatus(bool level)
{
  if (!level) _oregonMessageBuffer[4] = 0x0C;
  else _oregonMessageBuffer[4] = 0x00;
}

void OS_v2_1_temp_hum_sensor::setTemperature(float temp)
{
  // Set temperature sign
  if (temp < 0)
  {
    _oregonMessageBuffer[6] = 0x08;
    temp *= -1;
  }
  else
  {
    _oregonMessageBuffer[6] = 0x00;
  }

  // Determine decimal and float part
  int tempInt = (int)temp;
  int td = (int)(tempInt / 10);
  int tf = (int)round((float)((float)tempInt / 10 - (float)td) * 10);

  int tempFloat =  (int)round((float)(temp - (float)tempInt) * 10);

  // Set temperature decimal part
  _oregonMessageBuffer[5] = (td << 4);
  _oregonMessageBuffer[5] |= tf;


  // Set temperature float part
  _oregonMessageBuffer[4] |= (tempFloat << 4);
}

void OS_v2_1_temp_hum_sensor::setHumidity(byte hum)
{
  _oregonMessageBuffer[7] = (hum / 10);
  _oregonMessageBuffer[6] |= (hum - _oregonMessageBuffer[7] * 10) << 4;
}

int OS_v2_1_temp_hum_sensor::Sum(byte count, const byte* data)
{
  int s = 0;

  for (byte i = 0; i < count; i++)
  {
    s += (_oregonMessageBuffer[i] & 0xF0) >> 4;
    s += (_oregonMessageBuffer[i] & 0xF);
  }

  if (int(count) != count)
    s += (_oregonMessageBuffer[count] & 0xF0) >> 4;

  return s;
}

void OS_v2_1_temp_hum_sensor::calculateAndSetChecksum(byte* data)
{
  _oregonMessageBuffer[8] = ((Sum(8, data) - 0xa) & 0xFF);//1A2D temp/hum
 
 //int s = ((Sum(6, data) + (data[6]&0xF) - 0xa) & 0xff);//EA4C temp
 //data[6] |=  (s&0x0F) << 4;     data[7] =  (s&0xF0) >> 4;
}



