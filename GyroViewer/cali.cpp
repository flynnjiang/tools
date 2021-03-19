#include <math.h>
#include <stdio.h>
#define ACSGM_MATRIC_MAX_DIM					12
#define ACS_DOUBLEZERO							1.0e-20
#define ACS_GYRO_SAMP_P							0.1	//�����ǵĲ�������


/********************************************
��������: ACSGM_MatCpy
��������: ���󿽱�
�������: pdbMatSrc����
		  nRowLen��������
		  nColLen��������
�������:pdbMatDes����Ŀ�����
����ֵ��: ��
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
��������: ��������
��������: ��������ֵ�ĸ���
�������: cpdbVecSor: Ҫ���п���������;
		  unL: ��������;
�������: pdbVecDes;
����ֵ��: ��
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
��������: ����������ɵ����߱�֤���������г������
�������: pdbMat1����1
��������  pdbMat2 ����2
					   nRowLen��������
					   nColLen��������
�������:pdbMatResult = pdbMat1 - pdbMat2
����ֵ��: ��
��ע��pdbMatResult���Ժ�cpdbMat1��cpdbMat2��ͬ
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
��������: ����ӷ����ɵ����߱�֤���������г������
�������: pdbMat1����1
��������  pdbMat2 ����2
					   nRowLen��������
					   nColLen��������
�������:pdbMatResult = pdbMat1 + pdbMat2
����ֵ��: ��
��ע��pdbMatResult���Ժ�cpdbMat1��cpdbMat2��ͬ
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
��������: ����˷�
�������: pdbMat1 ����1 M*N
		  pdbMat2 ����2 N*M
		  nRowLen1����1�г���
		  nCol1Row2Len����1�г������Ǿ���2�г���
		  nColLen2����2�г���
�������:pdbMatResult = ����1 * ����2
����ֵ��:��
��ע��1��pdbMatResult���Ժ�cpdbMat1��cpdbMat2��ͬ
	  2������ά��������ACSGM_MATRIC_MAX_DIM��������
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
��������: ����ת��
�������: pdbMat��ת�õľ���
		  nRowLen��������
		  nColLen��������
�������:pdbMatResult = pdbMat'
����ֵ��: ��
��ע��1��pdbMatResult���Ժ�cpdbMat��ͬ
	  2������ά�����ܳ���ACSGM_MATRIC_MAX_DIM���ᷢ������Խ��
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
				dbMatTemp[j * unRowLen + i] = cpdbMat[i * unColLen + j];	/* ���� */
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
��������: 3*3������棬ֱ��չ��
�������: cpdbMat3by3 ������ķ���, Ԫ�ز���̫����
�������:pdbMat3by3Inv�������
����ֵ��: ����ACSGM_FALSE������治����
					   ACSGM_TRUE����������
��ע��1��pdbMat3by3Inv���Ժ�cpdbMat3by3��ͬ
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
��������: x����ת�������Ҿ���
�������: [in]dSita: ��ʱ����x�����ת�Ƕȣ���λ����
�������: [out]pdbMat3DCMResult: 3*3�������Ҿ���
����ֵ��: ��
��ע: Rx(Sita) = [1   	    0 	          0
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
��������: y����ת�������Ҿ���
�������: [in]fSita: ��ʱ����y�����ת�Ƕȣ���λ����
�������: [out]pdbMat3DCMResult: 3*3�������Ҿ���
����ֵ��: ��
��ע: Ry(Sita) = [cos(Sita)   	    0 	       -sina(Sita)
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
��������: z����ת�������Ҿ���
�������: [in]fSita: ��ʱ����z�����ת�Ƕȣ���λ����
�������: [out]pdbMat3DCMResult: 3*3�������Ҿ���
����ֵ��: ��
��ע: Ry(Sita) = [cos(Sita)   	    sina(Sita) 	       0
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
��������: �����������ĳ˷�
�������: cpdbMat ����
		  cpdbVec ����
		  unRowLen����1�г���
		  unColLen����1�г���
�������: pdbMatResult = cpdbMat * cpdbVec
����ֵ��: ��
��ע��1��pdbMatResult���Ժ�cpdbMat��cpdbVec��ͬ
	  2������ά��������ACSGM_MATRIC_MAX_DIM��������
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

#define ACS_MAGMFCALI_MAGNIFY_COEF	5e4f	/*Ϊ��ֹ��شų�̫С����Hk���죬���еشų��������붼���Ը÷Ŵ�ϵ��������Ҳ�ᵼ��Xk������ͬ�����Ŵ�*/
/************************************************************************
�������ƣ���ǿ�����У������
�������ܣ��Ե�����С���˷��������ǿ�����ģ��ϵ�������ƫ����
�����ǿ�����ģ��Ϊ���Ի�ģ��Z=AX+B��XΪ����ϵ�´ų�����ֵ��������ZΪ����ϵ�±�ƴų���
AΪ���ϵ����BΪ���ƫ�����������·���������С���˵��Ƽ���A��B��
XkΪ״̬����������A���9��Ԫ�غͣ�������˳��洢��B������3��Ԫ�أ���ʼֵ�ٶ�AΪ��λ��B��Ԫ��Ϊ0
Pk��ʼֵΪ12X12��λ��HKΪ3X12���󣬶�������
Hk=[Z1 Z2 Z3 0 0 0 0 0 0 1 0 0;0 0 0 Z1 Z2 Z3 0 0 0 0 1 0;0 0 0 0 0 0 Z1 Z2 Z3 0 0 1];Y1Ϊ����ϵ��x�����ֵ���Դ�����
Hk'ΪHkת�ã�IΪ��λ��ZkΪ�����ڱ�ƴų���kp1�����±�k+1
Pkp1 = Pk - Pk * Hk'* [inv(I + Hk * Pk * Hk')] * Hk * Pk
Xkp1 = Xk + Pkp1 * Hk'*(Zk - Hk * Xk)
���Σ�adbXk-������״̬�������ƽ����12X1������ǰ9Ԫ��ΪA��Ԫ�أ������ȣ������٣�����3Ԫ��ΪB��Ԫ�أ���λT��
	  adbPk-������Pk���ƽ����12X12����
��Σ�adbXk-������״̬�������ƽ����12X1������ǰ9Ԫ��ΪA��Ԫ�أ������ȣ������٣�����3Ԫ��ΪB��Ԫ�أ���λT��
	  adbPk-������Pk���ƽ����12X12����
	  adbRefBb-�ɵشų�ģ�ͼ���ֵ����̬������ĵشų��ڱ���ϵ�����꣬����Ϊ�Ǳ���ϵ��ʵ�ʴų����꣬��λT
	  adbMagMBb-�ɴ�ǿ�Ʋ���ֵ��õĴų��ڱ���ϵ�����꣨������װ����������λT
************************************************************************/
void ACSATTD_MagMCalibrate(double adbXk[12], double adbPk[12 * 12], double adbMagMBb[3], double adbRefBb[3])
{
	double adbMatTmp[144];
	double adbHk[36];
	double adbHkTrans[36]; /*Hkת��*/
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

	adbXk[9] = adbXk[9] * ACS_MAGMFCALI_MAGNIFY_COEF; /*����ǰ��ʱ�Ŵ����ʱ�ڵȱ�����С*/
	adbXk[10] = adbXk[10] * ACS_MAGMFCALI_MAGNIFY_COEF;
	adbXk[11] = adbXk[11] * ACS_MAGMFCALI_MAGNIFY_COEF;

	afZk[0] = adbRefBb[0] * ACS_MAGMFCALI_MAGNIFY_COEF;
	afZk[1] = adbRefBb[1] * ACS_MAGMFCALI_MAGNIFY_COEF;
	afZk[2] = adbRefBb[2] * ACS_MAGMFCALI_MAGNIFY_COEF;

	ACSGM_MatTrans(adbHkTrans, adbHk, 3, 12);

	ACSGM_MatProd(adbMatTmp, adbPk, adbHkTrans, 12, 12, 3); /*Pk * Hk'��12X3*/

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

	adbXk[9] = adbXk[9] / ACS_MAGMFCALI_MAGNIFY_COEF; /*֮ǰ��ʱ�Ŵ����ʱ�ڵȱ�����С*/
	adbXk[10] = adbXk[10] / ACS_MAGMFCALI_MAGNIFY_COEF;
	adbXk[11] = adbXk[11] / ACS_MAGMFCALI_MAGNIFY_COEF;
}

/********************************************
��������: ��������
��������: ����������һ��������˵�����
�������: cpdbVec: Ҫ�������������;
		  unL: ��������;
		  fNum: Ҫ��������ı���;
		  pdbResult: �洢����Ľ��;
�������: pdbResult������Ľ��;
����ֵ��: ��
��ע��1��pdbResult���Ժ�cpdbVec��ͬ
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
��������: �����ӷ�
��������: ���������ļӷ�����
�������: cpdbVec1: Ҫ�������������1;
		  cpdbVec2: Ҫ�������������2;
		  unL: ��������;
		  pdbResult: �洢����Ľ��;
�������: pdbResult������Ľ��;
����ֵ��: ��
��ע��1��pdbResult���Ժ�cpdbVec1��cpdbVec2��ͬ
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
//�����������ݼ���ų��ο�ֵ
//��Σ�adbMagB-���Ĵ�ǿ�ƶ�����������ת�������ݴ�ǿ�Ʊ���ϵ
		adbGyroW-��һ���������ݣ�������ת�������ݴ�ǿ�Ʊ���ϵ
//���Σ�adbMagBRef-������Ĵų��ο�ֵ������ϵ��
************************************************************************/
void ACS_CalMagRefB(double adbMagRef[3],double adbMagB[3],double adbGyroW[3], double adbInterval)
{
	double adbTheta[3];
	double adbDcmx[9];
	double adbDcmy[9];
	double adbDcmz[9];
	double adbDcm[9];

    ACSGM_VecMulNum(adbTheta,adbGyroW,3, adbInterval);/*���ٶ�*ʱ�䣬�����ת�Ƕ�rad*/

	ACSGM_DCMx(adbDcmx, adbTheta[0]); /*x����ת����*/
	ACSGM_DCMy(adbDcmy, adbTheta[1]); /*y����ת����*/
	ACSGM_DCMz(adbDcmz, adbTheta[2]); /*z����ת����*/

	/*��312��ת˳�������յ���ת����*/
	ACSGM_MatProd(adbDcm, adbDcmx, adbDcmz, 3, 3, 3);
	ACSGM_MatProd(adbDcm, adbDcmy, adbDcm, 3, 3, 3);

	/*��ôų��ο�ֵ��ΪУ�������Ĺ۲�ֵ*/
	ACSGM_MatProdColumnVec(adbMagRef, adbDcm, adbMagB, 3, 3);
}

//ȫ�ֱ�����ʼ����������С���˼����ǿ��У������
static double xk[12] = { 1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000 };
static double pk[144] = { 1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000 };


#if 0
void main()
{
	int i;
	double b[3] = { 20020.41 * 1e-9, -41591.31 * 1e-9,    15004.36 * 1e-9 };/*��ʼֵΪ��ɳ���صشų�����λT*/
	double w[3] = { 0.5,-0.6,0.7 };/*���ٶȣ���λrad/s*/
	double bref[3];/*�ο��شų�*/
	double mtx[9] = { 0.329292590816961, -0.944227702972822, -0.000659220541077,
					  0.944225759442446, 0.329293250565424, -0.001915812463079,
					  0.002026040076151, 0.000008409833448, 0.999997947543336};/*ƫ��������ڹ����ǿ������*/
	double vec[3] = { 1000 * 1e-9,-2000 * 1e-9,1500 * 1e-9 };/*ƫ�����������ڹ����ǿ������*/
	
    
	for (i = 0; i < 360; i++)
	{
		/*������תһ���ǶȺ�Ĳο��شų�*/
		ACS_CalMagRefB(bref, b, w);
		printf("bref:%f,%f,%f\r\n", bref[0], bref[1], bref[2]);
		/*�����ǿ������*/
		ACSGM_MatProdColumnVec(b, mtx, bref, 3, 3);
		ACSGM_VecSum(b, b, vec, 3);
		ACSATTD_MagMCalibrate(xk, pk, b, bref);
		printf("xk:%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n", xk[0], xk[1], xk[2], xk[3], xk[4], xk[5], xk[6], xk[7], xk[8], xk[9], xk[10], xk[11]);
	}
}
#endif
