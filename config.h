
#define APSSID "LightMeter"
#define APPSK  "babcia12"
#define RED_pin 15 
#define GREEN_pin 12
#define BLUE_pin 13
#define TIMER_pin 15
/* Set these to your desired credentials. */
// testing commit
const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);
#define MaxISOIndex             57
#define MaxApertureIndex        70
#define MaxTimeIndex            80
#define MaxNDIndex              13
#define MaxFlashMeteringTime    5000


unsigned long previousMillis = 0;
const long interval = 420;
const long timer_interval = 10;
float   lux;
boolean Overflow = 0;                                   // Sensor got Saturated and Display "Overflow"
float   ISOND = 1;
boolean measure=true;
float EV;
float EVcorr=0;

float T;
float A=1;
long  iso=8;

boolean RED_light=false;
boolean WHITE_light=false;
boolean TimerLock=false;// EXPOSURE LOCK
boolean Timer=false;
int TimerCounter=0;
