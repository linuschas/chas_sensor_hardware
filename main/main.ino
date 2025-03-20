#include <EEPROM.h>
#include <TimeLib.h> // Download in library manager
#define EEPROM_MAX 8192
#define EEPROM_WRITE_LED_PIN 13

#define GROUP_SECOND_INTERVAL 3 * 24 * 3600 // Group the value of the last 3 days, in seconds for easier testing

struct __attribute__((packed)) DataRecord {
  uint32_t timestamp; // 4 bytes
  uint16_t length; // 2 bytes
};

// Requires unix in seconds
// Gives us {"DD/MM", "YY"}
std::pair<String, String> convertTime(time_t time) {
  setTime(time);

  char dayMonthBuffer[11];
  char yearBuffer[3];

  sprintf(dayMonthBuffer, "%02d/%02d", day(), month());
  sprintf(yearBuffer, "%02d", year() % 100);

  return {String(dayMonthBuffer), String(yearBuffer)};
}

// Groups the time based on pre-defined values to display on the LCD
// Returns the time threshold & total time
std::pair<String, String> getTimeForThreshold(uint16_t address) {
  // Grab the address, and loop backwards until address is out of bounds, or the address value is out of bounds

  // Verify we have actually written to the EEPROM
  if (address == 4) return {"00/00-00/00 00", "0 Minutes"};

  uint16_t start = address - sizeof(DataRecord);
  if (start <= 4) return {"00/00-00/00 00", "0 Minutes"};

  DataRecord firstRecord;
  EEPROM.get(start, firstRecord);

  int maxTimeFromFirst(GROUP_SECOND_INTERVAL); // day * seconds per day
  int starterTime(firstRecord.timestamp);
  int recordSize = sizeof(DataRecord);
  int totalTime = 0;
  int lastTimestamp = starterTime;
 
  // i becomes the address
  for (int i(start); i > 4; i -= recordSize) {
    DataRecord record;
    EEPROM.get(i, record);
    if (record.timestamp == 0xFFFF) break;

    if (starterTime - record.timestamp > maxTimeFromFirst) break;

    totalTime += record.length;
    lastTimestamp = record.timestamp;
  }

  std::pair<String, String> startAddress = convertTime(starterTime);
  std::pair<String, String> lastAddress = convertTime(lastTimestamp);

  String fullDateStr = startAddress.first + "-" + lastAddress.first + " " + startAddress.second;
  String totalTimeStr = (String)(totalTime / 60) + " Minutes";

  return {fullDateStr, totalTimeStr};
}

// Development function to erase the first 4 bytes in case of mishaps where we store the address
void eraseStart() {
  for (int i = 0; i < 4; i++) {
    EEPROM.write(i, 0xFF);
  }
}

// Development to erase 8-byte alignment after packing struct
// Can also be used to completely wipe the EEPROM, but does use a lot of writes
// Should probably replace with a better checker to avoid writing unnecessarily if this function is ever needed in the future
void eraseEEPROM() {
    for (int i = 0; i < EEPROM_MAX; i++) {
        EEPROM.write(i, 0xFF);  // Writing 0xFF resets EEPROM
    }
    EEPROM.put(0, (uint16_t)4);  // Reset next write address to 4
    Serial.println("EEPROM fully erased.");
}

// Grabs address, and erases from address
void eraseFromLastAddress() {
  int address = getNextAddress();
  for (int i = 0; i > 0; i--) {
    EEPROM.write(i, 0xFF);
  }
  
  EEPROM.put(0, (uint16_t)4);
  Serial.println("EEPROM erased until last address stored.");
}

void insertTimestamp(uint16_t address, uint32_t timestamp, uint16_t length) {
  digitalWrite(EEPROM_WRITE_LED_PIN, HIGH);

  DataRecord record;
  record.timestamp = timestamp;
  record.length = length;

  EEPROM.put(address, record);
  updateLastAddress(address);

  digitalWrite(EEPROM_WRITE_LED_PIN, LOW);
}

// Provide the start of the address to read from
// Will read from start until end of size
void readRecord(uint16_t address) {
  DataRecord record;
  EEPROM.get(address, record);

  Serial.println("RECORD FOUND:");
  Serial.println(record.timestamp);
  Serial.println(record.length);
}

// Temporary dummy timer for now, will use sensor when implemented
uint32_t getTime() {
  uint16_t address = getNextAddress();
  DataRecord record;
  EEPROM.get(address - sizeof(DataRecord), record);

  int timestamp = record.timestamp;
  if (address == 4) {
    timestamp = 0;
  }

  return timestamp + 60000;
}

// Pass in the unmodified last address fetch
// In put, insert the new address that is the next address right after the last address of the last address range we wrote to
void updateLastAddress(uint16_t address) {
  uint8_t structSize = sizeof(DataRecord);
  uint16_t nextAddress = address + structSize; // Our original address + the size it would reach

  // Max storage minus our next address to write to, if it hits the max, we need to reset
  if (EEPROM_MAX - nextAddress <= structSize) {
    Serial.println("EEPROM is full! Resetting address to start");
    nextAddress = 4;
  }

  EEPROM.put(0, nextAddress);
}

// Grabs the latest stored address, which is the next address after the last one we wrote to (or starts from scratch)
uint16_t getNextAddress() {
  uint16_t address;
  EEPROM.get(0, address);

  uint8_t structSize = sizeof(DataRecord);

  // If no address is found, set first address at 4
  // If an address is found, set it to that address and start writing from it
  uint16_t nextAddress;
  if (address == 0xFFFF || address < 4 || (address + structSize) > EEPROM_MAX) {
    nextAddress = 4;
  } else {
    nextAddress = address;
  }

  return nextAddress;
}

void setup() {
  pinMode(EEPROM_WRITE_LED_PIN, OUTPUT); // On-board LED

  Serial.begin(115200);
  delay(1000);

  Serial.println("================================");

  // Create a new instane on the EEPROM
  // uint16_t address = getNextAddress();
  // Serial.println("Last Address:");
  // Serial.println(address);
  // insertTimestamp(address, getTime(), 20);

  // Get a readable analytics from the EEPROM
  int address = getNextAddress();
  std::pair<String, String> timeThreshold = getTimeForThreshold(address);
  Serial.println(timeThreshold.first);
  Serial.println(timeThreshold.second);

  // If you want to erase based on your last saved address, use this
  // eraseFromLastAddress();
}

void loop() {
}