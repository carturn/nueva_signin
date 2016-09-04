CC=g++
EXE=signin

ZBAR_LINK=-Wl,-force_load,/usr/local/Cellar/zbar/0.10_1/lib/libzbar.a -Wl,-force_load,/usr/local/opt/jpeg/lib/libjpeg.a -liconv -weak_library /usr/lib/libSystem.B.dylib
CURL_LINK=-lcurl

OPENCV_LIBS=-lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc
OPENCV_FRAMWORKS=-framework OpenCL -framework Cocoa -framework AppKit -framework CoreFoundation -framework Foundation -framework QTKit -framework QuartzCore -framework CoreVideo 
OPENCV_EXTERNAL_LIBS=-lz -lc++ -Wl,-force_load,/usr/local/opt/webp/lib/libwebp.a
OPENCV_LINK=$(OPENCV_LIBS) $(OPENCV_FRAMWORKS) $(OPENCV_EXTERNAL_LIBS)

build:
	$(CC) main.cpp -o $(EXE) $(ZBAR_LINK) $(CURL_LINK) $(OPENCV_LINK) -mmacosx-version-min=10.7


# g++ main.cpp -o signin -Wl,-force_load,/usr/local/Cellar/zbar/0.10_1/lib/libzbar.a -Wl,-force_load,/usr/local/opt/jpeg/lib/libjpeg.a -liconv -lSystem -lcurl -lopencv_core -lz -framework OpenCL -lc++ -lSystem -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -framework Cocoa -framework AppKit -framework CoreFoundation -framework Foundation -framework QTKit -framework QuartzCore -framework CoreVideo -Wl,-force_load,/usr/local/opt/webp/lib/libwebp.a