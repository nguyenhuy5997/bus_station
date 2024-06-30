/*
 * config.h
 *
 *  Created on: Jun 26, 2024
 *      Author: chutc
 */

#ifndef MAIN_POSITION_CONFIG_H_
#define MAIN_POSITION_CONFIG_H_


#define SL_DD_WD 26U
#define SL_DD_WK 27U
#define SL_DD ((SL_DD_WD > SL_DD_WK) ? SL_DD_WD : SL_DD_WK)
#define SL_VT 1180
#define KC_MIN_GAN_DD 30 // gan diem dung 100m --> lay luon la diem dung hien tai
#define KHOANG_CACH_PHAT_NHAC_TOI_DA 50

//Vi do diem dung
float kd_dd_Arr[SL_DD];
float vd_dd_Arr[SL_DD];


//WEEKEND
float kd_dd_Arr_wk[SL_DD] = {
  21.02362849	,//Diem dau tien trong mang nay khong nam trong danh sach diem dung, no sat voi lo trinh va gan diem dau tien trong lotrinh
  21.02394896	,
  21.02496667	,
  21.02696667	,
  21.02771667	,
  21.03891667	,
  21.04213333	,
  21.04941667	,
  21.04716667	,
  21.04363333	,
  21.04363333	,
  21.0448	,
  21.05223333	,
  21.05543333	,
  21.0676	,
  21.06985	,
  21.0592	,
  21.04353333	,
  21.0414	,
  21.03836667	,
  21.034	,
  21.02788333	,
  21.03031667	,
  21.02591667	,
  21.02451667	,
  21.0228	,
  21.0228173	,//Diem cuoi cung trong mang nay khong nam trong danh sach diem dung, no sat voi lo trinh va gan diem cuoi trong lotrinh
};
float vd_dd_Arr_wk[SL_DD] = {
  105.8572358	,//Diem dau tien trong mang nay khong nam trong danh sach diem dung, no sat voi lo trinh va gan diem dau tien trong lotrinh
  105.8572412	,
  105.85675	,
  105.8561333	,
  105.8561167	,
  105.8520167	,
  105.8488667	,
  105.83885	,
  105.8369333	,
  105.8347833	,
  105.833	,
  105.81735	,
  105.8126833	,
  105.8114833	,
  105.8117333	,
  105.8132333	,
  105.8340667	,
  105.8360333	,
  105.83615	,
  105.8396	,
  105.8349	,
  105.8343167	,
  105.8362833	,
  105.8468333	,
  105.8464833	,
  105.8564833	,
  105.8574718	,//Diem cuoi cung trong mang nay khong nam trong danh sach diem dung, no sat voi lo trinh va gan diem cuoi trong lotrinh
};

///WEEK DAY

float kd_dd_Arr_wd[SL_DD] = {
  21.03185029	,
  21.0318	,
  21.03383333	,
  21.03693333	,
  21.04061667	,
  21.04251667	,
  21.04711667	,
  21.04365	,
  21.0437	,
  21.04485	,
  21.0524	,
  21.05555	,
  21.06763333	,
  21.07033333	,
  21.05911667	,
  21.04365	,
  21.0415	,
  21.03836667	,
  21.03401667	,
  21.02791667	,
  21.02791667	,
  21.0301	,
  21.02591667	,
  21.02395	,
  21.03045	,
  21.03084888	,
};
float vd_dd_Arr_wd[SL_DD] = {
  105.8525902	,
  105.85195	,
  105.8507833	,
  105.8494333	,
  105.8500167	,
  105.8485167	,
  105.8368833	,
  105.8346167	,
  105.8327667	,
  105.8171833	,
  105.8126167	,
  105.8112333	,
  105.8117833	,
  105.8134833	,
  105.8341333	,
  105.8360833	,
  105.83615	,
  105.8396167	,
  105.8348667	,
  105.83435	,
  105.83435	,
  105.8370833	,
  105.8468333	,
  105.8568833	,
  105.8538333	,
  105.8536041	,
};





#endif /* MAIN_POSITION_CONFIG_H_ */
