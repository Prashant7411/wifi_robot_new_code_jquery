//include libraries
#include <SoftwareSerial.h>


SoftwareSerial esp8266(3, 2); //RX pin = 3, TX pin = 2

//definition of variables
#define DEBUG true //show messages between ESP8266 and Arduino in serial port

int state = 5;



//*****
//SETUP
//*****
void setup()
{
  //start communication
  Serial.begin(9600);
  esp8266.begin(115200);
  
  sendData("AT+RST\r\n", 2000, DEBUG); //reset module
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG); //set station mode
  sendData("AT+CWJAP=\"prashant\",\"whitemoon\"\r\n", 2000, DEBUG); //connect wi-fi network
  while(!esp8266.find("OK")) { //wait for connection
  } 
  sendData("AT+CIFSR\r\n", 1000, DEBUG); //show IP address
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG); //allow multiple connections
  sendData("AT+CIPSERVER=1,167\r\n", 1000, DEBUG); // start web server on port 80
  
  //Define motor speed
 
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  
}

void loop()
{
  
  if (esp8266.available())  //verify incoming data
  {
    if (esp8266.find("+IPD,")) //if there is a message
    {
      String msg;
      esp8266.find("?"); //look for the message
      msg = esp8266.readStringUntil(' '); //read whole message
      String command = msg.substring(0, 3); //first 3 characters = command
      String valueStr = msg.substring(4);   //next 3 characters = value
      int value = valueStr.toInt();
      if (DEBUG) {
        Serial.println(command);
        Serial.println(value);
      }
           
      //move forward
      if(command == "cm1") {
          state = 1;
          
      }

      //move backward
      if(command == "cm2") {
          state = 2;
          
      }

      //turn right
      if(command == "cm3") {
          state = 3;
       }
       
       //turn left
       if(command == "cm4") {
          state = 4;
       }

       //do nothing       
       if(command == "cm5") {
          state = 5;
       }

       //test connection       
       if(command == "cm6") {
          state = 6;
       }

    }
  }
  
  //move forward
  if (state == 1) {
    digitalWrite(9,HIGH);
    digitalWrite(11,HIGH);    
  }
  //move backward
  if (state == 2) {
    digitalWrite(8,HIGH);
    digitalWrite(10,HIGH);    
  }
  //move right
  if (state == 3) {
     digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
      
  }
  //move left
  if (state == 4) {
    
    digitalWrite(8,HIGH);
    digitalWrite(11,HIGH); 
       
  }
  //do nothing
  if (state == 5) {
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
  }

  if (state == 6) {
    
  }
}

//*******************
//Auxiliary functions
//*******************

String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      char c = esp8266.read();
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
