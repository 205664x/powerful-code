#define pinDoorRear A0
boolean WRONG=0;
void checkDoor(uint8_t _temp)
{
  if ((_temp&10000000)>0) WRONG=1;
}


boolean checkRearDoor()
{
  static boolean firstRunCRD=0;
  if (firstRunCRD==0){
    
    pinMode(pinDoorRear,INPUT);
    firstRunCRD=1;
  }
if (digitalRead(pinDoorRear)==0)
  {
    if((sen.left()==0)or(sen.top()==0))
    return 1;
  }
  return(0);
}
unsigned long tooHotTimeOut=millis();
boolean tooHot=0;
void checkFaulty()
{
  if (((mcp.p1()<0)or(mcp.p1()>280)or(mcp.s1()<0)or(mcp.s1()>280)or(mcp.ir()<0)or(mcp.ir()>400)or
      (mcp.p2()<0)or(mcp.p2()>520)or(mcp.s2()<0)or(mcp.s2()>450)or
      (mcp.b1()<0)or(mcp.b1()>220)or(mcp.b2()<4)or(mcp.b2()>220)or
      (mcp.p1()==0)or(mcp.s1()==0)or(mcp.ir()==0)or(mcp.p2()==0)or
      (mcp.s2()==0)or(mcp.b1()==0)or(mcp.b2()==0)
      ))
      {
        if (tooHot==0)
          {
            tooHot=1;
           tooHotTimeOut=millis();
        }
      
      }else tooHot=0;
      
   // if ((tooHot==1)and((millis()-tooHotTimeOut)>20000))WRONG=1;
   if ((tooHot==1)and(setTime(&tooHotTimeOut,TIMEOUTHEAT))) WRONG=1;
}
