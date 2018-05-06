#pragma once
#include <iostream>
#include <windows.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

namespace screencapture{
	void PrintMessage();
	void screenshot(POINT a, POINT b);
	cv::Mat ToMemory();
	HBITMAP fullScreenshot();
	void readImg(cv::Mat image);
}