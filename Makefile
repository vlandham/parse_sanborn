GCC=g++

template_mather:template_match.o
	$(GCC) template_match.o -o template_matcher -L/usr/local/Cellar/opencv/2.2/lib/ -arch x86_64 -lopencv_core  -lopencv_highgui -lopencv_ml -lopencv_features2d -lopencv_contrib -lopencv_video -lopencv_imgproc

template_match.o:template_match.cpp
	$(GCC) -c template_match.cpp -I/usr/local/Cellar/opencv/2.2/include/opencv2/  -g  -arch x86_64

line_finder:line_find.o
	clang++ line_find.o -o line_finder -L/usr/local/Cellar/opencv/2.2/lib/ -arch x86_64 -lopencv_core  -lopencv_highgui -lopencv_ml -lopencv_features2d -lopencv_contrib -lopencv_video -lopencv_imgproc

line_find.o:line_find.cpp
	clang++ -c line_find.cpp -I/usr/local/Cellar/opencv/2.2/include/opencv2/  -g -L/usr/local/Cellar/opencv/2.2/lib/libopencv_core.dylib -arch x86_64


