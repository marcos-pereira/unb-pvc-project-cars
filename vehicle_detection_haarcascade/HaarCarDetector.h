//============================================================================
//  Universidade de Brasilia
//  Departamento de Ciencia da Computacao
//  Principios de Visao Computacional
//  Trabalho Final: Contador de Carros
//  Integrantes: Marcos, Felipe Dalosto, Maximillian, Felipe Spinola, Pedro, Bruno Donnici, Gustavo
//============================================================================


// HOW TO USE:
// Declare your class object as
// HaarCarDetector detector(path_to_xml_car_database_for_haarcascade)
// std::vector<std::vector<double>> cars; 
// Inside your loop getting video frames call
// cars = detector.GetCarsRectangles(IplImage* frame, frame number)
// After your loop ends, the vector of vector (like a matrix) will be stored at variable cars
// Each cars[row] contains in the following order: 
// frame number
// x (from rectangle around car)
// y (from rectangle around car)
// x_center (from rectangle around car)
// y_center (from rectangle around car)
// rectangle area (from crectangle around car)

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <sstream>
#include <fstream>

#include "cv.h" 
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

class HaarCarDetector
{
public:

	HaarCarDetector(std::string path_to_haar_base);
	virtual ~HaarCarDetector();

	CvHaarClassifierCascade *cascade_;
	CvMemStorage            *storage_;


	std::vector<std::vector<double>> GetCarsRectangles(IplImage *img, int frame_number);

private:
protected:
};



