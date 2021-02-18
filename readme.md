## 1.使用说明

把esp_server中的ino文件烧录到esp mcu中（我用的是ESP8266 NodeMCU V3）

把arduino_client中的文件烧录到arduino板子中（我用的是arduino uno）



正常情况下，观察串口监视器（串口监视ESP8266），会不停出现Json格式的信息，交替出现｛“Temperature”:"26"｝26｛“Temperature”:"0"｝0等信息。

此外阿里云上设备也会显示在线。



## 2.文件说明

ali_yun文件中存放的是esp连接阿里云的示例代码

json文件（由于esp和uno是通过json格式传输数据），这里面放的是解析json数据包的示例代码



## 3.网址参考

arduino平台安装esp库：

https://www.cnblogs.com/oeong/p/11853612.html

esp连接阿里云：

https://blog.csdn.net/NicolasLearner/article/details/113756019

阿里云网站开发：

https://blog.csdn.net/csdn35772148/article/details/111411587



## 4.功能改进

1.现在是网站，之后最好是app

2.现在有啥显示啥，之后最好摁下开关开始检测，之后摁下结束按钮，显示屏清空

3.梅美化坐标系以及背景

4.再说