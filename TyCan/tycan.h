#ifndef TYCAN_H
#define TYCAN_H

#include <stdint.h>
#include <queue>
#include <QVector>
#include "ECanVci.h"

typedef union ty_can_id_un {
    uint32_t value;
    struct {
        uint32_t seq:8;
        uint32_t info:1;
        uint32_t type:4;
        uint32_t dest:8;
        uint32_t src:8;
        uint32_t padding:3;
    } ext;
} ty_can_id_t;


struct ty_can_frame {
    ty_can_id_t can_id;     /**< CAN ID */
    uint8_t     data_type;  /**< 数据类型 */
    uint8_t     data_id;    /**< 数据编号 */
    uint16_t    data_len;   /**< 数据长度，仅表示data部分 */
    uint8_t     data[1024]; /**< 数据内容 */
};


class TyCan {

public:
    TyCan();
    ~TyCan();

    bool openDev(int canDevType, int canDevId, int canChId);
    bool closeDev(void);
    bool resetDev(void);

    bool sendFrame(const struct ty_can_frame *f);
    bool recvFrame(struct ty_can_frame *f, int timeout_ms);
    int recvRawFrame(CAN_OBJ *objs, int obj_num, int timeout_ms);

private:
    bool GetFrameFromBuffer(struct ty_can_frame *f);

private:
    DWORD m_devType;
    DWORD m_devId;
    DWORD m_chId;
    uint8_t m_frameSeq;
    std::queue<CAN_OBJ> m_objQueue;
    QVector<CAN_OBJ> m_objs;

    BOARD_INFO m_info;

    bool  m_isOpen;
};

#endif // TYCAN_H
