all:
	clear
	g++ `pkg-config --cflags --libs opencv` -I'include/'  -o OpticalFlowHS OpticalFlowHS.cpp src/HS.cpp src/segmentation.cpp src/flowCore.cpp `pkg-config --libs opencv` 
