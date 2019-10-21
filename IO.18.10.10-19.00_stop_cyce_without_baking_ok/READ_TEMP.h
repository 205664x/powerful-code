/*Read me
 * 
 * init dalas. in this program i
 *  dallas (22,1); for left sensor
    dallas (23,1); for right sensor
 * void refrese() is used to read temperature. To get temp, we have to do refresh first 
 */

#include <OneWire.h>
#define left_temp_ic 22
#define right_temp_ic 23
int16_t dallas(int x, byte start)// x is pin / start =1 mean just delay in first time
{
  OneWire ds (x); 
  byte i; 
  byte data[2]; 
  int16_t result; 
  do{
    ds.reset(); 
    ds.write (0xcc);// Skip Command 
    ds.write (0xBE);// Read 1st 2 bytes of Scratchpad 
    for (i = 0; i < 2; i++) data[i] = ds.read(); 
    result= (data[1]<<8)|data[0]; 
    result>=4; 
    if (data[1] &128) result |=61440; 
    if (data[0] &8) ++result; 
    ds.reset(); 
    ds.write (0xCC);// Skip Command 
    ds.write (0x44,1);// start conversion, assuming 5v connected 
//do in first time only    
    if (start) delay(1000); 
  } while (start--);
  return result/16;
}
//---------------------------------- read MCP3424------------------------------------------
#include <Wire.h>
#include <MCP342x.h>

MCP342x::Config config1(MCP342x::channel3, MCP342x::oneShot, MCP342x::resolution16, MCP342x::gain1);// left 1

MCP342x::Config config2(MCP342x::channel4, MCP342x::oneShot,MCP342x::resolution18, MCP342x::gain1);// left 2

MCP342x::Config config3(MCP342x::channel1, MCP342x::oneShot,MCP342x::resolution18, MCP342x::gain1);// left 3

MCP342x::Config config4(MCP342x::channel2, MCP342x::oneShot,MCP342x::resolution18, MCP342x::gain1);// left 4

MCP342x::Config config5(MCP342x::channel1, MCP342x::oneShot,MCP342x::resolution18, MCP342x::gain1);// right 1

MCP342x::Config config6(MCP342x::channel2, MCP342x::oneShot,MCP342x::resolution18, MCP342x::gain1);// right 2

MCP342x::Config config7(MCP342x::channel3, MCP342x::oneShot,MCP342x::resolution18, MCP342x::gain1);// right 3

MCP342x::Config config8(MCP342x::channel4, MCP342x::oneShot,MCP342x::resolution18, MCP342x::gain1);// right 4
MCP342x::Config status;

float templ[4],tempr[4];   
class MCP3424
{
  
  uint8_t leftAdd,rightAdd;
  MCP342x adc;
  int startConversion = 1;
  long value1 = 0;
  uint8_t err;
  public:
  MCP3424(uint8_t _leftAdd,uint8_t _rightAdd)
  {
    
    leftAdd=_leftAdd;
    rightAdd=_rightAdd;
    
  }
  void init()
  {
    Wire.begin();
    MCP342x::generalCallReset();
  }
  // must declare valuables here
  boolean trig=0;
  void refresh(int _coldLeft,int _coldRight)
  {
    trig=0;
    //MCP342x::generalCallReset();
    long value = 0;
    uint8_t err;
    MCP342x adc = MCP342x(leftAdd);
    Wire.requestFrom(leftAdd, (uint8_t)1);
    // Initiate a conversion; convertAndRead() will wait until it can be read
    // read left 1
    err = adc.convertAndRead(MCP342x::channel1, MCP342x::oneShot, MCP342x::resolution16, MCP342x::gain1,1000000, value, status);
    if (!err) volToTemp(value,_coldLeft,&templ[1]);
    // read left 2
    err = adc.convertAndRead(MCP342x::channel2, MCP342x::oneShot, MCP342x::resolution16, MCP342x::gain1,1000000, value, status);
    if (!err) volToTemp(value,_coldLeft,&templ[2]);
    // read left 3
    err = adc.convertAndRead(MCP342x::channel3, MCP342x::oneShot, MCP342x::resolution16, MCP342x::gain1,1000000, value, status);
    if (!err) volToTemp(value,_coldLeft,&templ[3]);
    
    adc = MCP342x(rightAdd);
    //read right 1
     err = adc.convertAndRead(MCP342x::channel1, MCP342x::oneShot, MCP342x::resolution16, MCP342x::gain1,1000000, value, status);
    if (!err) volToTemp(value,_coldRight,&tempr[1]);
    //read right 2
     err = adc.convertAndRead(MCP342x::channel2, MCP342x::oneShot, MCP342x::resolution16, MCP342x::gain1,1000000, value, status);
    if (!err) volToTemp(value,_coldRight,&tempr[2]);
    //read right 3
     err = adc.convertAndRead(MCP342x::channel4, MCP342x::oneShot, MCP342x::resolution16, MCP342x::gain1,1000000, value, status);
    if (!err) volToTemp(value,_coldRight,&tempr[3]);
    //read right 4
     err = adc.convertAndRead(MCP342x::channel3, MCP342x::oneShot, MCP342x::resolution16, MCP342x::gain1,1000000, value, status);
    if (!err) volToTemp(value,_coldRight,&tempr[4]);
    trig=1;
   
   }
   boolean event(){if (trig==1) return 1;else return 0;}
//temperature are here   
   float s2(){return templ[1];}
   float b1(){return templ[2];}
   float b2(){return templ[3];}
   float pare(){return templ[4];}
   float ir(){return tempr[1];}
   float p1(){return tempr[2];}
   float p2(){return tempr[3];}
   float s1(){return tempr[4];}
   
   void volToTemp(long _value, int _cold, float *_outTemp)
   {
      _value=_value;// for read temp no delay only -- delete this if use delay reading
      
      //*_outTemp=(float)_value*1.560+_cold*0.9+1;
      *_outTemp=(float)_value*1.5625+_cold*0.94;
      if ((*_outTemp>700)or(*_outTemp<5)) *_outTemp=911;
   }
};

 

 

 



  


