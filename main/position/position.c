/*
 * position.c
 *
 *  Created on: Jun 26, 2024
 *      Author: chutc
 */
#include <math.h>
#include <string.h>
#include "freertos/ringbuf.h"
#include "esp_log.h"
#include "config.h"
#include "nmea/nmea_parser.h"
#include "position.h"
#include "mqtt/mqtt.h"
#include "mp3_uart.h"
#include "LED/gpio.h"

static const char *TAG = "POSITION";
RingbufHandle_t gps_buf_handle;
extern esp_mqtt_client_handle_t client_emqx;
extern RingbufHandle_t client_thingsboard;
//
#define WEEKEND 1
#define WEEKDAY 2
uint8_t DAY_TIME = WEEKEND;
#define WEEK_TIME_PIN 2
#define WEEKEND_LED_PIN 10
#define WEEKDAY_LED_PIN 12
#define R_TRAI_DAT 6371000
#define COUNTER_SAI_DIEM_DUNG 50

const int8_t NGOAI_TUYEN = 1, TRONG_TUYEN = 2;

static int8_t mode = NGOAI_TUYEN;
int16_t counter_sai_diem_dung = 0;
static float kd = 0.000000, vd = 0.0000000;
static int8_t diem_dung_ht = 0;
static int8_t diem_dung_td = 0;
static uint8_t diem_dung_sai_ht = 0;
static uint8_t diem_dung_sai_td = 0;
volatile int mode_weektime_before = 0;
volatile int mode_weektime = 1;

static double d_bus_ponits_Arr[SL_DD];

double k_c = 5000;
//

void init_weekday_weekend() {
	if (DAY_TIME == WEEKEND) {
		for (int i = 0; i < SL_DD; i++) {
			kd_dd_Arr[i] = kd_dd_Arr_wk[i];
		}
		for (int i = 0; i < SL_DD; i++) {
			vd_dd_Arr[i] = vd_dd_Arr_wk[i];
		}
	}
	if (DAY_TIME == WEEKDAY) {
		for (int i = 0; i < SL_DD; i++) {
			kd_dd_Arr[i] = kd_dd_Arr_wd[i];
		}
		for (int i = 0; i < SL_DD; i++) {
			vd_dd_Arr[i] = vd_dd_Arr_wd[i];
		}
	}
}

void reset_data() {
	diem_dung_ht = 0;
	diem_dung_td = 0;
	debug();
}
void toi_cuoi_reset() {
	reset_data();
}

void ngoai_tuyen() {
	if (mode == NGOAI_TUYEN) {
		ESP_LOGI(TAG, "*************Ngoai tuyen*************");
	}
	ESP_LOGI(TAG, "d_bus_ponits_Arr[diem_dung_ht]: %.6f",
			d_bus_ponits_Arr[diem_dung_ht]);
	// diem_dung_td = diem_dung_ht;
	if (d_bus_ponits_Arr[diem_dung_ht] > k_c)
		mode = NGOAI_TUYEN;
	else
		mode = TRONG_TUYEN;
}

void trong_tuyen() {
	tim_diem_dung();
}

double tinh_khoang_cach(float k1, float k2, float v1, float v2) {
	double dLat = (k2 - k1) * (M_PI / 180);
	double dLon = (v2 - v1) * (M_PI / 180);
	double la1ToRad = k1 * (M_PI / 180);
	double la2ToRad = k2 * (M_PI / 180);
	double a = sin(dLat / 2) * sin(dLat / 2)
			+ cos(la1ToRad) * cos(la2ToRad) * sin(dLon / 2) * sin(dLon / 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	double d = R_TRAI_DAT * c;
	return d;
}

void sai_diem_dung(uint8_t _diem_dung_bi_sai) {
	diem_dung_sai_td = diem_dung_sai_ht;
	diem_dung_sai_ht = _diem_dung_bi_sai;
	if (diem_dung_sai_ht == diem_dung_sai_td + 1) {
		diem_dung_ht = diem_dung_sai_ht;
		//diem_dung_td = diem_dung_td; // clgt ????????
	}
}

#define HIEU_SO_CHENH_LECH_KHUC_GIAO_DI 5  //Khuc nay chenh lech giua 7 8 va 15 16 --> lay 5
#define HIEU_SO_CHENH_LECH_KHUC_GIAO_VE 0
void tim_diem_dung() {
	uint8_t so_luong_diem_dung_trong_tuyen;
	if (DAY_TIME == WEEKEND) {
		so_luong_diem_dung_trong_tuyen = SL_DD_WK;
	}
	if (DAY_TIME == WEEKDAY) {
		so_luong_diem_dung_trong_tuyen = SL_DD_WD;
	}
	for (uint8_t i = 0; i < so_luong_diem_dung_trong_tuyen; i++) {
		d_bus_ponits_Arr[i] = tinh_khoang_cach(kd_dd_Arr[i], kd, vd_dd_Arr[i],
				vd);
	}

	for (uint8_t j = 0; j < so_luong_diem_dung_trong_tuyen; j++) {
		//Gan diem dung 100m --> lay luon la diem dung hien tai
		if (d_bus_ponits_Arr[j] <= KC_MIN_GAN_DD) {
			//14/11: neu diem dung ht > j ==> ignore j.

			//Set  1 diem cuoi, trung voi diem dau, khi toi diem nay thi reset ve 0. diem nay khong co trong toa do nhe.
			if (j == SL_DD - 1) {
				toi_cuoi_reset();
				j = 0;
			}
			if (diem_dung_ht > j) {
				ESP_LOGI(TAG, "Diem dung ht dang bi sai lon hon j tren: %d",
						diem_dung_ht);
				ESP_LOGI(TAG, "J: %d", j);
				continue;
			}
			if (j == 0) {
				diem_dung_ht = 0;
				diem_dung_td = 0;
			} else {
				if (j - diem_dung_ht > HIEU_SO_CHENH_LECH_KHUC_GIAO_DI) {
					continue;
				}
				diem_dung_td = diem_dung_ht;
				diem_dung_ht = j;
			}
			//sau khi da co diem dung --> break luon vong lap.
			break;
		}
		//Neu khong gan diem dung --> compare xem khoang cach tu xe toi diem dung hien tai va tu xe toi cac diem dung khac the nao
		//Neu nho hon toi diem dung ht ==> check xem co phai diem dung tiep theo trong vector khong, neu khong thi tam thoi bo qua de check cho den khi phat hien diem dung tiep theo
		if (d_bus_ponits_Arr[j] < d_bus_ponits_Arr[diem_dung_ht]) {
			if (j == diem_dung_ht + 1) {  //td
			//j = diem dung truoc do + 1 ==>  diem dung ngon
				diem_dung_td = diem_dung_ht;
				diem_dung_ht = j;
				//reset diem dung sai:
				diem_dung_sai_ht = 0;
				diem_dung_sai_td = 0;
			} else {
				if (diem_dung_ht >= j) {
					if (diem_dung_sai_ht == diem_dung_ht + 1) {
						diem_dung_td = diem_dung_ht;
						diem_dung_ht = diem_dung_sai_ht;
					}
					ESP_LOGI(TAG, "Diem dung sai ht: %d", diem_dung_sai_ht);
					ESP_LOGI(TAG, "Diem dung ht dang bi sai lon hon j duoi: %d",
							diem_dung_ht);
					ESP_LOGI(TAG, "J: %d", j);
					if (counter_sai_diem_dung < COUNTER_SAI_DIEM_DUNG) {
						counter_sai_diem_dung++;
						if (counter_sai_diem_dung == COUNTER_SAI_DIEM_DUNG) {
							counter_sai_diem_dung = 0;
							reset_data();
						}
					}
					continue;
				}
				//Diem dung khong theo thu tu vector conflig khoang cach
				//sai diem dung
				ESP_LOGI(TAG, "Sai diem dung");
				uint8_t diem_dung_bi_sai = j;
				//                diem_dung_ht = j;
				sai_diem_dung(diem_dung_bi_sai);
			}
		}
	}
}

void check_and_play() {
	//Check neu xe cach diem dung ht < 30m --> phat nhac
	if (d_bus_ponits_Arr[diem_dung_ht] < KHOANG_CACH_PHAT_NHAC_TOI_DA) {
		if (diem_dung_ht == 0) {
			//ignore
			// show_inforln("001nhungkphat");
		} else {
			ESP_LOGI(TAG, "Khoang cach toi diem hien tai: %f",
					d_bus_ponits_Arr[diem_dung_ht]);
			ESP_LOGI(TAG, "Phat nhac: Bai hat so: %d", diem_dung_ht);

			char str_ht[10];
			sprintf(str_ht, "%02d", diem_dung_ht);
			if (DAY_TIME == WEEKEND) {
				char message[20];
				sprintf(message, "WK0%s", str_ht);
				send_uart_nhac(message);
				ESP_LOGW(TAG, "Phat nhac: %s", message);
			} else {
				char message[20];
				sprintf(message, "WD0%s", str_ht);
				send_uart_nhac(message);
				ESP_LOGW(TAG, "Phat nhac: %s", message);
			}
		}
	}
}
void debug() {
	ESP_LOGI(TAG,
			"****************************************************************************************************");
	ESP_LOGI(TAG, "mode hien tai: %d", mode);

	ESP_LOGI(TAG, "diem dung hien tai: %d", diem_dung_ht);

	ESP_LOGI(TAG, "toa do ht: %.6f     %.6f", kd, vd);

	ESP_LOGI(TAG, "'d' toi diem dung ht: %.6f", d_bus_ponits_Arr[diem_dung_ht]);

	ESP_LOGI(TAG, "Toa do diem dung ht   : %.6f, %.6f", kd_dd_Arr[diem_dung_ht],
			vd_dd_Arr[diem_dung_ht]);

	ESP_LOGI(TAG, "diem dung tt :  %d", diem_dung_ht + 1);

	ESP_LOGI(TAG, "Toa Do diem dung tt  : %.6f, %.6f",
			kd_dd_Arr[diem_dung_ht + 1], vd_dd_Arr[diem_dung_ht + 1]);

	ESP_LOGI(TAG, "diem dung td :  %d", diem_dung_td);

	ESP_LOGI(TAG, "Toa do diem dung td   : %.6f, %.6f", kd_dd_Arr[diem_dung_td],
			vd_dd_Arr[diem_dung_td]);

	ESP_LOGI(TAG, "'d' toi diem dung  td  :  %.8f",
			d_bus_ponits_Arr[diem_dung_td]);
}
void bus_router(void *pvParameters) {
	gps_t *gps_item = NULL;
	size_t item_size;
	init_weekday_weekend();
	configure_led();
	while (1) {
		gps_item = (gps_t*) xRingbufferReceiveFromISR(gps_buf_handle,
				&item_size);
		if ((item_size > 0) && gps_item && gps_item->latitude > 0) {
			kd = gps_item->latitude;
			vd = gps_item->longitude;
//	        ESP_LOGI(TAG, "%d/%d/%d %d:%d:%d => \r\n"
//	                 "\t\t\t\t\t\tlatitude   = %.05f\r\n"
//	                 "\t\t\t\t\t\tlongitude = %.05f\r\n"
//	                 "\t\t\t\t\t\taltitude   = %.02fm\r\n"
//	                 "\t\t\t\t\t\tspeed      = %fm/s",
//					 gps_item->date.year + YEAR_BASE, gps_item->date.month, gps_item->date.day,
//					 gps_item->tim.hour + TIME_ZONE, gps_item->tim.minute, gps_item->tim.second,
//					 gps_item->latitude, gps_item->longitude, gps_item->altitude, gps_item->speed);
			ESP_LOGI(TAG, "mode hien tai: %d", mode);
			ngoai_tuyen();
			ESP_LOGI(TAG, "mode hien tai: %d", mode);
			switch (mode) {
			case NGOAI_TUYEN: {
				ngoai_tuyen();
				break;
			}
			case TRONG_TUYEN: {
				trong_tuyen();
				break;
			}
			default:
				break;
			}
			if (d_bus_ponits_Arr[diem_dung_ht] < KHOANG_CACH_PHAT_NHAC_TOI_DA) {
				char data_mqtt[500];
				sprintf(data_mqtt,
						"{\"latitude\":%.6f,\"longitude\":%.6f,\"speed\":%.2f,\"time\":\"%d/%d/%d %d:%d:%d\",\"nxt_pnt\":{\"lat\":%.6f,\"long\":%.6f},\"near\":1, \"next\":%d}",
						kd, vd, gps_item->speed * 3.6,
						gps_item->date.year + YEAR_BASE, gps_item->date.month,
						gps_item->date.day, gps_item->tim.hour + TIME_ZONE,
						gps_item->tim.minute, gps_item->tim.second,
						kd_dd_Arr[diem_dung_ht + 1],
						vd_dd_Arr[diem_dung_ht + 1], diem_dung_ht + 1);
				esp_mqtt_client_publish(client_thingsboard,
						"v1/devices/me/telemetry", data_mqtt, 0, 0, 0);
				vTaskDelay(pdMS_TO_TICKS(500));
				memset(data_mqtt, 0, sizeof(data_mqtt));
				sprintf(data_mqtt,
						"{\"cur_pos\":{\"lat\":%.6f,\"long\":%.6f},\"nxt_pnt\":{\"lat\":%.6f,\"long\":%.6f},\"near\":1}",
						kd, vd, kd_dd_Arr[diem_dung_ht + 1],
						vd_dd_Arr[diem_dung_ht + 1]);
				esp_mqtt_client_publish(client_emqx, "bus/position", data_mqtt,
						0, 0, 0);
				gpio_set_level(BLINK_GPIO, 1);
			} else {
				char data_mqtt[500];
				sprintf(data_mqtt,
						"{\"latitude\":%.6f,\"longitude\":%.6f,\"speed\":%.2f,\"time\":\"%d/%d/%d %d:%d:%d\",\"nxt_pnt\":{\"lat\":%.6f,\"long\":%.6f},\"near\":0, \"next\":%d}",
						kd, vd, gps_item->speed * 3.6,
						gps_item->date.year + YEAR_BASE, gps_item->date.month,
						gps_item->date.day, gps_item->tim.hour + TIME_ZONE,
						gps_item->tim.minute, gps_item->tim.second,
						kd_dd_Arr[diem_dung_ht + 1],
						vd_dd_Arr[diem_dung_ht + 1], diem_dung_ht + 1);
				esp_mqtt_client_publish(client_thingsboard,
						"v1/devices/me/telemetry", data_mqtt, 0, 0, 0);
				vTaskDelay(pdMS_TO_TICKS(500));
				memset(data_mqtt, 0, sizeof(data_mqtt));
				sprintf(data_mqtt,
						"{\"cur_pos\":{\"lat\":%.6f,\"long\":%.6f},\"nxt_pnt\":{\"lat\":%.6f,\"long\":%.6f},\"near\":0}",
						kd, vd, kd_dd_Arr[diem_dung_ht + 1],
						vd_dd_Arr[diem_dung_ht + 1]);
				esp_mqtt_client_publish(client_emqx, "bus/position", data_mqtt,
						0, 0, 0);
				gpio_set_level(BLINK_GPIO, 0);
			}
			debug();
			vRingbufferReturnItem(gps_buf_handle, (void*) gps_item);
			check_and_play();
		}
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}
