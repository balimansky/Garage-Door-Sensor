#include <ESP8266WiFi.h>

class HallSense
{
  public:
    int pin;
    String sname;
    boolean currentState, priorState;
    long currentTime, priorTime;
    long timeVec[1000];
    int count;
    boolean moving;
    WiFiClient *client;
    
    HallSense(int, String);
    void Reset();
    void checkState(WiFiClient *);
    void tellWorld(long, WiFiClient *);
};

