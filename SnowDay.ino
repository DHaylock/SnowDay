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

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    Spark.publish("Start up", "Starting",60,PRIVATE);
    Serial.begin(9600);
    pinMode(led, OUTPUT);
    pinMode(photoresistor,INPUT);
    pinMode(pwr,OUTPUT);
    digitalWrite(pwr,HIGH);
}
//---------------------------------------------------
void loop() {
    resistorValue = analogRead(photoresistor);

    if(resistorValue < 100){

        if(getTweets == false) {
            Spark.publish("Flag", "Lock",60,PRIVATE);
            get();
        }
    }
    else if(resistorValue > 101 && getTweets) {
        digitalWrite(led,LOW);
        for(int i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(0,0,0));
        }
        strip.show();

        if(getTweets == true) {
            Spark.publish("Flag", "Unlock",60,PRIVATE);
            getTweets = false;
        }
    }
    else {

    }
    sprintf(value,"%d",resistorValue);
    //  For Debug Purposes only
    //  Spark.publish("Resistor",value,60,PRIVATE);
    delay(500);
}
//---------------------------------------------------
void get() {
    String response = "";
    int statusCode = client.get("", &response);
    Serial.println(statusCode);
    Serial.println(response);
    Spark.publish("Number of Tweets",response,60,PRIVATE);
    if(response.toInt() > 0) {
        digitalWrite(led,HIGH);
         for(int i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(0,0,255));
        }
        strip.show();
    }
    else{
        digitalWrite(led,LOW);
    }

    getTweets = true;
}
