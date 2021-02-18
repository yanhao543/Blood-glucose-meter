# include <ArduinoJson.h>

//#define ALINK_BODY_FORMAT "{\"id\":\"123\",\"version123\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":%s}"

#define ALINK_BODY_FORMAT "{\"temperature\":\"26\"}"

void setup() {
  // 将 ALINK_BODY_FORMAT 替换成你想处理的字符串
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, ALINK_BODY_FORMAT);
  JsonObject obj = doc.as<JsonObject>();

  Serial.begin(9600);
  String my_method = obj["temperature"];
  // my_method = obj["method"].as<String>();
  Serial.println(my_method);

}

void loop() {
  // put your main code here, to run repeatedly:

}
