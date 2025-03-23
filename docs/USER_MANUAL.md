## 1. Introduktion

Detta automatiska ljussystem är designat för att automatiskt tända och släcka en LED-lampa i ett garage baserat på ljusnivå och rörelse. Systemet använder en photoresistor för att detektera om det är mörkt och en PIR-rörelsesensor för att aktivera lampan vid rörelse. Dessutom loggas varje ljusaktivering i ett EEPROM-minne.

## 2. Installation

2.1 Hårdvarukomponenter

För att installera systemet behövs följande komponenter:
- Arduino UNO R4 WIFI (mikrokontroller)
- Photoresistor (ljussensor)
- PIR rörelsedetektor HC-SR501
- LED-lampa
- LCD-display (2x16)
- DS1302 realtidsklocka
- Kopplingskablar och breadboard

2.2 Kopplingsschema

1. Koppla photoresistorn till A0.
2. Koppla PIR-rörelsesensorn till digital pin 3.
3. Koppla LED-lampan till digital pin 4.
4. Koppla LCD-skärmen enligt dess I2C-adress (vanligtvis 0x27 eller 0x3F).
5. Koppla realtidsklockan (DS1302) till Arduino enligt databladet.

## 3. Användning

3.1 Starta systemet

1. Säkerställ att systemet är korrekt kopplat enligt kopplingsschemat.
2. Ladda upp koden till Arduino med Arduino IDE.
3. Anslut strömförsörjningen (USB eller extern strömkälla).
4. LCD-displayen visar initialiseringsstatus och senaste loggning.

3.2 Hur systemet fungerar

När en person går in i garaget på kvällen:

1. Photoresistorn mäter ljusnivån.
2. Om det är mörkt och rörelse upptäcks tänds LED-lampan.
3. Lampan förblir tänd i 10 sekunder (kan konfigureras).
4. Om ingen ny rörelse upptäcks inom 10 sekunder släcks lampan.
5. Tidpunkten för tändning och släckning loggas i EEPROM.

När en person går förbi under dagtid:

- Photoresistorn känner av att det är ljust och hindrar lampan från att tändas.
- Ingen loggning sker.

3.3 Visning av loggdata

- Användare kan se senaste tändnings- och släckningstider på LCD-displayen.
- Loggdata kan även exporteras via EEPROM-lagring.

## 4. Felsökning 

Problem: Ljuset tänds inte alls
Orsak: Photoresistorn blockerar tändning. 
Lösning: Testa i ett mörkt rum. 

Problem: Ljuset tänds dagtid
Orsak: Felaktig photoresistor-kalibrering
Lösning:Justera tröskelvärdet i koden

Problem: Ingen rörelsedetektering
Orsak: PIR-sensorn är felkopplad
Lösning: Kontrollera anslutningar

Problem: LCD-displayen fungerar inte
Orsak: Felaktig I2C-adress
Lösning: Testa annan adress (0x27 eller 0x3F)

## 5. Underhåll och vidareutveckling

- Rengör sensorerna regelbundet för att bibehålla exakt detektion.
- EEPROM-minnet kan bli fullt efter lång användning, se till att rensa det vid behov.
- Framtida utveckling: Möjlighet att lagra data på en extern server via WiFi eller SD-kort.
