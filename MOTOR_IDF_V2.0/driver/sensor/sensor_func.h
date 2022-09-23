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