//Använder mitt lokala WiFi för att få tillgång till Network Time Protocol för korrekt tid


#include <WiFiS3.h> //Hanterar Wi-Fi-anslutning på Arduino UNO R4 WiFi.
#include <NTPClient.h> // Hämtar tiden från en NTP-server.
#include <WiFiUdp.h> //Använder UDP-protokollet för att kommunicera med NTP-servern.

const char* ssid = " ";   // Här byter du till ditt WIFI namn. Ansluter enklare med 2.4 GHz
const char* password = " ";      // Byt till ditt Wi-Fi-lösenord

WiFiUDP udp; //Skapar ett UDP-nätverksobjekt.
NTPClient timeClient(udp, "pool.ntp.org", 3600);  // UTC +1 timme (Sverige)
//udp → Använder ett UDP-objekt (WiFiUDP) för nätverkskommunikation.
//"pool.ntp.org" → Adressen till NTP-servern (en offentlig tidsserver).
//3600 → Tidszonens justering i sekunder (3600 sekunder = 1 timme).

void setup() {
  Serial.begin(9600);
  
  // Anslut till Wi-Fi
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {  // Begränsa till max 10 försök
    delay(500);
    Serial.print(".");  // Visa punkt varje gång det försöker
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nAnsluten till WiFi");
  } else {
    Serial.println("\nFel vid anslutning till WiFi.");
  }

  timeClient.begin();
}

void loop() {
  timeClient.update();  // Uppdatera tiden från NTP-servern.

  // Hämta och skriv ut tiden
  displayTime();

  delay(1000);  // Vänta 1 sekund innan nästa uppdatering.
}

void connectToWiFi() {
  Serial.print("Ansluter till WiFi...");
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(500);
    Serial.print(".");  // Visa punkt varje gång det försöker.
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nAnsluten till WiFi");
  } else {
    Serial.println("\nFel vid anslutning till WiFi.");
  }
}

void displayTime() {
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  int seconds = timeClient.getSeconds();

  // Utskrift av tid i formatet HH:MM:SS
  printWithLeadingZero(hours);
  Serial.print(":");
  printWithLeadingZero(minutes);
  Serial.print(":");
  printWithLeadingZero(seconds);
  Serial.println();
}

void printWithLeadingZero(int value) {
  if (value < 10) {
    Serial.print("0");  // Lägg till ledande nolla om värdet är mindre än 10.
  }
  Serial.print(value);
}
