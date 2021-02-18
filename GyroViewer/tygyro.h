#ifndef TYGYRO_H
#define TYGYRO_H

#include <stdint.h>


#define MMC5983_DEV_NUM 2
#define ADIS_DEV_NUM 2

#define ADIS_GYRO_AXIS_NUM 3
#define ADIS_ACCL_AXIS_NUM 3
#define MMC5983_AXIS_NUM 3


#pragma pack(1)

struct __attribute__ ((__packed__)) gyro_tele_hdr {
    uint8_t ver;		/**< Data format version number */
    uint8_t req_cnt;	/**< Telemetry request count */
    uint8_t can_ok_cnt;	/**< CAN correct frame count */
    uint8_t can_err_cnt;	/**< CAN error frame count */
    uint32_t time;		/**< Timestamp */
    uint8_t csys;		/**< Coordinate System */
    uint8_t reserved;
};

struct __attribute__ ((__packed__)) gyro_tele_mag_data {
    int16_t data[MMC5983_AXIS_NUM];
    int16_t temp;		/**< Temperature, 100/LSB */
};

struct __attribute__ ((__packed__)) gyro_tele_gyro_data {
    int32_t gyro[ADIS_GYRO_AXIS_NUM]; /**< Gyro data */
    int32_t accl[ADIS_ACCL_AXIS_NUM]; /**< Accl data */
    int16_t temp;		/**< Temperature, 100/LSB */
};

struct __attribute__ ((__packed__)) gyro_tele_gyro_exdata {
    struct gyro_tele_gyro_data data; /**< Gyro data */
    uint8_t stat_reg;	/**< ADIS14770 DIAG_STAT register value */
    uint16_t time_reg;	/**< ADIS16470 TIME_STAMP register value */
    uint16_t data_cnt_reg;	/**< ADIS16470 DATA_CNTR register value */
};

/** Telemetry Resposne */
struct __attribute__ ((__packed__)) gyro_tele_rsp {
    struct gyro_tele_hdr hdr;
    struct gyro_tele_mag_data mag;
    struct gyro_tele_gyro_data gyro;
    uint8_t mag_stat[MMC5983_DEV_NUM];	/**< Mag sensor status */
    uint8_t gyro_stat[ADIS_DEV_NUM]; /**< Gyro sensor status */
};

/** Telemetry Response (Raw Data) */
struct __attribute__ ((__packed__)) gyro_tele_raw_rsp {
    struct gyro_tele_hdr hdr;
    struct gyro_tele_mag_data mag;
    struct gyro_tele_gyro_data gyro;
    uint8_t mag_stat[MMC5983_DEV_NUM];
    uint8_t gyro_stat[ADIS_DEV_NUM];
    struct gyro_tele_mag_data mag_raw[MMC5983_DEV_NUM];
    struct gyro_tele_gyro_exdata gyro_raw[ADIS_DEV_NUM];
};

#pragma pack(4)


#endif // TYGYRO_H
