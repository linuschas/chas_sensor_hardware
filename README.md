## ProjektMedlemmar - Grupp 4:
Fabian Åman, Markus Reutervik, Linus Nilsson, Ayah Shehada, Mario Doka

Länkar: 
- [Användarmanual](https://github.com/linuschas/chas_sensor_hardware/blob/main/USER_MANUAL.md)
- [SRS](https://github.com/linuschas/chas_sensor_hardware/blob/main/docs/SRS.md)

# Projektbeskrivning
Vi ska bygga ett rörelse lampsystem som ska kunna logga när lampan tänds och släcks. Systemet ska kunna känna av om det är ljust eller mörkt ute och därav vara släckt om det är ljust ute och tänd om det är mörkt. Systemet ska kunna känna av om det går någonting förbi och tända sig själv om det är mörkt. Det ska också kunna logga tider som lampan släcks och tänds som visas via displayen. 

## Material
- LCD-display arduino AZ-1602A
- Sensor:
  - Photoresistor
  - PIR Motion Sensor Module
- LED-lampa
- Arduino UNO REV 4 WIFI
- BreadBoard
- Kablar

## Utvecklingspotential
- Extern förvaring av datan, istället för det inbyggda flash-minnet. Både med ett påkopplat flash-minne men även att kunna använda WIFI för att skicka till någon annan dator.
- Extern strömkälla till exempelvis en större mängd lampor, just nu kommer all strömmen från Arduinon.
- Konfigurerbart att den ska stå igång under specifika tider även om man inte har kört igång den med rörelsesensorn.
- Moduler man kan koppla ihop med den centrala produkten för att utöka räckvidden. Både med rörelsedektorer och ljuset.
- Konfigurerbara notifikationer, exempelvis att skicka om den går igång under specifika tider.
- Spara data, kunna räkna ut hur mycket strömförsörjning som går åt baserat på hur länge den är tänd.
- Någon slags dimmer för att konfigurera ljusstyrkan.
