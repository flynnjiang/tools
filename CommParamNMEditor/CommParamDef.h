#ifndef TERMCOMPARAM_H
#define TERMCOMPARAM_H

/************************************************
 * 移植Linux相关定义
 ************************************************/
typedef unsigned int in_addr_t;
struct in_addr {
    in_addr_t s_addr;
};

typedef unsigned short in_port_t;



//-------------commparamcfg使用参数类型--------------

typedef struct
{
    char ucAPNName[32];    //apn
    char ucUserName[32];   //用户名
    char ucUserPasswd[32]; //用户密码
}APNPARAM;


typedef struct
{
    struct in_addr  stHostIP1;       //主站1地址
    in_port_t       iHostPort1;      //主站1端口
    struct in_addr  stHostIP2;       //主站2地址
    in_port_t       iHostPort2;      //主站2端口
    struct in_addr  stHostIP3;       //主站3地址
    in_port_t       iHostPort3;      //主站3端口
    struct in_addr  stMGateway;     //网关
    in_port_t       iGatewayPort;   //gateway port;
}GPRS_NETPARAM;


typedef struct
{
    int     iHeartBeat;			      //心跳周期
    int     iRedialInterval;          //重拨间隔时间（分钟）
    int     iRedialCnt;               //重拨次数

    // 预留的扩展参数
    int     ext1;                   
    int     ext2;
    int     ext3;
    int     ext4;
    int     ext5;
}EXT_PARAM;


typedef struct
{
	/* bParamValid:当前运营商特定制式下的参数是否有效
	该参数仅仅在未锁定模块网络制式进行自适应切换网络制式时使用
	界面指定某网络制式时参数的有效性由用户保证，程序不检查
    例如 stSysinfoParam[2][0].bParamValid == false，表示电信2G参数无效
    电信卡自适应拨号时将跳过2G
	*/
    bool           bParamValid;
    char _pack[3]; // 手动字节对齐
	APNPARAM       st_Dialfile;
	GPRS_NETPARAM  st_NetParam;
    //EXT_PARAM      st_extParam;
}SYSINFO_PARAM;

typedef struct
{   
    /* 制式，0,1,2,3,4分别表示模块自适应，锁定4/2/3G并循环，锁定2G,锁定3G,锁定4G，其它值为无效值 
    本值为全局 */
	int  iSetNetMode; 

    /* 自适应模式下：登录主站后是否锁定为登录时使用的制式。目前不使用 */
    bool lockAfterLogin;
    char _pack[3]; // 手动字节对齐

    
	/* SYSINFO_PARAM 数组。顺序：中国移动2,3,4G，中国联通2,3,4G，中国电信2,3,4G 
    例如stSysinfoParam[0][0]表示移动2G参数，
    [0][1]表示移动3G参数，[1][0]表示联通2G参数，以此类推*/
	SYSINFO_PARAM stSysinfoParam[3][3];
}COMMPARAMCFG;
//-------------END　commparamcfg使用参数类型--------------


#endif
