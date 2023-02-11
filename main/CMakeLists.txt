idf_component_register(SRCS "main.c" 
                            "adc.c" 
                            "joystick.c"
                            "temperature_analogic.c"
                            "temperature_dht11.c" 
                            "motor.c"
                            "states.h"
                    INCLUDE_DIRS "."
                    REQUIRES "esp32-DHT11" "esp_adc")
