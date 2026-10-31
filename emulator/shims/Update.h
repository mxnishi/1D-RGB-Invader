#pragma once
#include "Arduino.h"
#define UPDATE_SIZE_UNKNOWN 0
struct UpdateT{bool hasError(){return true;}bool begin(int){return false;}size_t write(uint8_t*,size_t){return 0;}bool end(bool){return false;}void printError(SerialT&){} };inline UpdateT Update;
