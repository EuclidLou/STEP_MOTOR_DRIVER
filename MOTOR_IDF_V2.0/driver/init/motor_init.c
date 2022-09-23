#include "motor_init.h"

void motor_uart_init() {
    uart_config_t uart_config = {
        .baud_rate = 115200,			        //波特率
        .data_bits = UART_DATA_8_BITS,	        //数据位
        .parity = UART_PARITY_DISABLE,	        //奇偶校验
        .stop_bits = UART_STOP_BITS_1,	        //停止位
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,	//流控
        .rx_flow_ctrl_thresh = 122,		        //硬件RTS阈值
        .source_clk = UART_SCLK_DEFAULT,
    };
    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, GPIO_NUM_21, GPIO_NUM_20, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    const int uart_buffer_size = (1024 * 2);
    QueueHandle_t uart_queue;
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0));

    // char* test_str = GREEN"This is a test string.\r\n";
    // for(;;) {
    //     uart_write_bytes(UART_NUM_0, (const char*)test_str, strlen(test_str));
    //     vTaskDelay(2000 / portTICK_PERIOD_MS);
    // }
}

void motor_i2c_init() {
    i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = i2c_sda,         // select GPIO specific to your project
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = i2c_scl,         // select GPIO specific to your project
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = i2c_speed,  // select frequency specific to your project
        // .clk_flags = 0,          /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &i2c_conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));
}

void Delay(int delayms) {
    vTaskDelay(delayms / portTICK_PERIOD_MS);
}

void motor_gpio_init() {
    gpio_config_t io_conf = {
        //disable interrupt
        .intr_type = GPIO_INTR_DISABLE,
        //set as output mode
        .mode = GPIO_MODE_INPUT,
        //bit mask of the pins that you want to set,e.g.21
        .pin_bit_mask = 1ULL << nhome,
        //disable pull-down mode
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        //disable pull-up mode
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };   
    //configure GPIO with the given settings
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = 1ULL << sensor_out;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;  
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << mood0) | (1ULL << mood1) | (1ULL << mood2) | \
                            (1ULL << step) | (1ULL << sensor_dir) | (1ULL << nenbl) | (1ULL << driver_dir);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;  
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    // io_conf.intr_type = GPIO_INTR_DISABLE;
    // io_conf.mode = GPIO_MODE_OUTPUT_OD;
    // io_conf.pin_bit_mask =  (1ULL << 11);
    // io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    // io_conf.pull_up_en = GPIO_PULLUP_DISABLE;  
    // ESP_ERROR_CHECK(gpio_config(&io_conf));
    // gpio_set_level(11, 1);d
}

void drv_wakeup() {
    gpio_pullup_en(nsleep);
    gpio_pulldown_dis(nsleep);
}

void drv_sleep() {
    gpio_pullup_dis(nsleep);
    gpio_pulldown_en(nsleep);
}

void drv_reset() {
    gpio_pullup_dis(nreset);
    gpio_pulldown_en(nreset);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    gpio_pullup_en(nreset);
    gpio_pulldown_dis(nreset);
}

void drv_slow_decay() {
    gpio_pullup_dis(ndecay);
    gpio_pulldown_en(ndecay);
}

void drv_mix_decay() {
    gpio_pullup_dis(ndecay);
    gpio_pulldown_dis(ndecay);
}

void drv_fast_decay() {
    gpio_pullup_en(ndecay);
    gpio_pulldown_dis(ndecay);
}

void drv_init() {
    gpio_pullup_en(nreset);
    gpio_pulldown_dis(nreset);
    gpio_set_level(nenbl, 0);
}

void drv_speed(int step_mode) {
    gpio_set_level(mood0, step_mode & 0x01);
    gpio_set_level(mood1, (step_mode >> 1) & 0x01);
    gpio_set_level(mood2, (step_mode >> 2) & 0x01);
}

void drv_step() {
    gpio_set_level(step, 0);
    for(int i=100; i>0; i--){};
    gpio_set_level(step, 1);
    for(int i=100; i>0; i--){};
}