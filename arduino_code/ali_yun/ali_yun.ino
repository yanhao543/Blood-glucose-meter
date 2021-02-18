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
#define WIFI_SSID "Honor 8X Max"
#define WIFI_PASSWD "qwertyui"

void setup()
{
    Serial.begin(115200);
    
    // 初始化 wifi
    wifiInit(WIFI_SSID, WIFI_PASSWD);
    
    // 初始化 iot，需传入 wifi 的 client，和设备产品信息
    AliyunIoTSDK::begin(espClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, REGION_ID);
    
    // 绑定一个设备属性回调，当远程修改此属性，会触发 powerCallback
    // PowerSwitch 是在设备产品中定义的物联网模型的 id
    //AliyunIoTSDK::bindData("PowerSwitch", powerCallback);
    
    // 发送一个数据到云平台，LightLuminance 是在设备产品中定义的物联网模型的 id
    //AliyunIoTSDK::send("CurrentTemperature",26);
}

void loop()
{
    AliyunIoTSDK::loop();//必要函数
    // 绑定一个设备属性回调，当远程修改此属性，会触发 powerCallback
    // Temperature 是在设备产品中定义的物联网模型的 id
    AliyunIoTSDK::bindData("Temperature", powerCallback);
    
    // 发送一个数据到云平台，Temperature 是在设备产品中定义的物联网模型的 id
    AliyunIoTSDK::send("Temperature",26);
    delay(5000);
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

// 电源属性修改的回调函数
void powerCallback(JsonVariant p)
{
    int PowerSwitch = p["PowerSwitch"];
    if (PowerSwitch == 1)
    {
        // 启动设备
    } 
}
