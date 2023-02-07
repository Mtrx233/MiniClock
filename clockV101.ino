#include "main.h"

#define  VERSION   "V101"
dht11 DHT11;//定义传感器类型
#define DHT11PIN 2//定义传感器连接引脚。此处的PIN2在NodeMcu8266开发板上对应的引脚是D4

void connect_wifi()                               //联网
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);           //用固定的账号密码连接网络

  while (WiFi.status() != WL_CONNECTED) {         
    for (uint8_t n = 0; n < 10; n++){             //每500毫秒检测一次状态
      PowerOn_Loading(50);
    }
  }
  
  while(loadNum < 194){                           
    PowerOn_Loading(1);
  }
  
}

void PowerOn_Loading(uint8_t delayTime)           //开机联网显示的进度条，输入延时时间
{
  clk.setColorDepth(8);
  clk.createSprite(200, 50);                      //创建Sprite
  clk.fillSprite(0x0000);                         //填充颜色

  clk.drawRoundRect(0,0,200,16,8,0xFFFF);         //画一个圆角矩形
  clk.fillRoundRect(3,3,loadNum,10,5,0xFFFF);     //画一个填充的圆角矩形
  clk.setTextDatum(CC_DATUM);                     //显示对齐方式
  clk.setTextColor(TFT_GREEN, 0x0000);            //文本的前景色和背景色
  clk.drawString("Connecting to WiFi",100,40,2);  //显示文本
  clk.pushSprite(20,130);                         //Sprite中内容一次推向屏幕
  clk.deleteSprite();                             //删除Sprite
  loadNum += 1;                                   //进度条位置变化，直到加载完成
  if(loadNum>=194){
    loadNum = 194;
  }
  delay(delayTime);
}

void digitalClockDisplay()                        //时间显示
{
  TJpgDec.drawJpg(0,0,watchtop, sizeof(watchtop));//显示顶部图标 240*20
  TJpgDec.drawJpg(0,220,watchbottom, sizeof(watchbottom));//显示底部图标 240*20
  
  //绘制线框
  tft.drawFastHLine(0,54,240,TFT_BLACK);          //这些坐标都是窗体内部坐标
  tft.drawFastVLine(150,20,34,TFT_BLACK);
  tft.drawFastHLine(0,186,240,TFT_BLACK);
  tft.drawFastVLine(60,186,34,TFT_BLACK);
  tft.drawFastVLine(160,186,34,TFT_BLACK);

  TJpgDec.drawJpg(161,190,temperature, sizeof(temperature));//温度图标
  TJpgDec.drawJpg(159,150,humidity, sizeof(humidity));  //湿度图标
  
  clk.setColorDepth(8);

  //--------------------中间时间区显示开始--------------------
  //时分
  clk.createSprite(140, 48);                      //创建Sprite，先在Sprite内存中画点，然后将内存中的点一次推向屏幕，这样刷新比较快
  clk.fillSprite(bgColor);                        //背景色
  clk.setTextDatum(CC_DATUM);                     //显示对齐方式
  clk.setTextColor(TFT_BLACK, bgColor);           //文本的前景色和背景色
  clk.drawString(hourMinute(),70,24,7);           //绘制时和分
  clk.pushSprite(28,60);                          //Sprite中内容一次推向屏幕
  clk.deleteSprite();                             //删除Sprite
  clk.unloadFont();

  clk.loadFont(FxLED_32); 
  //秒
  clk.createSprite(40, 32);
  clk.fillSprite(bgColor);
  clk.loadFont(FxLED_32);                         //加载字体
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, bgColor); 
  clk.drawString(num2str(second()),20,16);
  clk.unloadFont();                               //卸载字体
  clk.pushSprite(170,80);
  clk.deleteSprite();

  //--------------------底部时间区显示开始--------------------
   clk.loadFont(ZdyLwFont_20);                     //加载汉字字体
  //星期
  clk.createSprite(58, 32);
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, bgColor);
  clk.drawString(week(),29,16);                   
  clk.pushSprite(1,188);
  clk.deleteSprite();
  
  //月日
  clk.createSprite(98, 32);
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, bgColor);  
  clk.drawString(monthDay(),49,16);               
  clk.pushSprite(61,188);
  clk.deleteSprite();

  clk.unloadFont();                               //卸载字体
}

void digitalTemDisplay()                        //温度显示
{
  TJpgDec.drawJpg(0,0,watchtop, sizeof(watchtop));//显示顶部图标 240*20
  TJpgDec.drawJpg(0,220,watchbottom, sizeof(watchbottom));//显示底部图标 240*20
  
  clk.loadFont(ZdyLwFont_20);                     //加载汉字字体
  DHT11.read(DHT11PIN); //更新传感器所有信息

  TJpgDec.drawJpg(0,140,temperature, sizeof(temperature));//温度图标
  TJpgDec.drawJpg(0,180,humidity, sizeof(humidity));  //湿度图标

  float tem = ((float)DHT11.temperature - 20) / 10;
  if(tem > 1){
    tem = 1;
  }
  float tems = ((float)DHT11.temperature) / 50;
  float width = (float)DHT11.humidity /100;

  TJpgDec.drawJpg(80,30,gImage_qq_logo,sizeof(gImage_qq_logo));    //温度图标
  clk.loadFont(ZdyLwFont_20);                     //加载汉字字体
  
  clk.setColorDepth(8);
  clk.createSprite(240, 40);                      //创建Sprite
  clk.fillSprite(bgColor);                         //填充颜色
  clk.drawRoundRect(93,0,100,16,8,TFT_GREEN);         //画一个圆角矩形
  clk.fillRoundRect(96,3,tems*100,10,5,TFT_RED);     //画一个填充的圆角矩形
  clk.setTextDatum(CC_DATUM);                     //显示对齐方式
  clk.setTextColor(TFT_BLACK, bgColor);            //文本的前景色和背景色
  clk.drawString(temGet(),45,10);                 //显示文本
  clk.pushSprite(36,140);                         //Sprite中内容一次推向屏幕
  clk.deleteSprite();                             //删除Sprite

  
  
  clk.setColorDepth(8);
  clk.createSprite(240, 40);                      //创建Sprite
  clk.fillSprite(bgColor);                         //填充颜色
  clk.drawRoundRect(93,0,100,16,8,TFT_GREEN);         //画一个圆角矩形
  clk.fillRoundRect(96,3,width*100,10,5,TFT_RED);     //画一个填充的圆角矩形
  clk.setTextDatum(CC_DATUM);                     //显示对齐方式
  clk.setTextColor(TFT_BLACK, bgColor);            //文本的前景色和背景色
  clk.drawString(widthGet(),45,10);  //显示文本
  clk.pushSprite(36,180);                         //Sprite中内容一次推向屏幕
  clk.deleteSprite();                             //删除Sprite
  temGet();
  widthGet();
  clk.unloadFont();
}

void digitalClassDisplay()
{
  clk.loadFont(Class_32);
  TJpgDec.drawJpg(0,0,watchtop, sizeof(watchtop));//显示顶部图标 240*20
  TJpgDec.drawJpg(0,220,watchbottom, sizeof(watchbottom));//显示底部图标 240*20
  TJpgDec.drawJpg(0,20,sid, sizeof(sid));//显示底部图标 240*20
  
  
  TJpgDec.drawJpg(15,65,morning, sizeof(morning));  //温度图标
  TJpgDec.drawJpg(15,105,noon, sizeof(noon));  //湿度图标
  TJpgDec.drawJpg(15,145,afternoon, sizeof(afternoon));  //温度图标
  TJpgDec.drawJpg(15,185,night, sizeof(night));  //湿度图标

  TJpgDec.drawJpg(50,65,one, sizeof(one));  //温度图标
  TJpgDec.drawJpg(50,105,two, sizeof(two));  //湿度图标
  TJpgDec.drawJpg(50,145,three, sizeof(three));  //温度图标
  TJpgDec.drawJpg(50,185,four, sizeof(four));  //湿度图标

  clk.setColorDepth(8);
  clk.createSprite(165,30);                       //创建Sprite
  clk.fillSprite(bgColor);                        //填充颜色
  clk.setTextColor(TFT_BLACK, bgColor);           //文本的前景色和背景色
  clk.drawString("暂无",65,10,2);            //显示文本
  clk.pushSprite(85,185);                         //Sprite中内容一次推向屏幕
  clk.deleteSprite();                             //删除Sprite

  clk.setColorDepth(8);
  clk.createSprite(165,30);                       //创建Sprite
  clk.fillSprite(bgColor);                        //填充颜色
  clk.setTextColor(TFT_BLACK, bgColor);           //文本的前景色和背景色
  clk.drawString("暂无",65,10,2);                 //显示文本
  clk.pushSprite(85,145);                         //Sprite中内容一次推向屏幕
  clk.deleteSprite();                             //删除Sprite

  clk.setColorDepth(8);
  clk.createSprite(165,30);                       //创建Sprite
  clk.fillSprite(bgColor);                        //填充颜色
  clk.setTextColor(TFT_BLACK, bgColor);           //文本的前景色和背景色
  clk.drawString("移动互联概论",65,10,2);                  //显示文本
  clk.pushSprite(85,105);                         //Sprite中内容一次推向屏幕
  clk.deleteSprite();                             //删除Sprite

  clk.setColorDepth(8);
  clk.createSprite(165,30);                       //创建Sprite
  clk.fillSprite(bgColor);                        //填充颜色
  clk.setTextColor(TFT_BLACK, bgColor);           //文本的前景色和背景色
  clk.drawString("嵌入式软件开发技术",65,10,2);               //显示文本
  clk.pushSprite(85,65);                          //Sprite中内容一次推向屏幕
  clk.deleteSprite();                             //删除Sprite
  clk.unloadFont();
}

String temGet(){
  DHT11.read(DHT11PIN); //更新传感器所有信息
  String s = "温度:";
  int tem = (int)DHT11.temperature;
  Serial.print("温度:");
  Serial.println((float)DHT11.temperature, 2);
  Serial.println(tem);
  s += tem;
  s += "℃";
  return s;
}

String widthGet(){
  DHT11.read(DHT11PIN); //更新传感器所有信息
  String s = "湿度:";
  int width = (int)DHT11.humidity;
  s += width;
  s += "%";
  return s;
}

String week()                                     //星期
{
  String wk[7] = {"日","一","二","三","四","五","六"};
  String s = "周" + wk[weekday()-1];
  return s;
}

String monthDay()                                 //月日
{
  String s = String(month());
  s = s + "月" + day() + "日";
  return s;
}

String hourMinute()                               //时分
{
  String s = num2str(hour());
  s = s + ":" + num2str(minute());
  return s;
}

String num2str(int digits)                        //数字转换成字符串，保持2位显示
{
  String s = "";
  if (digits < 10)
    s = s + "0";
  s = s + digits;
  return s;
}

void printDigits(int digits)                      //打印时间数据
{
  Serial.print(":");
  if (digits < 10)                                //打印两位数字
    Serial.print('0');
  Serial.print(digits);
}

time_t getNtpTime()                               //获取NTP时间
{
  IPAddress ntpServerIP;                          //NTP服务器的IP地址

  while (Udp.parsePacket() > 0) ;                 //之前的数据没有处理的话一直等待 discard any previously received packets
  WiFi.hostByName(ntpServerName, ntpServerIP);    //从网站名获取IP地址
  
  sendNTPpacket(ntpServerIP);                     //发送数据包
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();                 //接收数据
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);    //从缓冲区读取数据
      
      unsigned long secsSince1900;
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0;                                       //没获取到数据的话返回0
}

void sendNTPpacket(IPAddress &address)            //发送数据包到NTP服务器
{
  memset(packetBuffer, 0, NTP_PACKET_SIZE);       //缓冲区清零

  packetBuffer[0] = 0b11100011;                   //LI, Version, Mode   填充缓冲区数据
  packetBuffer[1] = 0;                            //Stratum, or type of clock
  packetBuffer[2] = 6;                            //Polling Interval
  packetBuffer[3] = 0xEC;                         //Peer Clock Precision
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;

  Udp.beginPacket(address, 123);                  //NTP服务器端口123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);       //发送udp数据
  Udp.endPacket();                                //发送结束
}

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)//显示回调函数
{
  if (y >= tft.height()) return 0;
  tft.pushImage(x, y, w, h, bitmap);
  return 1;
}

void getCityCode()                                //发送HTTP请求并且将服务器响应通过串口输出
{
  String URL = "http://wgeo.weather.com.cn/ip/?_="+String(now());

  httpClient.begin(URL);                          //配置请求地址。此处也可以不使用端口号和PATH而单纯的
  httpClient.setUserAgent("esp8266");             //用户代理版本，其实没什么用 最重要是后端服务器支持
  httpClient.addHeader("Referer", "http://www.weather.com.cn/");
 
  int httpCode = httpClient.GET();                //启动连接并发送HTTP请求
  Serial.print("Send GET request to URL: ");
  Serial.println(URL);
  
  if (httpCode == HTTP_CODE_OK) {                 //如果服务器响应OK则从服务器获取响应体信息并通过串口输出
    String str = httpClient.getString();
    int aa = str.indexOf("id=");
    if (aa > -1){                                 //应答包里找到ID了
       cityCode = str.substring(aa+4,aa+4+9);     //9位长度
       Serial.println(cityCode); 
       getCityWeater();                           //获取天气信息
       LastTime2 = millis();
    }
    else{                                         //没有找到ID
      Serial.println("获取城市代码失败");  
    }
  } 
  else{
    Serial.println("请求城市代码错误：");
    Serial.println(httpCode);
  }
 
  httpClient.end();                               //关闭与服务器连接
}

void getCityWeater()                              //获取城市天气
{
  String URL = "http://d1.weather.com.cn/weather_index/" + cityCode + ".html?_="+String(now());

  httpClient.begin(URL);                          //配置请求地址。
  httpClient.setUserAgent("esp8266");             //用户代理版本，其实没什么用 最重要是后端服务器支持
  //httpClient.setUserAgent("Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");//设置请求头中的User-Agent
  httpClient.addHeader("Referer", "http://www.weather.com.cn/");
 
  int httpCode = httpClient.GET();                //启动连接并发送HTTP请求
  Serial.print("Send GET request to URL: ");
  Serial.println(URL);
  
  if (httpCode == HTTP_CODE_OK) {                 //如果服务器响应OK则从服务器获取响应体信息并通过串口输出
    String str = httpClient.getString();
    int indexStart = str.indexOf("weatherinfo\":");//寻找起始和结束位置
    int indexEnd = str.indexOf("};var alarmDZ");

    String jsonCityDZ = str.substring(indexStart+13,indexEnd);//复制字符串
    Serial.println(jsonCityDZ);

    indexStart = str.indexOf("dataSK =");         //寻找起始和结束位置
    indexEnd = str.indexOf(";var dataZS");
    String jsonDataSK = str.substring(indexStart+8,indexEnd);//复制字符串
    Serial.println(jsonDataSK);

    indexStart = str.indexOf("\"f\":[");          //寻找起始和结束位置
    indexEnd = str.indexOf(",{\"fa");
    String jsonFC = str.substring(indexStart+5,indexEnd);//复制字符串
    Serial.println(jsonFC);
    
    weaterData(&jsonCityDZ,&jsonDataSK,&jsonFC);  //显示天气信息
    Serial.println("获取成功");
    
  } 
  else {
    Serial.println("请求城市天气错误：");
    Serial.print(httpCode);
  }
 
  httpClient.end();                               //关闭与服务器连接
}

void weaterData(String *cityDZ,String *dataSK,String *dataFC)//天气信息写到屏幕上
{
  DynamicJsonDocument doc(512);
  deserializeJson(doc, *dataSK);
  JsonObject sk = doc.as<JsonObject>();

  clk.setColorDepth(8);
  clk.loadFont(ZdyLwFont_20);                     //加载汉字字体
  
  //温度显示
  clk.createSprite(54, 32);                       //创建Sprite
  clk.fillSprite(bgColor);                        //填充颜色
  clk.setTextDatum(CC_DATUM);                     //显示对齐方式
  clk.setTextColor(TFT_BLACK, bgColor);           //文本的前景色和背景色 
  clk.drawString(sk["temp"].as<String>()+"℃",27,16);//显示文本
  clk.pushSprite(185,188);                        //Sprite中内容一次推向屏幕
  clk.deleteSprite();                             //删除Sprite

  //城市名称显示
  clk.createSprite(88, 32); 
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, bgColor); 
  clk.drawString(sk["cityname"].as<String>(),44,16);
  clk.pushSprite(151,21);
  clk.deleteSprite();
  
  //PM2.5空气指数显示
  uint16_t pm25BgColor = tft.color565(156,202,127);//优
  String aqiTxt = "优";
  int pm25V = sk["aqi"];
  if (pm25V > 200){
    pm25BgColor = tft.color565(136,11,32);        //重度，显示颜色和空气质量程度
    aqiTxt = "重度";
  }
  else if (pm25V > 150){
    pm25BgColor = tft.color565(186,55,121);       //中度
    aqiTxt = "中度";
  }
  else if (pm25V > 100){
    pm25BgColor = tft.color565(242,159,57);       //轻
    aqiTxt = "轻度";
  }
  else if (pm25V > 50){
    pm25BgColor = tft.color565(247,219,100);      //良
    aqiTxt = "良";
  }
  clk.createSprite(50, 24); 
  clk.fillSprite(bgColor);
  clk.fillRoundRect(0,0,50,24,4,pm25BgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(0xFFFF); 
  clk.drawString(aqiTxt,25,13);
  clk.pushSprite(5,150);
  clk.deleteSprite();

  //湿度显示
  clk.createSprite(56, 24); 
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, bgColor); 
  clk.drawString(sk["SD"].as<String>(),28,13);
  //clk.drawString("100%",28,13);
  clk.pushSprite(180,150);
  clk.deleteSprite();

  scrollText[0] = "实时天气 " + sk["weather"].as<String>();//滚动显示的数据缓冲区
  scrollText[1] = "空气质量 " + aqiTxt;
  scrollText[2] = "风向 " + sk["WD"].as<String>()+sk["WS"].as<String>();
  
  //左上角滚动字幕
  deserializeJson(doc, *cityDZ);
  JsonObject dz = doc.as<JsonObject>();
  //Serial.println(sk["ws"].as<String>());
  //String aa = "今日天气:" + dz["weather"].as<String>() + "，温度:最低" + dz["tempn"].as<String>() + "，最高" + dz["temp"].as<String>() + " 空气质量:" + aqiTxt + "，风向:" + dz["wd"].as<String>() + dz["ws"].as<String>();
  //Serial.println(aa);
  scrollText[3] = "今日" + dz["weather"].as<String>();
  
  deserializeJson(doc, *dataFC);
  JsonObject fc = doc.as<JsonObject>();
  scrollText[4] = "最低温度"+fc["fd"].as<String>()+"℃";
  scrollText[5] = "最高温度"+fc["fc"].as<String>()+"℃";
  
  clk.unloadFont();                               //卸载字体
}

void scrollBanner()                               //天气滚动条显示
{
  now1 = millis();
  if(now1 - LastTime1 > 2500){                    //2.5秒切换一次显示内容
    if(scrollText[Dis_Count]){                    //如果滚动显示缓冲区有数据
      clkb.setColorDepth(8);
      clkb.loadFont(ZdyLwFont_20);                //加载汉字字体
      
      for(int pos = 24; pos>0; pos--){            //24点，每次移动一个点，从下往上移
        Dis_Scroll(pos);
      }
      
      //clkb.deleteSprite();                      //删除Sprite，这个我移动到Dis_Scroll函数里了
      clkb.unloadFont();                          //卸载字体
  
      if (Dis_Count >= 5){                        //总共显示五条信息
        Dis_Count = 0;                            //回第一个
      }
      else{
        Dis_Count += 1;                           //准备切换到下一个  
      }
      //Serial.println(Dis_Count);
    }
    LastTime1 = now1;
  }
}

void Dis_Scroll(int pos){                         //字体滚动
  clkb.createSprite(148, 24);                     //创建Sprite，先在Sprite内存中画点，然后将内存中的点一次推向屏幕，这样刷新比较快
  clkb.fillSprite(bgColor);                       //背景色
  clkb.setTextWrap(false);
  clkb.setTextDatum(CC_DATUM);                    //显示对齐方式
  clkb.setTextColor(TFT_BLACK, bgColor);          //文本的前景色和背景色
  clkb.drawString(scrollText[Dis_Count],74,pos+12);//打显示内容
  clkb.pushSprite(2,24);                           //Sprite中内容一次推向屏幕
  clkb.deleteSprite();                            //删除Sprite
}

void imgAnim(){
  int x=80,y=114,dt=30;                            //瘦子版dt=10毫秒 胖子30较为合适

  TJpgDec.drawJpg(x,y,i0, sizeof(i0));            //打一张图片延时一段时间，达到动画效果
  delay(dt);
  TJpgDec.drawJpg(x,y,i1, sizeof(i1));
  delay(dt);
  TJpgDec.drawJpg(x,y,i2, sizeof(i2));
  delay(dt);
  TJpgDec.drawJpg(x,y,i3, sizeof(i3));
  delay(dt);  
  TJpgDec.drawJpg(x,y,i4, sizeof(i4));
  delay(dt);  
  TJpgDec.drawJpg(x,y,i5, sizeof(i5));
  delay(dt);  
  TJpgDec.drawJpg(x,y,i6, sizeof(i6));
  delay(dt);  
  TJpgDec.drawJpg(x,y,i7, sizeof(i7));
  delay(dt);  
  TJpgDec.drawJpg(x,y,i8, sizeof(i8));
  delay(dt);  
  TJpgDec.drawJpg(x,y,i9, sizeof(i9));  
}

void imgsAnim(){
  int x=45,y=20,dt=30;                            
  TJpgDec.drawJpg(x,y,break_1, sizeof(break_1));
  delay(dt);
  TJpgDec.drawJpg(x,y,break_2, sizeof(break_2));
  delay(dt);
  TJpgDec.drawJpg(x,y,break_3, sizeof(break_3));
  delay(dt);  
  TJpgDec.drawJpg(x,y,break_4, sizeof(break_4));
  delay(dt);  
  TJpgDec.drawJpg(x,y,break_5, sizeof(break_5));
  delay(dt);  
  TJpgDec.drawJpg(x,y,break_6, sizeof(break_6));
  delay(dt);  
  TJpgDec.drawJpg(x,y,break_7, sizeof(break_7));
  delay(dt);  
  TJpgDec.drawJpg(x,y,break_8, sizeof(break_8));
  delay(dt);  
  TJpgDec.drawJpg(x,y,break_9, sizeof(break_9));  
  delay(dt);
  TJpgDec.drawJpg(x,y,break_10, sizeof(break_10));
  delay(dt);  
  TJpgDec.drawJpg(x,y,break_11, sizeof(break_11));
  delay(dt);  
  TJpgDec.drawJpg(x,y,break_12, sizeof(break_12));
  delay(dt);  
  TJpgDec.drawJpg(x,y,break_13, sizeof(break_13));
  delay(dt);  
  TJpgDec.drawJpg(x,y,break_14, sizeof(break_14));
  delay(dt);
  TJpgDec.drawJpg(x,y,break_15, sizeof(break_15));            //打一张图片延时一段时间，达到动画效果
   clk.loadFont(Class_32);
  clk.setColorDepth(8);
  clk.createSprite(240,30);                       //创建Sprite
  clk.fillSprite(bgColor);                        //填充颜色
  clk.setTextColor(TFT_BLACK, bgColor);           //文本的前景色和背景色
  clk.drawString("稍微休息一下吧",65,10,2);            //显示文本
  clk.pushSprite(50,190);                         //Sprite中内容一次推向屏幕
  clk.deleteSprite();                             //删除Sprite
   clk.unloadFont();
}

void setup()
{
  Serial.begin(115200);                           //初始化串口
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(TouchPin, INPUT);
  Serial.println();                               //打印回车换行
  
  tft.init();                                     //TFT初始化
  tft.setRotation(0);                             //旋转角度0-3
  tft.fillScreen(0x0000);                         //清屏
  tft.setTextColor(TFT_BLACK, bgColor);           //设置字体颜色

  connect_wifi();                                 //联网处理

  Serial.println("Starting UDP");                 //连接时间服务器
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

  TJpgDec.setJpgScale(1);                         //设置放大倍数
  TJpgDec.setSwapBytes(true);                     //交换字节
  TJpgDec.setCallback(tft_output);                //回调函数

  TJpgDec.drawJpg(0,0,watchtop, sizeof(watchtop));//显示顶部图标 240*20
  TJpgDec.drawJpg(0,220,watchbottom, sizeof(watchbottom));//显示底部图标 240*20

  //绘制一个窗口
  tft.setViewport(0, 0, 240, 240);               //中间的显示区域大小

  LastTime4 = millis();
  
  getCityCode();                                  //通过IP地址获取城市代码

}

void loop()
{
  digitalWrite(TrigPin, LOW);       //用低高低电平发送脉冲给Trigpin引脚
  delayMicroseconds(2);             //微秒级延时
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  cm = pulseIn(EchoPin, HIGH) / 58.0; //读取脉冲宽度，换算成厘米

  Serial.println(cm);
  
  if(cm > 40){
    LastTime4 = millis();
    if(flag == 1){
      TJpgDec.drawJpg(0,0,black, sizeof(black));//显示顶部图标 240*20
      TJpgDec.drawJpg(0,220,black, sizeof(black));//显示底部图标 240*20
      tft.fillScreen(0x0000);
    }
    flag = 0;
  }else{
    if(millis() - LastTime4 > 12000){
       tft.fillScreen(bgColor);
       imgsAnim();
       LastTime4 = millis();
       delay(1000);
       tft.fillScreen(bgColor);
     }
    if(flag == 0){
      tft.fillScreen(bgColor);
      window = 0;
      flag = 1;
    }
  }

  if(digitalRead(TouchPin) == HIGH){
    window++;
    tft.fillScreen(bgColor);                         //清屏
    tft.fillRoundRect(0,0,240,200,5,bgColor);       //实心圆角矩形
    if(window == 3){
      window = 0;
    }
    delay(100);
  }

  if(flag == 1){
    if(window == 0){
      if (timeStatus() != timeNotSet){                //已经获取到数据的话
      if (now() != prevDisplay){                    //如果本次数据和上次不一样的话，刷新
        prevDisplay = now();
        digitalClockDisplay();
        }
      }
      if(millis() - LastTime2 > 6000){              //10分钟更新一次天气
        LastTime2 = millis();
        getCityWeater();
        imgAnim(); 
      }
      digitalClockDisplay();
      scrollBanner();                                 //天气数据滚动显示
      imgAnim();                                      //太空人显示
    }else if(window == 1){
      digitalTemDisplay();
    }else{
      digitalClassDisplay();
    }
  }
  delay(30);
}
