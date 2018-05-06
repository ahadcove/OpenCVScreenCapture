#include "ScreenCapture.h"
#include <iostream>

namespace screencapture {
	void PrintMessage() {
		std::cout << "Awesome Hello Screen" << std::endl;
	}

	void screenshot(POINT a, POINT b)
	{
		// copy screen to bitmap
		HDC     hScreen = GetDC(NULL);
		HDC     hDC = CreateCompatibleDC(hScreen);

		HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, abs(b.x - a.x), abs(b.y - a.y));
		HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
		BOOL    bRet = BitBlt(hDC, 0, 0, abs(b.x - a.x), abs(b.y - a.y), hScreen, a.x, a.y, SRCCOPY);

		// save bitmap to clipboard
		OpenClipboard(NULL);
		EmptyClipboard();
		SetClipboardData(CF_BITMAP, hBitmap);
		CloseClipboard();

		// clean up
		SelectObject(hDC, old_obj);
		DeleteDC(hDC);
		ReleaseDC(NULL, hScreen);
		DeleteObject(hBitmap);
	}

	cv::Mat ToMemory() {

		HDC hwindowDC, hwindowCompatibleDC;

		int height, width, srcheight, srcwidth;
		HBITMAP hbwindow;
		cv::Mat src;
		BITMAPINFOHEADER  bi;

		hwindowDC = GetDC(NULL);
		hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
		SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

		int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		srcheight = nScreenHeight;
		srcwidth = nScreenWidth;
		height = nScreenHeight;  //change this to whatever size you want to resize to
		width = nScreenWidth;
		//height = nScreenHeight / 2;  //change this to whatever size you want to resize to
		//width = nScreenWidth / 2;

		src.create(height, width, CV_8UC4);

		// create a bitmap
		hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
		bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
		bi.biWidth = width;
		bi.biHeight = -height;  //this is the line that makes it draw upside down or not
		bi.biPlanes = 1;
		bi.biBitCount = 32;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;

		// use the previously created device context with the bitmap
		SelectObject(hwindowCompatibleDC, hbwindow);
		// copy from the window device context to the bitmap device context
		StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
		GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

		OpenClipboard(NULL);
		EmptyClipboard();
		SetClipboardData(CF_BITMAP, hbwindow);
		CloseClipboard();
		// avoid memory leak
		DeleteObject(hbwindow); DeleteDC(hwindowCompatibleDC); ReleaseDC(NULL, hwindowDC);

		return src;
	}

	HBITMAP fullScreenshot()
	{
		// copy screen to bitmap
		HDC     hScreen = GetDC(NULL);
		HDC     hDC = CreateCompatibleDC(hScreen);

		int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
		HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, nScreenWidth, nScreenHeight);
		HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
		BOOL    bRet = BitBlt(hDC, 0, 0, nScreenWidth, nScreenHeight, hScreen, 0, 0, SRCCOPY);

		//	SetClipboardData(CF_BITMAP, hBitmap);

		// clean up
		SelectObject(hDC, old_obj);
		DeleteDC(hDC);
		ReleaseDC(NULL, hScreen);
		//DeleteObject(hBitmap);
		return hBitmap;
	}

	void ClipboardScreenshot()
	{
		// copy screen to bitmap
		HDC     hScreen = GetDC(NULL);
		HDC     hDC = CreateCompatibleDC(hScreen);

		int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
		HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, nScreenWidth, nScreenHeight);
		HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
		BOOL    bRet = BitBlt(hDC, 0, 0, nScreenWidth, nScreenHeight, hScreen, 0, 0, SRCCOPY);

		// save bitmap to clipboard
		OpenClipboard(NULL);
		EmptyClipboard();
		SetClipboardData(CF_BITMAP, hBitmap);
		CloseClipboard();

		// clean up
		SelectObject(hDC, old_obj);
		DeleteDC(hDC);
		ReleaseDC(NULL, hScreen);
		DeleteObject(hBitmap);
	}

	void readImg(cv::Mat image)
	{

		// image = imread(argv[1], IMREAD_COLOR); // Read the file
		if (image.empty()) // Check for invalid input
		{
			std::cout << "Could not open or find the image" << std::endl;
			return;
		}
		cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
		imshow("Display window", image); // Show our image inside it.
		cv::waitKey(0); // Wait for a keystroke in the 
	}

	/*void toClipboard(CWnd * wnd, BOOL FullWnd)
	{
	CDC dc;
	if (FullWnd)
	HDC hdc = ::GetWindowDC(wnd->m_hWnd);
	dc.Attach(hdc);
	}
	else
	{ // client area only
	HDC hdc = ::GetDC(wnd->m_hWnd);
	dc.Attach(hdc);
	}

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap bm;
	CRect r;
	if (FullWnd)
	wnd->GetWindowRect(&r);
	else
	wnd->GetClientRect(&r);

	CString s;
	wnd->GetWindowText(s);
	CSize sz(r.Width(), r.Height());
	bm.CreateCompatibleBitmap(&dc, sz.cx, sz.cy);
	CBitmap * oldbm = memDC.SelectObject(&bm);
	memDC.BitBlt(0, 0, sz.cx, sz.cy, &dc, 0, 0, SRCCOPY);

	wnd->GetParent()->OpenClipboard();
	::EmptyClipboard();
	::SetClipboardData(CF_BITMAP, bm.m_hObject);
	CloseClipboard();

	memDC.SelectObject(oldbm);
	bm.Detach();  // make sure bitmap not deleted with CBitmap object
	}*/

}