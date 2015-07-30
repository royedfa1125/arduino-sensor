#include <pitches.h>
#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"
#define DHTPIN A0
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
#include <Wire.h>
#define ADDR 0b0100011
byte mac[] = { 0xF0, 0x7B, 0xCB, 0x4B, 0x7C, 0x9F };
#include <pitches.h>
EthernetServer server(80);
String data4;
EthernetClient client;

double t = 0;
double h = 0;

void setup() {
  Serial.begin(9600);

  Ethernet.begin(mac);
  IPAddress ip = Ethernet.localIP();
  server.begin();
  
  Serial.print("My IP address: ");
  Serial.println(ip);
  
  dht.begin();
  delay(1000);
  Wire.begin();
  Wire.beginTransmission(ADDR);
  Wire.write(0b00000001);
  Wire.endTransmission();
  delay(1000);
}

void loop(){

int thisnote = A3;
        int melody[] = {NOTE_G5, NOTE_E5, NOTE_E5, 0, NOTE_F5, NOTE_D5, NOTE_D5, 0, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_G5, NOTE_G5};
        int val = 0;
        Wire.beginTransmission(ADDR);
        Wire.write(0b00000111);
        Wire.endTransmission();
        delay(100);
        Wire.beginTransmission(ADDR);
        Wire.write(0b00100000);
        Wire.endTransmission();
        delay(120);
        Wire.requestFrom(ADDR, 2); // 2byte every time
        for (val=0; Wire.available()>=1; ) {
        char c = Wire.read();
        val = (val << 8) + (c & 0xFF);
        }
        val = val / 1.2;
          h = dht.readHumidity();
          t = dht.readTemperature();
          
          data4 = "temp="+(String)t+"&hum="+(String)h+"&lighting="+(String)val+"&location=輸入ID";
          Serial.println(val);
          if(t!=0){
            Serial.println(data4);
            if(client.connect("app.itlab.tw",80)){
        client.println("POST /app/arduino/sensor/ HTTP/1.1"); 
        client.println("Host:app.itlab.tw"); // SERVER ADDRESS HERE TOO
        client.println("Content-Type: application/x-www-form-urlencoded"); 
        client.print("Content-Length: "); 
        client.println(data4.length()); 
        client.println(); 
      client.print(data4); 
	}
          }
	if (client.connected()) { 
		client.stop();	// DISCONNECT FROM THE SERVER
	}

	delay(2780); // WAIT FIVE MINUTES BEFORE SENDING AGAIN
}

