static int stateC=0; //state cycle
boolean shuttleGetStuck = false;
static boolean _run=1,wait_time=0,firstStop=0,hot=1,stopBot=0;
int delayShuttle=0;// start Press plate latter when motor is stop and faster when motor running
static unsigned long countTimeWT=0,startPressMotor, timeAfterDoorClose;
static boolean _count=0;
int reSenLeft=0;
unsigned long pauseBakeTime,clutchTime;
boolean stateCon(uint8_t _in, char bitNumber)
{
  if ((_in>>bitNumber&1)==0) return 1;else return 0;
}
void bake()
{  
  
  switch (stateC){
    case 0:
      if (sen.top()==0) con.pm(1);
      if (sen.top()==1) {con.pm(0);stateC=1;}
      if ((sen.left()==1)and(reSenLeft==0)) {startPressMotor=millis();reSenLeft=1;}
    break;
    case 1:

      if ((sen.left()==1)and(reSenLeft==0)) {startPressMotor=millis();reSenLeft=1;}
      if ((sen.right()==0)) con.sm(1);
      if ((sen.right()==1)) {con.sm(0);stateC=2;}// after time let go to next step
     
    break;
    case 2:
      reSenLeft=0;
      if (sen.right()==1) con.pm(1);
      if (sen.bot()) {con.sm(1);stateC=0;firstPressBake=1;}
      
    break;
  }
  // when motor run in first time it is lower than non stop
  static int rememberDoorOpen;
  if (door.door()!=rememberDoorOpen) {// check time door open when it open dont do delay time function
    rememberDoorOpen=door.door();
    timeAfterDoorClose=millis();
    
    }//and (setTime(&timeAfterDoorClose,(12000)))
   //if ((delayShuttle==0)and((millis()-startPressMotor)>StartPressMotorAfterLeftSensorOff)and (reSenLeft==1)and ((millis()-timeAfterDoorClose)>12000)) con.pm(1);

    
// -------------- checking heat  
if ((mcp.p1()<SP)or(mcp.s1()<SS)) hot=0;
if ((mcp.p1()>SP)and(mcp.s1()>SS)and(sen.right())) hot=1;// prevent it run when if is not 4 second
    if ((hot==0) and (firstStop==0)and(sen.left()==1)and(firstPressBake==1))
    {
      con.sm(0);
      stateC=0;
      countTimeWT+=1;
      delayShuttle=1;
      
    }
    if ((firstStop==0)and(countTimeWT>STOPWHENTEMPTOOCOLD)){
      firstStop=1;
      clutchTime=millis();
    }
   if (sen.bot()) {
    firstStop=0;
    countTimeWT=0;
    delayShuttle=0;
   }
// CHECK SAFTY cluch
  //if (stateCon(con.state1(),1)) int a234;// shuttle motor
  //if (stateCon(con.state1(),2)) int a234;// press motor
  if (digitalRead(doughDoor)==0){ 
    clutchTime=millis();
    startPressMotor=millis()+2000;// RESET shuttle time delay
  }
  if ((digitalRead(doughDoor))and(stateCon(con.state1(),1))){
      static int rememberSenLeft,rememberSenRight;
      if (sen.left() != rememberSenLeft) {rememberSenLeft=sen.left();clutchTime=millis();}
      if (sen.right() != rememberSenRight) {rememberSenRight=sen.right();clutchTime=millis();}
      if (setTime(&clutchTime,stopMotorsWhenStuck)) {
        con.pm(0);
        con.sm(0);
        stateC=0;
        baking=0;
        shuttleGetStuck=true;
        }      
    }else clutchTime=millis();
//---- end checking heat -----
}
void cleanHome(){
  switch (stateC){
    case 0:
      if (sen.top()==0) con.pm(1);
      if ((sen.bot()==1)and(sen.left()==0)) con.sm(1);
      if (sen.left()==1) con.sm(0);
      if (sen.top()==1) {con.pm(0);stateC=1;}
    break;
    case 1:
      if (sen.left()==0) con.sm(1);   
      if (sen.left()==1) {con.sm(0);stateC=10;}
    break;
    case 2:      
    break;
  }  
}
unsigned long delay5s=millis();
boolean changingTemp(unsigned long CountTime)
{
  static float preP1,preS1;
  static unsigned long timeCount=millis();
  if (setTime(&delay5s,5000)==1){
    if ((abs(mcp.p1()-preP1)>6)and(mcp.p1()>45))timeCount=millis();
    preP1=mcp.p1();
    if ((abs(mcp.s1()-preS1)>6)and(mcp.s1()>45))timeCount=millis();
    preS1=mcp.s1();
    if ((mcp.p1()<190)or(mcp.s1()<190))timeCount=millis();
  }
  
  if((baking==0)or(btn.cycle()==1))timeCount=millis();
  pauseBakeTime=(unsigned long)(millis()-timeCount);
  
  if (((unsigned long)(millis()-timeCount))>CountTime) {
    timeCount=millis();
    return 1;
  } else return 0; 
}
boolean stopAfter20min(){
  static boolean first20min;
  static unsigned long preTimeStop;
  if(btn.cycle()==1)numberOfDoughToStop=0;
  //if (baking==0)numberOfDoughToStop=0;
  if ((baking)and(first20min==0)){
    first20min=1;
    preTimeStop=millis();
  }
  if (((unsigned long)(millis()-preTimeStop))>stopToClean){
     first20min=0;
     return 1;
  }
  return 0;
}


