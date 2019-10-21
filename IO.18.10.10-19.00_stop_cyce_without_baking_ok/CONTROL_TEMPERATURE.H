/*read me
   BASETEMP() control base temperature
   IRTEMP() control ir temperature
   PRESSTEMP() ver1 control press and shuttle
   PRESS1() ver2 for PRESS and Shuttle

*/
class BASETEMP {
    unsigned long updateTime;
    unsigned long count = millis();
    int out = 1;
  public: BASETEMP(unsigned long updateTimei)  {
      updateTime = updateTimei;
    }
    boolean up(int temp, int setpoint)  {
      static int firstControlState = 0;
      if (firstControlState == 0) {
        firstControlState = 1;
        if (temp < (setpoint)) out = 1;
        if (temp > (setpoint + 4))out = 0;
      }
      if ((unsigned long)(millis() - count) > updateTime)    {
        if (temp < setpoint) out = 1; else out = 0;
        count = millis();
      }
      return out;
    }
};
class IRTEMP
{
    unsigned long timeOn, timeOff;
    int stateIR = 0;
    //float temp,setpoint;
  public:
    IRTEMP()  {  }
    boolean up(float tempi, int setpointi, unsigned long timeOni, unsigned long timeOffi) {
      timeOn = timeOni;
      timeOff = timeOffi;
      static unsigned long countOn = millis(), countOff = millis();
      if ((stateIR == 1) and ((unsigned long)(millis() - countOn) > timeOn))
      {
        stateIR = 0;
        countOff = millis();
      }
      else if ((stateIR == 0) and ((unsigned long)(millis() - countOff) > timeOff))
      {
        stateIR = 1;
        countOn = millis();
        if (tempi > setpointi){
          stateIR = 0;
          countOff = millis();;
        }
      }
      
      if (tempi < (setpointi - 15))
      {
        stateIR = 1;
        countOn = millis();
      }
      return stateIR;
    }
};
class PRESSTEMP// turn on turn off regulary
{
    unsigned long timeOn, timeOff;
    int stateIR = 0;
    //float temp,setpoint;
  public:
    boolean up(float tempi, float tempair, int setpointi, int setpointair, unsigned long timeOni, unsigned long timeOffi) {
      timeOn = timeOni;
      timeOff = timeOffi;
      static unsigned long countOn = millis(), countOff = millis();
      if ((stateIR == 1) and ((unsigned long)(millis() - countOn) > timeOn))
      {
        stateIR = 0;
        countOff = millis();
      }
      else if ((stateIR == 0) and ((unsigned long)(millis() - countOff) > timeOff))
      {
        stateIR = 1;
        countOn = millis();
      }
      if ((tempi > setpointi) or (tempair > setpointair))
      {
        stateIR = 0;
        countOff = millis();;
      }
      if ((tempi < (setpointi - 20)) and (tempair < (setpointair - 60)))
      {
        stateIR = 1;
        countOn = millis();
      }
      return stateIR;
    }
};

class PRESS1
{
    unsigned long setime;
    public:
    PRESS1(unsigned long repeatime) {
      setime = repeatime;
    }
    unsigned long cycle = millis(), updatePI = millis(), setPItime;
    float tempSpeed = 0, distance, p1, tempSpeed_0, distance_0, p0;
    int prebase2, prebase1;
    boolean active = 0, active0 = 0;
    unsigned long timeON, timeOFF;
    boolean up(int base1, int base2, int setpoint, int safety)  {
      setPItime = 5000; // frequency time to get samples.
      if ((unsigned long)(millis() - updatePI) > setPItime)    {
        // control base temp
        tempSpeed_0 = (base1 - prebase1);
        prebase1 = base1;
        distance_0 = setpoint - base1;
        if ((tempSpeed_0 < (-1000)) or (tempSpeed_0 > 1000)) tempSpeed_0 = 0;
        tempSpeed_0 = (float)tempSpeed_0 / (setPItime / 1000);
        p0 = (float)distance_0 * 0.98 - (float)(tempSpeed_0 * 19);// 16 ver1 
        // control air temp
        tempSpeed = (base2 - prebase2);
        prebase2 = base2;
        distance = safety - base2;
        if ((tempSpeed < (-1000)) or (tempSpeed > 1000)) tempSpeed = 0;
        tempSpeed = (float)tempSpeed / (setPItime / 1000);
        p1 = (float)distance * 0.9 - (float)(tempSpeed * 16);
        updatePI = millis();
      }
      if ((unsigned long)(millis() - cycle) > setime)
      {
        if ((p1 > 0) and (p0 > 0)) active = 1; else active = 0;
        if (p0 > 8) active0 = 1; else if (p0 < 0) active0 = 0;
        cycle = millis();
      }
      if (base2>(safety+40)) active=0;
      return active;
    }
};
class SHUTTLE1
{
    unsigned long setime;
    public:
    SHUTTLE1(unsigned long repeatime) {
      setime = repeatime;
    }
    unsigned long cycle = millis(), updatePI = millis(), setPItime;
    float tempSpeed = 0, distance, p1, tempSpeed_0, distance_0, p0;
    int prebase2, prebase1;
    
    unsigned long timeON, timeOFF;
    boolean up(int base1, int base2, int setpoint, int safety)  {
      static boolean active = 0, active0 = 0;
      setPItime = 5000; // frequency time to get samples.
      if ((unsigned long)(millis() - updatePI) > setPItime)    {
        // control base temp
        tempSpeed_0 = (base1 - prebase1);
        prebase1 = base1;
        distance_0 = setpoint - base1;
        if ((tempSpeed_0 < (-1000)) or (tempSpeed_0 > 1000)) tempSpeed_0 = 0;
        tempSpeed_0 = (float)tempSpeed_0 / (setPItime / 1000);
        p0 = (float)distance_0 * 0.98 - (float)(tempSpeed_0 * 16);
        // control air temp
        tempSpeed = (base2 - prebase2);
        prebase2 = base2;
        distance = safety - base2;
        if ((tempSpeed < (-1000)) or (tempSpeed > 1000)) tempSpeed = 0;
        tempSpeed = (float)tempSpeed / (setPItime / 1000);
        p1 = (float)distance * 0.9 - (float)(tempSpeed * 20);
        updatePI = millis();
      }
      if ((unsigned long)(millis() - cycle) > setime)
      {
        if ((p1 > 0) and (p0 > 0)) active = 1; else active = 0;
        if (p0 > 8) active0 = 1; else if (p0 < 0) active0 = 0;
        cycle = millis();
      }
      if (base2>(safety+40)) active=0;
      return active;
    }
};



