#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define VBATPIN A7

int OLED_RESET = 4;
Adafruit_SSD1306 display(OLED_RESET);

//Set up variables
int readings = 0;
float temperature = 0;
int measurements = 10;
float voltage = 0;
float percent = 0;

void setup() {
  // Configure IO
  pinMode(A0, INPUT); //Sensor
  pinMode(12, OUTPUT); //Red
  pinMode(11, OUTPUT); //Green
  pinMode(10, OUTPUT); //Blue
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize OLED with the I2C addr 0x3C
  display.setTextSize(1); // Set the text size
  display.setTextColor(WHITE); // Set the text color
  
}

void loop() {
  analogReadResolution(12); //Set analog resolution to 12 bits

  //Measurement frequency
  readings = 0; // Reset the analog readings to 0
  for(int i=0; i<measurements; i++){ // Make multiple measurements to take average from
    readings += analogRead(A0); // Read the temperature sensor output
    delay(250); // Four updates per second
  }
  //Read temperature
  voltage = (((readings/measurements) * 3.3)/4096); //Convert analog input to voltage (3.3v sensor power and 12 bit ADC (4096)) 
  temperature = (voltage * 103.68969949110382) - 52.802792274538916 ; // Convert the average voltage output to degrees Celsius

  //Battery Voltage
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    
  measuredvbat *= 3.3;  
  measuredvbat /= 4096; 
  
  //Battery Percentage
  percent = measuredvbat;
  percent /=3.71379625958605;
  percent = pow(percent, 41.4569168530159);
  percent +=1;
  percent = 97.5919878147075 - 99.19825118747457 / percent;

  //OLED Display Output
  display.clearDisplay(); // Clear the display
  display.setCursor(0,0); // Set the cursor at the beginning of the display
  display.print("Temp:"); // Append the unit
  display.print(temperature,2); // Print temperature with single decimal
  display.println("C"); // Append the unit
  display.print("Voltage:"); //Print text
  display.print(voltage,3); // Print voltage
  display.println("V"); // Append the unit
  display.print("Battery:"); //Print text
  display.print(measuredvbat,2); // Print bat
  display.print("V "); // Append the unit
  display.print(percent,0); // Print percent
  display.println("%"); // Append the unit
  display.display(); // Display the temperature and unit on the display
  //LEDS
  //Red
  if (temperature <= 20) {
    digitalWrite(12, HIGH);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
  }
  //Green
  else if (20 < temperature && temperature <= 40) {
    digitalWrite(12, LOW);
    digitalWrite(11, HIGH);
    digitalWrite(10, LOW);
  }
  //Blue
  else if (temperature > 40) {
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, HIGH);
  }
  //Out of range
  else {
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
  }
}
