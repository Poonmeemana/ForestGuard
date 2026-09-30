#pragma once
 
// วางไฟล์นี้ในโฟลเดอร์เดียวกับ gateway.ino แล้วกรอกค่าจริง
#define WIFI_SSID    "your-wifi-name"
#define WIFI_PASSWORD "your-wifi-password"
 
// URL ของ Apps Script Web app (ลงท้าย /exec)
#define GAS_WEBAPP_URL "https://script.google.com/macros/s/XXXXXXXX/exec"
 
// LINE Messaging API
#define LINE_CHANNEL_ACCESS_TOKEN "your-channel-access-token"
#define LINE_TARGET_ID            "Uxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"  // userId หรือ groupId
