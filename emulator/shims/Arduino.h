#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <random>
using std::min;using std::max;
class String:public std::string{public:using std::string::string;String():std::string(){}String(const std::string&s):std::string(s){}String(int n):std::string(std::to_string(n)){}String(unsigned int n):std::string(std::to_string(n)){}String(long n):std::string(std::to_string(n)){}String(unsigned long n):std::string(std::to_string(n)){}String(double n):std::string(std::to_string(n)){}int indexOf(char c,int start=0)const{auto p=find(c,start);return p==npos?-1:int(p);}String substring(int a)const{return substr(std::min(size(),size_t(std::max(0,a))));}String substring(int a,int b)const{return substr(a,std::max(0,b-a));}int toInt()const{return std::atoi(c_str());}float toFloat()const{return std::atof(c_str());}operator const char*()const{return c_str();}};
inline String operator+(const String&a,const String&b){return String(static_cast<const std::string&>(a)+static_cast<const std::string&>(b));}
inline String operator+(const char*a,const String&b){return String(a)+b;}inline String operator+(const String&a,const char*b){return a+String(b);}
inline uint32_t clockMs=0;inline int buttonMask=0;inline bool restarting=false;
inline unsigned long millis(){return clockMs;}inline void delay(unsigned long n){clockMs+=n;}
#define LOW 0
#define HIGH 1
#define INPUT_PULLUP 2
inline int digitalRead(int pin){int bit=pin==16?1:pin==8?2:pin==15?4:pin==9?8:0;return buttonMask&bit?LOW:HIGH;}inline void pinMode(int,int){}
inline long map(long x,long a,long b,long c,long d){return (x-a)*(d-c)/(b-a)+c;}
inline std::mt19937 rng(42);inline long random(long a,long b){return std::uniform_int_distribution<long>(a,b-1)(rng);}inline long random(long b){return random(0,b);}
struct SerialT{void begin(int){}template<class... T>void printf(const char*,T...){} };inline SerialT Serial;
struct Reboot{};struct ESPT{void restart(){restarting=true;throw Reboot{};}};inline ESPT ESP;
using QueueHandle_t=void*;using TaskHandle_t=void*;inline std::vector<int> soundEvents;
inline QueueHandle_t xQueueCreate(int,int){return nullptr;}inline int xQueueSend(QueueHandle_t,const void*p,int){soundEvents.push_back(*(const int*)p);return 1;}inline int xQueueReceive(QueueHandle_t,void*,int){return 0;}inline void xTaskCreatePinnedToCore(void(*)(void*),const char*,int,void*,int,TaskHandle_t*,int){}inline void vTaskDelay(int){}
#define portMAX_DELAY 0
#define pdTRUE 1
#define portTICK_PERIOD_MS 1
