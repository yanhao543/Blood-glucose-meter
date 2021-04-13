#include <ArduinoJson.h>
#include <ESP8266WiFi.h> //ESP8266 Library
#include <ESP8266WiFi.h>
static WiFiClient espClient;

// 引入阿里云 IoT SDK
#include <AliyunIoTSDK.h>

// 设置产品和设备的信息，从阿里云设备信息里查看
#define PRODUCT_KEY "a1Cv6WGX7Es"
#define DEVICE_NAME "device1"
#define DEVICE_SECRET "ece8868f3536024046768be67a9a2099"

#define REGION_ID "cn-shanghai"

// 设置 wifi 信息
#define WIFI_SSID "Honor 8X Max"
#define WIFI_PASSWD "qwertyui"

static int is_monitor = 0;
static float save_data = 0.0;
static int is_save = 0;

void setup() {
  Serial.begin(9600);
  // 初始化 wifi
  wifiInit(WIFI_SSID, WIFI_PASSWD);

  // 初始化 iot，需传入 wifi 的 client，和设备产品信息
  AliyunIoTSDK::begin(espClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, REGION_ID);

  // 绑定一个设备属性回调，当远程修改此属性，会触发 monitor_Callback
  // Monitor_Switch 是在设备产品中定义的物联网模型的 id
  AliyunIoTSDK::bindData("Monitor_Switch_On", monitor_Callback_On);
  AliyunIoTSDK::bindData("Monitor_Switch_Off", monitor_Callback_Off);

  AliyunIoTSDK::bindData("Monitor_Is_Save", monitor_Callback_Save);

}

void loop() {
  AliyunIoTSDK::loop();//必要函数

  //接收client传来的数据
  String jsonData = recvjson();
  Serial.print("JsonData:");
  Serial.println(jsonData);

  //判断是否有数据传来
  if (jsonData.length() > 0) {
    // const size_t capacity = JSON_OBJECT_SIZE(1) + 10;
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, jsonData);
    JsonObject obj = doc.as<JsonObject>();

    float temperature = obj["temperature"].as<float>(); //
    Serial.print("temperature:");
    Serial.println(temperature);

    if (is_monitor == 1) {
      // 发送一个数据到云平台，Temperature 是在设备产品中定义的物联网模型的 id
      AliyunIoTSDK::send("Temperature", temperature);
      if (is_save == 1) {
        save_data = temperature;
        // AliyunIoTSDK::send("Save_Data", save_data);
        is_save = 0;
      }
    }
    //int numtemp = obj["temp"].as<int>();
    //Serial.print("numtemp:");
    //Serial.println(numtemp);
    //digitalWrite(8, numKey);
    delay(300);
  }
  else {
    //不做操作
    delay(1000);
  }
}

// 初始化 wifi 连接
void wifiInit(const char *ssid, const char *passphrase)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passphrase);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println(WL_CONNECTED);
    Serial.println("WiFi not Connect");
  }
  Serial.println("Connected to AP");
}

// 监测属性修改的回调函数
void monitor_Callback_On(JsonVariant p)
{
  int Monitor_Switch_On = p["monitor_Callback_Save"];
  if (Monitor_Switch_On == 1)
  {
    // 启动监测
    is_monitor = 1;
  }
}

void monitor_Callback_Off(JsonVariant p)
{
  int Monitor_Switch_Off = p["Monitor_Switch_Off"];
  if (Monitor_Switch_Off == 1)
  {
    // 关闭监测
    is_monitor = 0;
  }
}

void monitor_Callback_Save(JsonVariant p)
{
  int Monitor_Is_Save = p["Monitor_Is_Save"];
  if (Monitor_Is_Save == 1)
  {
    // 开启保存（一次性）
    is_save = 1;
  }
}

/*Json相关函数*/
//创建Json字符串
String sendJson(String data) {
  const size_t capacity = JSON_OBJECT_SIZE(1) + 10;
  DynamicJsonDocument doc(capacity);
  doc["temperature"] = data;
  String josn;
  serializeJson(doc, josn);//创建Json字符串
  return  josn;
}

//获取arduino上发送过来的数据
String recvjson() {
  String comdata = "";
  while (Serial.available() > 0) { //串口接收到数据
    comdata += char(Serial.read());//获取串口接收到的数据
    delay(2);
  }
  //Serial.println(comdata);
  return comdata;
}
