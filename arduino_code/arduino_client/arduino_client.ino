/**
   接收esp8266发送的json数据，控制led灯
*/
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
SoftwareSerial ESP_Serial(6, 7); //Tx,Rx
int i=0;
void setup() {
  Serial.begin(9600);
  ESP_Serial.begin(9600);
  delay(2000);
  Serial.println("Hello ESP8266.");// "初始化输出信息 hello world"
  //将8号引脚设置为输出模式
  //pinMode(8,OUTPUT);//设置数字IO引脚为输出模式
}

void loop() {
  //发送
  if(i==0){
    ESP_Serial.print(sendJson("0"));//发送数据给ESP
    i=1;
  }else{
    ESP_Serial.print(sendJson("26"));//发送数据给ESP
    i=0;
  }
  delay(2000);
}

String sendJson(String data){
  const size_t capacity = JSON_OBJECT_SIZE(1)+10;
  DynamicJsonDocument doc(capacity);
  doc["temperature"] = data;
  String josn;
  serializeJson(doc, josn);//创建Json字符串
  return  josn;
}

//获取sep8266上发送过来的数据
String recvjson() {
  String comdata = "";
  while (ESP_Serial.available() > 0) { //串口接收到数据
    comdata += char(ESP_Serial.read());//获取串口接收到的数据
    delay(2);
  }
  Serial.println(comdata);
  return comdata;
}
