void printTemp(int _in){
  if (_in<10) Serial1.print("  ");
  else if (_in<100) Serial1.print(" ");
  Serial1.print(_in);
}
void contactorState(uint8_t _in, char bitNumber)
{
  if ((_in>>bitNumber&1)==0) Serial1.print("H");else Serial1.print("L");
}

void printToScreen(){
//start line 1      
      Serial1.print("~!1 ");
      if (WRONG==1)
        Serial1.print("ERROR, Oven too hot");
      else if (shuttleGetStuck==true){
        Serial1.print("Shuttle is Jammed");
      }
// cleaning time before heating      
      else if (numberOfDoughToStop>2){
        Serial1.print("Stop Baking in: ");Serial1.print(numberOfDoughBeforeStop-numberOfDoughToStop+1);
      }
      else if (firstRunIR==0){
         Serial1.print("Heating in:");
         Serial1.print((delayBeforeheat-millis())/60000);
         Serial1.print(":");
         Serial1.print((delayBeforeheat-millis())/1000%60);
      }else if (firstRunIR==1){
        
        if ((mcp.b1()>(SB-5))or(mcp.b2()>(SB-5))){
      
          Serial1.print("Ready to Bake");
        }
        else {
          Serial1.print("Heating up");
          static int numberOfDot=0;
          for (int i=0;i<=numberOfDot;i++) Serial1.print(".");
          numberOfDot+=1;
          if (numberOfDot==10)numberOfDot=0;
        }
      }
      
//start line 2      
      Serial1.print("!2");
      //Serial1.print(btn.white());
      if (numberOfDoughToStop>2) Serial1.print("For Cleaning ");
      else if (btn.white())Serial1.print("Bake White");else Serial1.print("Bake Meal");
      if (mode==0) Serial1.print(" -Slow");else Serial1.print(" -High ");
      if ((mcp.p2()>360)or(mcp.s2()>360)) Serial1.print("-Hot");else Serial1.print("-Ok");
//Start line 3      
      Serial1.print("!3");
      Serial1.print("Wheel:");
      contactorState(con.state1(),3);
      Serial1.print(" IR:");printTemp(mcp.ir());contactorState(con.state1(),4); Serial1.print(" ");
      if (btn.white())Serial1.print(SIR1);else Serial1.print(SIR2);
//Start line 4   
      Serial1.print("!4");
      Serial1.print("B1:");printTemp(mcp.b1());contactorState(con.state2(),4);
      Serial1.print(" B2:");printTemp(mcp.b2());contactorState(con.state2(),3);
      Serial1.print(" ");Serial1.print(SB);
//Srart line 5
      Serial1.print("!5");
      Serial1.print("P1:");printTemp(mcp.p1());contactorState(con.state2(),5);
      Serial1.print(" P2:");printTemp(mcp.p2());
      Serial1.print("  ");Serial1.print(SP);
//Srart line 6      
      Serial1.print("!6");
      Serial1.print("S1:");printTemp(mcp.s1());contactorState(con.state2(),7);
      Serial1.print(" S2:");printTemp(mcp.s2());
      Serial1.print("  ");Serial1.print(SS);
//Srart line 7      
      Serial1.print("!7");
      Serial1.print("CL");printTemp(tLeft);
      Serial1.print("CR");printTemp(tRight);
      static boolean dot=0;
      if (dot) Serial1.print("|");else Serial1.print(" ");
      dot=not dot;
      Serial1.print((stopWithoutBake-pauseBakeTime)/1000);
      Serial1.print("|");
      Serial1.print(digitalRead(doughDoor));
      

//Start line 8 for new screen code onely
      Serial1.print("!8"); 
      if ((numberOfDoughToStop>2)or(shuttleGetStuck==true)) Serial1.print("1");else Serial1.print("0");// flashing LED
      
//End      
      Serial1.println("#");
      //data.out(mcp.ir(),mcp.p1(),mcp.p2(),mcp.s1(),mcp.s2(),mcp.b1(),mcp.b2(),tLeft,tRight,SIR2,SB,SP,SS,con.state1(),con.state2());   
}
boolean checkHeat(){
  return 1;
}

