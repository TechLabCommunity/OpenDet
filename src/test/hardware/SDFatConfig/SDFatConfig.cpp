#include <ArduinoJson.h>
#include <Global.h>
#include <SPI.h>
#include <SdFat.h>

// Configuration that we'll store on disk
struct Config {
  uint screenTimeout;
  uint detPrice_1;
  // 20 char max for detergent name (due to LCD size)
  char detName_1[21];
};

SdFat SD;
Config config;

#define SD_CS_PIN 4
#define FILENAME "config.json"

File myFile;

// Loads the configuration from a file
void loadConfiguration(const char *filename, Config &config) {
  // Open file for reading
  File file = SD.open(filename, FILE_READ);

  // Allocate the memory pool on the stack.
  // Don't forget to change the capacity to match your JSON document.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonBuffer<256> jsonBuffer;

  // Parse the root object
  JsonObject &root = jsonBuffer.parseObject(file);

  if (!root.success())
    Serial.println(F("Failed to read file, using default configuration"));

  // Copy values from the JsonObject to the Config
  config.screenTimeout = root["timeout"] | 100;
  config.detPrice_1 = root["detPrice_1"] | 200;
  strlcpy(config.detName_1,                    // <- destination
          root["detName_1"] | "Detergente 1",  // <- source
          sizeof(config.detName_1));           // <- destination's capacity

  // Close the file (File's destructor doesn't close the file)
  file.close();
}

// Saves the configuration to a file
void saveConfiguration(const char *filename, const Config &config) {
  // Delete existing file, otherwise the configuration is appended to the file
  if (!SD.remove(filename)) {
    Serial.println("Failed to remove file");
  }
  // Open file for writing
  File file = SD.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  // Allocate the memory pool on the stack
  // Don't forget to change the capacity to match your JSON document.
  // Use https://arduinojson.org/assistant/ to compute the capacity.
  StaticJsonBuffer<256> jsonBuffer;

  // Parse the root object
  JsonObject &root = jsonBuffer.createObject();

  // Set the values
  root["timeout"] = config.screenTimeout;
  root["detName_1"] = config.detName_1;
  root["detPrice_1"] = config.detPrice_1;

  // Serialize JSON to file
  if (root.printTo(file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file (File's destructor doesn't close the file)
  file.close();
}

// Prints the content of a file to the Serial
void printFile(const char *filename) {
  // Open file for reading
  File file = SD.open(filename, FILE_READ);
  if (!file) {
    Serial.println(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) {
    Serial.print((char)file.read());
  }
  Serial.println();

  // Close the file (File's destructor doesn't close the file)
  file.close();
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // set RTC SS pin LOW, it should avoid conflict on the bus
  Controllino_SetRTCSS(0);

  Serial.print("Initializing SD card...");

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  while (!SD.begin()) {
    Serial.println(F("Failed to initialize SD library"));
    delay(1000);
  }

  // Should load default config if run for the first time
  Serial.println(F("Loading configuration..."));
  loadConfiguration(FILENAME, config);

  Serial.println("Det Price: " + (String)config.detPrice_1);
  Serial.println("Det Name: " + (String)config.detName_1);
  Serial.println("Timeout: " + (String)config.screenTimeout);

  // Create configuration file
  Serial.println(F("Saving configuration..."));
  saveConfiguration(FILENAME, config);

  // Dump config file
  Serial.println(F("Print config file..."));
  printFile(FILENAME);
}

void loop(void) {}