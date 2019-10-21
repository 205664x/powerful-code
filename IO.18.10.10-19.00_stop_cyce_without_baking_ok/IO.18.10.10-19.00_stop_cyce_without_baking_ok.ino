static char _input_tem,firstPressBake=0;// first press bake to avoid first running of shuttle and press motor
boolean firstRunIR =0;
unsigned long dataPeriod=millis(),dataFrequency=millis(),mcpUpdate=millis();
static int baking=0,cleanFlag=0; // no baking value =0 baking mean after press cycle value =1
unsigned long noConnectTime=0;
int tLeft,tRight;
static int numberOfDoughToStop=0;
static int rememberBot=0;
static int mode=1;// low mode and high mode 0: low mode 1: high mode default

#include "CONFIGURATION.H"

#include "L9822_CONTACTOR.h"
L9822 con(13,14,4);//chip 1 ,chip 2, reset pin

#include "READ_TEMP.h"// read mcp3424 and DS18B20Z
MCP3424 mcp(0x68,0x6E);// left address,right address
#include "BUTTON_SENSOR.H"
BUTTON btn(bt1,bt2,bt3,bt4);
SENSOR sen(senTop,senBot,senLeft,senRight);// pin proximity sensor top bot left right
DIGITALINPUT door(doughDoor);
#include "CYCLE_BAKE.H"

#include "CONTROL_TEMPERATURE.H"
BASETEMP base1(20000),base2(20000);
IRTEMP ir;
PRESSTEMP pr,sh;
PRESS1 p1(12000);
SHUTTLE1 s1(12000);
#include "SAFETY.H"
#include "PRINT_TO_SCREEN.H"// input and output data via Serial1 port

#include "STARTUPFUNCTION.H"
void setup() {  
  delay(1000);
  pinMode(24,OUTPUT);digitalWrite(24,1);
  pinMode(doughDoor,INPUT);
  mcp.init();
  dallas (left_temp_ic,1); // left ic temperature
  dallas (right_temp_ic,1); // right ic temperature
  con.isoV1(1); 
  Serial1.begin(SERIALSPEED); 
  // start 1ms interrupt 
  OCR0A = 0xAF;TIMSK0 |= _BV(OCIE0A);

// special function
  keepWaiting();
  
}
SIGNAL(TIMER0_COMPA_vect){ //1ms function
  btn.refresh();
  sen.refresh();
  door.refresh();
// start baking        
  if ((btn.cycle()==1) and(baking==0)) {
      baking=1;
      cleanFlag=0;
      stateC=0;
      firstPressBake=0;// dont paulse when temperature is too cold for first time press bake button
      shuttleGetStuck=false;// reset bake when shuttle is stuck
      clutchTime=millis();
     }
  if (baking==1) bake();
// go to clean position     
 if ((btn.clean()==1)and (cleanFlag==0)){
      stateC=0;
      baking=0;      
      cleanFlag=1;
      clutchTime=millis();
    }
    if (cleanFlag==1) cleanHome();
    // rotate
  if (btn.rotate()==1) con.rm(1); else con.rm(0);
    if (WRONG==1) con.allOff();     
   con.updateContactor();    
}


void loop() {
  // Pause heating IR element 
  static boolean firstCheckIR=true;
  if ((millis()>5000)and(firstCheckIR)){
    if (mcp.ir()>100)firstRunIR=1;
    firstCheckIR=false;
  }
  if (millis()>delayBeforeheat){
    firstRunIR=1;
  }
//update new temperature  mcp.ir()
   if ((setTime(&dataPeriod,1500))){        
      tLeft=dallas (left_temp_ic,0);tRight=dallas (right_temp_ic,0);
      mcp.refresh(tLeft,tRight);// update temperature from mcp3424 update 1 chanel per time     
   }    
   if ((setTime(&dataFrequency,SCREENoutputSpeed))){
    printToScreen();
   } 
//checkDoor(con.state1());
   if (btn.rotate()==1) {
      con.b1(base1.up(mcp.b1(),SB+3)); 
      con.b2(base2.up(mcp.b2(),SB+3)); 
   }else {con.b1(0);con.b2(0);}  
//wait 10min let oven cold to clean oven 
   if ((firstRunIR==1)){  
      
      con.cano(1);    
      //Ir temperature control
      con.ir12(1);
      if (btn.white()==1) con.ir3(ir.up(mcp.ir(),SIR1,16000,25000));   // white  
      else if (btn.white()==0) con.ir3(ir.up(mcp.ir(),SIR2,22000,25000));// wholemeal

      
      if (mode==0){
// Low mode        
        // press temperature control
        if ((mcp.p1()<190) and (mcp.p2()<450))
        con.pe(p1.up(mcp.p1(),mcp.p2(),200,350));
        else con.pe(p1.up(mcp.p1(),mcp.p2(),190,280));

        // shuttle
        if ((mcp.s1()<190) and (mcp.s2()<450))
        con.se(s1.up(mcp.s1(),mcp.s2(),200,330));
        else con.se(s1.up(mcp.s1(),mcp.s2(),190,250));
      }else{
// High Mode        
          // press temperature control
        if ((mcp.p1()<175) and (mcp.p2()<500))
        con.pe(p1.up(mcp.p1(),mcp.p2(),165,380));
        else con.pe(p1.up(mcp.p1(),mcp.p2(),165,330));

        // shuttle
        if ((mcp.s1()<180) and (mcp.s2()<430))
        con.se(s1.up(mcp.s1(),mcp.s2(),170,360));
        else con.se(s1.up(mcp.s1(),mcp.s2(),170,340));
      }
      
   }
   
// check element faulty
   checkFaulty();
// stop without baking 2 mins
  
  if((changingTemp(stopWithoutBake)))
  {
    stateC=0;
    baking=0;      
    cleanFlag=1;
  }
//-----------------------------------------------------//  
  if (stopAfter20min()){
    numberOfDoughToStop=1;
  }
  
  if (numberOfDoughToStop>0){
    if ((sen.bot()==1)and(rememberBot==0)){
      rememberBot=1;
      numberOfDoughToStop+=1;
    }
    if (sen.top()==1)rememberBot=0;
  }
   if (numberOfDoughToStop>numberOfDoughBeforeStop){
    //numberOfDoughToStop=0;
    stateC=0;
    baking=0;
    cleanFlag=1;
   }
///----Slow mode and High Mode------------------------------------------------------------//
/*
static int cyclePress=0;
static int firstPressCycle=0;
if (baking==0){
// change defaul mode here    
    mode =1;
    firstPressCycle=0;
  }
if ((btn.cycle()==1)and(cyclePress==0)){
  cyclePress=1;     
}

if (btn.cycle()==0){
  if ((cyclePress==1)){
    if (firstPressCycle==0) firstPressCycle=1;
    else mode=not mode;
  }
  cyclePress=0;
}
*/
}


