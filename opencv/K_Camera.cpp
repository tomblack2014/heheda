#include "K_Camera.h"

K_Camera::
	K_Camera()
{};

K_Camera::
	~K_Camera()
{
	oniDepthStream.destroy();
	oniColorStream.destroy();
	device.close();
	OpenNI::shutdown();
}

bool K_Camera::
	OpenKinect()
{
	Status rc = STATUS_OK;
	rc = OpenNI::initialize();
	
	const char * deviceURL = openni::ANY_DEVICE;
	rc = device.open(deviceURL);
	if (STATUS_OK != rc)
	{
		cerr << "无法打开设备：" << OpenNI::getExtendedError() << endl;
		return false;
	}

	rc = oniDepthStream.create(device, SENSOR_DEPTH);
	if( STATUS_OK == rc )
	{
		modeDepth.setResolution(640,480);
		modeDepth.setFps(30); 
		modeDepth.setPixelFormat(PIXEL_FORMAT_DEPTH_1_MM); 
		oniDepthStream.setVideoMode(modeDepth);

		oniDepthStream.start(); 
		if(STATUS_OK !=  rc)
		{
			cerr << "无法打开深度数据流："<<OpenNI::getExtendedError()<<endl;
			oniDepthStream.destroy();
			return false;
		}
	}
	else
	{
		cerr << "无法创建深度数据流："<<OpenNI::getExtendedError()<<endl;
		return false;
	}


	rc = oniColorStream.create(device, SENSOR_COLOR);
	if(STATUS_OK == rc)
	{
		modeColor.setResolution(640,480);
		modeColor.setFps(30);
		modeColor.setPixelFormat(PIXEL_FORMAT_RGB888);

		if(device.isImageRegistrationModeSupported(IMAGE_REGISTRATION_DEPTH_TO_COLOR))
		{
			device.setImageRegistrationMode(IMAGE_REGISTRATION_DEPTH_TO_COLOR); 
		}

		rc = oniColorStream.start(); 
		if( STATUS_OK != rc )
		{
			cerr<< "无法打开彩色数据流："<<OpenNI::getExtendedError()<<endl;
			oniColorStream.destroy();
			return false;
		}
	}
	else
	{
		cerr << "无法创建彩色数据流："<<OpenNI::getExtendedError()<<endl;
		return false;
	}

	if (!oniDepthStream.isValid() || !oniColorStream.isValid())
	{
		cerr << "彩色或深度数据流不合法"<<endl;
		OpenNI::shutdown();
		return false;
	}

	return true;
}
	
Mat* K_Camera::
	GetDepthImg()
{
	//printf("GetDepthImg\n");

	if(STATUS_OK == oniDepthStream.readFrame(&oniDepthImg))
	{
		//oniDepthStream.readFrame(&oniDepthImg);
		cvDepthImg = Mat(oniDepthImg.getHeight(), oniDepthImg.getWidth(), CV_16UC1, (void*)oniDepthImg.getData());
		m_pDepth = (DepthPixel *)oniDepthImg.getData();
		return &cvDepthImg;
	}
	return nullptr;
}

Mat* K_Camera::
	GetBGRImg()
{
	//printf("GetBGRImg\n");

	if(STATUS_OK == oniColorStream.readFrame(&oniColorImg))
	{
		//oniColorStream.readFrame(&oniColorImg);
		cvBGRImg = Mat(oniColorImg.getHeight(), oniColorImg.getWidth(), CV_8UC3, (void*)oniColorImg.getData());
		return &cvBGRImg;
	}
	return nullptr;
}

DepthPixel K_Camera::
GetDepthMsg(int idx)
{
	return m_pDepth[idx];
}

VideoStream * K_Camera::
GetDepthStream()
{
	return &oniDepthStream;
}
VideoStream * K_Camera::
GetColorStream()
{
	return &oniColorStream;
}