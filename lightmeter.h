
boolean gain;     // Gain setting, 0 = X1, 1 = X16;
unsigned int ms = 402; // interval; // Integration ("shutter") time in milliseconds
void printError(byte error)
// If there's an I2C error, this function will
// print out an explanation.
{
  Serial.print("I2C error: ");
  Serial.print(error, DEC);
  Serial.print(", ");

  switch (error)
  {
    case 0:
      Serial.println("success");
      break;
    case 1:
      Serial.println("data too long for transmit buffer");
      break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");
      break;
    case 3:
      Serial.println("received NACK on data");
      break;
    case 4:
      Serial.println("other error");
      break;
    default:
      Serial.println("unknown error");
  }
}

void configureSensor(void) {
  // For more information see the hookup guide at: https://learn.sparkfun.com/tutorials/getting-started-with-the-tsl2561-luminosity-sensor

  light.begin();

  // Get factory ID from sensor:
  // (Just for fun, you don't need to do this to operate the sensor)

  unsigned char ID;

  if (light.getID(ID))
  {
    Serial.print("Got factory ID: 0X");
    Serial.print(ID, HEX);
    Serial.println(", should be 0X5X");
  }

  else
  {
    byte error = light.getError();
    printError(error);
  }

  // If gain = false (0), device is set to low gain (1X)
  // If gain = high (1), device is set to high gain (16X)

  gain = 1;

  // If time = 0, integration will be 13.7ms
  // If time = 1, integration will be 101ms
  // If time = 2, integration will be 402ms
  // If time = 3, use manual start / stop to perform your own integration

  unsigned char time = 2;

  // setTiming() will set the third parameter (ms) to the
  // requested integration time in ms (this will be useful later):

  Serial.println("Set timing...");
  light.setTiming(gain, time, ms);

  // To start taking measurements, power up the sensor:

  Serial.println("Powerup...");
  light.setPowerUp();

  // The sensor will now gather light during the integration time.
  // After the specified time, you can retrieve the result from the sensor.
  // Once a measurement occurs, another integration period will start.


}

float getLux() {
  unsigned int data0, data1;

  if (light.getData(data0, data1)) {
    Serial.println("Measurement Completed.");
  } else {
    Serial.println("Measurement ERROR!");
  }


  //  Serial.print("data0: ");
  //  Serial.print(data0);
  //  Serial.print(" data1: ");
  //  Serial.print(data1);

  // To calculate lux, pass all your settings and readings
  // to the getLux() function.

  // The getLux() function will return 1 if the calculation
  // was successful, or 0 if one or both of the sensors was
  // saturated (too much light). If this happens, you can
  // reduce the integration time and/or gain.
  // For more information see the hookup guide at: https://learn.sparkfun.com/tutorials/getting-started-with-the-tsl2561-luminosity-sensor


  double lux;    // Resulting lux value
  boolean good;  // True if neither sensor is saturated

  // Perform lux calculation:

  good = light.getLux(gain, ms, data0, data1, lux);

  // Print out the results:
  //
  //  Serial.print(" lux: ");
  //  Serial.print(lux);


  return lux; //* DomeMultiplier;             // DomeMultiplier = 2.17 (calibration)*/
}

float log2(float x) {
  return log(x) / log(2);
}

float lux2ev(float lux) {
  return log2(lux / 2.5);
}

// return aperture value (1.4, 1.8, 2.0) by index in sequence (0, 1, 2, 3, ...).
float getApertureByIndex(uint8_t indx) {
  float roundIndx = 10.0;

  if (indx > 39) {
    roundIndx = 1;
  }

  float f = round(pow(2, indx / 3.0 * 0.5) * roundIndx) / roundIndx;

  // the formula returns exact value, but photographers uses more memorable values.
  // convert it.

  if (f >= 1.1 && f < 1.2) {
    f = 1.1;
  } else if (f >= 1.2 && f < 1.4) {
    f = 1.2;
  } else if (f > 3.2 && f < 4) {
    f = 3.5;
  } else if (f > 5 && f < 6.3) {
    f = 5.6;
  } else if (f > 10 && f < 11) {
    f = 10;
  } else if (f >= 11 && f < 12) {
    f = 11;
  } else if (f >= 12 && f < 14) {
    f = 13;
  } else if (f >= 14 && f < 16) {
    f = 14;
  } else if (f >= 20 && f < 22) {
    f = 20;
  } else if (f >= 22 && f < 25) {
    f = 22;
  } else if (f >= 24 && f < 28) {
    f = 25;
  } else if (f >= 28 && f < 40) {
    f = 36;
  } else if (f >= 40 && f < 45) {
    f = 40;
  } else if (f >= 45 && f < 50) {
    f = 45;
  } else if (f >= 50 && f < 57) {
    f = 51;
  } else if (f >= 71 && f < 80) {
    f = 72;
  } else if (f >= 80 && f < 90) {
    f = 80;
  } else if (f >= 90 && f < 101) {
    f = 90;
  }

  return f;
}

// Return ISO value (100, 200, 400, ...) by index in sequence (0, 1, 2, 3, ...).
long getISOByIndex(uint8_t indx) {
  if (indx < 0 || indx > MaxISOIndex) {
    indx = 0;
  }

  indx += 10;

  //  float iso = pow(10, (indx - 1) / 10.0);
  //  iso = (long)(round(iso / 10.0) * 10);

  long int factor = 1;
  float iso = 0;

  if (indx > 60) {
    indx -= 50;
    factor = 100000;
  } else if (indx > 50) {
    indx -= 40;
    factor = 10000;
  } else if (indx > 40) {
    indx -= 30;
    factor = 1000;
  } else if (indx > 30) {
    indx -= 20;
    factor = 100;
  } else if (indx > 20) {
    indx -= 10;
    factor = 10;
  }

  if (indx == 10) {
    iso = 8;
  } else if (indx == 11) {
    iso = 10;
  } else if (indx == 12) {
    iso = 12.5;
  } else if (indx == 13) {
    iso = 16;
  } else if (indx == 14) {
    iso = 20;
  } else if (indx == 15) {
    iso = 25;
  } else if (indx == 16) {
    iso = 32;
  } else if (indx == 17) {
    iso = 40;
  } else if (indx == 18) {
    iso = 50;
  } else if (indx == 19) {
    iso = 64;
  } else if (indx == 20) {
    iso = 80;
  }

  iso = (long)floor(iso * factor);
  return iso;
}

float getMinDistance(float x, float v1, float v2) {
  if (x - v1 > v2 - x) {
    return v2;
  }

  return v1;
}

float getTimeByIndex(uint8_t indx) {
  if (indx < 0 || indx >= MaxTimeIndex) {
    indx = 0;
  }

  float factor = 0;
  float t = 0;

  if (indx < 10) {
    factor = 100.0;
  } else if (indx < 20) {
    indx -= 10;
    factor = 10.0;
  } else if (indx < 30) {
    indx -= 20;
    factor = 1.0;
  } else if (indx < 40) {
    indx -= 30;
    factor = 0.1;
  } else if (indx < 50) {
    indx -= 40;
    factor = 0.01;
  } else if (indx < 60) {
    indx -= 50;
    factor = 0.001;
  } else if (indx < 70) {
    indx -= 60;
    factor = 0.0001;
  } else if (indx < 80) {
    indx -= 70;
    factor = 0.00001;
  }

  if (indx == 0) {
    t = 100;
  } else if (indx == 1) {
    t = 80;
  } else if (indx == 2) {
    t = 64;
  } else if (indx == 3) {
    t = 50;
  } else if (indx == 4) {
    t = 40;
  } else if (indx == 5) {
    t = 32;
  } else if (indx == 6) {
    t = 25;
  } else if (indx == 7) {
    t = 20;
  } else if (indx == 8) {
    t = 16;
  } else if (indx == 9) {
    t = 12.5;
  }

  t = 1 / (t * factor);
  return t;
}

// Convert calculated time (in seconds) to photograpy style shutter speed.

//double fixTime(double t) {
//  double divider = 1;
//
//  float maxTime = getTimeByIndex(MaxTimeIndex);
//
//  if (t < maxTime) {
//    return maxTime;
//  }
//
//  t = 1 / t;
//
//  if (t > 99999) {
//    divider = 10000;
//  } else if (t > 9999) {
//    divider = 1000;
//  } else if (t > 999) {
//    divider = 100;
//  } else if (t > 99) {
//    divider = 10;
//  }
//
//  t = t / divider;
//
//  if (t >= 10 && t <= 12.5) {
//    t = getMinDistance(t, 10, 12.5);
//  } else if (t >= 12.5 && t <= 16) {
//    t = getMinDistance(t, 12.5, 16);
//  } else if (t >= 16 && t <= 20) {
//    t = getMinDistance(t, 16, 20);
//  } else if (t >= 20 && t <= 25) {
//    t = getMinDistance(t, 20, 25);
//  } else if (t >= 25 && t <= 32) {
//    t = getMinDistance(t, 25, 32);
//  } else if (t >= 32 && t <= 40) {
//    t = getMinDistance(t, 32, 40);
//  } else if (t >= 40 && t <= 50) {
//    t = getMinDistance(t, 40, 50);
//  } else if (t >= 50 && t <= 64) {
//    t = getMinDistance(t, 50, 64);
//  } else if (t >= 64 && t <= 80) {
//    t = getMinDistance(t, 64, 80);
//  } else if (t >= 80 && t <= 100) {
//    t = getMinDistance(t, 80, 100);
//  }
//
//  t = t * divider;
//
//  if (t == 32) {
//    t = 30;
//  }
//
//  if (t == 16) {
//    t = 15;
//  }
//
//  t = 1 / t;
//
//  return t;
//}
//
//// Convert calculated aperture value to photograpy style aperture value.
//float fixAperture(float a) {
//  for (int i = 0; i < MaxApertureIndex; i++) {
//    float a1 = getApertureByIndex(i);
//    float a2 = getApertureByIndex(i + 1);
//
//    if (a1 < a && a2 >= a) {
//      return getMinDistance(a, a1, a2);
//    }
//  }
//
//  return 0;
//}
//
///*
//  Return ND from ndIndex
//  int ND[] = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48}; // eg.: 1) 0.3 ND = -1 stop = 2^2 = 4; 2) 0.9 ND = -3 stop = 2^3 = 16;
//*/
//uint8_t getND(uint8_t ndIndex) {
//  if (ndIndex == 0) {
//    return 0;
//  }
//
//  return 3 + (ndIndex - 1) * 3;
//}
//
//
//
