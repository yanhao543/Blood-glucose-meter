// 引入 wifi 模块，并实例化，不同的芯片这里的依赖可能不同
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
#define WIFI_SSID "realme X7 Pro"
#define WIFI_PASSWD "12345678"

static int is_monitor = 0;
static float save_data = 0.0;
static int is_save = 0;
float temperature = 26;

void setup()
{
    Serial.begin(115200);
    
    // 初始化 wifi
    wifiInit(WIFI_SSID, WIFI_PASSWD);
    
    // 初始化 iot，需传入 wifi 的 client，和设备产品信息
    AliyunIoTSDK::begin(espClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, REGION_ID);
    
    // 绑定一个设备属性回调，当远程修改此属性，会触发 powerCallback
    AliyunIoTSDK::bindData("Monitor_Switch", monitor_Callback);
    //AliyunIoTSDK::bindData("Monitor_Switch_Off", monitor_Callback_Off);
    AliyunIoTSDK::bindData("Monitor_Is_Save", monitor_Callback_Save);
}

void loop()
{
    AliyunIoTSDK::loop();//必要函数
    
    if (is_monitor == 1) {
        // 发送一个数据到云平台，Temperature 是在设备产品中定义的物联网模型的 id
        AliyunIoTSDK::send("Temperature", temperature);
        if (is_save == 1) {
            save_data = temperature - 1;
            AliyunIoTSDK::send("Save_Data", save_data);
            is_save = 0;
        }
    }
    delay(1000);
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
void monitor_Callback(JsonVariant p)
{
  int Monitor_Switch = p["Monitor_Switch"];
  //if (Monitor_Switch_On == 1)
  // 改变监测状态
  is_monitor = Monitor_Switch;
  Serial.println(is_monitor);
}

void monitor_Callback_Save(JsonVariant p)
{
  int Monitor_Is_Save = p["Monitor_Is_Save"];
  if (Monitor_Is_Save == 1)
  {
    // 开启保存（一次性）（上传一次需要保存的数据）
    is_save = 1;
  }
}
