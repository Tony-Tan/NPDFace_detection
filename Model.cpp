#include "Model.h"

#include <fstream>
#include <pthread.h>
static bool isFileExists(std::string filePath)
{
	std::fstream file;
	file.open(filePath.c_str(), std::ios::in);
	if (!file)
	{
		return false;
	}
	else
	{
		file.close();
		return true;
	}
	return false;
}
void intersection_union(FaceBox box1, FaceBox box2,
                               int * intersection_size, int *union_size)
{
    int x1 = box1.x;
    int x2 = box1.x + box1.width;
    int y1 = box1.y;
    int y2 = box1.y + box1.height;

    int x3 = box2.x;
    int x4 = box2.x + box2.width;
    int y3 = box2.y;
    int y4 = box2.y + box2.height;
    *intersection_size = 0;
    int union_size_ = box1.width * box1.height + box2.width * box2.height;
    int x5, y5;
    if ((x3 >= x1) && (x3 <= x2) && (y3 >= y1) && (y3 <= y2))
    {
        x5 = x4 < x2 ? x4 : x2;
        y5 = y4 < y2 ? y4 : y2;
        *intersection_size = (x5 - x3)*(y5 - y3);
    }
    else if ((x4 >= x1) && (x4 <= x2) && (y3 >= y1) && (y3 <= y2))
    {

        x5 = x3 > x1 ? x3 : x1;
        y5 = y4 < y2 ? y4 : y2;
        *intersection_size = -(x5 - x4)*(y5 - y3);
    }
    else if ((x3 >= x1) && (x3 <= x2) && (y4 >= y1) && (y4 <= y2))
    {
        x5 = x4 < x2 ? x4 : x2;
        y5 = y3 > y1 ? y3 : y1;
        *intersection_size = -(x5 - x3)*(y5 - y4);
    }
    else if ((x4 >= x1) && (x4 <= x2) && (y4 >= y1) && (y4 <= y2))
    {
        x5 = x3 > x1 ? x3 : x1;
        y5 = y3 > y2 ? y3 : y1;
        *intersection_size = (x5 - x4)*(y5 - y4);
    }
    else
    {
        x3 = box1.x;
        x4 = box1.x + box1.width;
        y3 = box1.y;
        y4 = box1.y + box1.height;

        x1 = box2.x;
        x2 = box2.x + box2.width;
        y1 = box2.y;
        y2 = box2.y + box2.height;
        if ((x3 >= x1) && (x3 <= x2) && (y3 >= y1) && (y3 <= y2))
        {
            x5 = x4 < x2 ? x4 : x2;
            y5 = y4 < y2 ? y4 : y2;
            *intersection_size = (x5 - x3)*(y5 - y3);
        }
        else if ((x4 >= x1) && (x4 <= x2) && (y3 >= y1) && (y3 <= y2))
        {
            x5 = x3 > x1 ? x3 : x1;
            y5 = y4 < y2 ? y4 : y2;
            *intersection_size = -(x5 - x4)*(y5 - y3);
        }
        else if ((x3 >= x1) && (x3 <= x2) && (y4 >= y1) && (y4 <= y2))
        {
            x5 = x4 < x2 ? x4 : x2;
            y5 = y3>y1 ? y3 : y1;
            *intersection_size = -(x5 - x3)*(y5 - y4);
        }
        else if ((x4 >= x1) && (x4 <= x2) && (y4 >= y1) && (y4 <= y2))
        {
            x5 = x3 > x1 ? x3 : x1;
            y5 = y3 > y2 ? y3 : y1;
            *intersection_size = (x5 - x4)*(y5 - y4);
        }
    }
    *union_size = union_size_ - *intersection_size;
}

static std::vector<FaceBox> nonMaximumSuppression(std::vector<FaceBox> faceBoxes)
{
    bool *removeMask = new bool[faceBoxes.size()];
    memset(removeMask, 0, sizeof(bool)*faceBoxes.size());
    for (int j = 0; j < faceBoxes.size(); j++)
    {
        if (!removeMask[j])
        {
            for (int i = j + 1; i < faceBoxes.size(); i++)
            {
                if (!removeMask[i]) {
                    int un_, inter_;
                    intersection_union(faceBoxes[i], faceBoxes[j], &inter_, &un_);
                    if (double(inter_) / un_>0.1)
                    {
                        if (faceBoxes[i].score > faceBoxes[j].score)
                        {
                            removeMask[j] = true;
                            //faceBoxes[i].mergedTime+=faceBoxes[j].mergedTime;
                        }
                        else
                        {
                            removeMask[i] = true;
                            //faceBoxes[j].mergedTime+=faceBoxes[i].mergedTime;
                        }
                    }
                }
            }
        }
    }
    std::vector<FaceBox> faceBoxesDst;
    for (int i = 0; i < faceBoxes.size(); i++)
    {
        if (!removeMask[i] /*&& faceBoxes[i].mergedTime>=2*/)
            faceBoxesDst.push_back(faceBoxes[i]);
    }
    delete[] removeMask;
    return faceBoxesDst;
}


Mat8::Mat8(int rows_,int cols_):
rows(rows_),cols(cols_)
{
    data=(unsigned char *)malloc(sizeof(unsigned char)*rows_*cols_);
    if(data==NULL)
    {
        std::cout<<"Can not malloc memory"<<std::endl;
        exit(0);
    }
}
Mat8::Mat8()
:rows(0),cols(0),data(NULL)
{

}
Mat8::Mat8(cv::Mat src)
{
    rows=src.rows;
    cols=src.cols;
    data=(unsigned char *)malloc(sizeof(unsigned char)*src.rows*src.cols);
    if(data==NULL)
    {
        std::cout<<"Can not malloc memory"<<std::endl;
        exit(0);
    }
    for(int j=0;j<rows;j++)
    {
        for(int i=0;i<cols;i++)
        {
            data[j*cols+i]=src.at<uchar>(j,i);
        }
    }

}
void Mat8::releaseMat()
{
    if(data!=NULL)
    {
        free(data);
    }
}
cv::Mat Mat8::convert2Mat()
{
    cv::Mat dst(rows,cols,CV_8UC1);
    for(int j=0;j<rows;j++)
        for(int i=0;i<cols;i++)
        {
            dst.at<uchar>(j,i)=data[j*cols+i];
        }
    return dst;
}
Mat8 Mat8::resize(int rows_,int cols_)
{
    Mat8 dst(rows_,cols_);
    float rowRate=(float)rows_/rows;
    float colRate=(float)cols_/cols;
    for(int j=0;j<rows_;j++)
    {
        unsigned char * dstRow=dst.data+j*cols_;
        unsigned char * srcRow=data+(int)(j/rowRate)*cols;
        for(int i=0;i<cols_;i++)
        {
            dstRow[i]=srcRow[int(i/colRate)];
        }
    }
    return dst;
}
#if USE_OPENCL
int convertToString(const char *filename, std::string& s)
{
    size_t size;
    char*  str;
    std::fstream f(filename, (std::fstream::in | std::fstream::binary));

    if(f.is_open())
    {
        size_t fileSize;
        f.seekg(0, std::fstream::end);
        size = fileSize = (size_t)f.tellg();
        f.seekg(0, std::fstream::beg);
        str = new char[size+1];
        if(!str)
        {
            f.close();
            return 0;
        }
        f.read(str, fileSize);
        f.close();
        str[size] = '\0';
        s = str;
        delete[] str;
        return 0;
    }
    std::cout<<"Error: failed to open file\n:"<<filename<<std::endl;
    return -1;
}
int getPlatform(cl_platform_id &platform)
{
    platform = NULL;//the chosen platform

    cl_uint numPlatforms;//the NO. of platforms
    cl_int    status = clGetPlatformIDs(0, NULL, &numPlatforms);
    if (status != CL_SUCCESS)
    {
        std::cout<<"Error: Getting platforms!"<<std::endl;
        return -1;
    }

    /**For clarity, choose the first available platform. */
    if(numPlatforms > 0)
    {
        cl_platform_id* platforms =
                (cl_platform_id* )malloc(numPlatforms* sizeof(cl_platform_id));
        status = clGetPlatformIDs(numPlatforms, platforms, NULL);
        platform = platforms[0];
        free(platforms);
    }
    else
        return -1;
}

/**Step 2:Query the platform and choose the first GPU device if has one.*/
cl_device_id *getCl_device_id(cl_platform_id &platform)
{
    cl_uint numDevices = 0;
    cl_device_id *devices=NULL;
    cl_int    status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
    if (numDevices > 0) //GPU available.
    {
        devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
        status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, numDevices, devices, NULL);
    }
    return devices;
}
int Model::initOCL()
{
    cl_int    status;
    /**Step 1: Getting platforms and choose an available one(first).*/
    cl_platform_id platform;
    getPlatform(platform);

    /**Step 2:Query the platform and choose the first GPU device if has one.*/
    cl_device_id *devices=getCl_device_id(platform);

    /**Step 3: Create context.*/
    context = clCreateContext(NULL,1, devices,NULL,NULL,NULL);

    /**Step 4: Creating command queue associate with the context.*/
    commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);

    /**Step 5: Create program object */
    const char *filename = "HelloWorld_Kernel.cl";
    std::string sourceStr;
    status = convertToString(filename, sourceStr);
    const char *source = sourceStr.c_str();
    size_t sourceSize[] = {strlen(source)};
    program = clCreateProgramWithSource(context, 1, &source, sourceSize, NULL);

    /**Step 6: Build program. */
    status=clBuildProgram(program, 1,devices,NULL,NULL,NULL);

    /**Step 7: Initial input,output for the host and create memory objects for the kernel*/
    const int NUM=512000;
    double* input = new double[NUM];
    for(int i=0;i<NUM;i++)
        input[i]=i;
    double* output = new double[NUM];

    cl_mem inputBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, (NUM) * sizeof(double),(void *) input, NULL);
    cl_mem outputBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY , NUM * sizeof(double), NULL, NULL);

    /**Step 8: Create kernel object */
    kernel = clCreateKernel(program,"helloworld", NULL);
    return 0;
}
void Model::runOCL()
{
    cl_int    status;
    /**Step 9: Sets Kernel arguments.*/
    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&inputBuffer);
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&outputBuffer);

    /**Step 10: Running the kernel.*/
    size_t global_work_size[1] = {NUM};
    cl_event enentPoint;
    status = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, &enentPoint);
    clWaitForEvents(1,&enentPoint); ///wait
    clReleaseEvent(enentPoint);

    /**Step 11: Read the cout put back to host memory.*/
    status = clEnqueueReadBuffer(commandQueue, outputBuffer, CL_TRUE, 0, NUM * sizeof(double), output, 0, NULL, NULL);
    std::cout<<output[NUM-1]<<std::endl;
}
void Model::releaseOCL()
{
    cl_int    status;
    /**Step 12: Clean the resources.*/
    status = clReleaseKernel(kernel);//*Release kernel.
    status = clReleaseProgram(program);    //Release the program object.
    status = clReleaseCommandQueue(commandQueue);//Release  Command queue.
    status = clReleaseContext(context);//Release context.

    if (output != NULL)
    {
        free(output);
        output = NULL;
    }

    if (devices != NULL)
    {
        free(devices);
        devices = NULL;
    }
}
#endif
Model::Model(std::string modelPath_)
	:stageNum(0),xStepSize(3),yStepSize(3)
{
#if DEBUG_MESSAGE
    framesNum=0;
    totalTime=0;
#endif
	modelPath = modelPath_;
	modelIndex = modelPath + std::string("modelIndex.txt");
	if (isFileExists(modelIndex))
	{
		loadModel();
	}
}
Model::~Model()
{

}

void Model::loadModel()
{
	FILE* modelIndexFile = fopen(modelIndex.c_str(), "r");
	if (NULL == modelIndexFile)
	{
		Message("Model::loadModel", "model index file open faile");
		stageNum = 0;
		return;
	}
	while (!feof(modelIndexFile))
	{
		int stageId = 0;
		int treeId = 0;
		char treePath[100];
		fscanf(modelIndexFile,"stage:%d tree:%d %s\n",
			&stageId, &treeId,treePath);
		DQTree tree(modelPath + std::string(treePath),stageId,treeId);
		treeArray.push_back(tree);
		stageNum = stageId + 1;
	}
	fclose(modelIndexFile);
}


/************************************************************************************/
//using pthread to speed up
/************************************************************************************/
#if USE_PTHREAD
typedef struct DetectParam_
{
    cv::Mat* image;
    Model* model;
    std::vector<FaceBox>* faces;
    std::vector<float> scaleArray;
    int startScale;
    int endScale;
}DetectParam;
void* detectKernel(void * arg)
{
    DetectParam *param=(DetectParam* )arg;
    Model * model=param->model;
    cv::Mat * image=param->image;
    for (int s = param->startScale; s < param->endScale; s++)
    {
        int j = 0;
        int i = 0;
        float scale=1.0f/param->scaleArray[s];
        int width=int(SAMPLE_WIDTH*scale+0.6f);
        int height=int(SAMPLE_HEIGHT*scale+0.6f);
        int stepY=int(model->yStepSize*scale+0.6f);
        int stepX=int(model->xStepSize*scale+0.6f);
        for (j = 0; j < image->rows - height; j += stepY) {
            for (i = 0; i < image->cols - width; i += stepX) {
                float fit = 0.0;
                bool pass = true;
                int t = 0;
                for (t=0 ; t < model->treeArray.size(); t++)
                {
                    int pos=0;
                    while (model->treeArray[t].y_1[pos]!=-1)
                    {
                        int pixel1=image->at<uchar>( int(model->treeArray[t].y_1[pos]*scale+0.6f) + j ,
                                                     int(model->treeArray[t].x_1[pos]*scale+0.6f) + i);
                        int pixel2=image->at<uchar>( int(model->treeArray[t].y_2[pos]*scale+0.6f) + j ,
                                                     int(model->treeArray[t].x_2[pos]*scale+0.6f) + i);

                        unsigned char fea = model->npdFea.NPDtable[pixel1][pixel2];
                        if (fea<model->treeArray[t].cut_1[pos] || fea>model->treeArray[t].cut_2[pos])
                        {
                            pos=pos*2+1;
                        }
                        else
                        {
                            pos=pos*2+2;
                        }
                    }
                    fit += model->treeArray[t].fit[pos];
                    if (fit < model->treeArray[t].threshold)
                    {
                        pass = false;
                        break;
                    }
                }
                if (pass) {
                    FaceBox newFace(i, j, width, height, fit);
                    param->faces->push_back(newFace);
                }
            }
        }
    }
    return NULL;
}

/************************************************************************************/
std::vector<FaceBox> Model::detect(cv::Mat image,int minWindowSize,float factor)
{

    float startScale=SAMPLE_WIDTH/((float)minWindowSize);
    std::vector<float> scaleArray;
    while(startScale*image.rows>=SAMPLE_HEIGHT&&
          startScale*image.cols>=SAMPLE_WIDTH)
    {
        scaleArray.push_back(startScale);
        startScale*=factor;
    }
    pthread_t tid[4];
    DetectParam param;
    param.model=this;
    param.image=&image;
    std::vector<FaceBox> faces;
    param.faces=&faces;
    param.startScale=0;
    param.scaleArray = scaleArray;
    param.endScale=scaleArray.size()/15;
    pthread_create(&tid[0], NULL,detectKernel,&param);

    DetectParam param1;
    param1.model=this;
    param1.image=&image;
    std::vector<FaceBox> faces1;
    param1.faces=&faces1;
    param1.scaleArray = scaleArray;
    param1.startScale=scaleArray.size()/15;
    param1.endScale=scaleArray.size()*1/10;
    pthread_create(&tid[1], NULL,detectKernel,&param1);

    DetectParam param2;
    param2.model=this;
    param2.image=&image;
    std::vector<FaceBox> faces2;
    param2.faces=&faces2;
    param2.scaleArray = scaleArray;
    param2.startScale=scaleArray.size()*1/10;
    param2.endScale=scaleArray.size()*1/4;
    pthread_create(&tid[2], NULL,detectKernel,&param2);

    DetectParam param3;
    param3.model=this;
    param3.image=&image;
    std::vector<FaceBox> faces3;
    param3.faces=&faces3;
    param3.scaleArray = scaleArray;
    param3.startScale=scaleArray.size()*1/4;
    param3.endScale=scaleArray.size();
    pthread_create(&tid[3], NULL,detectKernel,&param3);
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    pthread_join(tid[2],NULL);
    pthread_join(tid[3],NULL);

    for(int i=0;i<faces1.size();i++)
        faces.push_back(faces1[i]);
    for(int i=0;i<faces2.size();i++)
        faces.push_back(faces2[i]);
    for(int i=0;i<faces3.size();i++)
        faces.push_back(faces3[i]);
    return nonMaximumSuppression(faces);

}
#endif
#define SAMPLE_SIZE 24
std::vector<FaceBox> Model::detect(cv::Mat image,int minWindowSize,float factor)
{
    float startScale=SAMPLE_WIDTH/((float)minWindowSize);
    std::vector<float> scaleArray;
    while(startScale*image.rows>=SAMPLE_HEIGHT&&
          startScale*image.cols>=SAMPLE_WIDTH)
    {
        scaleArray.push_back(startScale);
        startScale*=factor;
    }

    std::vector<FaceBox> faces;
    for (unsigned int s = 0; s < scaleArray.size(); s++)
    {
        cv::Mat imageResize;
        int resizeRows=image.rows*scaleArray[s];
        int resizeCols=image.cols*scaleArray[s];
        cv::resize(image, imageResize, cv::Size(resizeCols, resizeRows));
        unsigned char *data=(unsigned char*)imageResize.data;
        int j = 0;
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (j = 0 ; j < resizeRows - SAMPLE_SIZE; j += yStepSize)
        {
            for (int i = 0; i < resizeCols - SAMPLE_SIZE; i += xStepSize)
            {
                float fit_temp = 0.0;
                bool pass = true;
                int t = 0;
                for (t = 0; t < treeArray.size(); t++)
                {
                    int pos=0;
                    const signed char* y_1=treeArray[t].y_1;
                    const signed char* y_2=treeArray[t].y_2;
                    const signed char* x_1=treeArray[t].x_1;
                    const signed char* x_2=treeArray[t].x_2;
                    const int *cut_1=treeArray[t].cut_1;
                    const int *cut_2=treeArray[t].cut_2;
                    const float *fit=treeArray[t].fit;
                    while (treeArray[t].y_1[pos]!=(signed char)-1)
                    {
                        int pixel_1 = data[((int) y_1[pos] + j)*resizeCols+(int) x_1[pos] + i];
                        int pixel_2 = data[((int) y_2[pos] + j)*resizeCols+(int) x_2[pos] + i];
                        unsigned char fea = npdFea.NPDtable[pixel_1][pixel_2];
                        if (fea < cut_1[pos] || fea > cut_2[pos]) {
                            pos = (pos <<1) + 1;
                        } else {
                            pos = (pos <<1) + 2;
                        }
                    }
                    fit_temp += fit[pos];
                    if (fit_temp < treeArray[t].threshold)
                    {
                        pass = false;
                        break;
                    }
                }
#ifdef _OPENMP
#pragma omp critical
#endif
                {
                    if (pass)
                    {
                        FaceBox newFace(int(i / scaleArray[s]), int(j / scaleArray[s]),
                                        SAMPLE_SIZE / scaleArray[s], SAMPLE_SIZE / scaleArray[s], fit_temp);
                        faces.push_back(newFace);
                    }
                }
            }
        }
    }
    //stop = time(NULL);
    return nonMaximumSuppression(faces);
}