/*
 **********************************************************************
 *	Copyright (C) 2015.
 *	All Rights Reserved.
 **********************************************************************
 *
 *	@file		K_Camera.hpp
 *	@brief		��ȡKinect�����ͼ��
 *
 *	Date        Name        Description
 *	03/18/15	LB			Creation.
 *  03/22/15    tom black   ����ṹ����CheckPoint����Image����֤�˷�װ��
 *
 *********************************************************************/

#ifndef KinectCamera
#define KinectCamera

#include "GoalKeeper.h"

class K_Camera
{
public:

	/*==========================================================================*/
	/*	���� ���� \ ����														*/
	/*==========================================================================*/
	K_Camera();

	~K_Camera();

	/**
	 *	\brief	��kinect����ͷ
	 */
	bool OpenKinect();

	/**
	 *	\brief	��ȡ�������ǰ֡���ͼ
	 */
	Mat* GetDepthImg();

	/**
	 *  \brief ��ȡ�������ǰ֡BGRͼ
	 */
	Mat* GetBGRImg();

	/**
	*  \brief ��ȡ���������ָ��
	*/
	VideoStream *GetDepthStream();

	/**
	*  \brief ��ȡRGB������ָ��
	*/
	VideoStream *GetColorStream();

	DepthPixel GetDepthMsg(int idx);

private:

	Device device;  /*�豸*/

	VideoStream oniDepthStream;  /*���������*/
	VideoStream oniColorStream;  /*RGB������*/

	VideoFrameRef oniDepthImg;
	VideoFrameRef oniColorImg;

	VideoMode modeDepth;
	VideoMode modeColor;

	Mat cvDepthImg;  /*һ֡���ͼ*/
	Mat cvBGRImg;  /*һ֡RGBͼ*/

	DepthPixel* m_pDepth;		/*�����Ϣָ��*/
};

#endif