#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <time.h>
#include "env.h"   // WIFI_SSID, WIFI_PASSWORD, GAS_WEBAPP_URL, GATEWAY_ID

// LoRa PIN configuration
#define LORA_NSS_PIN    5
#define LORA_RESET_PIN  14
#define LORA_DIO0_PIN   26
#define LORA_FREQUENCY  923E6

const char *LINE_PUSH_URL = "https://api.line.me/v2/bot/message/push";

void connectWiFi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(300);
    }
}

// เวลาปัจจุบันจาก NTP (UTC+7) แบบ "YYYY-MM-DD HH:MM:SS"
String getTime()
{
    struct tm t;
    if (!getLocalTime(&t)) return "unknown";
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &t);
    return String(buf);
}

// ยิง HTTPS POST แบบ JSON สำหรับ LINE และ Google Sheet
void postJSON(const char *url, String body, bool withLineAuth)
{
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");
    if (withLineAuth) {
        http.addHeader("Authorization", String("Bearer ") + LINE_CHANNEL_ACCESS_TOKEN);
    }

    http.POST(body);
    http.end();
}

void setup()
{
    Serial.begin(115200);
    connectWiFi();
    configTime(7 * 3600, 0, "pool.ntp.org");

    LoRa.setPins(LORA_NSS_PIN, LORA_RESET_PIN, LORA_DIO0_PIN);
    LoRa.begin(LORA_FREQUENCY);
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED) {
        connectWiFi();
    }

    int size = LoRa.parsePacket();
    if (size == 0) return;

    String msg = "";
    while (LoRa.available()) {
        msg += (char)LoRa.read();
    }

    String time = getTime();

    // ข้อความเดิมจาก Node และ เวลาที่ตรวจจับ
    String fullMsgJson = msg + "\nTime: " + time;
    fullMsgJson.replace("\n", "\\n"); 

    // ส่งแจ้งเตือนเข้า LINE
    postJSON(LINE_PUSH_URL,
             "{\"to\":\"" + String(LINE_TARGET_ID) +
             "\",\"messages\":[{\"type\":\"text\",\"text\":\"" + fullMsgJson + "\"}]}",
             true);

    // บันทึกลง Google Sheet
    postJSON(GAS_WEBAPP_URL,
             "{\"datetime\":\"" + time +
             "\",\"message\":\"" + fullMsgJson +
             "\",\"sender\":\"" + String(GATEWAY_ID) + "\"}",
             false);
}
