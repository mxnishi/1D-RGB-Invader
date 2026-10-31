#pragma once
#include "Arduino.h"
#define WIFI_OFF 0
#define WIFI_AP_STA 1
struct IPAddress{bool fromString(String){return true;}};struct WiFiT{void mode(int){}void begin(const char*,const char*){}void config(IPAddress,IPAddress,IPAddress,IPAddress){}void softAP(const char*,const char*){} };inline WiFiT WiFi;
