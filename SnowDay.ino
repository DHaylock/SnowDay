#include "rest_client.h"

RestClient client = RestClient("");

int photoresistor = A0;
int pwr = A5;
int led = D7;
bool getTweets = false;
int resistorValue = 0;
char value[5];

//---------------------------------------------------
void setup() {
    Serial.begin(9600);
    // Spark.connect();
    pinMode(led, OUTPUT);
    pinMode(photoresistor,INPUT);
    pinMode(pwr,OUTPUT);
    digitalWrite(pwr,HIGH);
}
//---------------------------------------------------
void loop() {
    resistorValue = analogRead(photoresistor);

    if(resistorValue < 10){
        digitalWrite(led, HIGH);
        if(getTweets == false) {
            Spark.publish("Flag", "Lock",60,PRIVATE);
            get();
        }
    }
    else if(resistorValue > 11) {
        digitalWrite(led, LOW);
        if(getTweets == true) {
            Spark.publish("Flag", "Unlock",60,PRIVATE);
            getTweets = false;
        }
    }
    else {

    }
    sprintf(value,"%d",resistorValue);
    // Spark.publish("LDR", value,60,PRIVATE);
    delay(500);
}
//---------------------------------------------------
void get() {
    String response = "";
    int statusCode = client.get("", &response);
    Serial.println(statusCode);
    Serial.println(response);
    Spark.publish("Number of Tweets",response,60,PRIVATE);
    getTweets = true;
}
