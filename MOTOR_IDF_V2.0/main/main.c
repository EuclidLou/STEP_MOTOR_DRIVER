#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "driver/gpio.h"
#include "driver/uart.h"
#include "hal/uart_types.h"
#include "main.h"
#include "string.h"
#include "esp_system.h"
#include "esp_log.h"
#include "motor_init.h"
#include "sensor_func.h"

QueueHandle_t motor_queue;
QueueHandle_t step_queue;
QueueHandle_t dir_queue;
void Atask(void *pvParameters);
void Btask(void *pvParameters);
void Ctask(void *pvParameters);

void app_main(void)
{
    motor_uart_init();
    motor_i2c_init();
    motor_gpio_init();
    
    motor_queue = xQueueCreate(2, sizeof(int));
    step_queue = xQueueCreate(2, sizeof(int));
    dir_queue = xQueueCreate(1, sizeof(int));
    xTaskCreate(Atask, "A", 65536, NULL, 3, NULL);
    xTaskCreate(Btask, "B", 65536, NULL, 1, NULL);
    xTaskCreate(Ctask, "C", 65536, NULL, 2, NULL);
    for(;;);
    esp_restart();
}

void Atask(void *pvParameters) {
    TickType_t Alastwaketime;
    Alastwaketime = xTaskGetTickCount();
    int mode1 = 1;
    int mode0 = 0;
    int speed0 = step_full;
    volatile int dir0 = 0;
    volatile int buffer = 0;
    
    drv_init();
    drv_wakeup();

    for(;;) {
        // printf(YELLOW"This is ATask !\r\n");
        xQueuePeek(dir_queue, &buffer, 0);
        dir0 = buffer;
        xQueueReceive(motor_queue, &buffer, 0);
        speed0 = buffer;
        xQueueReceive(motor_queue, &buffer, 0);
        mode0 = buffer;
        drv_speed(speed0);
        gpio_set_level(driver_dir, dir0);
        // One Step
        gpio_set_level(step, mode0);
        for(int i=100; i>0; i--){};
        gpio_set_level(step, mode1);
        for(int i=100; i>0; i--){};
        vTaskDelayUntil(&Alastwaketime, 1);
    }
    vTaskDelete(NULL);
}

void Btask(void *pvParameters) {
    TickType_t Blastwaketime;
    Blastwaketime = xTaskGetTickCount();
    int DEF1 = 1;
    int DEF0 = 0;
    int mode0 = step_full;
    int mode1 = step_2;
    int mode2 = step_4;
    int mode3 = step_8;
    int mode4 = step_16;

    uint8_t rawdata[2] = {0, 0};
    uint8_t regaddr[1] = {0x0E};
    int angle = 0;
    int pos = 0;

    for(;;) {
        // printf(GREEN"This is BTask !\r\n");
        ESP_ERROR_CHECK(i2c_master_write_read_device(I2C_NUM_0, sensor_addr, regaddr, 1, rawdata, 2, 1));
        angle = (int)((((uint16_t)rawdata[0]<<8) | rawdata[1])*360.0/4096.0);
        xQueuePeek(step_queue, &pos, 0);
        xQueueSendToBack(step_queue, &pos, 0);
        xQueueSendToBack(step_queue, &angle, 0);

        // if(((pos + 360 - angle)%360 <= 5)) {
        //     xQueueSendToBack(motor_queue, &mode4, 0);
        // }
        // else if ((pos + 360 - angle)%360 <= 10)
        // {
        //     xQueueSendToBack(motor_queue, &mode3, 0);
        // }
        // else if ((pos + 360 - angle)%360 <= 15)
        // {
        //     xQueueSendToBack(motor_queue, &mode2, 0);
        // }
        // else if ((pos + 360 - angle)%360 <= 20)
        // {
        //     xQueueSendToBack(motor_queue, &mode1, 0);
        // }
        // else xQueueSendToBack(motor_queue, &mode0, 0);
        xQueueSendToBack(motor_queue, &mode0, 0);

        if((pos - angle + 360)%360 <= 1) {
            xQueueSendToBack(motor_queue, &DEF1, 0);
            drv_sleep();
            printf(YELLOW"%d\r\n", pos);
        }
        else {
            xQueueSendToBack(motor_queue, &DEF0, 0);
        }
        // printf(GREEN"Angle is %0.2f\r\n", angle);
        // for(int i=50000; i>0; i--){};
        vTaskDelayUntil(&Blastwaketime, 1);
    }
    vTaskDelete(NULL);
}

void Ctask(void *pvParameters) {
    TickType_t Clastwaketime;
    Clastwaketime = xTaskGetTickCount();
    long int pos = 0;
    int angle = 0;
    
    int dir1 = 1;
    int dir0 = 0;
    int buffer = 0;

    for(;;) {
        drv_wakeup();
        xQueueReceive(step_queue, &pos, 0);
        xQueueReceive(step_queue, &angle, 0);
        pos = random() % 360;
        xQueueSendToBack(step_queue, &pos, 0);
        xQueueSendToBack(step_queue, &angle, 0);

        xQueueReceive(dir_queue, &buffer, 0);
        if((pos + 360 - angle)%360 >= 180) {
            xQueueSendToBack(dir_queue, &dir1, 0);
        }
        else {
            xQueueSendToBack(dir_queue, &dir0, 0);
        }

        vTaskDelayUntil(&Clastwaketime, 1000);
    }
    vTaskDelete(NULL);
}