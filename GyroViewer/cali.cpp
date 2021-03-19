#include <math.h>
#include <stdio.h>
#define ACSGM_MATRIC_MAX_DIM					12
#define ACS_DOUBLEZERO							1.0e-20
#define ACS_GYRO_SAMP_P							0.1	//陀螺仪的采样周期


/********************************************
函数名称: ACSGM_MatCpy
功能描述: 矩阵拷贝
输入参数: pdbMatSrc矩阵
		  nRowLen矩阵行数
		  nColLen矩阵列数
输出参数:pdbMatDes拷贝目标矩阵
返回值　: 无
*********************************************/
void ACSGM_MatCpy(double* pdbMatDes, const double* pdbMatSrc, unsigned int unRowLen, unsigned int unColLen)
{
	unsigned int i;
	for (i = 0; i < unRowLen * unColLen; i++)
	{

		pdbMatDes[i] = pdbMatSrc[i];
	}
	return;
}
/********************************************
函数名称: 向量拷贝
函数功能: 进行向量值的复制
输入参数: cpdbVecSor: 要进行拷贝的向量;
		  unL: 向量长度;
输出参数: pdbVecDes;
返回值　: 无
*********************************************/
void ACSGM_VecCpy(double* pdbVecDes, const double* cpdbVecSor, unsigned int unL)
{
	unsigned int i;

	for (i = 0; i < unL; i++)
	{
		pdbVecDes[i] = cpdbVecSor[i];
	}
	return;
}
/********************************************
功能描述: 矩阵减法，由调用者保证两矩阵行列长度相等
输入参数: pdbMat1矩阵1
　　　　  pdbMat2 矩阵2
					   nRowLen矩阵行数
					   nColLen矩阵列数
输出参数:pdbMatResult = pdbMat1 - pdbMat2
返回值　: 无
备注：pdbMatResult可以和cpdbMat1或cpdbMat2相同
*********************************************/
void ACSGM_MatMinus(double* pdbMatResult, const double* cpdbMat1, const double* cpdbMat2,
	unsigned int unRowLen, unsigned int unColLen)
{
	unsigned int i;

	for (i = 0; i < unRowLen * unColLen; i++)
	{

		pdbMatResult[i] = cpdbMat1[i] - cpdbMat2[i];
	}
	return;
}
/********************************************
功能描述: 矩阵加法，由调用者保证两矩阵行列长度相等
输入参数: pdbMat1矩阵1
　　　　  pdbMat2 矩阵2
					   nRowLen矩阵行数
					   nColLen矩阵列数
输出参数:pdbMatResult = pdbMat1 + pdbMat2
返回值　: 无
备注：pdbMatResult可以和cpdbMat1或cpdbMat2相同
*********************************************/
void ACSGM_MatSum(double* pdbMatResult, const double* cpdbMat1, const double* cpdbMat2,
	unsigned int unRowLen, unsigned int unColLen)
{
	unsigned int i;
	for (i = 0; i < unRowLen * unColLen; i++)
	{

		pdbMatResult[i] = cpdbMat1[i] + cpdbMat2[i];
	}
	return;
}

/********************************************
功能描述: 矩阵乘法
输入参数: pdbMat1 矩阵1 M*N
		  pdbMat2 矩阵2 N*M
		  nRowLen1矩阵1行长度
		  nCol1Row2Len矩阵1列长度且是矩阵2行长度
		  nColLen2矩阵2列长度
输出参数:pdbMatResult = 矩阵1 * 矩阵2
返回值　:无
备注：1、pdbMatResult可以和cpdbMat1或cpdbMat2相同
	  2、输入维数若超过ACSGM_MATRIC_MAX_DIM，不处理
*********************************************/
void ACSGM_MatProd(double* pdbMatResult, const double* cpdbMat1, const double* cpdbMat2,
	unsigned int unRowLen1, unsigned int unCol1Row2Len, unsigned int unColLen2)
{
	unsigned int i, j, k, u;
	double dbMatTemp[ACSGM_MATRIC_MAX_DIM * ACSGM_MATRIC_MAX_DIM] = { 0.0f };

	if ((unRowLen1 <= ACSGM_MATRIC_MAX_DIM) && (unCol1Row2Len <= ACSGM_MATRIC_MAX_DIM) &&
		(unColLen2 <= ACSGM_MATRIC_MAX_DIM))
	{
		for (i = 0; i < unRowLen1; i++)
		{
			for (j = 0; j < unColLen2; j++)
			{
				u = i * unColLen2 + j;
				dbMatTemp[u] = 0.0f;
				for (k = 0; k < unCol1Row2Len; k++)
				{
					dbMatTemp[u] = dbMatTemp[u] + cpdbMat1[i * unCol1Row2Len + k] * cpdbMat2[k * unColLen2 + j];
				}
			}
		}
		/*Modified by HF 20130903*/
		ACSGM_MatCpy(pdbMatResult, dbMatTemp, unRowLen1, unColLen2);
	}
	return;
}
/********************************************
功能描述: 矩阵转置
输入参数: pdbMat等转置的矩阵
		  nRowLen矩阵行数
		  nColLen矩阵列数
输出参数:pdbMatResult = pdbMat'
返回值　: 无
备注：1、pdbMatResult可以和cpdbMat相同
	  2、输入维数不能超过ACSGM_MATRIC_MAX_DIM，会发生数组越界
*********************************************/
void ACSGM_MatTrans(double* pdbMatResult, const double* cpdbMat,
	unsigned int unRowLen, unsigned int unColLen)
{
	unsigned int i, j;
	double dbMatTemp[ACSGM_MATRIC_MAX_DIM * ACSGM_MATRIC_MAX_DIM] = { 0.0f };

	if (pdbMatResult == cpdbMat)
	{
		for (i = 0; i < unRowLen; i++)
		{
			for (j = 0; j < unColLen; j++)
			{
				/*dbMatTemp[i * unColLen + j] = cpdbMat[j * unRowLen + i];*/
				dbMatTemp[j * unRowLen + i] = cpdbMat[i * unColLen + j];	/* 更改 */
			}
		}

		ACSGM_MatCpy(pdbMatResult, dbMatTemp, unRowLen, unColLen);
	}
	else
	{
		for (i = 0; i < unRowLen; i++)
		{
			for (j = 0; j < unColLen; j++)
			{
				pdbMatResult[j * unRowLen + i] = cpdbMat[i * unColLen + j];
			}
		}

	}
	return;
}
/********************************************
功能描述: 3*3方阵的逆，直接展开
输入参数: cpdbMat3by3 待求逆的方阵, 元素不能太奇异
输出参数:pdbMat3by3Inv方阵的逆
返回值　: 返回ACSGM_FALSE方阵的逆不存在
					   ACSGM_TRUE方阵的逆存在
备注：1、pdbMat3by3Inv可以和cpdbMat3by3相同
*********************************************/
void ACSGM_Mat3by3Inv(double* pdbMat3by3Inv, const double* cpdbMat3by3)
{
	double adbMatTemp[9] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	double dbDet = (cpdbMat3by3[0] * cpdbMat3by3[4] * cpdbMat3by3[8] + cpdbMat3by3[1] * cpdbMat3by3[5] * cpdbMat3by3[6] +
		cpdbMat3by3[2] * cpdbMat3by3[3] * cpdbMat3by3[7] - cpdbMat3by3[0] * cpdbMat3by3[5] * cpdbMat3by3[7] -
		cpdbMat3by3[1] * cpdbMat3by3[3] * cpdbMat3by3[8] - cpdbMat3by3[2] * cpdbMat3by3[4] * cpdbMat3by3[6]);

	if (fabs(dbDet) < ACS_DOUBLEZERO)
	{
		pdbMat3by3Inv[0] = 0.0f;
		pdbMat3by3Inv[1] = 0.0f;
		pdbMat3by3Inv[2] = 0.0f;
		pdbMat3by3Inv[3] = 0.0f;
		pdbMat3by3Inv[4] = 0.0f;
		pdbMat3by3Inv[5] = 0.0f;
		pdbMat3by3Inv[6] = 0.0f;
		pdbMat3by3Inv[7] = 0.0f;
		pdbMat3by3Inv[8] = 0.0f;
		return;
	}
	adbMatTemp[0] = (cpdbMat3by3[4] * cpdbMat3by3[8] - cpdbMat3by3[5] * cpdbMat3by3[7]) / dbDet;
	adbMatTemp[1] = -(cpdbMat3by3[1] * cpdbMat3by3[8] - cpdbMat3by3[2] * cpdbMat3by3[7]) / dbDet;
	adbMatTemp[2] = (cpdbMat3by3[1] * cpdbMat3by3[5] - cpdbMat3by3[2] * cpdbMat3by3[4]) / dbDet;
	adbMatTemp[3] = -(cpdbMat3by3[3] * cpdbMat3by3[8] - cpdbMat3by3[5] * cpdbMat3by3[6]) / dbDet;
	adbMatTemp[4] = (cpdbMat3by3[0] * cpdbMat3by3[8] - cpdbMat3by3[2] * cpdbMat3by3[6]) / dbDet;
	adbMatTemp[5] = -(cpdbMat3by3[0] * cpdbMat3by3[5] - cpdbMat3by3[2] * cpdbMat3by3[3]) / dbDet;
	adbMatTemp[6] = (cpdbMat3by3[3] * cpdbMat3by3[7] - cpdbMat3by3[4] * cpdbMat3by3[6]) / dbDet;
	adbMatTemp[7] = -(cpdbMat3by3[0] * cpdbMat3by3[7] - cpdbMat3by3[1] * cpdbMat3by3[6]) / dbDet;
	adbMatTemp[8] = (cpdbMat3by3[0] * cpdbMat3by3[4] - cpdbMat3by3[1] * cpdbMat3by3[3]) / dbDet;

	ACSGM_VecCpy(pdbMat3by3Inv, adbMatTemp, 9);
	return;
}

/********************************************
功能描述: x轴旋转方向余弦矩阵
输入参数: [in]dSita: 逆时针绕x轴的旋转角度，单位弧度
输出参数: [out]pdbMat3DCMResult: 3*3方向余弦矩阵
返回值　: 无
备注: Rx(Sita) = [1   	    0 	          0
				  0         cos(Sita)     sina(Sita)
				  0        -sina(Sita)    cos(Sita)]
*********************************************/
void ACSGM_DCMx(double* pdbMat3DCMResult, double dSita)
{
	pdbMat3DCMResult[0] = 1.0f;
	pdbMat3DCMResult[1] = 0.0f;
	pdbMat3DCMResult[2] = 0.0f;
	pdbMat3DCMResult[3] = 0.0f;
	pdbMat3DCMResult[4] = (double)cos(dSita);
	pdbMat3DCMResult[5] = (double)sin(dSita);
	pdbMat3DCMResult[6] = (double)0.0f;
	pdbMat3DCMResult[7] = (double)(-pdbMat3DCMResult[5]);
	pdbMat3DCMResult[8] = (double)pdbMat3DCMResult[4];
	return;
}


/********************************************
功能描述: y轴旋转方向余弦矩阵
输入参数: [in]fSita: 逆时针绕y轴的旋转角度，单位弧度
输出参数: [out]pdbMat3DCMResult: 3*3方向余弦矩阵
返回值　: 无
备注: Ry(Sita) = [cos(Sita)   	    0 	       -sina(Sita)
				  0                 1          0
				  sina(Sita)        0          cos(Sita)]
*********************************************/
void ACSGM_DCMy(double* pdbMat3DCMResult, double dSita)
{
	pdbMat3DCMResult[0] = (double)cos(dSita);
	pdbMat3DCMResult[1] = 0.0f;
	pdbMat3DCMResult[2] = (double)(-sin(dSita));
	pdbMat3DCMResult[3] = 0.0f;
	pdbMat3DCMResult[4] = 1.0f;
	pdbMat3DCMResult[5] = 0.0f;
	pdbMat3DCMResult[6] = (double)(-pdbMat3DCMResult[2]);
	pdbMat3DCMResult[7] = 0.0f;
	pdbMat3DCMResult[8] = (double)pdbMat3DCMResult[0];
	return;
}


/********************************************
功能描述: z轴旋转方向余弦矩阵
输入参数: [in]fSita: 逆时针绕z轴的旋转角度，单位弧度
输出参数: [out]pdbMat3DCMResult: 3*3方向余弦矩阵
返回值　: 无
备注: Ry(Sita) = [cos(Sita)   	    sina(Sita) 	       0
				  -sina(Sita)       cos(Sita)          0
				  0                 0                  1]
*********************************************/
void ACSGM_DCMz(double* pdbMat3DCMResult, double dSita)
{
	pdbMat3DCMResult[0] = (double)cos(dSita);
	pdbMat3DCMResult[1] = (double)sin(dSita);
	pdbMat3DCMResult[2] = 0.0f;
	pdbMat3DCMResult[3] = (double)(-pdbMat3DCMResult[1]);
	pdbMat3DCMResult[4] = (double)pdbMat3DCMResult[0];
	pdbMat3DCMResult[5] = 0.0f;
	pdbMat3DCMResult[6] = 0.0f;
	pdbMat3DCMResult[7] = 0.0f;
	pdbMat3DCMResult[8] = 1.0f;

	return;
}

/********************************************
功能描述: 矩阵与向量的乘法
输入参数: cpdbMat 矩阵
		  cpdbVec 向量
		  unRowLen矩阵1行长度
		  unColLen矩阵1列长度
输出参数: pdbMatResult = cpdbMat * cpdbVec
返回值　: 无
备注：1、pdbMatResult可以和cpdbMat或cpdbVec相同
	  2、输入维数若超过ACSGM_MATRIC_MAX_DIM，不处理
*********************************************/
void ACSGM_MatProdColumnVec(double* pdbResult, const double* cpdbMat, const double* cpdbVec,
	unsigned int unRowLen, unsigned int unColLen)
{
	unsigned int i, j;
	double adbTemp[ACSGM_MATRIC_MAX_DIM] = { 0.0f };

	if ((unRowLen <= ACSGM_MATRIC_MAX_DIM) && (unColLen <= ACSGM_MATRIC_MAX_DIM)) /*overflow, overwrite the value of pdbResult*/
	{
		/*use new array to save the result*/
		for (i = 0; i < unRowLen; i++)
		{
			adbTemp[i] = 0;
			for (j = 0; j < unColLen; j++)
			{
				adbTemp[i] = adbTemp[i] + cpdbMat[i * unColLen + j] * cpdbVec[j];
			}
		}
		/*copy result to param*/
		for (i = 0; i < unRowLen; i++)
		{
			pdbResult[i] = adbTemp[i];
		}
	}
	return;
}

#define ACS_MAGMFCALI_MAGNIFY_COEF	5e4f	/*为防止因地磁场太小导致Hk奇异，所有地磁场测量输入都乘以该放大系数，但这也会导致Xk最后三项被同比例放大*/
/************************************************************************
函数名称：磁强计误差校正函数
函数功能：以递推最小二乘方法计算磁强计误差模型系数矩阵和偏移量
假设磁强计误差模型为线性化模型Z=AX+B，X为本体系下磁场测量值（含误差），Z为本体系下标称磁场，
A为误差系数阵，B为误差偏移量，用如下方法进行最小二乘递推计算A和B：
Xk为状态向量，包含A阵的9个元素和（行优先顺序存储）B向量的3个元素，初始值假定A为单位阵，B各元素为0
Pk初始值为12X12单位阵，HK为3X12矩阵，定义如下
Hk=[Z1 Z2 Z3 0 0 0 0 0 0 1 0 0;0 0 0 Z1 Z2 Z3 0 0 0 0 1 0;0 0 0 0 0 0 Z1 Z2 Z3 0 0 1];Y1为本体系下x向测量值，以此类推
Hk'为Hk转置，I为单位阵，Zk为本周期标称磁场，kp1代表下标k+1
Pkp1 = Pk - Pk * Hk'* [inv(I + Hk * Pk * Hk')] * Hk * Pk
Xkp1 = Xk + Pkp1 * Hk'*(Zk - Hk * Xk)
出参：adbXk-本周期状态向量递推结果，12X1向量，前9元素为A阵元素（行优先，无量纲），后3元素为B中元素（单位T）
	  adbPk-本周期Pk递推结果，12X12矩阵
入参：adbXk-上周期状态向量递推结果，12X1向量，前9元素为A阵元素（行优先，无量纲），后3元素为B中元素（单位T）
	  adbPk-上周期Pk递推结果，12X12矩阵
	  adbRefBb-由地磁场模型计算值和姿态计算出的地磁场在本体系下坐标，可认为是本体系下实际磁场坐标，单位T
	  adbMagMBb-由磁强计测量值获得的磁场在本体系下坐标（经过安装矩阵处理），单位T
************************************************************************/
void ACSATTD_MagMCalibrate(double adbXk[12], double adbPk[12 * 12], double adbMagMBb[3], double adbRefBb[3])
{
	double adbMatTmp[144];
	double adbHk[36];
	double adbHkTrans[36]; /*Hk转置*/
	double afZk[3];
	adbHk[0] = adbMagMBb[0] * ACS_MAGMFCALI_MAGNIFY_COEF;
	adbHk[1] = adbMagMBb[1] * ACS_MAGMFCALI_MAGNIFY_COEF;
	adbHk[2] = adbMagMBb[2] * ACS_MAGMFCALI_MAGNIFY_COEF;
	adbHk[3] = 0.0f;
	adbHk[4] = 0.0f;
	adbHk[5] = 0.0f;
	adbHk[6] = 0.0f;
	adbHk[7] = 0.0f;
	adbHk[8] = 0.0f;
	adbHk[9] = 1.0f;
	adbHk[10] = 0.0f;
	adbHk[11] = 0.0f;

	adbHk[12] = 0.0f;
	adbHk[13] = 0.0f;
	adbHk[14] = 0.0f;
	adbHk[15] = adbMagMBb[0] * ACS_MAGMFCALI_MAGNIFY_COEF;
	adbHk[16] = adbMagMBb[1] * ACS_MAGMFCALI_MAGNIFY_COEF;
	adbHk[17] = adbMagMBb[2] * ACS_MAGMFCALI_MAGNIFY_COEF;
	adbHk[18] = 0.0f;
	adbHk[19] = 0.0f;
	adbHk[20] = 0.0f;
	adbHk[21] = 0.0f;
	adbHk[22] = 1.0f;
	adbHk[23] = 0.0f;

	adbHk[24] = 0.0f;
	adbHk[25] = 0.0f;
	adbHk[26] = 0.0f;
	adbHk[27] = 0.0f;
	adbHk[28] = 0.0f;
	adbHk[29] = 0.0f;
	adbHk[30] = adbMagMBb[0] * ACS_MAGMFCALI_MAGNIFY_COEF;
	adbHk[31] = adbMagMBb[1] * ACS_MAGMFCALI_MAGNIFY_COEF;
	adbHk[32] = adbMagMBb[2] * ACS_MAGMFCALI_MAGNIFY_COEF;
	adbHk[33] = 0.0f;
	adbHk[34] = 0.0f;
	adbHk[35] = 1.0f;

	adbXk[9] = adbXk[9] * ACS_MAGMFCALI_MAGNIFY_COEF; /*计算前临时放大，输出时在等比例缩小*/
	adbXk[10] = adbXk[10] * ACS_MAGMFCALI_MAGNIFY_COEF;
	adbXk[11] = adbXk[11] * ACS_MAGMFCALI_MAGNIFY_COEF;

	afZk[0] = adbRefBb[0] * ACS_MAGMFCALI_MAGNIFY_COEF;
	afZk[1] = adbRefBb[1] * ACS_MAGMFCALI_MAGNIFY_COEF;
	afZk[2] = adbRefBb[2] * ACS_MAGMFCALI_MAGNIFY_COEF;

	ACSGM_MatTrans(adbHkTrans, adbHk, 3, 12);

	ACSGM_MatProd(adbMatTmp, adbPk, adbHkTrans, 12, 12, 3); /*Pk * Hk'，12X3*/

	ACSGM_MatProd(adbMatTmp, adbHk, adbMatTmp, 3, 12, 3); /*Hk * Pk * Hk', 3X3*/

	adbMatTmp[0] = adbMatTmp[0] + 1.0f;
	adbMatTmp[4] = adbMatTmp[4] + 1.0f;
	adbMatTmp[8] = adbMatTmp[8] + 1.0f; /*I + Hk * Pk * Hk', 3X3*/
	ACSGM_Mat3by3Inv(adbMatTmp, adbMatTmp); /*inv(I + Hk * Pk * Hk'), 3X3*/

	ACSGM_MatProd(adbMatTmp, adbMatTmp, adbHk, 3, 3, 12); /*[inv(I + Hk * Pk * Hk')] * Hk, 3X12*/

	ACSGM_MatProd(adbMatTmp, adbMatTmp, adbPk, 3, 12, 12); /* [inv(I + Hk * Pk * Hk')] * Hk * Pk, 3X12 */
	ACSGM_MatProd(adbMatTmp, adbHkTrans, adbMatTmp, 12, 3, 12); /* Hk'* [inv(I + Hk * Pk * Hk')] * Hk * Pk, 12X12 */

	ACSGM_MatProd(adbMatTmp, adbPk, adbMatTmp, 12, 12, 12); /* Pk * Hk'* [inv(I + Hk * Pk * Hk')] * Hk * Pk, 12X12 */
	ACSGM_MatMinus(adbPk, adbPk, adbMatTmp, 12, 12); /* Pkp1 = Pk -Pk * Hk'* [inv(I + Hk * Pk * Hk')] * Hk * Pk, 12X12 */

	ACSGM_MatProd(adbMatTmp, adbHk, adbXk, 3, 12, 1); /*Hk * Xk, 3X1*/
	ACSGM_MatMinus(adbMatTmp, afZk, adbMatTmp, 3, 1); /*Zk - Hk * Xk, 3X1*/
	ACSGM_MatProd(adbMatTmp, adbHkTrans, adbMatTmp, 12, 3, 1); /*Hk'* (Zk - Hk * Xk), 12X1*/
	ACSGM_MatProd(adbMatTmp, adbPk, adbMatTmp, 12, 12, 1); /* Pkp1 * Hk'*(Zk - Hk * Xk), 12X1*/
	ACSGM_MatSum(adbXk, adbXk, adbMatTmp, 12, 1); /*Xkp1 = Xk + Pkp1 * Hk'*(Zk - Hk * Xk), 12X1*/

	adbXk[9] = adbXk[9] / ACS_MAGMFCALI_MAGNIFY_COEF; /*之前临时放大，输出时在等比例缩小*/
	adbXk[10] = adbXk[10] / ACS_MAGMFCALI_MAGNIFY_COEF;
	adbXk[11] = adbXk[11] / ACS_MAGMFCALI_MAGNIFY_COEF;
}

/********************************************
函数名称: 向量数乘
函数功能: 进行向量与一个标量相乘的运算
输入参数: cpdbVec: 要进行运算的向量;
		  unL: 向量长度;
		  fNum: 要进行运算的标量;
		  pdbResult: 存储运算的结果;
输出参数: pdbResult，计算的结果;
返回值　: 无
备注：1、pdbResult可以和cpdbVec相同
*********************************************/
void ACSGM_VecMulNum(double* pdbResult, const double* cpdbVec, unsigned int unL, double dbNum)
{
	unsigned int i;

	for (i = 0; i < unL; i++)
	{
		pdbResult[i] = cpdbVec[i] * dbNum;
	}
	return;
}

/********************************************
函数名称: 向量加法
函数功能: 进行向量的加法运算
输入参数: cpdbVec1: 要进行运算的向量1;
		  cpdbVec2: 要进行运算的向量2;
		  unL: 向量长度;
		  pdbResult: 存储运算的结果;
输出参数: pdbResult，计算的结果;
返回值　: 无
备注：1、pdbResult可以和cpdbVec1或cpdbVec2相同
*********************************************/
void ACSGM_VecSum(double* pdbResult, const double* cpdbVec1, const double* cpdbVec2, unsigned int unL)
{
	unsigned int i;

	for (i = 0; i < unL; i++)
	{
		pdbResult[i] = cpdbVec1[i] + cpdbVec2[i];
	}
	return;
}

/************************************************************************
//根据陀螺数据计算磁场参考值
//入参：adbMagB-上拍磁强计读数，数据需转换到陀螺磁强计本体系
		adbGyroW-上一拍陀螺数据，数据需转换到陀螺磁强计本体系
//出参：adbMagBRef-计算出的磁场参考值，本体系下
************************************************************************/
void ACS_CalMagRefB(double adbMagRef[3],double adbMagB[3],double adbGyroW[3], double adbInterval)
{
	double adbTheta[3];
	double adbDcmx[9];
	double adbDcmy[9];
	double adbDcmz[9];
	double adbDcm[9];

    ACSGM_VecMulNum(adbTheta,adbGyroW,3, adbInterval);/*角速度*时间，获得旋转角度rad*/

	ACSGM_DCMx(adbDcmx, adbTheta[0]); /*x轴旋转矩阵*/
	ACSGM_DCMy(adbDcmy, adbTheta[1]); /*y轴旋转矩阵*/
	ACSGM_DCMz(adbDcmz, adbTheta[2]); /*z轴旋转矩阵*/

	/*按312旋转顺序获得最终的旋转矩阵*/
	ACSGM_MatProd(adbDcm, adbDcmx, adbDcmz, 3, 3, 3);
	ACSGM_MatProd(adbDcm, adbDcmy, adbDcm, 3, 3, 3);

	/*获得磁场参考值作为校正函数的观测值*/
	ACSGM_MatProdColumnVec(adbMagRef, adbDcm, adbMagB, 3, 3);
}

//全局变量初始化，用于最小二乘计算磁强计校正参数
static double xk[12] = { 1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000 };
static double pk[144] = { 1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000 };


#if 0
void main()
{
	int i;
	double b[3] = { 20020.41 * 1e-9, -41591.31 * 1e-9,    15004.36 * 1e-9 };/*初始值为长沙本地地磁场，单位T*/
	double w[3] = { 0.5,-0.6,0.7 };/*角速度，单位rad/s*/
	double bref[3];/*参考地磁场*/
	double mtx[9] = { 0.329292590816961, -0.944227702972822, -0.000659220541077,
					  0.944225759442446, 0.329293250565424, -0.001915812463079,
					  0.002026040076151, 0.000008409833448, 0.999997947543336};/*偏差矩阵，用于构造磁强计输入*/
	double vec[3] = { 1000 * 1e-9,-2000 * 1e-9,1500 * 1e-9 };/*偏差向量，用于构造磁强计输入*/
	
    
	for (i = 0; i < 360; i++)
	{
		/*计算旋转一定角度后的参考地磁场*/
		ACS_CalMagRefB(bref, b, w);
		printf("bref:%f,%f,%f\r\n", bref[0], bref[1], bref[2]);
		/*构造磁强计输入*/
		ACSGM_MatProdColumnVec(b, mtx, bref, 3, 3);
		ACSGM_VecSum(b, b, vec, 3);
		ACSATTD_MagMCalibrate(xk, pk, b, bref);
		printf("xk:%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n", xk[0], xk[1], xk[2], xk[3], xk[4], xk[5], xk[6], xk[7], xk[8], xk[9], xk[10], xk[11]);
	}
}
#endif
