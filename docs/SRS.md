## Automatiskt ljussystem för garage  

**Grupp 4 – Projektmedlemmar**  
- **Fabian Åman**  
- **Markus Reutervik**  
- **Linus Nilsson**  
- **Ayah Shehada**  
- **Mario Doka**  

**GitHub-repository:** [chas_sensor_hardware](https://github.com/linuschas/chas_sensor_hardware/tree/main)  

---

## **Innehållsförteckning**
1. [Introduktion](#introduktion)  
2. [Kravspecifikation](#kravspecifikation)  
   - [Funktionella krav](#funktionella-krav)  
   - [Icke-funktionella krav](#icke-funktionella-krav)  
3. [Systemarkitektur](#systemarkitektur)  
4. [Användningsfall](#användningsfall)  
5. [Utvecklingspotential](#utvecklingspotential)  
6. [Slutsats](#slutsats)  

---

## 1️⃣ **Introduktion**  
Detta projekt syftar till att skapa ett **automatiskt ljussystem för garage** som använder en **photoresistor** och en **PIR-rörelsesensor** för att tända och släcka en **LED-lampa** baserat på omgivningens ljusnivå och rörelse.  

**Systemet ska:**  
- **Identifiera rörelse** nära garaget med hjälp av sensorer.  
- **Aktivera ljuset** endast under kvälls- och nattetid baserat på photoresistor.  
- **Ha en energisparfunktion** som släcker ljuset om ingen rörelse upptäcks efter en viss tid.  
- **Logga tider** när ljuset tänds och släcks med hjälp av en **realtidsklocka (RTC)** och spara data i **flashminne**.  

**Målgrupp:**  
- Garageföretagets personal och besökare.  
- Systemet ska vara självgående och kräva **minimalt underhåll**.  

---

## 2️⃣ **Kravspecifikation**  
### **Funktionella krav**  
| ID | Beskrivning |
|----|------------|
| **FR1** | Systemet ska kunna upptäcka rörelse inom en radie av minst **5 meter**. |
| **FR2** | Ljuset ska endast tändas om det är **mörkt** i omgivningen. |
| **FR3** | Ljuset ska **släckas automatiskt efter 5 minuter** om ingen rörelse upptäcks. |
| **FR4** | Systemet ska **logga alla händelser** där ljuset tänds och släcks. |
| **FR5** | Loggdata ska kunna **exporteras och granskas av administratörer**. |

### **Icke-funktionella krav**  
| ID | Beskrivning |
|----|------------|
| **NFR1** | Systemet ska vara **energieffektivt** och använda **LED-lampor**. |
| **NFR2** | **Latensen** mellan rörelsedetektering och tändning får **inte överstiga 1 sekund**. |

---

## 3️⃣ **Systemarkitektur**  
**Hårdvarukomponenter**  
| Komponent | Modell |
|-----------|--------|
| **Mikrokontroller** | Arduino UNO 4 WIFI |
| **Ljussensor** | Photoresistor |
| **Rörelsesensor** | HC-SR04 ultraljudssensor |
| **Display** | LCD 2x16 DEM 16217 SYH |
| **Lagring** | EEPROM |
| **Tidsloggning** | DS1302 realtidsklockmodul |
| **Kopplingar** | Breadboard & kablar |

---

## 4️⃣ **Användningsfall**  
### **Scenario 1: En person närmar sig garaget på kvällen**  
1️⃣ Rörelsesensorn **upptäcker en person**.  
2️⃣ Ljussensorn **kontrollerar om det är mörkt**.  
3️⃣ Om det är mörkt, **tänds ljuset automatiskt**.  
4️⃣ Om ingen ny rörelse upptäcks **släcks ljuset efter 5 minuter**.  
5️⃣ Händelsen **loggas i EEPROM**.  

### **Scenario 2: En person går förbi på dagen**  
1️⃣ Rörelsesensorn **upptäcker en person**.  
2️⃣ Ljussensorn **identifierar att det är ljust** ute.  
3️⃣ **Ljuset förblir släckt**.  
4️⃣ **Ingen loggning sker**.  

---

## 5️⃣ **Utvecklingspotential**  
**Möjliga framtida förbättringar:**  
**Extern datalagring** via WiFi eller SD-kort istället för flashminne.  
**Fjärrstyrning** genom en mobilapp.  
**Dimmerfunktion** för att justera ljusstyrkan.  
**Längre räckvidd** genom att koppla fler rörelsesensorer.  
**Notifikationer** om lampan tänds vid ovanliga tider.  
**Energiövervakning** för att räkna ut exakt strömförbrukning.  

---

## 6️⃣ **Slutsats**  
Detta ljussystem kommer att **förbättra säkerheten och bekvämligheten** vid garaget samtidigt som det **sparar energi** genom att endast aktiveras när det behövs.  

Vidare kan **loggning av ljusanvändning** ge insikter om **rörelsemönster**, vilket kan användas för framtida optimeringar.  

**GitHub Repository:**  
 [chas_sensor_hardware](https://github.com/linuschas/chas_sensor_hardware/tree/main)  
