#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <zbar.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace zbar;
using namespace cv;

int main(int argc, char* argv[]){

	ImageScanner scanner;

	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

	VideoCapture capture;
	capture = VideoCapture(0);

	// Image scanning loop
	while(true){
		Mat cv_image;

		capture >> cv_image;
		
		Mat cv_gray;
		cvtColor(cv_image, cv_gray, CV_RGB2GRAY);

		imshow("Image", cv_image);
		imshow("Grayscale", cv_gray);
		
		int img_width = cv_gray.size().width;
		int img_height = cv_gray.size().height;
		
		const void * img_data  = cv_gray.data;

		Image zbar_image(img_width, img_height, "Y800", img_data, img_width*img_height);

		scanner.scan(zbar_image);

		for(Image::SymbolIterator symbol = zbar_image.symbol_begin(); symbol != zbar_image.symbol_end(); ++symbol){
			cout << symbol->get_type() << "\t";
			cout << symbol->get_data() << "\n";
		}
		
		if(waitKey(30) >= 0){
			break;
		}
	}
}
