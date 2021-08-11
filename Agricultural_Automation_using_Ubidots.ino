
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#define dhtPin 4
int ldrPin = 34;
int ledPin = 2;
int led1Pin = 5;
int gasPin = 32;
int motorPin = 13;
int lightPin = 15;



#define dhtType DHT11
DHT dht(dhtPin, dhtType);


#define WIFISSID "Thakur" // Put  WifiSSID here
#define PASSWORD "Karan@85" // Put  wifi password here
#define TOKEN "BBFF-yQdkx7RLMzMRdtWPM1BWxUfQDQmOjH" // Put  Ubidots' TOKEN
#define MQTT_CLIENT_NAME "karan_thakur" // MQTT client Name, please enter  own 8-12 alphanumeric character ASCII string; 
                                           //it should be a random and unique ascii string and different from all other devices
#define DEVICE_LABEL "agricultural_automation" // Assig the device label
#define VARIABLE_LABEL1 "temperature" // Assing the variable label
#define VARIABLE_LABEL2 "humidity"  // Assing the variable labe2
#define VARIABLE_LABEL3 "moisture"  // Assing the variable labe3
#define VARIABLE_LABEL4 "ldr"  // Assing the variable labe4
#define VARIABLE_LABEL5 "gas"  // Assing the variable labe5

/****************************************
 * Define Constants
 ****************************************/
char mqttBroker[]  = "things.ubidots.com";
char payload[100];
char topic[150];

/****************************************
 * Auxiliar Functions
 ****************************************/
WiFiClient ubidots;
PubSubClient client(ubidots);


// this function if optional 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    
    // Attemp to connect
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
      Serial.println("Connected");
      } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}

/****************************************
 * Main Functions
 ****************************************/
void setup() {
  dht.begin();
  Serial.begin(115200);

  pinMode(motorPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(led1Pin, OUTPUT);
  
  WiFi.begin(WIFISSID, PASSWORD);
  Serial.println();
  Serial.print("Wait for WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
}

void loop() {
  if (!client.connected()) {
       reconnect();
  }

  
  
  float temp = dht.readTemperature();
  float humi = dht.readHumidity();
  float ldr= analogRead(ldrPin);     //use moisture sensor on ldr pin
  float moisture = ( 100 - (( ldr / 4095.00) * 100.00 )) ;
  float light= analogRead(lightPin);  // use ldr sensor on light pin
  float gas= analogRead(gasPin);





  Serial.println(temp);
  Serial.println(humi);
  Serial.println(moisture);
  Serial.println(light);
  Serial.println(gas);

  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", ""); // Cleans the payload
  sprintf(payload, "{\"%s\":%f,\"%s\":%f,\"%s\":%f,\"%s\":%f,\"%s\":%f}", VARIABLE_LABEL1,temp,VARIABLE_LABEL2,humi,VARIABLE_LABEL3,moisture,VARIABLE_LABEL4,light,VARIABLE_LABEL5,gas); // Adds the variable label
  Serial.println("Publishing data to Ubidots Cloud");
  client.publish(topic, payload);
  client.loop();
  delay(5000);


 {
  if (moisture < 30){ 
 
    digitalWrite(motorPin, LOW);
    digitalWrite(ledPin, HIGH);
 
  }
  else{
    digitalWrite(motorPin, HIGH); 
    }
    }

  if (gas > 1700)
  {
    digitalWrite(led1Pin, HIGH);
    delay(10); 
  }

  
}
