#include <DHT.h>
#include <SD.h>

#define DHTPIN 6
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Set the pins used
#define cardSelect 4
int sensorVcc = A1;

File logfile;

void error(uint8_t errno) {
  while(1) {
    uint8_t i;
    for (i=0; i<errno; i++) {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(100);
    }
    for (i=errno; i<10; i++) {
      delay(200);
    }
  }
}

#ifdef ARDUINO_SAMD_ZERO
   #define Serial SerialUSB
#endif

void setup() {
  Serial.begin(1200);
  dht.begin();

  // see if the card is present and can be initialized:
  if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
    error(2);
  }
  char filename[8];
  strcpy(filename, "data.csv");

  logfile = SD.open(filename, FILE_WRITE);
  if( ! logfile ) {
    Serial.print("Couldnt create "); 
    Serial.println(filename);
    error(3);
  }
  Serial.print("Writing to "); 
  Serial.println(filename);

  pinMode(13, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(sensorVcc, OUTPUT);
  Serial.println("Ready!");
  logfile.println("temperature,humidity,heat_index");
  delay(5000);
}

uint8_t i=0;
void loop() {
  digitalWrite(sensorVcc, HIGH);
  delay(5000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float hidx = dht.computeHeatIndex(t, h, false);
  digitalWrite(sensorVcc, LOW);

  digitalWrite(8, HIGH);
  logfile.print(t);
  logfile.print(",");
  logfile.print(h);
  logfile.print(",");
  logfile.println(hidx);
  logfile.flush();
  digitalWrite(8, LOW);
  
  // DISPLAY DATA
  Serial.print(h, 1);
  Serial.print(",\t");
  Serial.print(t, 1);
  Serial.print(",\t");
  Serial.print(hidx, 1);
  Serial.print("\n");

  delay(900000);
}
