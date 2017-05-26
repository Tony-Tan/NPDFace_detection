#include "Message.h"
#include <opencv2/opencv.hpp>
#include "Model.h"
#include <fstream>
using namespace cv;
void drawFace(Mat image_show,FaceBox face,cv::Scalar color,double scale_=1.0)
{
    rectangle(image_show, cv::Rect(face.x*scale_, face.y*scale_, face.width*scale_, face.height*scale_),
                  color, 1, 8, 0);
    char score[512];
    sprintf(score,"%lf",face.score);
    putText(image_show,score,cv::Point(face.x*scale_, face.y*scale_),CV_FONT_HERSHEY_DUPLEX,0.5f,color);
}
int main()
{
/*
    Model npdModel("/Users/tony/Project/NPDFace/Data/2016-12-24/Model/");
    //Model npdModel("/Users/tony/Project/NPDFace/Data/Model/");
    for(int i=5;i<7;i++)
    {
        char imageName[256];
        sprintf(imageName,"/Users/Tony/Project/NPDFace/Data/test2/%d.jpg",i);
        std::cout<<imageName<<std::endl;
        cv::Mat image=cv::imread(imageName,0);
        cv::Mat image_show=cv::imread(imageName,1);
        std::vector<FaceBox> faces=npdModel.detect(image,6,0.93);
        for(int i=0;i<faces.size();i++)
        {
            printf("%f %f %f %f %f\n", (float) faces[i].x,
                   (float) faces[i].y, (float) faces[i].width,
                   (float) faces[i].height, (float) faces[i].score);
            if(faces[i].score<10.)
            {
                drawFace(image_show,faces[i],CV_RGB(255, 0, 0));
            }else if(faces[i].score>=10&&faces[i].score<15.)
            {
                drawFace(image_show,faces[i],CV_RGB(255, 255, 0));
            }else if(faces[i].score>=15.&&faces[i].score<20.)
            {
                drawFace(image_show,faces[i],CV_RGB(0, 255, 0));
            }else if(faces[i].score>=20.)
            {
                drawFace(image_show,faces[i],CV_RGB(0, 0, 255));
            }

        }
        cv::imshow("test",image_show);
        cv::waitKey(0);
        sprintf(imageName,"/Users/Tony/Project/NPDFace/Data/test2/%d_%d.jpg",i,npdModel.stageNum);
        cv::imwrite(imageName,image_show);
    }
    return 0;
*/

    //FDDB

    char* FDDB_path="/home/tony/FDDB/originalPics/";
    char* FDDB_imagePathFile="/home/tony/FDDB/FDDB-fold.txt";
    char* FDDB_resultFile="/home/tony/FDDB/output.txt";
    std::fstream fin(FDDB_imagePathFile);  //打开文件
    std::string imageLocalPath;
    FILE* result=fopen(FDDB_resultFile,"w+");

    Model npdModel("/home/tony/Project/NPDFace/Data/Model/");
    while(getline(fin,imageLocalPath))
    {
        std::string imageName=std::string(FDDB_path)+imageLocalPath+".jpg";
        std::cout<<"detecting "<<imageName<<std::endl;
        cv::Mat image=cv::imread(imageName,0);
        cv::Mat image_show=cv::imread(imageName,1);
        std::vector<FaceBox> faces=npdModel.detect(image,24,0.96);
        //npdModel.detect(image,60,0.9,true);
        fprintf(result,"%s\n",imageLocalPath.c_str());
        fprintf(result,"%d\n",faces.size());
        for(int i=0;i<faces.size();i++)
        {
            faces[i].x-=0.05*(float)faces[i].width;
            faces[i].y-=0.2*(float)faces[i].height;
            faces[i].width*=1.1;
            faces[i].height*=1.3;
            printf("%f %f %f %f %f\n",(float)faces[i].x,
                    (float)faces[i].y,
                    (float)faces[i].width,
                    (float)faces[i].height,
                    (float)faces[i].score);
            fprintf(result,"%f %f %f %f %f\n",(float)faces[i].x,
                    (float)faces[i].y,
                    (float)faces[i].width,
                    (float)faces[i].height,
                    (float)faces[i].score);

            if(faces[i].score<10.)
            {
                drawFace(image_show,faces[i],CV_RGB(255, 0, 0));
            }else if(faces[i].score>=10&&faces[i].score<15.)
            {
                drawFace(image_show,faces[i],CV_RGB(255, 255, 0));
            }else if(faces[i].score>=15.&&faces[i].score<20.)
            {
                drawFace(image_show,faces[i],CV_RGB(0, 255, 0));
            }else if(faces[i].score>=20.)
            {
                drawFace(image_show,faces[i],CV_RGB(0, 0, 255));
            }
        }
        cv::imshow("test",image_show);
        cv::waitKey(10);

    }

    fin.close();
    fclose(result);
	return 0;


/*
    char* FDDB_path="/Users/Tony/FDDB/originalPics/";
    char* FDDB_imagePathFile="/Users/Tony/FDDB/FDDB-fold.txt";
    char* FDDB_resultFile="/Users/Tony/FDDB/output.txt";
    std::fstream fin(FDDB_imagePathFile);  //打开文件
    std::string imageLocalPath;
    //FILE* result=fopen(FDDB_resultFile,"w+");

    Model npdModel("/Users/tony/Project/NPDFace/Data/Model/");
    while(getline(fin,imageLocalPath))
    {
        std::string imageName=std::string(FDDB_path)+imageLocalPath+".jpg";
        std::cout<<"detecting "<<imageName<<std::endl;
        cv::Mat image=cv::imread(imageName,0);
        cv::Mat image_show=cv::imread(imageName,1);
        std::vector<FaceBox> faces=npdModel.detect(image,30,0.93);
        //fprintf(result,"%s\n",imageLocalPath.c_str());
        //fprintf(result,"%d\n",faces.size());
        for(int i=0;i<faces.size();i++) {
            if (faces[i].score>-100.0) {
                printf("%f %f %f %f %f\n", (float) faces[i].x,
                       (float) faces[i].y, (float) faces[i].width,
                       (float) faces[i].height, (float) faces[i].score);
                if(faces[i].score<10.)
                {
                    drawFace(image_show,faces[i],CV_RGB(255, 0, 0));
                }else if(faces[i].score>=10&&faces[i].score<15.)
                {
                    drawFace(image_show,faces[i],CV_RGB(255, 255, 0));
                }else if(faces[i].score>=15.&&faces[i].score<20.)
                {
                    drawFace(image_show,faces[i],CV_RGB(0, 255, 0));
                }else if(faces[i].score>=20.)
                {
                    drawFace(image_show,faces[i],CV_RGB(0, 0, 255));
                }
            }
        }
        cv::imshow("test",image_show);
        cv::waitKey(0);

    }
    fin.close();
    return 0;
    */
    //camera
/*
    Model npdModel("/Users/tony/Project/NPDFace/Data/2016-12-24/Model/");
    bool stop= false;
    VideoCapture cap(0);
    if(!cap.isOpened())
    {
        return -1;
    }
    Mat frame;
    while(!stop)
    {
        cap>>frame;
        cv::Mat image;
        cv::Mat image_=frame;
        cv::resize(frame,image_,cv::Size(WIDTH_720P,HEIGHT_720P));
        cv::cvtColor(image_,image,cv::COLOR_RGB2GRAY);
        cv::Mat image_show=image_;

        clock_t start,end;
        start = clock();
        //std::vector<FaceBox> faces=npdModel.detectM(image);
        std::vector<FaceBox> faces=npdModel.detectM(image,80,0.9);
        end =clock();
        std::cout<<"Using time per frame(s):"<<double(end-start)/CLOCKS_PER_SEC<<"s"<<std::endl;
        for(int i=0;i<faces.size();i++)
        {
            if (faces[i].score>10.0) {
                printf("%f %f %f %f %f\n", (float) faces[i].x,
                       (float) faces[i].y, (float) faces[i].width,
                       (float) faces[i].height, (float) faces[i].score);

                if(faces[i].score>=20.&&faces[i].score<30.)
                {
                    drawFace(image_show,faces[i],CV_RGB(0, 255, 0));
                }else if(faces[i].score>=30.)
                {
                    drawFace(image_show,faces[i],CV_RGB(0, 0, 255));
                }
            }
        }
        imshow("test",image_show);
        if('q'==cv::waitKey(10))
        {
            std::cout<<"average using time :"<<(double)npdModel.totalTime/npdModel.framesNum<<std::endl;
            stop=true;
        }
    }
*/

/*
    Model npdModel("/Users/tony/Project/NPDFace/Data/Model/");
    bool stop= false;
    VideoCapture cap(0);
    if(!cap.isOpened())
    {
        return -1;
    }
    //cap.set(CV_CAP_PROP_BRIGHTNESS, 0.9);


    Mat frame;
    VideoWriter writer("/Users/tony/Project/NPDFace/Data/Movie/tony20.mp4",
                      CV_FOURCC('M', 'J', 'P', 'G'), 12.0, Size(1280, 720));
    while(!stop)
    {
        cap>>frame;
        cv::Mat image;
        cv::Mat image_;
        cv::resize(frame,image_,cv::Size(frame.cols/2,frame.rows/2));
        cv::cvtColor(image_,image,cv::COLOR_RGB2GRAY);
        cv::Mat image_show=frame;
        std::vector<FaceBox> faces=npdModel.detect(image,40,0.9);
        for(int i=0;i<faces.size();i++)
        {

            if (faces[i].score>15.0) {
                printf("%f %f %f %f %f\n", (float) faces[i].x,
                       (float) faces[i].y, (float) faces[i].width,
                       (float) faces[i].height, (float) faces[i].score);
                if(faces[i].score>=15.&&faces[i].score<20.)
                {
                    drawFace(image_show,faces[i],CV_RGB( 0,255, 0),2);
                }else if(faces[i].score>=20.)
                {
                    drawFace(image_show,faces[i],CV_RGB(0, 0, 255),2);
                }
            }
        }
        imshow("test",image_show);
        writer << image_show;
        if('q'==cv::waitKey(10))
            stop=true;

    }
*/

}