// This #include statement was automatically added by the Particle IDE.
#include "neopixel.h"
#include "rest_client.h"

// RestClient client = RestClient("dev.tommetcalfe.net");
RestClient client = RestClient("dev.tommetcalfe.net");
#define PIXEL_PIN D2
#define PIXEL_COUNT 8
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

int photoresistor = A0;
int pwr = A1;
int led = D7;
bool getTweets = false;
bool gotResponse = false;
int resistorValue = 0;
char value[5];

//---------------------------------------------------
void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");
    strip.begin();
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

    if(resistorValue > 6 && !getTweets){
        // Spark.publish("Flag", "Get Data",60,PRIVATE);
            // getTweets = true;
            get();
    }
    else if(resistorValue < 5  && getTweets) {
        allOff();
        // Spark.publish("Flag", "Reset",60,PRIVATE);
        getTweets = false;
    }
    else {

    }
    sprintf(value,"%d",resistorValue);
    //  For Debug Purposes only
    //  Spark.publish("Resistor",value,60,PRIVATE);
    // Serial.println("...");
    delay(500);
}
String response;
//---------------------------------------------------
void get() {
    bail:
    response = "";
    String body = "";
    // client.setHeader("X-Test-Header: true");
    int statusCode = client.get("/searchTwitter.php", &response);
    Serial.println("RESP");
    Serial.println(response);
    char code[20];

    sprintf(code,"%d",statusCode);
    //Spark.publish("Status Code",code,60,PRIVATE);
    if(statusCode == 0) {
        delay(500);
        goto bail;
    }
    else {
        if(response != "null") {
            Spark.publish("Number of Tweets",response,60,PRIVATE);
            int uniqueResponseCode = response.toInt();
            if(uniqueResponseCode > 0) {
                successGlow(50, 45, 252);
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
    for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=1) {
        for(int i=0; i<strip.numPixels(); i++) {
            // sets the value (range from 0 to 255):
            strip.setPixelColor(i, strip.Color(fadeValue,fadeValue,fadeValue));
        }
        strip.show();
        delay(10);
    }
    // Wait a while
    delay(5000);
    for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=1) {
        for(int i=0; i<strip.numPixels(); i++) {
            // sets the value (range from 0 to 255):
            strip.setPixelColor(i, strip.Color(fadeValue,fadeValue,fadeValue));
        }
        strip.show();
        delay(3);
    }
}
//---------------------------------------------------
void allOff(){
    for(int i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0,0,0));
    }
    strip.show();
}
