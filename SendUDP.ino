#include <ESP8266WiFi.h> //bibloteket som brukes
#include <WiFiUdp.h> //bibloteket jeg brukes
#define FORCE_SENSOR_PIN A0 //der trykksensoren er koblet til


const char* ssid = "Idas iPhone 11"; //internettnavn den skal koble seg til
const char* password = "juicy123"; //internettnavn den skal koble seg til med

WiFiUDP Udp;
unsigned int localUdpPort = 4210; // lokal port den skal lytte på
char incomingPacket[255];  // Gir pakkene mulighet til å holde en byte
char  replyPacket[1];  //hva som skal sendes til den andre ESP'en

/*
 * Flere av linjene med kode i setup() ble brukt som testkoder for å sjekke 
 * at pakkene ble sendt som de skulle i prosessen til å få ESP'ene til å snakke sammen. 
 * Nå som kommunikasjonen fungerer er de ikke nødvendige for kjørbar kode, 
 * men er greie å ha tilfelle endring av kode for testing for at alt forsatt fungerer.
 */
void setup()
{
  Serial.begin(115200); //bits per sekund
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Udp.begin(localUdpPort);
  
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}


void loop()
{
  int analogReading = analogRead(FORCE_SENSOR_PIN); //hente data fra trykksensoren

  //Serial.print("Force sensor reading = ");
  //Serial.print(analogReading);
  IPAddress ip = IPAddress(172, 20, 10, 8); //setter den til å sende over gitt IP 

  //En if-sjekk som avgjør hvorvidt den skal sende pakke med tallet 1 eller 2 som avgjør
  //hvorvidt mottakeren skal reagere med å lyse og lage lyd
  if (analogReading > 200) {
    Serial.println(" -> kaffekopp på plass");
    replyPacket[0] = 1;
  }else{
    Serial.println(" -> kaffekopp ikke på plass");
    replyPacket[0] = 2;
  }

  Udp.beginPacket(ip, 4210); //lager forbindelse for å sende pakke
  Udp.write(replyPacket); //legger replypacket som innhold i pakken
  Serial.println("wrote a package\n");
  Udp.endPacket(); //lukker pakken
  delay(10);
}
