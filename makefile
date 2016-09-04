build:
	g++ -o signin main.cpp -Wl, -Bstatic -lzbar -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs -I/usr/local/Cellar/opencv3/3.1.0_3/include -L/usr/local/Cellar/opencv3/3.1.0_3/lib -lcurl -Wl, -Ddynamic
