/* Dependencies */
#include <Wire.h>
#include "KB_Motor.h"

KB_MOTOR::KB_MOTOR()
{

}

void KB_MOTOR::begin(uint8_t address1, uint8_t address2) {

  /* Store the I2C address and init the Wire library */
  _address1 = address1;
  _address2 = address2;
  Wire.begin(4, 5); // join another i2c bus edit KB_Relay.h default for i2c KidBright32

  // WriteCommand(0x00, 0x00);

  Wire.beginTransmission(_address1);
  Wire.write(0x01);
  Wire.write(0b100000000);
  Wire.endTransmission();

  Wire.beginTransmission(_address2);
  Wire.write(0x01);
  Wire.write(0b100000000);
  Wire.endTransmission();
}


void KB_MOTOR::WriteCommand1(uint8_t level1, uint8_t cw_or_ccw1) {
  int m1 = level1 << 2 | cw_or_ccw1;
  Wire.beginTransmission(0x66);
  Wire.write(0x00);
  Wire.write(m1);
  //  Serial.println(m, DEC);
  Wire.endTransmission(true);
  //  Serial.println(Wire.endTransmission(true));
}

void KB_MOTOR::WriteCommand2(uint8_t level2, uint8_t cw_or_ccw2) {
  int m2 = level2 << 2 | cw_or_ccw2;
  Wire.beginTransmission(0x68);
  Wire.write(0x00);
  Wire.write(m2);
  //  Serial.println(m, DEC);
  Wire.endTransmission(true);
  //  Serial.println(Wire.endTransmission(true));
}
//speed 0-63
void KB_MOTOR::i2c_motor_write(int ch, int mspeed, int stat) {
  int _ch = ch;
  uint8_t _mspeed = mspeed / 4;
  int _stat = stat;
  uint8_t temp_stat;
  int flag_stat = 0;

  if (_stat >= 0) {
    if (_stat == 0) {
      temp_stat = 0x00;
      flag_stat = 1;
    } else if (_stat == 1) {
      temp_stat = 0x02;
      flag_stat = 1;
    } else if (_stat == 2) {
      temp_stat = 0x01;
      flag_stat = 1;
    } else {
      Serial.println("format error");
      flag_stat = 0;
    }
  }


  if (flag_stat == 1 ) {
    if (_mspeed > 63) {
      Serial.println("Over Speed!!!!");
      flag_stat = 0;
    } else if (_mspeed < 0 ) {
      Serial.println("Lower Speed!!!!");
      flag_stat = 0;
    } else  {
      switch (_ch)
      {
        case 1:
          /* code */
          WriteCommand1(_mspeed, temp_stat);
          flag_stat = 0;
          break;
        case 2:
          /* code */
          WriteCommand2(_mspeed, temp_stat);
          flag_stat = 0;
          break;
        default:
          Serial.println("Channel Error");
          flag_stat = 0;
          break;
      }
      flag_stat = 0;
    }
  }
}

void KB_MOTOR::faultCheck1(int _address1) {
  int _addr1  = _address1;
  Wire.beginTransmission(_addr1);
  Wire.write(0x01);
  Wire.endTransmission();

  Wire.requestFrom(_addr1, 1);
  byte error = Wire.read();
  //  printf(error);
  //  printf("\n");
  if (error & 0b00000011) {
    Serial.println("Over current1");
  }

  if (error & 0b00000101) {
    //    Serial.println("Low voltage1");
  }

  if (error & 0b00001001) {
    Serial.println("Over Temperature1");
  }

  if (error & 0b00010001) {
    Serial.println("Limit1");
  }

  Wire.beginTransmission(_addr1);
  Wire.write(0x01);
  Wire.write(0b100000000);
  Wire.endTransmission();
}



void KB_MOTOR::faultCheck2(int _address2) {
  int _addr2 = _address2;

  Wire.beginTransmission(_addr2);
  Wire.write(0x01);
  Wire.endTransmission();

  Wire.requestFrom(_addr2, 1);
  byte error = Wire.read();
  if (error & 0b00000011) {
    Serial.println("Over current2");
  }

  if (error & 0b00000101) {
    //    Serial.println("Low voltage2");
  }

  if (error & 0b00001001) {
    Serial.println("Over Temperature2");
  }

  if (error & 0b00010001) {
    Serial.println("Limit2");
  }

  Wire.beginTransmission(_addr2);
  Wire.write(0x01);
  Wire.write(0b100000000);
  Wire.endTransmission();


}
