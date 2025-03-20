## 1. Introduktion

Detta automatiska ljussystem är designat för att automatiskt tända och släcka en LED-lampa i ett garage baserat på ljusnivå och rörelse. Systemet använder en photoresistor för att detektera om det är mörkt och en PIR-rörelsesensor för att aktivera lampan vid rörelse. Dessutom loggas varje ljusaktivering i ett EEPROM-minne.

## 2. Installation

2.1 Hårdvarukomponenter

För att installera systemet behövs följande komponenter:

Arduino UNO R4 WIFI (mikrokontroller)

Photoresistor (ljussensor)

HC-SR04 (ultraljudssensor för rörelse)

LED-lampa

LCD-display (2x16)

DS1302 realtidsklocka

Kopplingskablar och breadboard

2.2 Kopplingsschema

Koppla photoresistorn till A0.

Koppla PIR-rörelsesensorn till digital pin 3.

Koppla LED-lampan till digital pin 4.

Koppla LCD-skärmen enligt dess I2C-adress (vanligtvis 0x27 eller 0x3F).

Koppla realtidsklockan (DS1302) till Arduino enligt databladet.

## 3. Användning

3.1 Starta systemet

Säkerställ att systemet är korrekt kopplat enligt kopplingsschemat.

Ladda upp koden till Arduino med Arduino IDE.

Anslut strömförsörjningen (USB eller extern strömkälla).

LCD-displayen visar initialiseringsstatus och senaste loggning.

3.2 Hur systemet fungerar

När en person går in i garaget på kvällen:

Photoresistorn mäter ljusnivån.

Om det är mörkt och rörelse upptäcks tänds LED-lampan.

Lampan förblir tänd i 10 sekunder (kan konfigureras).

Om ingen ny rörelse upptäcks inom 10 sekunder släcks lampan.

Tidpunkten för tändning och släckning loggas i EEPROM.

När en person går förbi under dagtid:

Photoresistorn känner av att det är ljust och hindrar lampan från att tändas.

Ingen loggning sker.

3.3 Visning av loggdata

Användare kan se senaste tändnings- och släckningstider på LCD-displayen.

Loggdata kan även exporteras via EEPROM-lagring.

**bold**  ## 4. Felsökning

Problem

Orsak

Lösning

Ljuset tänds inte alls

Photoresistorn blockerar tändning

Testa i ett mörkt rum

Ljuset tänds dagtid

Felaktig photoresistor-kalibrering

Justera tröskelvärdet i koden

Ingen rörelsedetektering

PIR-sensorn är felkopplad

Kontrollera anslutningar

LCD-displayen fungerar inte

Felaktig I2C-adress

Testa annan adress (0x27 eller 0x3F)

## 5. Underhåll och vidareutveckling

Rengör sensorerna regelbundet för att bibehålla exakt detektion.

EEPROM-minnet kan bli fullt efter lång användning, se till att rensa det vid behov.

Framtida utveckling: Möjlighet att lagra data på en extern server via WiFi eller SD-kort.
