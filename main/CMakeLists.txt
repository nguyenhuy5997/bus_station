idf_component_register(SRCS "main.c"
							"nmea/nmea_parser.c"
							"wifi/wifi.c"
							"mqtt/mqtt.c"
							"position/position.c"
							"position/mp3_uart.c"
							"LED/gpio.c"
                    INCLUDE_DIRS ".")
