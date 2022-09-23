#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "driver/gpio.h"
#include "driver/uart.h"
#include "driver/i2c.h"
#include "hal/uart_types.h"
#include "main.h"
#include "string.h"
#include "esp_system.h"
#include "esp_log.h"

void motor_uart_init();

void motor_i2c_init();

void Delay(int delayms);

void motor_gpio_init();

void drv_wakeup();

void drv_sleep();

void drv_reset();

void drv_slow_decay();

void drv_mix_decay();

void drv_fast_decay();

void drv_init();

void drv_speed(int step_mode);

void drv_step();