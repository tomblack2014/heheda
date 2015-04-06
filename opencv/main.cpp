#include "Image.h"
#include "K_Camera.h"
#include "GoalKeeper.h"

int main()
{
	K_Camera tempCamera;
	Image tempImage;
	
	int min[3] = { 59, 154, 0 }, max[3] = { 108, 255, 255 };
	
	tempImage.SetHsvVal(min, max);
	tempImage.SetRGBMaxDomainVal(1000, 50000);
	tempImage.SetDepthMaxDomainVal(1000, 50000);
	
	if (!tempCamera.OpenKinect()){
		std::cout << "´ò¿ªKinectÊ§°Ü£¡\n";
		return 0;
	}
	
	while(true){
		tempImage.Clear();
		tempImage.SetRGBImage(&(IplImage)*tempCamera.GetBGRImg());
		tempImage.SetDepthImage(tempCamera.GetDepthImg());
		
		if(!tempImage.CheckPoint(tempCamera)){
			cout << "Lose Ball" << endl;
		}
		
		cvShowImage("RGB",tempImage.GetRGBImage());
		cvShowImage("Threshold", tempImage.GetThresholdImage());
		
		cvWaitKey(30);
	}

	cvDestroyAllWindows();
	return 0;
}