#include <TridentTD_EasyFreeRTOS32.h>


// defining all variables to store values for sensors
struct {
  int sensorValue1;
  int sensorValue2;
} aStruct;

int ch3Value;

char buffer[40]; // used to print all variables in one line

static void readSensor1(void*) { // lets say sensor 1. generating random values for now, analog read already done
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
    // Serial.print(ch3Value);
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
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(12, INPUT);
  digitalWrite(LED_BUILTIN, LOW);

  delay(5000);

  Serial.println("\r\nRunning FreeRTOS kernel " tskKERNEL_VERSION_NUMBER ". Built by gcc " __VERSION__ ".");

  xTaskCreate(readSensor1, "readSensor1", 128, nullptr, 2, nullptr);
  xTaskCreate(wirelessEStop, "wirelessEStop", 128, nullptr, 2, nullptr);
  xTaskCreate(conditionalThingy, "conditionalThingy", 128, nullptr, 3, nullptr);

  Serial.println("setup(): starting scheduler...");
  Serial.flush();

  vTaskStartScheduler();
}

void loop() {}
