#include <EEPROM.h>
#define EEPROM_MAX 8192
#define EEPROM_WRITE_LED_PIN 13

struct __attribute__((packed)) DataRecord {
  uint32_t timestamp; // 4 bytes
  uint16_t length; // 2 bytes
};

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

void insertTimestamp(uint16_t address, uint32_t timestamp, uint16_t length) {
  digitalWrite(EEPROM_WRITE_LED_PIN, HIGH);
  delay(1000); // Temp to see effect

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
  return 69;
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
  // eraseEEPROM();
  // delay(1000);

  // eraseStart();

  uint16_t address = getNextAddress();
  Serial.println("Last Address:");
  Serial.println(address);
  insertTimestamp(address, getTime(), 20);

  delay(100);

  readRecord(address - sizeof(DataRecord));
}

void loop() {
}
