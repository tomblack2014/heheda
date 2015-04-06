//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/opencv.hpp>
//#include <iostream>
////#include "Picture.h"
//#include "OpenNI.h"
//
//using namespace openni;
//using namespace cv;
//using namespace std;
//
//
//void RGBtoHSI(IplImage* img,IplImage* image_hsi);
//void on_mouse(int event, int x, int y, int flags, void* param);
//void GetMaxMin(IplImage* image,CvRect zone,int min[],int max[]);
//void DrawHistogram(IplImage* picture,IplImage* mask,IplImage* p,CvRect zone);
//IplImage* EqualizeRgb(IplImage* img);
//int KeyboardHandle(char c);
//Mat* DepthCorrect(Mat* img);
//
//IplImage *frame = 0,*image = 0,*pic = 0; 
//
//int select_object = 0;
//int track_object = 0;
//int hdims = 51;
//int min[3] = {0,0,0},max[3] = {0,0,0};
//float DepthCoe = 1;
//float hranges_arr[] = {0,256};
//float* hranges = hranges_arr;
//
//CvPoint origin;
//CvRect selection;
//
//VideoStream oniColorStream;  //RGB数据流
//VideoStream oniDepthStream; //深度数据流
//
//void on_mouse(int event, int x, int y, int flags, void* param)
//{
//	if (!image)
//		return;
//
//	if (image->origin)
//		y = image->height - y;
//
//	if (select_object)
//	{
//		selection.x = MIN(x, origin.x);
//		selection.y = MIN(y, origin.y);
//		selection.width = selection.x + CV_IABS(x - origin.x);
//		selection.height = selection.y + CV_IABS(y - origin.y);
//
//		selection.x = MAX(selection.x, 0);
//		selection.y = MAX(selection.y, 0);
//		selection.width = MIN(selection.width, image->width);
//		selection.height = MIN(selection.height, image->height);
//		selection.width -= selection.x;
//		selection.height -= selection.y;
//	}
//
//	switch (event)
//	{
//	case CV_EVENT_LBUTTONDOWN:
//		origin = cvPoint(x, y);
//		selection = cvRect(x, y, 0, 0);
//		select_object = 1;
//		break;
//	case CV_EVENT_LBUTTONUP:
//		select_object = 0;
//		if (selection.width > 0 && selection.height > 0)
//		{
//			cvRectangle(image,cvPoint(selection.x,selection.y),cvPoint(selection.x + selection.width,selection.y + selection.height),cvScalar(150,150,150),8);
//			track_object = -1;
//		}
//		break;
//	}
//}
//
//void GetMaxMin(IplImage* image,CvRect zone,int min[],int max[])
//{
//	IplImage *framed = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 3);
//	IplImage *frame0 = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1);
//    IplImage *frame1 = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1);
//	IplImage *frame2 = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1);
//    double tmin[3] = {0,0,0},tmax[3] = {0,0,0};
//
//	RGBtoHSI(image,framed);
//	//cvCvtColor(image, framed, CV_BGR2HSV);
//	//cvCopy(image,framed);
//
//	cvSplit(framed,frame0,frame1,frame2,NULL);
//    cvSetImageROI(frame0,zone);
//    cvSetImageROI(frame1,zone);
//    cvSetImageROI(frame2,zone);
//
//    cvMinMaxLoc(frame0,&tmin[0],&tmax[0],NULL,NULL,NULL);
//    cvMinMaxLoc(frame1,&tmin[1],&tmax[1],NULL,NULL,NULL);
//    cvMinMaxLoc(frame2,&tmin[2],&tmax[2],NULL,NULL,NULL);
//	
//	min[0] = (int)tmin[0]; min[1] = (int)tmin[1]; min[2] = (int)tmin[2];
//	max[0] = (int)tmax[0]; max[1] = (int)tmax[1]; max[2] = (int)tmax[2];
//
//	cvSetTrackbarPos("hmax","H",max[0]);
//	cvSetTrackbarPos("hmin","H",min[0]);
//	cvSetTrackbarPos("smax","S",max[1]);
//	cvSetTrackbarPos("smin","S",min[1]);
//	cvSetTrackbarPos("imax","I",max[2]);
//	cvSetTrackbarPos("imin","I",min[2]);
//
//	printf("%d %d %d %d %d %d\n",min[0],min[1],min[2],max[0],max[1],max[2]);
//
//	cvReleaseImage(&framed);
//	cvReleaseImage(&frame0);
//	cvReleaseImage(&frame1);
//	cvReleaseImage(&frame2);
//}
//
//void DrawHistogram(IplImage* picture,IplImage* mask,IplImage* p,CvRect zone)
//{
//	CvHistogram *hist = cvCreateHist(1,&hdims,CV_HIST_ARRAY,&hranges,1);
//	/*IplImage* histpicture = cvCreateImage( cvSize(320,200), 8, 3);*/
//	CvScalar color = cvScalar(255,255,255);
//	float max_val = 0.f;
//	int bin_w = p->width / hdims;
//
//	cvSetImageROI(picture,zone);
//	cvSetImageROI(mask,zone);
//
//	cvCalcHist( &picture, hist, 0, mask );
//
//	cvGetMinMaxHistValue(hist,0,&max_val,0,0);
//
//	cvConvertScale(hist->bins,hist->bins,max_val ? 255./max_val:0.,0);
//
//	cvResetImageROI(picture);
//	cvResetImageROI(mask);
//
//	//cvZero(histpicture);
//
//	for(int i = 0; i < hdims; i++ )
//    {
//        int val = cvRound( cvGetReal1D(hist->bins,i)*p->height/255 );
//        cvRectangle(p, cvPoint(i*bin_w,p->height),
//        cvPoint((i+1)*bin_w,p->height - val),color, -1, 8, 0 );
//    }
//
//	cvReleaseHist(&hist);
//}
//
//int KeyboardHandle(char c)
//{
//	if(c == 27)
//		return 1;
//    if(c == 'z' || c == 'Z')
//	{
//		if(selection.width > 0 && selection.height > 0)
//		{
//			selection.x = selection.x + selection.width/8;
//	    	selection.y = selection.y + selection.height/8;
//		    selection.width = selection.width*6/8;
//		    selection.height = selection.height*6/8;
//		}
//		return 0;
//	}
//	else if(c == 'x' || c == 'X')
//	{
//		if(selection.x - selection.width/6 >= 0)
//			selection.x = selection.x - selection.width/6;
//		else
//			selection.x = 0;
//		if(selection.y - selection.height/6 >= 0)
//			selection.y = selection.y - selection.height/6;
//		else
//			selection.y = 0;
//		if(selection.width*8/6 <= image->width)
//			selection.width = selection.width*8/6;
//		else
//			selection.width = image->width;
//		if(selection.height*8/6 <= image->height)
//			selection.height = selection.height*8/6;
//		else
//			selection.height = image->height;	
//		return 0;
//	}
//	else if(c == 'a' || c == 'A')
//	{
//		if(selection.x - selection.width/8 >= 0)
//			selection.x = selection.x - selection.width/8;
//		else
//			selection.x = 0;
//	}
//	else if(c == 'd' || c == 'D')
//	{
//		if(selection.x + selection.width*9/8 <= image->width)
//			selection.x = selection.x + selection.width/8;
//		else
//			selection.x = image->width - selection.width;
//	}
//	else if(c == 'w' || c == 'W')
//	{
//		if(selection.y - selection.height/8 >= 0)
//			selection.y = selection.y - selection.height/8;
//		else
//			selection.y = 0;
//	}
//	else if(c == 'w' || c == 'W')
//	{
//		if(selection.y + selection.height*9/8 <= image->height)
//			selection.y = selection.y + selection.height/8;
//		else 
//			selection.y = image->height - selection.height;
//	}
//	return -1;
//}
//
//void RGBtoHSI(IplImage* img,IplImage* image_hsi)
//{ 
//
// IplImage *H  = cvCreateImage( cvGetSize(img),8, 1);
// IplImage *S  = cvCreateImage( cvGetSize(img),8, 1);
// IplImage *I  = cvCreateImage( cvGetSize(img),8, 1);
// IplImage *temp = cvCreateImage( cvGetSize(img),8, 3);
//
// cvCvtColor(img,temp,CV_RGB2HSV);
// cvSplit(temp,H,S,NULL,NULL);
//
// //CvScalar p;
// uchar *img_p,*I_p;
// img_p =  cvPtr2D(img,0,0);
// I_p = cvPtr2D(I,0,0);
//
// for(int i = 0;i < img->width;i++)
// {
//	 for(int j = 0;j < img->height;j++)
//	 {
//		/* p = cvGet2D(img, j, i);
//		 cvSet2D(I,j,i,cvScalar((p.val[0] + p.val[1] + p.val[2])/3,0,0));*/
//		 /*q = cvPtr2D(img,j,i);
//		 *(cvPtr2D(I,j,i)) = (*(q) + *(q + sizeof(uchar)) + *(q + 2*sizeof(uchar)))/3;*/
//		 *(I_p) = (*(img_p) + *(img_p + sizeof(uchar)) + *(img_p + 2*sizeof(uchar)))/3;
//		 if(!(i == img->width - 1 && j == img->height -1))
//		 {  
//			 img_p += 3;
//			 I_p += 1;
//		 }
//	 }
// }
//
// //cvEqualizeHist(H,H);
// cvMerge(H,S,I,NULL,image_hsi);
//
// cvReleaseImage(&H);
// cvReleaseImage(&S);
// cvReleaseImage(&I);
// cvReleaseImage(&temp);
//} 
//
///**
// *   Corrent Depth
// */
//Mat* DepthCorrect(Mat* cvDepthImg)
//{
//	int i = 0,j = 0,x = 0,y = 0;
//	
//	Mat* cvTemImg = 0;
//	cvZero(cvTemImg);
//	/*printf("%d\n",cvTemImg->depth);*/
//	imshow("p",*cvDepthImg);
//
//	uchar* pDepth = cvPtr2D(cvDepthImg,0,0);
//
//	for(;i < cvDepthImg->rows;i++)
//	{
//		for(;j < cvDepthImg->cols;j++)
//		{
//			CoordinateConverter::convertDepthToColor(oniDepthStream,
//                            oniColorStream,
//							i,j, *pDepth, &x, &y);
//			if(*pDepth != 0 && *pDepth < 255)
//			{
//				*cvPtr2D(cvTemImg,x,y) = *pDepth;
//		     	cout << *pDepth << endl;
//			}
//			pDepth ++;
//		}
//	}
//	/*cvReleaseImage(&cvDepthImg);*/
//	return cvTemImg;
//}
//
//
//
///**
// *   testing
// */
////IplImage* EqualizeRgb(IplImage* img)
////{
////	IplImage* R = cvCreateImage(cvGetSize(img),8,1);
////	IplImage* G = cvCreateImage(cvGetSize(img),8,1);
////	IplImage* B = cvCreateImage(cvGetSize(img),8,1);
////	IplImage* RGB = cvCreateImage(cvGetSize(img),8,3);
////
////	cvSplit(img,R,G,B,NULL);
////	cvEqualizeHist(R,R);
////	cvEqualizeHist(G,G);
////	cvEqualizeHist(B,B);
////
////	cvReleaseImage(&img);
////	cvMerge(R,G,B,NULL,RGB);
////	return RGB;
////}
//
//int main()
//{
//	Status rc = STATUS_OK; // OpenNI函数执行结果
//
//	VideoFrameRef oniDepthImg, oniColorImg;
//
//	//Mat cvDepthImg, cvBGRImg, cvFusionImg;
//	IplImage* cvDepthImg = 0;
//	IplImage* cvRGBImg = 0;
//	IplImage* cvFusionImg = 0;
//	Mat cvRawImg16U;
//	Mat* DepthMat = 0;
//
//	rc = OpenNI::initialize();
//
//	Device device;
//	const char * deviceURL = openni::ANY_DEVICE;  //设备名
//	rc = device.open(deviceURL);
//
//	//VideoStream oniDepthStream; //深度数据流
//	rc = oniDepthStream.create(device, SENSOR_DEPTH); //创建深度数据流
//	if( STATUS_OK == rc )
//	{
//		VideoMode modeDepth;
//		modeDepth.setResolution(/*320,240*/640,480); //分辨率
//		modeDepth.setFps(30); //帧率
//		modeDepth.setPixelFormat(PIXEL_FORMAT_DEPTH_1_MM); //深度像素格式
//		oniDepthStream.setVideoMode(modeDepth);
//
//		oniDepthStream.start(); // 打开深度数据流
//		if(STATUS_OK !=  rc)
//		{
//			cerr << "无法打开深度数据流："<<OpenNI::getExtendedError()<<endl;
//			oniDepthStream.destroy();
//		}
//	}
//	else
//	{
//		cerr << "无法创建深度数据流："<<OpenNI::getExtendedError()<<endl;
//	}
//
//	//VideoStream oniColorStream;  //RGB数据流
//	rc = oniColorStream.create(device, SENSOR_COLOR);
//	if(STATUS_OK == rc)
//	{
//		VideoMode modeColor;
//		modeColor.setResolution(640,480/*320,240*//*1280,1040*/);//分辨率
//		modeColor.setFps(30);//帧率
//		modeColor.setPixelFormat(PIXEL_FORMAT_RGB888);
//
//		if(device.isImageRegistrationModeSupported(IMAGE_REGISTRATION_DEPTH_TO_COLOR))
//		{
//			device.setImageRegistrationMode(IMAGE_REGISTRATION_DEPTH_TO_COLOR); //深度到彩色图配准
//		}
//
//		rc = oniColorStream.start(); //打开彩色数据流
//		if( STATUS_OK != rc )
//		{
//			cerr<< "无法打开彩色数据流："<<OpenNI::getExtendedError()<<endl;
//			oniColorStream.destroy();
//		}
//	}
//	else
//	{
//		cerr << "无法创建彩色数据流："<<OpenNI::getExtendedError()<<endl;
//	}
//
//	if (!oniDepthStream.isValid() || !oniColorStream.isValid())
//	{
//		cerr << "彩色或深度数据流不合法"<<endl;
//		OpenNI::shutdown();
//		return 1;
//	}
//
//	//Picture m_pic;
//	int min[3] = {0,173,0},max[3] = {100, 253, 255},bin_w;
//	//m_pic.SetHsvVal(min,max);
//
//	IplImage *frame = 0,* pic1 = 0,*h = 0,*s = 0,*i = 0,*hsi = 0,*mask1 = 0,*mask2 = 0,*mask3 = 0;
//	int val;
//	int h_max = 0,h_min = 0,s_max = 0,s_min = 0,i_max = 0,i_min = 0;
//
//	CvScalar hsv_min,hsv_max;
//
//	
//	cvNamedWindow("CamShiftDemo", 1);
//	cvNamedWindow("pic",CV_WINDOW_AUTOSIZE);
//	cvNamedWindow("H",CV_WINDOW_AUTOSIZE);
//	cvNamedWindow("S",CV_WINDOW_AUTOSIZE);
//	cvNamedWindow("I",CV_WINDOW_AUTOSIZE);
//
//	cvCreateTrackbar("hmax","H",&h_max,360,0);
//	cvCreateTrackbar("hmin","H",&h_min,360,0);
//	cvCreateTrackbar("smax","S",&s_max,255,0);
//	cvCreateTrackbar("smin","S",&s_min,255,0);
//	cvCreateTrackbar("imax","I",&i_max,255,0);
//	cvCreateTrackbar("imin","I",&i_min,255,0);
//
//
//	cvSetMouseCallback("CamShiftDemo", on_mouse, 0);
//
//	if( STATUS_OK == oniColorStream.readFrame(&oniColorImg))
//		{
//			cvRawImg16U = Mat(oniColorImg.getHeight(), oniColorImg.getWidth(), CV_8UC3, (void*)oniColorImg.getData());
//			//cvRGBImg = cvCreateImage(cvSize(oniColorImg.getHeight(),oniColorImg.getWidth()),8,3);
//			frame = &IplImage(cvRawImg16U);
//			cvCvtColor(frame, frame, CV_RGB2BGR);
//			//cvShowImage("BGR",cvRGBImg);
//		}
//	if (!frame)
//		return 0;
//
//	image = cvCreateImage(cvGetSize(frame), 8, 3);
//	pic1 = cvCreateImage(cvGetSize(frame), 8, 3);
//	pic = cvCreateImage(cvGetSize(frame), 8, 1);
//	hsi = cvCreateImage( cvGetSize(frame), 8, 3);
//	mask1 = cvCreateImage( cvGetSize(frame), 8, 1 );
//	mask2 = cvCreateImage( cvGetSize(frame), 8, 1 );
//	mask3 = cvCreateImage( cvGetSize(frame), 8, 1 );
//	h = cvCreateImage( cvGetSize(frame), 8, 1 );
//	s = cvCreateImage( cvGetSize(frame), 8, 1 );
//	i = cvCreateImage( cvGetSize(frame), 8, 1 );
//
//	while(1)
//	{
//		/*frame = cvQueryFrame(capture);*/
//
//		if( STATUS_OK == oniColorStream.readFrame(&oniColorImg))
//		{
//			cvRawImg16U = Mat(oniColorImg.getHeight(), oniColorImg.getWidth(), CV_8UC3, (void*)oniColorImg.getData());
//			//cvRGBImg = cvCreateImage(cvSize(oniColorImg.getHeight(),oniColorImg.getWidth()),8,3);
//			frame = &IplImage(cvRawImg16U);
//			/*frame = EqualizeRgb(frame);*/
//
//			cvCvtColor(frame, frame, CV_RGB2BGR);
//			//imshow("hehe",cvRawImg16U);
//			//cvShowImage("pp",frame);
//			//cvShowImage("BGR",cvRGBImg);
//		}
//
//		if (!frame)
//			break;
//		cvCopy(frame, image, 0);
//
//		if( STATUS_OK == oniDepthStream.readFrame(&oniDepthImg))
//		{
//			cvRawImg16U = Mat(oniDepthImg.getHeight(), oniDepthImg.getWidth(), CV_16UC1, (void*)oniDepthImg.getData());
//			DepthCoe = 255.0/(oniDepthStream.getMaxPixelValue());
//			cvRawImg16U.convertTo(cvRawImg16U, CV_8U, DepthCoe); /*这里的图像像素数据类型也已经改变*/
//			//cvDepthImg = cvCreateImage(cvSize(oniDepthImg.getHeight(),oniDepthImg.getWidth()),16,1);
//			/*imshow("q",cvRawImg16U);
//			DepthMat = DepthCorrect(&cvRawImg16U);*/
//			cvDepthImg = &IplImage(cvRawImg16U);
//		}
//		//cvDepthImg = DepthCorrect(cvDepthImg);
//		cvShowImage("Depth",cvDepthImg);
//
//		max[0] = cvGetTrackbarPos("hmax","H");
//    	min[0] = cvGetTrackbarPos("hmin","H");
//    	max[1] = cvGetTrackbarPos("smax","S");
//	    min[1] = cvGetTrackbarPos("smin","S");
//	    max[2] = cvGetTrackbarPos("imax","I");
//	    min[2] = cvGetTrackbarPos("imin","I");
//
//		IplImage* image_h = cvCreateImage( cvSize(320,200), 8, 3 );
//	    IplImage* image_s = cvCreateImage( cvSize(320,200), 8, 3);
//	    IplImage* image_i = cvCreateImage( cvSize(320,200), 8, 3 );
//
//		DrawHistogram(h,mask1,image_h,selection);
//		DrawHistogram(s,mask2,image_s,selection);
//		DrawHistogram(i,mask3,image_i,selection);
//
//		cvLine(image_h,cvPoint(min[0]*image_h->width/360,0),cvPoint(min[0]*image_h->width/360,image_h->height),cvScalar(0,0,0),2);
//		cvLine(image_h,cvPoint(max[0]*image_h->width/360,0),cvPoint(max[0]*image_h->width/360,image_h->height),cvScalar(0,0,0),2);
//		cvLine(image_s,cvPoint(min[1]*image_s->width/255,0),cvPoint(min[1]*image_s->width/255,image_s->height),cvScalar(0,0,0),2);
//		cvLine(image_s,cvPoint(max[1]*image_s->width/255,0),cvPoint(max[1]*image_s->width/255,image_s->height),cvScalar(0,0,0),2);
//		cvLine(image_i,cvPoint(min[2]*image_i->width/255,0),cvPoint(min[2]*image_i->width/255,image_i->height),cvScalar(0,0,0),2);
//		cvLine(image_i,cvPoint(max[2]*image_i->width/255,0),cvPoint(max[2]*image_i->width/255,image_i->height),cvScalar(0,0,0),2);
//
//		hsv_min = cvScalar(min[0],min[1],min[2], 0);
//        hsv_max = cvScalar(max[0],max[1],max[2], 0);
//
//		RGBtoHSI(image,pic1);
//		//cvCvtColor(image, pic1, CV_BGR2HSV);
//		//cvCopy(image,pic1);
//
//		cvInRangeS(pic1,hsv_min,hsv_max,pic);
//
//		if(selection.width > 0 && selection.height > 0 )
//		{
//			cvRectangle(image,cvPoint(selection.x,selection.y),cvPoint(selection.x + selection.width,selection.y + selection.height),cvScalar(150,150,150),2);
//		}
//		if(track_object == -1)
//		{
//			GetMaxMin(frame,selection,min,max);
//
//
//			cvInRangeS(pic1, cvScalar(0,0,0,0),
//                        cvScalar(360,255,255,0), mask1 );
//			cvCopy(mask1,mask2);
//			cvCopy(mask1,mask3);
//
//			cvSplit(pic1,h,s,i,0);
//
//			DrawHistogram(h,mask1,image_h,selection);
//			DrawHistogram(s,mask2,image_s,selection);
//		    DrawHistogram(i,mask3,image_i,selection);
//
//			track_object = 0;
//		}
//
//		cvShowImage("H",image_h);
//		cvShowImage("S",image_s);
//		cvShowImage("I",image_i);
//		cvShowImage("pic",pic);
//		cvShowImage("CamShiftDemo",image);
//
//		int flag = KeyboardHandle(cvWaitKey(10));
//		if(flag == 1)
//			break;
//		else if(flag == 0)
//		{
//			GetMaxMin(frame,selection,min,max);
//		}
//		cvReleaseImage(&image_h);
//		cvReleaseImage(&image_s);
//		cvReleaseImage(&image_i);
//	}
//
//	cvDestroyAllWindows();
//	oniDepthStream.destroy();
//	oniColorStream.destroy();
//	device.close();
//	OpenNI::shutdown();
//
//	return 0;
//}
//
//
//
//
