#ifndef NPDFACE_MODEL_H
#define NPDFACE_MODEL_H
#ifdef _OPENMP
#include <omp.h>
#endif
#include "DQTree.h"
#include "NPDFeature.h"
#include <vector>
#define USE_OPENMP 1
#define USE_PTHREAD 0
#define USE_OPENCL 0
#define USE_MAT8 0
#define DEBUG_MESSAGE 1
#define WIDTH_720P 1080
#define HEIGHT_720P 720

#include "opencv2/opencv.hpp"
class Mat8
{
public:
    Mat8(int rows_,int cols_);
    Mat8();
    Mat8(cv::Mat src);
    cv::Mat convert2Mat();
    void releaseMat();
    Mat8 resize(int rows_,int cols_);
public:
    unsigned char * data;
    int cols;
    int rows;
};
class FaceBox
{
public:
	int x;
	int y;
	int width;
	int height;
	double score;
    int mergedTime;
	int id;
public:
	FaceBox()
			:x(0),y(0),width(0),height(0),score(0.0),mergedTime(1),id(-1)
	{
	}
	FaceBox(int x_,int y_,int width_,int height_,double score_)
			:x(x_),y(y_),width(width_),height(height_),score(score_)
	{
	}

};
class Model
{
public:
	std::vector<DQTree> treeArray;
	int stageNum;
	std::string modelIndex;
	std::string modelPath;
	NPDfeature npdFea;
    int xStepSize;
    int yStepSize;
    bool useRASW;
#if USE_OPENCL
    cl_context context;
    cl_command_queue commandQueue;
    cl_program program;
    cl_device_id device;
    cl_kernel kernel;
    int initOCL();
    void runOCL();
    void releaseOCL();
#endif
public:

public:
	Model(std::string modlepath_);
	~Model();
public:
	void loadModel();
    std::vector<FaceBox> detect(cv::Mat image,int minWindowSize,float factor);

#if DEBUG_MESSAGE
    clock_t  totalTime;
    int framesNum;
#endif
};
void intersection_union(FaceBox box1, FaceBox box2,int * intersection_size, int *union_size);
#endif//NPDFACE_MODEL_H
