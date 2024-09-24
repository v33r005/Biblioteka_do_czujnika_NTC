/*
 * Copyright (C) AC SOFTWARE SP. Z O.O
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef SRC_SUPLA_SENSOR_NTC10K_H_
#define SRC_SUPLA_SENSOR_NTC10K_H_

#include <supla/sensor/thermometer.h>

#ifdef ARDUINO_ARCH_ESP32
#define adcResolution 4059
#else
#define  adcResolution 1023
#endif

namespace Supla {
namespace Sensor {
class NTC10k : public Thermometer {
 public:
  NTC10k(int pin, float Thermistor_Resistance_25_Degree = 10000, float Rref = 10000, float B_Value = 3950) 
  : pin(pin), 
    Thermistor_Resistance_25_Degree(Thermistor_Resistance_25_Degree), 
    Rref(Rref), 
    B_Value(B_Value) {

  }

  void onInit() {
      channel.setNewValue(getValue());
  }

  double getValue() {
    readSensor();
    return tempC;
  }
  void iterateAlways() {
    if (millis() - lastReadTime > 1000) {
      lastReadTime = millis();
      channel.setNewValue(getValue());
    }
  }
  void readSensor(){
    analogValue = analogRead(pin);  // odczyt wartości z pinu
    Vout = analogValue * (3.3 / adcResolution); 
    Rntc = (Rref * Vout) / (3.3 - Vout);    // obliczenie rezystancji termistora NTC
    tempK = (1 / ((log(Rntc / Thermistor_Resistance_25_Degree) / B_Value) + (1 / T0_kelvin)));  // temperatura w kelwinach
    tempC = tempK - 273.15; //przeliczanie na celciusze
    return tempC;
  }

 protected:
    int pin;  
    int analogValue;
    float Vout;
    float Rntc;
    float tempK;
    float tempC;
    float Thermistor_Resistance_25_Degree;
    float B_Value;
    const float T0_kelvin = 298.15;
    float Rref;
};

};  // namespace Sensor
};  // namespace Supla

#endif  // SRC_SUPLA_SENSOR_NTC10K_H_
