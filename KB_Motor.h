#ifndef _KB_MOTOR_H
#define _KB_MOTOR_H

#if ARDUINO >= 100
#include "Arduino.h"
#define I2CWRITE(x) Wire.write(x)
#define I2CREAD() Wire.read()
#else
#include "WProgram.h"
#define I2CWRITE(x) Wire.send(x)
#define I2CREAD() Wire.receive()
#define INPUT_PULLUP 2
#endif

class KB_MOTOR {
  public:
    /**
       Create a new KB_MOTOR instance
    */
    KB_MOTOR();

    /**
       Start the I2C controller and store the KB_MOTOR_Module address
    */
    void begin(uint8_t address1, uint8_t address2);
    void WriteCommand1(byte level1, byte cw_or_ccw1);
    void WriteCommand2(byte level2, byte cw_or_ccw1);
    void faultCheck1(int);
    void faultCheck2(int);
    void i2c_motor_write(int ch, int mspeed, int stat);


  private:
    uint8_t _address1;
    uint8_t _address2;
    byte level1;
    byte cw_or_ccw1;
    byte level2;
    byte cw_or_ccw2;
};
#endif
