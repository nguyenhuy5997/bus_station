/*
 * mp3_uart.h
 *
 *  Created on: Jun 27, 2024
 *      Author: chutc
 */

#ifndef MAIN_POSITION_MP3_UART_H_
#define MAIN_POSITION_MP3_UART_H_

#define ECHO_TEST_TXD (CONFIG_EXAMPLE_UART_TXD)
#define ECHO_TEST_RXD (CONFIG_EXAMPLE_UART_RXD)
#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

#define ECHO_UART_PORT_NUM      (CONFIG_EXAMPLE_UART_PORT_NUM)
#define ECHO_UART_BAUD_RATE     (CONFIG_EXAMPLE_UART_BAUD_RATE)
#define ECHO_TASK_STACK_SIZE    (CONFIG_EXAMPLE_TASK_STACK_SIZE)


#define BUF_SIZE (1024)

void init_uart_mp3();

void send_uart_nhac(char* message);

#endif /* MAIN_POSITION_MP3_UART_H_ */
