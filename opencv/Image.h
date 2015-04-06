/*
**********************************************************************
*	Copyright (C) 2015.
*	All Rights Reserved.
**********************************************************************
*
*	@file		Image.h
*	@brief		����һ��ͼ����(��֮ǰ��RgbPicture��DepthPicture�ϲ��õ�)
*				����֮ǰ����������в���(��ֵ���������ͨ�������ͼ�ķֲ㡢�ҵ���ͼ�������ͨ����λ��ƥ���)
*
*	Date        Name        Description
*	03/20/15	tom black	Creation.
*   03/22/15    tom black   ������������˴����ļ򻯣��������ڴ���ԣ�����������ڴ�й¶����
*   03/28/15    tom black	������ע�ͣ���һ������ṹ
*   
*
*********************************************************************/

#include "K_Camera.h"

#define m_DepthLayerQuanlity 8
#define Depth_Distance 2500/m_DepthLayerQuanlity

class Image
{
public:
	/*==========================================================================*/
	/*	���� ���� \ ����														*/
	/*==========================================================================*/
	Image();

	~Image();

	/*==========================================================================*/
	/*	�������ͼ�Ͳ�ɫͼ����ƥ�䲿��											*/
	/*==========================================================================*/
public:
	/**
	*   \brief ������Ƿ�������֮��
	*
	*	\param	camera kinect����ͷ��
	*
	*	\return	�����Ƿ�ɹ�
	**/
	bool CheckPoint(K_Camera & camera); 

	void Clear();

private:
	void DrawCountour();

	int FindBall;					/*�Ƿ��ҵ���*/

	CvMemStorage * m_DepthStorage;	/*�������������Ϣ���ڴ��*/
	CvMemStorage * m_RGBStorage;	/*�����ɫ������Ϣ���ڴ��*/

	/*==========================================================================*/
	/*	RGBͼ�񲿷�																*/
	/*==========================================================================*/
public:
	/**
	*	\brief	����ԭʼRGBͼƬ
	*
	*	\param	pic ����Ĳ�ɫͼָ��
	*
	*	\return	�����Ƿ�ɹ�
	*/
	bool SetRGBImage(IplImage* pic);

	/**
	*	\brief	����ͼƬHSVͨ����ֵ
	*
	*	\param	min[3] HSV����ͨ������Сֵ
	*
	*	\param	max[3] HSV����ͨ�������ֵ
	*
	*	\return	�����Ƿ�ɹ�
	*/
	bool SetHsvVal(int min[3], int max[3]);

	/**
	*	\brief	������ͨ������ķ�Χ
	*
	*	\param	min ��ͨ���������Сֵ
	*
	*	\param	max ��ͨ����������ֵ
	*
	*	\return	�����Ƿ�ɹ�
	*/
	bool SetRGBMaxDomainVal(int min, int max);

	/**
	*	\brief	��ȡԭʼͼƬ
	*
	*	\return	��ɫͼָ��
	*/
	IplImage* GetRGBImage();

	/**
	*	\brief	��ȡHSV��ͨ����ֵ�����ͼƬ
	*
	*	\return	HSV��ͨ����ֵ�����ͼƬָ��
	*/
	IplImage* GetThresholdImage();

private:
	/**
	*	\brief	ת��RGBͼ��ΪHSIͼ��
	*
	*	\return	�����Ƿ�ɹ�
	*/
	bool RGBToHSI();

	/**
	*	\brief	HSV��ͨ����ֵ����
	*
	*	\return	�����Ƿ�ɹ�
	*/
	bool ThresholdImage();

	/**
	*	\brief	��ȡ��ֵ��ͼ��������ͨ��
	*/
	void GetMaxDomain();

	int m_RGBDomainMax;					 /*��ͨ���������޶�*/
	int m_RGBDomainMin;					 /*��ͨ�������С�޶�*/

	int m_MinHSI[3];					/*��ͨ����ֵ��������Χ(HSI)����Сֵ*/
	int m_MaxHSI[3];					/*��ͨ����ֵ��������Χ(HSI)�����ֵ*/
		
	int m_Width;						/*����ͼ��Ŀ�*/
	int m_Height;						/*����ͼ��ĸ�*/

	IplImage* m_pRGBImage;				/*ԭʼRGBͼ��*/
	IplImage* m_pHSIImage;				/*��RGBͼ��ת�����HSIͼ��*/
	IplImage* m_pThresholdImage;		/*��ֵ�������ͼ��*/

	CvPoint m_Center;					/*�����ͨ�����ͼ���������������*/

	CvSeq* m_pRGBContourMax;			/*��ȡ������*/

	/*==========================================================================*/
	/*	���ͼ�񲿷�															*/
	/*==========================================================================*/
public:
	/**
	*   \brief ����ԭʼͼ��
	*
	*	\param	pic ��������ͼָ��
	*
	*	\return	�����Ƿ�ɹ�
	**/
	bool SetDepthImage(Mat* pic);

	/**
	*   \brief ������ͨ���С��Χ
	*
	*	\param	min ��ͨ�����Сֵ
	*
	*	\param	max ��ͨ������ֵ
	*
	*	\return	�����Ƿ�ɹ�
	**/
	bool SetDepthMaxDomainVal(int min, int max);

	/**
	*   \brief ��ȡԭʼ���ͼƬ
	*
	*	\return	���ͼ��ָ��
	**/
	Mat* GetDepthImage();

private:
	/**
	*   \brief �ָ����ͼ��
	*
	*	\return	�����Ƿ�ɹ�
	**/
	bool CutDepthPic();

	/**
	*   \brief �ڸ������ͼѰ���������
	*
	*	\param frame[] �������ͼ
	*
	*	\return	�����Ƿ�ɹ�
	*/
	bool FindContour(Mat frame[]);

	int m_DepthDomainMax;				 /*��ͨ���������޶�*/
	int m_DepthDomainMin;				 /*��ͨ�������С�޶�*/

	Mat* m_pDepthImage;					 /*ԭʼ���ͼ*/
	//Mat* m_pMaxContourImage;			 /*Ѱ�ҵ�����������ͼ��*/

	CvSeq* m_pDepthContourMax[5];        /*����������*/
};