

TaskHandle_t Task1;
TaskHandle_t Task2;


#define led 2;
#define but 22;

#include <ArduinoJson.h>
#include <Keypad.h>
#include <HTTPClient.h>
#include <WiFi.h>

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 3; //three columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte pin_rows[ROW_NUM] = {12, 33, 25, 27}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {14, 13, 26}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

void codeForTask1( void * parameter) {
  //  for(;;){
  ////    Serial.println("1");
  //
  //
  //    char key = keypad.getKey();
  //
  //    if (key){
  //      Serial.println("--");
  //    }
  //
  ////    Serial.println(uxTaskGetStackHighWaterMark(NULL));
  ////    Serial.println(xPortGetFreeHeapSize());
  //
  //    int ptr = (int)pvPortMalloc(1024 * sizeof(int));
  //
  //    vPortFree(ptr);
  //
  //    delay(100);
  //  }
  //
  //  vTaskDelete(NULL);



}

bool is_on;
bool online;
bool alarmm;
bool corrupt;
char corrupt_mess;

void codeForTask2( void * parameter) {
  while (1) {

    if (WiFi.status() == WL_CONNECTED) {

      HTTPClient http;

      http.begin("https://smarthomesys.herokuapp.com/importantDoors/25");
      int httpCode = http.GET();

      if (httpCode > 0) {
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);

        StaticJsonDocument<192> doc;
        DeserializationError err = deserializeJson(doc, payload);

        if (err) {
          Serial.print("Error: ");
          Serial.println(err.c_str());
        }

        is_on = doc["is_on"];
        online = doc["online"];
        alarmm = doc["states"]["alarm"];
        corrupt = doc["states"]["corrupt"];
        corrupt_mess = doc["states"]["corrupt_mess"];

        Serial.println(alarmm);
        Serial.println(is_on);

        if (is_on == false) {
          Serial.println(is_on);
          digitalWrite(2, HIGH);
          delay(1000);
          digitalWrite(2, LOW);
        }

      } else {
        Serial.println("Error on HTTP req");
      }

      http.end();

    } else {

      Serial.println("connection lost");

    }

    Serial.println(uxTaskGetStackHighWaterMark(NULL));
    Serial.println(xPortGetFreeHeapSize());

    int ptr = (int)pvPortMalloc(1024 * sizeof(int));

    vTaskDelay(2000 / portTICK_PERIOD_MS);

//    vPortFree(ptr);

  }

  vTaskDelete(NULL);

}

const char* ssid = "tavi2.4";
const char* password = "acasa246";

void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(2, OUTPUT);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connection to WiFi...");
  };
  Serial.println("Connected to the WiFi network");

    xTaskCreatePinnedToCore(
      codeForTask1,
      "Task1",
      100,
      NULL,
      1,
      NULL,
      0
    );
  
    delay(500);

  xTaskCreatePinnedToCore(
    codeForTask2,
    "Task2",
    5000,
    NULL,
    CONFIG_ARDUINO_RUNNING_CORE,
    NULL,
    1
  );
  delay(500);
}

void loop() {

}
