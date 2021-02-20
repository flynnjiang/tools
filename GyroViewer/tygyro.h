#ifndef TYGYRO_H
#define TYGYRO_H

#include <stdint.h>


#define MMC5983_DEV_NUM 2
#define ADIS_DEV_NUM 2

#define ADIS_GYRO_AXIS_NUM 3
#define ADIS_ACCL_AXIS_NUM 3
#define MMC5983_AXIS_NUM 3

/********************************************************************
 * 遥测数据包定义
 ********************************************************************/

#pragma pack(1)

struct __attribute__ ((__packed__)) ty_gyr_gyr_data {
    int32_t gyr[3];	/**< 陀螺仪数据，单位：0.1/65536 deg/s */
    int32_t acc[3]; /**< 加速度计数据，单位：1.25/65536 mg */
    int16_t temp;	/**< 温度，单位：0.01摄氏度 */
};

struct __attribute__ ((__packed__)) ty_gyr_mag_data {
    int32_t data[3];/**< 磁强计数据，单位：0.0625 mG */
    int16_t temp;	/**< 温度，单位：0.01摄氏度 */
};

struct __attribute__ ((__packed__)) ty_gyr_tele_data {
    uint8_t req_cnt;	/**< 遥测请求计数 */
    uint8_t can_ok_cnt;	/**< CAN总线正确帧计数 */
    uint8_t can_err_cnt;	/**< CAN总线错误帧计数 */
    uint8_t lastest_cmd;	/**< 最近执行指令码 */
    uint32_t time;		/**< 遥测时间戳 */
    uint8_t mag_stat[2];	/**< 磁强计状态，1-有效，0-无效 */
    uint8_t gyr_stat[2];	/**< 陀螺仪状态，1-有效，0-无效 */
    struct ty_gyr_mag_data mag[2];	/**< 磁强计数据 */
    struct ty_gyr_gyr_data gyr[2];	/**< 陀螺仪数据 */
    uint32_t param32[3];		/**< 可选参数1，2，3 */
    uint16_t param16;		/**< 可选参数4 */
    uint8_t  param_id;		/**< 可选参数标识 */
};

#pragma pack(4)


#endif // TYGYRO_H
