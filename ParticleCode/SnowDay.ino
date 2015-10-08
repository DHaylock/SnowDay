#include "neopixel.h"
#include "rest_client.h"

RestClient client = RestClient("");

#define PIXEL_PIN D2
#define PIXEL_COUNT 16
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

int photoresistor = A0;
int pwr = A5;
int led = D7;
bool getTweets = false;
int resistorValue = 0;
char value[5];

//---------------------------------------------------
void setup() {
    Serial.begin(9600);
    strip.begin();
    Serial.println("Starting...");
    strip.show();

    Spark.publish("Start up", "Starting",60,PRIVATE);

    pinMode(led, OUTPUT);
    pinMode(photoresistor,INPUT);
    pinMode(pwr,OUTPUT);
    digitalWrite(pwr,HIGH);
}
//---------------------------------------------------
void loop() {
    resistorValue = analogRead(photoresistor);

    if(resistorValue < 10 && !getTweets){
        // Spark.publish("Flag", "Lock",60,PRIVATE);
        get();
    }
    else if(resistorValue > 11 && getTweets) {
        allOff();
        // Spark.publish("Flag", "Unlock",60,PRIVATE);
        getTweets = false;
    }
    else {

    }
    sprintf(value,"%d",resistorValue);
    //  For Debug Purposes only
    //  Spark.publish("Resistor",value,60,PRIVATE);
    Serial.println("...");
    delay(500);
}
//---------------------------------------------------
void get() {
    bail:
    String response = "";
    int statusCode = client.get("", &response);
    char code[20];

    sprintf(code,"%d",statusCode);
    // Spark.publish("Status Code",code,60,PRIVATE);
    if(statusCode == 0) {
        delay(500);
        goto bail;
    }
    else {
        if(response != "null") {
            int uniqueResponseCode = response.toInt();
            Spark.publish("Number of Tweets",response,60,PRIVATE);
            if(uniqueResponseCode > 0) {
                successGlow(50, 45, 252);
                delay(10000);
                allOff();
                getTweets = true;
            }
            else if (uniqueResponseCode == 0) {
                // Do nothing there are no tweets
                getTweets = true;
            }
            else if (uniqueResponseCode < 0) {
                errorFlash(uniqueResponseCode);
                getTweets = true;
            }
        }
    }
}
//---------------------------------------------------
void errorFlash(int whichError){
    int dTimer = 250;
    switch(whichError) {
        case -4:
            for (int d = 0; d < 3; d++) {
                for(int i=0; i<strip.numPixels(); i++) {
                    strip.setPixelColor(i, strip.Color(255,0,0));
                }
                strip.show();
                delay(dTimer);
                allOff();
                delay(dTimer);
            }
            break;
        case -439:
            for (int d = 0; d < 3; d++) {
                for(int i=0; i<strip.numPixels(); i++) {
                    strip.setPixelColor(i, strip.Color(109, 93, 252));
                }
                strip.show();
                delay(dTimer);
                allOff();
                delay(dTimer);
            }
            break;
        case -5:
            for (int d = 0; d < 3; d++) {
                for(int i=0; i<strip.numPixels(); i++) {
                    strip.setPixelColor(i, strip.Color(255,255,0));
                }
                strip.show();
                delay(dTimer);
                allOff();
                delay(dTimer);
            }
            break;
        default:

        break;
    }
}
//---------------------------------------------------
void successGlow(int r,int g, int b){
    for(int i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(r,g,b));
    }
    strip.show();
}
//---------------------------------------------------
void allOff(){
    for(int i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0,0,0));
    }
    strip.show();
}
