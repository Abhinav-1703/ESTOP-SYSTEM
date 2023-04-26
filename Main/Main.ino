#include <TridentTD_EasyFreeRTOS32.h>
float Vin; 
float Vout;
float Vin1;
float Vout1; 
float R1 = 2963; 
float R2 = 29960; 
float R3 = 21500;
float R4 = 2963;
int rawValue;
int rawValue1; 


void task1(void* parameters) {
  for (;;) {
    vTaskDelay(500 / portTICK_PERIOD_MS);
    rawValue = analogRead(2);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    Vout = rawValue * (3.3 / 4096.0);
    Vin = Vout * ((R1 + R2) / R1);
    Vin=Vin+1;
    if (Vin > 23) {
      digitalWrite(13, HIGH);
      digitalWrite(15, HIGH);
      Serial.printf("1,%f,%s", Vin, "HIGH");
    } else if (Vin > 19.5 && Vin <= 23) {
      digitalWrite(13, HIGH);
      digitalWrite(15, LOW);
      Serial.printf("1,%f,%s",Vin,"HIGH LOW");
    } else {
      digitalWrite(13, LOW);
      digitalWrite(15, LOW);
      Serial.printf("1,%f,%s",Vin,"LOW");
    }
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
    Vin1=Vin1+4;
    Serial.println(Vin1);
    
    if (Vin1 > 23) {
      digitalWrite(13, HIGH);
      digitalWrite(15, HIGH);
       Serial.printf("2,%f,%s",Vin1,"HIGH");
    } else if (Vin1 > 19.5 && Vin1 <= 23) {
      digitalWrite(13, HIGH);
      digitalWrite(15, LOW);
      Serial.printf("2,%f,%s",Vin1,"HIGHLOW");
    } else {
      digitalWrite(13, LOW);
      digitalWrite(15, LOW);
      Serial.printf("2,%f,%s",Vin1,"LOW");
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  pinMode(13,OUTPUT);
  pinMode(15,OUTPUT);
  Serial.begin(9600);
  xTaskCreate(
    task1,
    "task 1",
    1000,
    NULL,
    1,
    NULL
  );
  xTaskCreate(
    task2,
    "task 2",
    1000,
    NULL,
    1,
    NULL
  );
}

void loop() {
}
