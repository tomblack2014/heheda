/*
**********************************************************************
*	Copyright (C) 2015.
*	All Rights Reserved.
**********************************************************************
*
*	@file		Image.h
*	@brief		包括一个图像类(由之前的RgbPicture和DepthPicture合并得到)
*				包括之前两个类的所有操作(阈值化、最大连通域处理、深度图的分层、找到两图中最大连通域后的位置匹配等)
*
*	Date        Name        Description
*	03/20/15	tom black	Creation.
*   03/22/15    tom black   对整个类进行了大量的简化，进行了内存测试，结果程序无内存泄露问题
*   03/28/15    tom black	完善了注释，进一步精简结构
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
	/*	公有 构造 \ 析构														*/
	/*==========================================================================*/
	Image();

	~Image();

	/*==========================================================================*/
	/*	根据深度图和彩色图轮廓匹配部分											*/
	/*==========================================================================*/
public:
	/**
	*   \brief 检验点是否在轮廓之中
	*
	*	\param	camera kinect摄像头类
	*
	*	\return	操作是否成功
	**/
	bool CheckPoint(K_Camera & camera); 

	void Clear();

private:
	void DrawCountour();

	int FindBall;					/*是否找到球*/

	CvMemStorage * m_DepthStorage;	/*储存深度轮廓信息的内存块*/
	CvMemStorage * m_RGBStorage;	/*储存彩色轮廓信息的内存块*/

	/*==========================================================================*/
	/*	RGB图像部分																*/
	/*==========================================================================*/
public:
	/**
	*	\brief	设置原始RGB图片
	*
	*	\param	pic 传入的彩色图指针
	*
	*	\return	操作是否成功
	*/
	bool SetRGBImage(IplImage* pic);

	/**
	*	\brief	设置图片HSV通道虑值
	*
	*	\param	min[3] HSV三个通道的最小值
	*
	*	\param	max[3] HSV三个通道的最大值
	*
	*	\return	操作是否成功
	*/
	bool SetHsvVal(int min[3], int max[3]);

	/**
	*	\brief	设置连通域面积的范围
	*
	*	\param	min 连通域面积的最小值
	*
	*	\param	max 连通域面积的最大值
	*
	*	\return	操作是否成功
	*/
	bool SetRGBMaxDomainVal(int min, int max);

	/**
	*	\brief	获取原始图片
	*
	*	\return	彩色图指针
	*/
	IplImage* GetRGBImage();

	/**
	*	\brief	获取HSV三通道阈值处理后图片
	*
	*	\return	HSV三通道阈值处理后图片指针
	*/
	IplImage* GetThresholdImage();

private:
	/**
	*	\brief	转换RGB图像为HSI图像
	*
	*	\return	操作是否成功
	*/
	bool RGBToHSI();

	/**
	*	\brief	HSV三通道阈值处理
	*
	*	\return	操作是否成功
	*/
	bool ThresholdImage();

	/**
	*	\brief	获取阈值后图像的最大连通域
	*/
	void GetMaxDomain();

	int m_RGBDomainMax;					 /*连通域面积最大限度*/
	int m_RGBDomainMin;					 /*连通域面积最小限度*/

	int m_MinHSI[3];					/*三通道虑值的三个范围(HSI)的最小值*/
	int m_MaxHSI[3];					/*三通道虑值的三个范围(HSI)的最大值*/
		
	int m_Width;						/*处理图像的宽*/
	int m_Height;						/*处理图像的高*/

	IplImage* m_pRGBImage;				/*原始RGB图像*/
	IplImage* m_pHSIImage;				/*将RGB图像转化后的HSI图像*/
	IplImage* m_pThresholdImage;		/*阈值化处理后图像*/

	CvPoint m_Center;					/*最大连通域处理后图像区域的中心坐标*/

	CvSeq* m_pRGBContourMax;			/*获取的轮廓*/

	/*==========================================================================*/
	/*	深度图像部分															*/
	/*==========================================================================*/
public:
	/**
	*   \brief 设置原始图像
	*
	*	\param	pic 传入的深度图指针
	*
	*	\return	操作是否成功
	**/
	bool SetDepthImage(Mat* pic);

	/**
	*   \brief 设置连通域大小范围
	*
	*	\param	min 连通域的最小值
	*
	*	\param	max 连通域的最大值
	*
	*	\return	操作是否成功
	**/
	bool SetDepthMaxDomainVal(int min, int max);

	/**
	*   \brief 获取原始深度图片
	*
	*	\return	深度图的指针
	**/
	Mat* GetDepthImage();

private:
	/**
	*   \brief 分割深度图像
	*
	*	\return	操作是否成功
	**/
	bool CutDepthPic();

	/**
	*   \brief 在各大深度图寻找最大轮廓
	*
	*	\param frame[] 各大深度图
	*
	*	\return	操作是否成功
	*/
	bool FindContour(Mat frame[]);

	int m_DepthDomainMax;				 /*连通域面积最大限度*/
	int m_DepthDomainMin;				 /*连通域面积最小限度*/

	Mat* m_pDepthImage;					 /*原始深度图*/
	//Mat* m_pMaxContourImage;			 /*寻找到最大轮廓后的图像*/

	CvSeq* m_pDepthContourMax[5];        /*最大五个轮廓*/
};