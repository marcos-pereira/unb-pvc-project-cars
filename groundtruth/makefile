all: compile

compile:
	g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g -o GroundTruth  *.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui


clean:
	rm -rf GroundTruth