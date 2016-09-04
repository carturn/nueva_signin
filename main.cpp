/*
  Copyright (c) 2016 Carter Turnbaugh

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>

#include <zbar.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace zbar;
using namespace cv;

int main(int argc, char* argv[]){

	// Zbar setup
	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

	// OpenCV setup
	VideoCapture capture;
	capture = VideoCapture(0);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	// curl setup
	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURL * curl;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, "http://tercatech.com/signin.php");
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	// Deals with retransmitting data across redirects
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_POSTREDIR, 3);
	// Deals with SSL (These should probably be removed)
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

	// Image scanning loop
	while(true){
		Mat cv_image;

		capture >> cv_image;
		
		Mat cv_gray;
		cvtColor(cv_image, cv_gray, CV_RGB2GRAY);

		imshow("Please display QR code", cv_image);
		
		int img_width = cv_gray.size().width;
		int img_height = cv_gray.size().height;
		
		const void * img_data  = cv_gray.data;

		Image zbar_image(img_width, img_height, "Y800", img_data, img_width*img_height);

		scanner.scan(zbar_image);

		for(Image::SymbolIterator symbol = zbar_image.symbol_begin(); symbol != zbar_image.symbol_end(); ++symbol){
			string data = "data="+symbol->get_data();
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.length());

			cout << data << "\n";
			curl_easy_perform(curl);
		}
		
		if(waitKey(30) >= 0){
			break;
		}
	}

	curl_easy_cleanup(curl);
	curl_global_cleanup();

	return 0;
}
