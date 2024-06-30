/*
 * position.h
 *
 *  Created on: Jun 26, 2024
 *      Author: chutc
 */

#ifndef MAIN_POSITION_POSITION_H_
#define MAIN_POSITION_POSITION_H_


void bus_router(void *pvParameters);
void tim_diem_dung();
void sai_diem_dung(uint8_t _diem_dung_bi_sai);
double tinh_khoang_cach(float k1, float k2, float v1, float v2);
void debug();
void trong_tuyen();
void check_and_play();
void init_weekday_weekend();
void toi_cuoi_reset();
void reset_data();
void check_time();  //thu 7 chu nhat hay cac ngay trong tuan
void send_uart_nhac(char* message);
#endif /* MAIN_POSITION_POSITION_H_ */
