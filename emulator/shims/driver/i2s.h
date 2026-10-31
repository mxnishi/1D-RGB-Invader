#pragma once
#include "Arduino.h"
using i2s_mode_t=int;
#define I2S_MODE_MASTER 1
#define I2S_MODE_TX 2
#define I2S_BITS_PER_SAMPLE_16BIT 16
#define I2S_CHANNEL_FMT_ONLY_LEFT 0
#define I2S_COMM_FORMAT_STAND_I2S 0
#define ESP_INTR_FLAG_LEVEL1 1
#define I2S_PIN_NO_CHANGE -1
#define I2S_NUM_0 0
struct i2s_config_t{int mode,sample_rate,bits_per_sample,channel_format,communication_format,intr_alloc_flags,dma_buf_count,dma_buf_len;bool use_apll,tx_desc_auto_clear;};struct i2s_pin_config_t{int bck_io_num,ws_io_num,data_out_num,data_in_num;};inline void i2s_write(int,void*,size_t n,size_t*w,int){*w=n;}inline void i2s_driver_install(int,i2s_config_t*,int,void*){}inline void i2s_set_pin(int,i2s_pin_config_t*){}inline void i2s_zero_dma_buffer(int){}
