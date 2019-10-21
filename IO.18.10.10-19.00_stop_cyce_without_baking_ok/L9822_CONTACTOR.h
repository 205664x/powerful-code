#include "SPI.h"// spi config for L9822
SPISettings settingsA(2000000, MSBFIRST, SPI_MODE1);
class L9822 {
    int sl1, sl2, rs;
  public:
    L9822(int _select1, int _select2, int _reset) { // chip 1 ,chip 2, reset pin
      pinMode(_select1, OUTPUT);
      pinMode(_select2, OUTPUT);
      pinMode(_reset, OUTPUT);
      pinMode(7, OUTPUT);
      pinMode(isoPin,OUTPUT);
      sl1 = _select1;
      sl2 = _select2;
      rs = _reset;
      digitalWrite(rs, 0);
      digitalWrite(rs, 1);
      SPI.begin();
      SPI.beginTransaction(settingsA);
      digitalWrite(sl1, 0); SPI.transfer(0b11111111); digitalWrite(sl1, 1);
      digitalWrite(sl2, 0); SPI.transfer(0b11111111); digitalWrite(sl2, 1);
    }
    uint8_t ic1 = 0xff, ic2 = 0xff, _state1, _state2; // store value
    void iso(int i) {if (i == 1) ic1 &= 0b00111111;else ic1 |= 0b11000000;}
    void isoV1(int i){if (i == 1) digitalWrite(isoPin,1);else digitalWrite(isoPin,0);}
    void ir3(int i) {if (i == 1) ic1 &= 0b11001111;else ic1 |= 0b00110000;}// IR 3 contactor - high frequency on/off
    void rm(int i) {if (i == 1) ic1 &= 0b11110111;else ic1 |= 0b00001000;} // rotation motor
    void pm(int i) {if (i == 1) ic1 &= 0b11111011;else ic1 |= 0b00000100;} // press motor
    void sm(int i) {if (i == 1) ic1 &= 0b11111101;else ic1 |= 0b00000010;} // shutle motor
    void cano(int i) {if (i == 1) ic1 &= 0b11111110;else ic1 |= 0b00000001;} // canopy motor

    void b1(int i) {if (i == 1) ic2 &= 0b11101111;else ic2 |= 0b00010000;} // base 1 element contactor
    void b2(int i) {if (i == 1) ic2 &= 0b11110111;else ic2 |= 0b00001000;} // base 2 element contactor
    void ir12(int i) {if (i == 1) ic2 &= 0b10111111;else ic2 |= 0b01000000;} // IR 1 2 contactor
    void pe(int i) {if (i == 1) ic2 &= 0b11011111;else ic2 |= 0b00100000;} // press element contactor
    void se(int i) {if (i == 1) ic2 &= 0b01111111;else ic2 |= 0b10000000;}// shuttle element contactor
    void speaker(int i) {if (i == 1) ic2 &= 0b11111011;else ic2 |= 0b00000100;}// speaker
    
    void allOff(){
      ic1=0b11111111;
      ic2=0b11111111;
      digitalWrite(isoPin,0);
    }
    void allOn(){
      ic1=0b00000000;
    }
    void updateContactor() {
      digitalWrite(sl1, 0); _state1 = SPI.transfer(ic1); digitalWrite(sl1, 1);
      digitalWrite(sl2, 0); _state2 = SPI.transfer(ic2); digitalWrite(sl2, 1);
    }
    uint8_t state1() {return _state1;}
    uint8_t state2() {return _state2;}
};

