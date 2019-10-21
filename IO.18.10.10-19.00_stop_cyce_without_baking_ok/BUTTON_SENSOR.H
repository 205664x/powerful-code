#define delaySensor 40
#define delayButtonPress 30

class BUTTON
{
  int pinBt1,pinBt2,pinBt3,pinBt4;
  public: 
  BUTTON(int _bt1,int _bt2, int _bt3, int _bt4)
  {
    pinBt1=_bt1;
    pinBt2=_bt2;
    pinBt3=_bt3;
    pinBt4=_bt4;
    pinMode(pinBt1,INPUT);
    pinMode(pinBt2,INPUT);
    pinMode(pinBt3,INPUT);
    pinMode(pinBt4,INPUT);
  }
  boolean btRotate=0,btSelect=0,btCycle=0,btClean=0;
  unsigned int btR0,btR1,btS0,btS1,btCY0,btCY1,btCL0,btCL1;// button count time zero and 1
  
  void refresh()
  {
// cycle bake button    
    if (digitalRead(bt3)==0){ 
       if (btCL0<1000) btCL0+=1;  
       btCL1=0;     
    }
    else {
      if (btCL1<1000)btCL1+=1;
      btCL0=0;
    }    
// clean  button    
    if (digitalRead(bt4)==0) { 
       if (btCY0<1000) btCY0+=1;  
       btCY1=0;     
    }
    else {
      if (btCY1<1000)btCY1+=1;
      btCY0=0;
    }    
// select button
    if (digitalRead(bt2)==0) { 
       if (btS0<1000) btS0+=1;  
       btS1=0;     
    }
    else {
      if (btS1<1000)btS1+=1;
      btS0=0;
    }         
// rotate button    
    if (digitalRead(bt1)==0) { 
       if (btR0<1000) btR0+=1;  
       btR1=0;     
    }
    else {
      if (btR1<1000)btR1+=1;
      btR0=0;
    }    
               
    
  }
  int clean()
  {
    if (btCL1>delayButtonPress) return 1;
    else if (btCL0>delayButtonPress) return 0;
    else return 2;
  }
  int cycle()
  {
    
    if (btCY1>delayButtonPress) return 1;
    else if (btCY0>delayButtonPress) return 0;
    else return 2;
  }
  int rotate()
  {
    
    if (btR1>delayButtonPress) return 1;
    else if (btR0>delayButtonPress) return 0;
    else return 2;
  }
  int white()
  {
    
    if (btS1>delayButtonPress) return 1;
    else if (btS0>delayButtonPress) return 0;
    else return 2;
  }  
};


class SENSOR{
  int pinTop,pinBot,pinLeft,pinRight;
  unsigned cT,cB,cL,cR;
  public: 
  SENSOR(int _Top,int _Bot, int _Left, int _Right)
  {
    pinTop=_Top;
    pinBot=_Bot;
    pinLeft=_Left;
    pinRight=_Right;
    pinMode(pinTop,INPUT);
    pinMode(pinBot,INPUT);
    pinMode(pinLeft,INPUT);
    pinMode(pinRight,INPUT);
  }
  void refresh(){
    if (digitalRead(pinTop)==1) {
      if (cT<1000)cT+=1;
    }else cT=0;
    if (digitalRead(pinBot)==1) {
      if (cB<1000)cB+=1;
    }else cB=0;
    if (digitalRead(pinLeft)==1) {
      if (cL<1000)cL+=1;
    }else cL=0;
    if (digitalRead(pinRight)==1) {
      if (cR<1000)cR+=1;
    }else cR=0;  
  }
  int top(){
    if (cT>delaySensor) return 1;else return 0;
  }  
  int bot(){
    if (cB>delaySensor) return 1;else return 0;
  }
  int left(){
    if (cL>delaySensor) return 1;else return 0;
  }
  int right(){
    if (cR>delaySensor) return 1;else return 0;
  }
};

class DIGITALINPUT{
  int pinDoor;
  unsigned cD;
  public: 
  DIGITALINPUT(int _Door)
  {
    pinDoor=_Door;
    pinMode(pinDoor,INPUT);
  }
  void refresh(){
    if (digitalRead(pinDoor)==1) {
      if (cD<1500)cD+=1;
    }else cD=0;
    
  }
  int door(){
    if (cD>delaySensor) return 1;else return 0;
  }  
  
};


