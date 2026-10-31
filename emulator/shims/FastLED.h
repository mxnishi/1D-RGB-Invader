#pragma once
#include "Arduino.h"
struct CHSV{uint8_t h,s,v;CHSV(int a,int b,int c):h(a),s(b),v(c){}};
struct CRGB{uint8_t r=0,g=0,b=0;CRGB(){}CRGB(int a,int c,int d):r(a),g(c),b(d){}CRGB(CHSV c){double h=c.h/256.0*6,s=c.s/255.0,v=c.v;double f=h-floor(h),p=v*(1-s),q=v*(1-f*s),t=v*(1-(1-f)*s);switch(int(h)%6){case 0:r=v;g=t;b=p;break;case 1:r=q;g=v;b=p;break;case 2:r=p;g=v;b=t;break;case 3:r=p;g=q;b=v;break;case 4:r=t;g=p;b=v;break;default:r=v;g=p;b=q;}}void nscale8(int n){r=r*n/256;g=g*n/256;b=b*n/256;}static const CRGB Black,White,Red,Green,Blue,Yellow;};
inline const CRGB CRGB::Black{0,0,0},CRGB::White{255,255,255},CRGB::Red{255,0,0},CRGB::Green{0,128,0},CRGB::Blue{0,0,255},CRGB::Yellow{255,255,0};
#define WS2812B 1
#define GRB 1
inline void fill_solid(CRGB*p,int n,CRGB c){std::fill(p,p+n,c);}
struct FastLEDT{CRGB*data=nullptr;int count=0,brightness=128;std::vector<CRGB> frame;template<int,int,int>void addLeds(CRGB*p,int n){data=p;count=n;}void clear(){if(data)std::fill(data,data+count,CRGB{});}void show(){if(data)frame.assign(data,data+count);}void setBrightness(int n){brightness=n;}void setDither(int){}void setMaxPowerInVoltsAndMilliamps(int,int){} };inline FastLEDT FastLED;
