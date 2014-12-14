#include <Bridge.h>
#include <Console.h>
#include <FileIO.h>
#include <HttpClient.h>
#include <Mailbox.h>
#include <Process.h>
#include <YunClient.h>
#include <YunServer.h>

// Listen on default port 5555
YunServer server;
// Rememember if lights are 'on' or 'off'
String currentStatus;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  currentStatus = "off";
  Bridge.begin();
  
  // listen for connections only from localhost
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  YunClient client = server.accept();
  
  if (client) {
    process(client);
    client.stop();
  }
  
  delay(50);
}

// process request
void process(YunClient client) {
  String command = client.readStringUntil('/');
  
  if (command == "digital") {
    digitalCommand(client);
  }
}

void digitalCommand(YunClient client) {
  int pin, value;
  
  pin = client.parseInt();
  
  if (client.read() == '/') {
    value = client.parseInt();
    // turn light on or off
    digitalWrite(pin, value);
    if (value == 1) {
      currentStatus = "on";
      // send http request to node server 'on'
    } else {
      currentStatus = "off";
      // send http request to node server 'off'
    }
  } else {
    value = digitalRead(pin);
    client.print(currentStatus);
  }
  
}
