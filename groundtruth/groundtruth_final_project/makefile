all: compile compile2 compile3 compile4

compile:
	g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g -o GroundTruth  generate_groundtruth.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui


compile2:
	g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g -o getGT  draw_rectangles.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui

compile3:
	g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g -o DrawBinaries  draw_binaries.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui

compile4:
	g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g -o MatchResult  match.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui

clean:
	rm -rf GroundTruth
	rm -rf getGT
	rm -rf DrawBinaries
	rm -rf MatchResult
