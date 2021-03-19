#include <string.h>
#include <stdio.h>
#include <QtEndian>

#include "tycan.h"

TyCan::TyCan()
{
    m_devType = 0;
    m_devId = 0;
    m_chId = 0;

    memset(&m_info, 0, sizeof(m_info));

    m_isOpen = false;
}

TyCan::~TyCan()
{
    if (m_isOpen) {
        closeDev();
        m_isOpen = false;
    }
}


bool TyCan::openDev(int devType, int devId, int chId)
{
    DWORD retVal = 0;
    INIT_CONFIG cfg;

    if (m_isOpen) {
        return true;
    }

    m_devType = devType;
    m_devId = devId;
    m_chId = chId;

    retVal = OpenDevice(m_devType, m_devId, m_chId);
    if (1 != retVal) {
        qDebug("OpenDevice error");
        return false;
    }

    memset(&cfg, 0, sizeof(cfg));
    cfg.AccCode = 0;
    cfg.AccMask = 0xFFFFFF;
    cfg.Filter = 0;
    cfg.Timing0 = 0;
    cfg.Timing1 = 0x1C;
    cfg.Mode = 0;
    retVal = InitCAN(m_devType, m_devId, m_chId, &cfg);
    if (1 != retVal) {
        qDebug("初始化CAN适配器失败！");
        return false;
    }

    memset(&m_info, 0, sizeof(m_info));
    retVal = ReadBoardInfo(m_devType, m_devId, &m_info);
    if (! retVal) {
        qDebug("读取CAN适配器信息失败！");
        return false;
    }

    retVal = StartCAN(m_devType, m_devId, m_chId);
    if (! retVal) {
        qDebug("启动CAN通道失败！");
        return false;
    }

    m_isOpen = true;

    qDebug("CAN启动成功!");

    return true;
}

bool TyCan::closeDev(void)
{
    DWORD retVal = 0;

    if (m_isOpen) {
        retVal = CloseDevice(m_devType, m_devId);
        if (0 == retVal) {
            return false;
        }
    }

    m_isOpen = false;

    return true;
}

bool TyCan::resetDev(void)
{
    DWORD retVal = 0;

    if (! m_isOpen)
        return false;

    retVal = ResetCAN(m_devType, m_devId, m_chId);
    if (0 == retVal) {
        return false;
    }

    return true;
}


bool TyCan::sendFrame(const struct ty_can_frame *f)
{
    int i = 0;
    ULONG retVal = 0;
    CAN_OBJ obj;
    ty_can_id_t can_id;
    uint16_t len;
    uint16_t tx_bytes = 0;
    uint8_t crc = 0;


    if (f->data_len > 6) { /* 复合帧 */

        /* 发送首帧 */
        can_id.value = f->can_id.value;
        can_id.ext.src = 0;
        can_id.ext.info = 0;
        can_id.ext.seq = m_frameSeq;
        can_id.ext.type = 2;

        memset(&obj, 0, sizeof(obj));
        obj.ID = can_id.value;
        obj.SendType = 0;
        obj.RemoteFlag = 0;
        obj.ExternFlag = 1;
        obj.DataLen = 8;

        len = (uint16_t)qToBigEndian((uint16_t)(f->data_len + 2));
        memcpy(obj.Data, &len, sizeof(len));

        obj.Data[2] = f->data_type;
        obj.Data[3] = f->data_id;

        memcpy(&obj.Data[4], f->data, 4);
        tx_bytes = 4;

        Transmit(m_devType, m_devId, m_chId, &obj, 1);


        /* 发送中间帧 */
        while ((f->data_len - tx_bytes) >= 8) {

            can_id.value = f->can_id.value;
            //can_id.ext.src = 0;
            can_id.ext.info = 0;
            can_id.ext.seq = m_frameSeq;
            can_id.ext.type = 3;

            memset(&obj, 0, sizeof(obj));
            obj.ID = can_id.value;
            obj.SendType = 0;
            obj.RemoteFlag = 0;
            obj.ExternFlag = 1;
            obj.DataLen = 8;

            memcpy(obj.Data, f->data + tx_bytes, 8);
            tx_bytes += 8;

            Transmit(m_devType, m_devId, m_chId, &obj, 1);
        }


        /* 发送尾帧 */
        can_id.value = f->can_id.value;
        //can_id.ext.src = 0;
        can_id.ext.info = 0;
        can_id.ext.seq = m_frameSeq;
        can_id.ext.type = 3;

        memset(&obj, 0, sizeof(obj));
        obj.ID = can_id.value;
        obj.SendType = 0;
        obj.RemoteFlag = 0;
        obj.ExternFlag = 1;
        obj.DataLen = f->data_len - tx_bytes;
        memcpy(obj.Data, f->data + tx_bytes, obj.DataLen);
        tx_bytes += obj.DataLen;

        /* CRC */
        crc = (len & 0xFF);
        crc += ((len >> 8) & 0xFF);
        crc += f->data_type;
        crc += f->data_id;
        for (i = 0; i < f->data_len; i++) {
            crc += f->data[i];
        }
        obj.Data[obj.DataLen] = crc;
        obj.DataLen++;

        Transmit(m_devType, m_devId, m_chId, &obj, 1);

        m_frameSeq++;


        return true;

    } else { /* 单帧 */
        memset(&obj, 0, sizeof(obj));

        can_id.value = f->can_id.value;
        //can_id.ext.src = 0;
        can_id.ext.info = 0;
        can_id.ext.seq = m_frameSeq++;
        can_id.ext.type = 1;

        obj.ID = can_id.value;
        obj.SendType = 0;
        obj.RemoteFlag = 0;
        obj.ExternFlag = 1;
        obj.DataLen = 8;

        memset(obj.Data, 0, sizeof(obj.Data));
        obj.Data[0] = f->data_type;
        obj.Data[1] = f->data_id;
        memcpy(&obj.Data[2], f->data, f->data_len);

        retVal = Transmit(m_devType, m_devId, m_chId, &obj, 1);
        if (1 == retVal) {
            qDebug("send OK\n");
            return true;
        }
    }

    qDebug("send error\n");

    return false;
}

bool TyCan::GetFrameFromBuffer(struct ty_can_frame *f)
{
    int i = 0;
    CAN_OBJ obj;
    ty_can_id_t can_id;
    struct ty_can_frame frame;
    uint16_t longFrameSize = 0;
    bool hasLongFrame = false;

    memset(&frame, 0, sizeof(frame));

    for (i = 0; i < m_objs.size(); i++) {
        obj = m_objs[i];

        can_id.value = obj.ID;
        switch (can_id.ext.type) {

        case 2: // 复合帧的起始帧
            if (hasLongFrame) {
                qDebug("May Drop some frames");
            }

            hasLongFrame = true;
            longFrameSize = qFromBigEndian(*((uint16_t *)(obj.Data))) - 1;

            memset(&frame, 0, sizeof(frame));
            frame.can_id.value = obj.ID;
            frame.data_type = obj.Data[2];
            frame.data_id = obj.Data[3];
            memcpy(frame.data, &obj.Data[4], 4);
            frame.data_len += 4;

            break;

        case 3: // 复合帧的中间帧
            if (hasLongFrame) {
                memcpy(frame.data + frame.data_len, obj.Data, obj.DataLen);
                frame.data_len += obj.DataLen;

                /* FIXME:未检查CRC */
                if (frame.data_len == longFrameSize) {
                    frame.data_len--;
                    memcpy(f, &frame, sizeof(frame));
                    hasLongFrame =false;
                    longFrameSize = 0;
                    m_objs.remove(0, i + 1); // 从缓冲区移除已处理的帧
                    return true;
                } else if (frame.data_len > longFrameSize) {
                    // 错误帧需要丢弃
                    qDebug("Drop frame, frame.data_len = %u, longFrameSize = %u", frame.data_len, longFrameSize);
                    hasLongFrame = false;
                    longFrameSize = 0;
                    m_objs.remove(0, i + 1); // 从缓冲区移除已处理的帧
                    i = 0; // 然后重头开始查找
                }
            }
            break;

        case 1: // 单帧
        case 0: // 恢复帧
        case 4: // 时间同步帧
        default:
            break;
        }
    }

    return false;
}

bool TyCan::recvFrame(struct ty_can_frame *f, int timeout_ms)
{
    int i = 0;
    DWORD retVal = 0;
    CAN_OBJ objs[100];
    ty_can_id_t can_id;

    //if (GetFrameFromBuffer(f)) {
    //    return true;
    //}

    memset(objs, 0, sizeof(objs));
    retVal = Receive(m_devType, m_devId, m_chId, objs, 100, timeout_ms);
    if (0 == retVal || 0xFFFFFFFF == retVal) {
        return false;
    }

    //qDebug("Receive: %d", retVal);

    for (i = 0; i < retVal; i++) {
        can_id.value = objs[i].ID;
        if (0x06 == can_id.ext.src) {
            m_objs.push_back(objs[i]);
        }
    }

    return GetFrameFromBuffer(f);
}


int TyCan::recvRawFrame(CAN_OBJ *objs, int obj_num, int timeout_ms)
{
    DWORD retVal = 0;

    retVal = Receive(m_devType, m_devId, m_chId, objs, obj_num, timeout_ms);
    if (0 == retVal || 0xFFFFFFFF == retVal) {
        return 0;
    }

    return retVal;
}
