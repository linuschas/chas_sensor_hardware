#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <TimeLib.h> 

#define MOTION_CHECK 1 // Time interval to check motion sensor (in seconds)
#define EEPROM_MAX 8192 //8192 bytes = 8 KB
#define ON_INTERVAL 10 // Interval for motion detection in seconds
#define GROUP_SECOND_INTERVAL 3 * 24 * 3600 // Group the value of the last 3 days

// Structure to store timestamps and data length in EEPROM
struct __attribute__((packed)) DataRecord
{
  uint32_t timestamp; // 4 bytes - stores timestamp as a 32-bit unsigned integer
  uint16_t length; // 2 bytes - stores the length of the recorded data
};

int pho = A0; // Analog input pin for photo sensor
int led = 4; // Digital output pin for LED
int motion = 3; // Digital input pin for motion sensor

LiquidCrystal_I2C lcd_1(32, 16, 2); // LCD object for controlling the display

// Convert a timestamp to a human-readable date format
std::pair<String, String> convertTime(time_t time)
{
  setTime(time); // Set the global time using TimeLib

  char dayMonthBuffer[11]; // Buffer for "DD/MM"
  char yearBuffer[3]; // Buffer for "YY"


  sprintf(dayMonthBuffer, "%02d/%02d", day(), month());  // Format the day and month
  sprintf(yearBuffer, "%02d", year() % 100);   // Format the year as the last two digits

  return {String(dayMonthBuffer), String(yearBuffer)};  // Return the formatted date and year
}

// Get the time threshold based on the EEPROM address
std::pair<String, String> getTimeForThreshold(uint16_t address) 
{
  // Grab the address, and loop backwards until address is out of bounds, or the address value is out of bounds

  // Verify we have actually written to the EEPROM
  if (address == 4) return {"00/00-00/00 00", "0 Minutes"}; // Return default values if no data

  uint16_t start = address - sizeof(DataRecord); // Get the start address
  if (start <= 4) return {"00/00-00/00 00", "0 Minutes"}; // Return default values if no data

  DataRecord firstRecord; 
  EEPROM.get(start, firstRecord); // Get the first record from EEPROM

  int maxTimeFromFirst(GROUP_SECOND_INTERVAL); // day * seconds per day
  int starterTime(firstRecord.timestamp); // Get the starting timestamp
  int recordSize = sizeof(DataRecord); // Size of the record
  int totalTime = 0;  // Total time accumulated
  int lastTimestamp = starterTime;  // Store the last timestamp
 
  // i becomes the address
  for (int i(start); i > 4; i -= recordSize) 
  {
    DataRecord record;
    EEPROM.get(i, record); // Get the current record
    if (record.timestamp == 0xFFFF) break; // Break if we hit an invalid timestamp

    if (starterTime - record.timestamp > maxTimeFromFirst) break; // Break if the time difference exceeds the limit

    totalTime += record.length; // Add the record length to total time
    lastTimestamp = record.timestamp;  // Update the last timestamp
  }

  // Convert the start and end timestamps to human-readable format
  std::pair<String, String> startAddress = convertTime(starterTime); 
  std::pair<String, String> lastAddress = convertTime(lastTimestamp);
  
  // Format the result
  String fullDateStr = startAddress.first + "-" + lastAddress.first + " " + startAddress.second;
  String totalTimeStr = (String)(totalTime / 60) + " Minutes"; // Convert total time to minutes

  return {fullDateStr, totalTimeStr}; // Return the formatted strings
}

void setup() 
{
  pinMode(led, OUTPUT);
  pinMode(motion, INPUT); 
  Serial.begin(115200);

  delay(1000);

  Serial.println("================================");
  initScreen(); // Initialize the LCD screen

  int address = getNextAddress(); // Get the next available address in EEPROM
  std::pair<String, String> timeThreshold = getTimeForThreshold(address); // Get the time threshold for the data
  Serial.println(timeThreshold.first); // Print the start date of the data
  Serial.println(timeThreshold.second); // Print the total time accumulated

}

// Initialize LCD screen
void initScreen() 
{
  lcd_1.init();
  lcd_1.backlight();
  lcd_1.display();
  resetScreen();
}

// Reset the screen content
void resetScreen() 
{
  lcd_1.clear();
  lcd_1.setCursor(0, 0);
  lcd_1.print("Screen");
  lcd_1.setCursor(0, 1);
  lcd_1.print("Reset");
}

// Get the next address to write data to in EEPROM
uint16_t getNextAddress() 
{
  uint16_t address;
  EEPROM.get(0, address); // Read the current address from EEPROM

  uint8_t structSize = sizeof(DataRecord); // Size of the DataRecord structure

  // If no address is found, set first address at 4
  // If an address is found, set it to that address and start writing from it
  uint16_t nextAddress;
  if (address == 0xFFFF || address < 4 || (address + structSize) > EEPROM_MAX) {
    nextAddress = 4; // Start writing from address 4
  } else {
    nextAddress = address; 
  }

  return nextAddress; 
}

void loop() 
{
  // Check if motion is detected and light level is low
  if (analogRead(pho) <= 20 && digitalRead(motion) == 1) 
  {
    isDetected(); 
  } 
  else 
  {
    digitalWrite(led, LOW); // Otherwise, turn off the LED
  }
  Serial.println(analogRead(pho)); // Print the value of the photo sensor for debugging
}

void isDetected()
{
  int seconds = ON_INTERVAL; // Initialize seconds with the defined interval
  int totalSeconds = 0; // Temporary, when we get RTC we fetch time when done to get length
  digitalWrite(led, HIGH); // Turn on the LED during motion detection
  showCurrentTime(); // Display the current time on the LCD
  while (1) // Continuous loop for motion detection
  {
        
    // Display the interval time on the LCD
    /* lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Interval:");
    lcd_1.setCursor(0, 1);
    lcd_1.print(totalSeconds);
    */
    delay(MOTION_CHECK * 1000); // Wait for the motion check interval
    totalSeconds += MOTION_CHECK; // Add to the total time
    if (digitalRead(motion) == 1) 
    {
      seconds = ON_INTERVAL; // Reset the interval if motion is still detected
    } 
    
    else 
    {
      seconds -= MOTION_CHECK; // Decrease the interval if no motion is detected
    }
    
    if (seconds <= 0) break; // Exit loop if interval is over

    /* Serial.print("Tid: ");
    Serial.print(rtc.getHours());
    Serial.print(":");
    Serial.print(rtc.getMinutes());
    Serial.print(":");
    Serial.print(rtc.getSeconds());
    Serial.print(" - Sekunder kvar: ");
    Serial.println(seconds); */
  }
  digitalWrite(led, LOW); // Turn off the LED
  resetScreen();
}

void showCurrentTime() 
{
  time_t currentTime = now();  // Get the current time

  // Convert the current time to a human-readable format
  std::pair<String, String> timeString = convertTime(currentTime);

  lcd_1.clear(); // Clear the LCD screen
  lcd_1.setCursor(0, 0);
  lcd_1.print("Time: ");
  lcd_1.print(timeString.first); // Shows date
  lcd_1.print("-");
  lcd_1.print(timeString.second); // Shows year
}
