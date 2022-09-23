#ifndef MAIN_H
#define MAIN_H

#define sensor_out 0
#define sensor_dir 1       //低电平顺时针增加，高电平逆时针增加
#define i2c_scl    3
#define i2c_sda    4
#define nhome      6       //判断为最初位置时输出
#define mood2      7
#define mood1      8
#define mood0      9
#define step       10      //检测到上升沿触发一步
// 注意：gpio11 - gpio17 默认留给spi flash   P11 P12 P13 仍然可以通过IO MUX进行配置
#define nenbl      12      //低电平使能drv8824      
#define driver_dir 13      //方向控制
#define ndecay     15      //低电平慢衰减  高电平快衰减  浮空混合衰减 
#define nfault     15      //错误输出
#define nsleep     16      //低电平进入睡眠模式，高电平使能drv8824
#define nreset     17      //低电平复位
#define rxd        20
#define txd        21

#define uart0              0
#define uart_baudrate      115200
#define rxbuf              256
#define txbuf              256
#define uninverted         0
#define i2c_speed          1000000UL
#define i2c0               0

#define step_full          0
#define step_2             1
#define step_4             2
#define step_8             3
#define step_16            4
#define step_32            5

#define decay_fast         3
#define decay_mix          2
#define decay_slow         1

#define CCW                0
#define CW                 1

#define sensor_addr        0x36


// 字色
#define BLACK                "\e[0;30m"
#define L_BLACK              "\e[1;30m"
#define RED                  "\e[0;31m"
#define L_RED                "\e[1;31m"
#define GREEN                "\e[0;32m"
#define L_GREEN              "\e[1;32m"
#define BROWN                "\e[0;33m"
#define YELLOW               "\e[1;33m"
#define BLUE                 "\e[0;34m"
#define L_BLUE               "\e[1;34m"
#define PURPLE               "\e[0;35m"
#define L_PURPLE             "\e[1;35m"
#define CYAN                 "\e[0;36m"
#define L_CYAN               "\e[1;36m"
#define GRAY                 "\e[0;37m"
#define WHITE                "\e[1;37m"
 
#define BOLD                 "\e[1m"
#define UNDERLINE            "\e[4m"
#define BLINK                "\e[5m"
#define REVERSE              "\e[7m"
#define HIDE                 "\e[8m"
#define CLEAR                "\e[2J"
#define CLRLINE              "\r\e[K"






#endif