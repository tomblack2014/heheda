#include "Image.h"

Image::
Image() :
m_pRGBImage(nullptr),
m_pThresholdImage(nullptr),
m_pRGBContourMax(nullptr),
m_pHSIImage(nullptr),
m_pDepthImage(nullptr)
{
	std::fill(m_MinHSI, m_MinHSI + 3, 1);
	std::fill(m_MaxHSI, m_MaxHSI + 3, 0);
	std::fill(m_pDepthContourMax, m_pDepthContourMax + 5, nullptr);

	m_RGBStorage = cvCreateMemStorage(0);
	m_DepthStorage = cvCreateMemStorage(0);
};

Image::
~Image()
{
	cvReleaseImage(&m_pRGBImage);
	cvReleaseImage(&m_pHSIImage);
	cvReleaseImage(&m_pThresholdImage);
	if (m_pDepthImage != nullptr)
		m_pDepthImage->release();

	cvReleaseMemStorage(&m_RGBStorage);
	cvReleaseMemStorage(&m_DepthStorage);
};

bool Image::
CheckPoint(K_Camera & camera)
{
	if (m_pRGBContourMax == nullptr)
	{
		return false;
	}
	CvRect aRect = { -1, -1 };
	CvPoint2D32f Point = { -1, -1 };
	FindBall = -1;
	int DepthX = 0, DepthY = 0, idx = 0, tempW = 0, tempH = 0;

	for (int i = 4; i >= 0; i--)
	{
		if (m_pDepthContourMax[i] == nullptr)
			continue;
		aRect = cvBoundingRect(m_pDepthContourMax[i], 0);
		DepthX = (aRect.x + 1 / 2 * aRect.width);
		DepthY = (aRect.y + 1 / 2 * aRect.height);
		idx = DepthY * 640 + DepthX;
		tempW = 1 / 4 * aRect.width;
		tempH = 1 / 4 * aRect.height;

		CoordinateConverter::convertDepthToColor(*(camera.GetDepthStream()),/*oniDepthStream,*/
			/*oniColorStream,*/*(camera.GetColorStream()),
			DepthX, DepthY, /*m_pDepth[idx],*/camera.GetDepthMsg(idx),
			&DepthX, &DepthY);


		Point.x = (float)DepthX;
		Point.y = (float)DepthY;

		if (cvPointPolygonTest(m_pRGBContourMax, Point, 1) >= 0)
		{
			FindBall = i;
			break;
		}

		Point.x = (float)(DepthX - tempW);
		Point.y = (float)(DepthY - tempH);

		if (cvPointPolygonTest(m_pRGBContourMax, Point, 1) >= 0)
		{
			FindBall = i;
			break;
		}

		Point.x = (float)(DepthX + tempW);
		Point.y = (float)(DepthY - tempH);

		if (cvPointPolygonTest(m_pRGBContourMax, Point, 1) >= 0)
		{
			FindBall = i;
			break;
		}

		Point.x = (float)(DepthX - tempW);
		Point.y = (float)(DepthY + tempH);

		if (cvPointPolygonTest(m_pRGBContourMax, Point, 1) >= 0)
		{
			FindBall = i;
			break;
		}

		Point.x = (float)(DepthX + tempW);
		Point.y = (float)(DepthY + tempH);

		if (cvPointPolygonTest(m_pRGBContourMax, Point, 1) >= 0)
		{
			FindBall = i;
			break;
		}
	}

	if (FindBall < 0 )
		return false;

	DrawCountour();

	return true;
}

void Image::
Clear()
{
	if (m_pThresholdImage != nullptr)
		cvReleaseImage(&m_pThresholdImage);
	if (m_pHSIImage != nullptr)
		cvReleaseImage(&m_pHSIImage);

	std::fill(m_pDepthContourMax, m_pDepthContourMax + 5, nullptr);
	m_pRGBContourMax = nullptr;
}

void Image::
DrawCountour()
{
	//此处可获得连通域中心坐标
	CvRect m_Rect = { -1, -1, 0, 0 };				/*彩色图最大连通域轮廓的外接矩形*/

	m_Rect = cvBoundingRect(m_pDepthContourMax[FindBall], 0);
	m_Center.x = m_Rect.x + 1 / 2 * m_Rect.width;
	m_Center.y = m_Rect.y + 1 / 2 * m_Rect.height;
	
	cvDrawContours(m_pRGBImage, m_pDepthContourMax[FindBall],
		CV_RGB(255, 0, 0), CV_RGB(255, 0, 0),
		0, 2, 8, cvPoint(0, 0));
}

bool Image::
SetRGBImage(IplImage* pic)
{
	m_pRGBImage = pic;

	cvCvtColor(m_pRGBImage, m_pRGBImage, CV_RGB2BGR);

	m_Height = pic->height;
	m_Width = pic->width;

	RGBToHSI();
	if (!ThresholdImage())
		return false;

	

	GetMaxDomain();

	return true;
}

bool Image::
SetHsvVal(int min[3], int max[3])
{
	if (min[0] > max[0] || min[1] > max[1] || min[2] > max[2])
		return false;

	m_MinHSI[0] = min[0];
	m_MinHSI[1] = min[1];
	m_MinHSI[2] = min[2];
	m_MaxHSI[0] = max[0];
	m_MaxHSI[1] = max[1];
	m_MaxHSI[2] = max[2];

	return true;
}

bool Image::
SetRGBMaxDomainVal(int min, int max)
{
	if (min > max)
		return false;
	m_RGBDomainMin = min;
	m_RGBDomainMax = max;

	return true;
}

IplImage* Image::
GetRGBImage()
{
	return m_pRGBImage;
}

IplImage* Image::
GetThresholdImage()
{
	return m_pThresholdImage;
}

bool Image::
RGBToHSI()
{
	IplImage *H = cvCreateImage(cvGetSize(m_pRGBImage), 8, 1);
	IplImage *S = cvCreateImage(cvGetSize(m_pRGBImage), 8, 1);
	IplImage *I = cvCreateImage(cvGetSize(m_pRGBImage), 8, 1);
	m_pHSIImage = cvCreateImage(cvGetSize(m_pRGBImage), 8, 3);

	cvCvtColor(m_pRGBImage, m_pHSIImage, CV_RGB2HSV);
	cvSplit(m_pHSIImage, H, S, nullptr, nullptr);

	uchar *img_p, *I_p;
	img_p = cvPtr2D(m_pRGBImage, 0, 0);
	I_p = cvPtr2D(I, 0, 0);

	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			*(I_p) = (*(img_p)+*(img_p + 1) + *(img_p + 2)) / 3;
			if (!(i == m_Width - 1 && j == m_Height - 1))
			{
				img_p += 3;
				I_p += 1;
			}
		}
	}
	cvMerge(H, S, I, nullptr, m_pHSIImage);

	cvReleaseImage(&H);
	cvReleaseImage(&S);
	cvReleaseImage(&I);

	return true;
}

bool Image::
ThresholdImage()
{
	CvScalar m_HsiMin;          /*Hsi三通道阈值的最小值*/
	CvScalar m_HsiMax;          /*Hsi三通道阈值的最大值*/

	for (int i = 0; i<3; i++)
		if (m_MinHSI[i] > m_MaxHSI[i] || m_MaxHSI[i] > 255 || m_MinHSI[i] < 0)
			return false;

	m_pThresholdImage = cvCreateImage(cvGetSize(m_pHSIImage), 8, 1);

	m_HsiMin = cvScalar(m_MinHSI[0], m_MinHSI[1], m_MinHSI[2], 0);
	m_HsiMax = cvScalar(m_MaxHSI[0], m_MaxHSI[1], m_MaxHSI[2], 0);

	cvInRangeS(m_pHSIImage, m_HsiMin, m_HsiMax, m_pThresholdImage);

	return true;
}

void Image::
GetMaxDomain()
{
	bool Flag = false;

	cvClearMemStorage(m_RGBStorage);

	CvSeq *contour = 0;

	int mode = CV_RETR_EXTERNAL;
	double maxArea = 10;
	double area;

	/*此处会将原来的二值化图变成轮廓图*/
	cvFindContours(m_pThresholdImage, m_RGBStorage, &contour, sizeof(CvContour), mode, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	for (; contour; contour = contour->h_next)
	{
		area = fabs(cvContourArea(contour, CV_WHOLE_SEQ));

		if (area <= m_RGBDomainMax && area >= m_RGBDomainMin)
		{
			if (area > maxArea)
			{
				m_pRGBContourMax = contour;
				maxArea = area;
			}

			Flag = true;
		}
	}

	///*此处为绘制彩色图最大连通域轮廓*/
	//if (Flag == true)
	//{
	//	cvDrawContours(m_pRGBImage, m_pRGBContourMax,
	//		CV_RGB(0, 0, 255), CV_RGB(0, 0, 255),
	//		0, 2, 8, cvPoint(0, 0));
	//}
}

bool Image::
SetDepthImage(Mat* pic)
{
	m_pDepthImage = pic;
	CutDepthPic();

	return true;
}

bool Image::
SetDepthMaxDomainVal(int min, int max)
{
	m_DepthDomainMin = min;
	m_DepthDomainMax = max;

	return true;
}

Mat* Image::
GetDepthImage()
{
	return m_pDepthImage;
}

bool Image::
CutDepthPic()
{
	Mat frame[m_DepthLayerQuanlity];

	for (int i = 0; i < m_DepthLayerQuanlity; i++)
	{
		frame[i] = Mat(480, 640, CV_8UC1, Scalar::all(0.0));
	}

	CV_Assert(m_pDepthImage->depth() != sizeof(uchar));
	int channels = m_pDepthImage->channels();
	int nRows = m_pDepthImage->rows;
	int nCols = m_pDepthImage->cols* channels;
	if (m_pDepthImage->isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}
	int i, j, k;

	DepthPixel* p;
	uchar* ptr[m_DepthLayerQuanlity];

	for (i = 0; i < nRows; ++i)
	{
		p = m_pDepthImage->ptr<DepthPixel>(i);
		for (k = 0; k < m_DepthLayerQuanlity; k++)
		{
			ptr[k] = frame[k].ptr<uchar>(i);
		}
		for (j = 0; j < nCols; ++j)
		{
			if (p[j] != 0)
			{
				for (k = 0; k < m_DepthLayerQuanlity; k++)
				{
					if (p[j] > 500 + Depth_Distance*k && p[j] < 500 + Depth_Distance*(k + 1))
					{
						ptr[k][j] = 255;
						break;
					}
				}
			}
		}
	}

	if (FindContour(frame))
		return true;

	return false;
}

bool Image::
FindContour(Mat frame[])
{
	bool Flag = false;
	int mode = CV_RETR_EXTERNAL;
	double area;
	double maxArea[5] = { 6, 7, 8, 9, 10 };

	cvClearMemStorage(m_DepthStorage);
	CvSeq *contour = 0;

	IplImage* pDst = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 1);
	IplImage* pContourImg = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);
	IplImage* pImg = nullptr;

	for (int i = 0; i < 5; i++)
		m_pDepthContourMax[i] = nullptr;

	for (int j = 0; j < m_DepthLayerQuanlity; j++)
	{
		contour = 0;
		pImg = &IplImage(frame[j]);
		cvCvtColor(pImg, pContourImg, CV_GRAY2BGR);
		cvFindContours(pImg, m_DepthStorage, &contour, sizeof(CvContour), mode, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

		for (; contour; contour = contour->h_next)
		{
			area = fabs(cvContourArea(contour, CV_WHOLE_SEQ)); //获取当前轮廓面积
			if (area <= m_DepthDomainMax && area >= m_DepthDomainMin)
			{
				if (area > maxArea[4])
				{
					m_pDepthContourMax[4] = contour;
					maxArea[4] = area;
				}
				else if (area > maxArea[3])
				{
					m_pDepthContourMax[3] = contour;
					maxArea[3] = area;
				}
				else if (area > maxArea[2])
				{
					m_pDepthContourMax[2] = contour;
					maxArea[2] = area;
				}
				else if (area > maxArea[1])
				{
					m_pDepthContourMax[1] = contour;
					maxArea[1] = area;
				}
				else if (area > maxArea[0])
				{
					m_pDepthContourMax[0] = contour;
					maxArea[0] = area;
				}
			}
		}
	}

	for (int i = 0; i < m_DepthLayerQuanlity; i++)
		frame[i].release();

	cvReleaseImage(&pDst);
	cvReleaseImage(&pContourImg);
	return true;
}