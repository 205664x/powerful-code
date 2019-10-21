#define SIR1 335 // safeIR white dough  temperature 
#define SIR2 345 // safe IR whole meal dough temperature 
#define SB 180 // safe base tempearture
#define SP 160 // safe press temperature
#define SS 160 // safe shuttle



#define SERIALSPEED 74880
#define SCREENoutputSpeed 500
#define StartPressMotorAfterLeftSensorOff 1250//orginal is 1250
#define PlusTimeWhenStartFromCold 2500
#define stopMotorsWhenStuck 6000
 
#define STOPWHENTEMPTOOCOLD 6000 // top cycle when oven too cold. in millis
#define DELAYIR 600000 // delay heating IR, press, and shuttle for cleaning
#define TIMEOUTHEAT 25000 // if element too hot after this time. shutdown oven
#define delayBeforeheat 600000 // in millis second let staff clean oven before preheat
#define stopWithoutBake 140000 // pause baking if no changing temperature in mimllis second
#define stopToClean 1200000// clean oven after this time =mins*60*1000 ; flashing LCD
#define numberOfDoughBeforeStop 12

#define stopWithOutConnection 36000
//proximity sensor pin
  #define senLeft 20 // out
  #define senRight 21 // in
  #define senTop 18
  #define senBot 19
// button pin
  #define bt1 8 // rotate button
  #define bt2 9 // select wholemeal or white 
  #define bt3 15 // bake button
  #define bt4 3 // clean button
// iso contactor pin
  #define isoPin 1 
#define doughDoor 31   

boolean setTime(unsigned long *_input, unsigned long _length )
{  
  if (((unsigned long)(millis()-*_input))>_length) {
    *_input=millis();
    return 1;   
  }else return 0;
}

  
