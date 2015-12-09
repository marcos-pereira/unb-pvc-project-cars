all:
	clear
	g++ `pkg-config --cflags --libs opencv2` -I'include/'  -o OpticalFlowHS OpticalFlowHS_run.cpp src/HS.cpp src/segmentation.cpp src/flowCore.cpp `pkg-config --libs opencv2` 
