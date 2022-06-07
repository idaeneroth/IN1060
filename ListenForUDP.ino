#include <ESP8266WiFi.h> //bibloteket som brukes
#include <WiFiUdp.h> //bibloteket som brukes

const char* ssid = "Idas iPhone 11"; //internettnavn den skal koble seg til
const char* password = "juicy123"; //internettpassord den skal koble seg til med

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // lokal port den skal lytte på
char incomingPacket[255];  // Gir pakkene mulighet til å holde en byte
char replyPacket[] = "Mottok pakke"; 


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

  pinMode(5, OUTPUT); //LED

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
boolean recivePacket(int* svar_addr){
  int packetSize = Udp.parsePacket();
  
  if (packetSize){
    // receive incoming UDP packets
    //Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    *svar_addr = incomingPacket[0];
    //Serial.println(*svar_addr);
    return true;
    
  }else{
    return false;
  }
  
}

//Det er dette som avgjør hvorvidt en "pakke" skal sendes
//til den andre ESP'en som gjør at den begynner å blinke og lage lyd

boolean kaffekopp = false;
void loop()
{
  int svar;
  boolean recievedPacket = recivePacket(&svar); //gjør om svar til en peker slik at den kan brukes i if-sjekken
  
  if (recievedPacket){
    Serial.println(svar);

    if (svar == 1){
      digitalWrite(5, HIGH);
      tone(2, 200, 100);
    } else {
      digitalWrite(5, LOW);
    }
  }
  
}
