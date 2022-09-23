idf_component_register( SRCS "main.c"
                        SRCS "./../driver/init/motor_init.c"
                        SRCS "./../driver/sensor/sensor_func.c"
                        
                        INCLUDE_DIRS "."
                        INCLUDE_DIRS "./../driver/init"
                        INCLUDE_DIRS "./../driver/sensor")
