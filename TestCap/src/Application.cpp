#include "ScreenCapture.h"
#include <iostream>
#include <opencv2/core.hpp>

int main() 
{
	screencapture::PrintMessage();
	cv::Mat buf = screencapture::ToMemory();
	cv::imwrite("capt.png", buf);
	screencapture::readImg(buf);

	return 0;
}