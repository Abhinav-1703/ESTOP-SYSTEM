#include <TridentTD_EasyFreeRTOS32.h>
float Vin; 
float Vout;
float Vin1;
float Vout1; 
float R1 = 2963; 
float R2 = 21500; 
float R3 = 29960;
float R4 = 2963;
int rawValue;
int rawValue1; 
struct {
  int sensorValue1;
  int sensorValue2;
} aStruct;

int ch3Value;
char buffer[40];


void task1(void* parameters) {
  for (;;) {
    vTaskDelay(500 / portTICK_PERIOD_MS);
    rawValue = analogRead(2);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    Vout = rawValue * (3.3 / 4096.0);
    Vin = Vout * ((R1 + R2) / R1);
    Serial.println( Vin);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void task2(void* parameters) {
  for (;;) {
    vTaskDelay(500 / portTICK_PERIOD_MS);
    rawValue1 = analogRead(4);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    Vout1 = rawValue1 * (3.3 / 4096.0);
    Vin1 = Vout1 * ((R3 + R4) / R4);
    Serial.println(Vin1);
    
    if (Vin1 > 23) {
      digitalWrite(13, HIGH);
      digitalWrite(15, HIGH);
  
    } else if (Vin1 > 19.5 && Vin1 <= 23) {
      digitalWrite(13, HIGH);
      digitalWrite(15, LOW);
    
    } else {
      digitalWrite(13, LOW);
      digitalWrite(15, LOW);
      
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
static void readSensor1(void* ) {
  while (true) {
    aStruct.sensorValue1 = random(0, 32);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue) {
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

static void wirelessEStop(void*) { // update value for wireless estop. need logic shifter or a resistor thingy
  while (true) {
    ch3Value = readChannel(12, -100, 100, -100);
     Serial.print(ch3Value);
  }
}
static void conditionalThingy(void*) { //lets say I wanna print data from both sensors from a struct
  while (true) {
    sprintf(buffer, "number bw 0-32 %d and Number bw 32-64 %d and wireless %d ", aStruct.sensorValue1, aStruct.sensorValue2, ch3Value);
    Serial.println(buffer);
    vTaskDelay(pdMS_TO_TICKS(100));

    //main condition statement to blink led if any sensor triggers.
    if (aStruct.sensorValue1 > 30 || ch3Value > 5) {
      digitalWrite(LED_BUILTIN, HIGH);
      //add CAN code to trigger message bot stopped
    }
    else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

void setup() {
  pinMode(13,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(12, INPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(5000);
  Serial.begin(9600);
  xTaskCreate(
    task1,
    "task 1",
    1000,
    NULL,
    2,
    NULL
  );
  xTaskCreate(
    task2,
    "task 2",
    1000,
    NULL,
    2,
    NULL
  );
  xTaskCreate(
    readSensor1,
    "readSensor1",
    128,
    NULL,
    1,
    NULL
   );
  xTaskCreate(
   wirelessEStop,
    "wirelessEStop",
    128,
    NULL,
    1,
    NULL
   ); 
   xTaskCreate(
   conditionalThingy,
    "conditionalThingy",
    128,
    NULL,
    1,
    NULL
   ); 
  Serial.println("setup(): starting scheduler...");
  Serial.flush(); 
  vTaskStartScheduler();
}

void loop() {
}
