/*
 **********************************************************************
 *	Copyright (C) 2015.
 *	All Rights Reserved.
 **********************************************************************
 *
 *	@file		K_Camera.hpp
 *	@brief		获取Kinect摄像机图像
 *
 *	Date        Name        Description
 *	03/18/15	LB			Creation.
 *  03/22/15    tom black   简化类结构，将CheckPoint移入Image，保证了封装性
 *
 *********************************************************************/

#ifndef KinectCamera
#define KinectCamera

#include "GoalKeeper.h"

class K_Camera
{
public:

	/*==========================================================================*/
	/*	公有 构造 \ 析构														*/
	/*==========================================================================*/
	K_Camera();

	~K_Camera();

	/**
	 *	\brief	打开kinect摄像头
	 */
	bool OpenKinect();

	/**
	 *	\brief	获取摄像机当前帧深度图
	 */
	Mat* GetDepthImg();

	/**
	 *  \brief 获取摄像机当前帧BGR图
	 */
	Mat* GetBGRImg();

	/**
	*  \brief 获取深度数据流指针
	*/
	VideoStream *GetDepthStream();

	/**
	*  \brief 获取RGB数据流指针
	*/
	VideoStream *GetColorStream();

	DepthPixel GetDepthMsg(int idx);

private:

	Device device;  /*设备*/

	VideoStream oniDepthStream;  /*深度数据流*/
	VideoStream oniColorStream;  /*RGB数据流*/

	VideoFrameRef oniDepthImg;
	VideoFrameRef oniColorImg;

	VideoMode modeDepth;
	VideoMode modeColor;

	Mat cvDepthImg;  /*一帧深度图*/
	Mat cvBGRImg;  /*一帧RGB图*/

	DepthPixel* m_pDepth;		/*深度信息指针*/
};

#endif