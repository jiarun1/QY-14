//
// Created by jiarun on 3/14/22.
//

#include "TemplateMatch.h"
#include <iostream>

const std::string source_path = "../Picture/Symbols/";

const std::string template_name[] = {
        "Alarm.png",
        "CountShape1.PNG",
        "CountShape2.png",
        "CountShape3.png",
        "Football.PNG",
        "MeasureDistance.PNG",
        "PlayAudio.png",
        "ShortCutBlue.PNG",
        "ShortcutGreen.PNG",
        "ShortcutRed.PNG",
        "ShortcutYellow.png",
        "TrafficLight.PNG"
};

int TemplateMatching_task() {
    using namespace cv;
    using namespace std;
    using namespace TP_Matching;

    Mat CalImg = imread("../Picture/1.jpg");
    Mat processed;
    Mat Output;
    Mat Hsv[3];
    Mat tmp;
    resize(CalImg,CalImg,Size(1024,720));
    Output = CalImg;
    GaussianBlur(CalImg,CalImg,Size(3,3),1);
    namedWindow("Output",WINDOW_NORMAL);
    namedWindow("Process",WINDOW_NORMAL);
    namedWindow("Origin",WINDOW_NORMAL);
    imshow("Origin",CalImg);
    cvtColor(CalImg,processed,COLOR_RGB2HSV_FULL);
    split(processed,Hsv);
    inRange(processed,Scalar(225,90,0),Scalar(250,180,255), processed);
    vector<vector<Point>> Shape;
    Shape = ShapeDetect(processed);

    for(int i = 0; i < Shape.size();i++) {
        if(Shape[i].size() == 4)
        {
            for (int j = 0; j < Shape[i].size(); j++) {
                Point P = Point(Shape[i][j].x,Shape[i][j].y);
//                Output.at<Vec3b>(P)[0] = 0;
//                Output.at<Vec3b>(P)[1] = 0;
//                Output.at<Vec3b>(P)[2] = 255;
                circle(Output,P,2,Scalar(0,0,255));
            }
        }
    }

    imshow("out",Output);

    imshow("Process",processed);

    waitKey(0);
    return 0;
}

__attribute__((unused))
int TP_Matching::Init(const std::string path,const std::string* name, int total_num) {
    TP_Matching::MatchTemplate.StartMat = new Mat[total_num];
    TP_Matching::MatchTemplate.Pic_name = new String[total_num];
    TP_Matching::MatchTemplate.Total_num = total_num;
    for (int i = 0; i < total_num; i++) {
        TP_Matching::MatchTemplate.StartMat[i] = cv::imread(path + name[i]);
        TP_Matching::MatchTemplate.Pic_name[i] = name[i];
    }
    return MATCH_OK;
}

__attribute__((unused))
int TP_Matching::ShowMatchImg(int number) {
    if (number >= TP_Matching::MatchTemplate.Total_num) {
        return MATCH_SIZE_LARGE;
    }
    cv::imshow(TP_Matching::MatchTemplate.Pic_name[number], TP_Matching::MatchTemplate.StartMat[number]);
    return MATCH_OK;
}

__attribute__((unused))
int TP_Matching::ShowMatchImg(const std::string Windows_name,int number) {
    if (number >= TP_Matching::MatchTemplate.Total_num) {
        return MATCH_SIZE_LARGE;
    }
    cv::imshow(Windows_name, TP_Matching::MatchTemplate.StartMat[number]);
    return MATCH_OK;
}

__attribute__((unused))
int TP_Matching::ShowMatchImg(int* array,int length) {
    for (int i = 0; i < length; i++) {
        if (array[i] >= TP_Matching::MatchTemplate.Total_num) {
            return MATCH_SIZE_LARGE;
        }
        cv::imshow(TP_Matching::MatchTemplate.Pic_name[array[i]], TP_Matching::MatchTemplate.StartMat[array[i]]);
    }
    return MATCH_OK;
}

__attribute__((unused))
int TP_Matching::TemplateMatch(cv::InputArray img,cv::OutputArray matched,int Matched_num,int method) {
    using namespace cv;
    cv::matchTemplate(img, MatchTemplate.StartMat[Matched_num], matched, method);
    return MATCH_OK;
}

__attribute__((unused))
int TP_Matching::Release() {
    delete TP_Matching::MatchTemplate.StartMat;
    TP_Matching::MatchTemplate.Total_num = 0;
    return MATCH_OK;
}

__attribute__((unused))
std::string TP_Matching::HashAvg(cv::InputArray input, cv::Size_<int> size) {
    using namespace cv;
    double sum = 0;
    double avg = 0;
    int height = size.height,width = size.width;
    std::string hash_string = "";
    Mat Processing_img;
    resize(input, Processing_img, size);
    cvtColor(Processing_img, Processing_img, cv::COLOR_RGB2GRAY);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            sum += Processing_img.at<uchar>(i, j);
        }
    }
    avg = (double)sum / (double)height / (double)width ;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (Processing_img.at<uchar>(i, j) > avg) {
                hash_string += '1';
            } else {
                hash_string += '0';
            }
        }
    }
    return hash_string;
}

__attribute__((unused))
std::string TP_Matching::HashDiff(cv::InputArray input, cv::Size_<int> size) {
    using namespace cv;
    Mat Processing_img;
    std::string hash_string = "";
    int height = size.height,width = size.width;

    resize(input,Processing_img,size);
    cvtColor(Processing_img,Processing_img,COLOR_RGB2GRAY);
    for(int i = 0; i < (width - 1); i++) {
        for (int (j) = 0; (j) < height; j++){
            if( Processing_img.at<uchar>(i,j) > Processing_img.at<uchar>(i+1,j)){
                hash_string += '1';
            } else{
                hash_string += '0';
            }
        }
    }
    return hash_string;
}

__attribute__((unused))
std::string TP_Matching::HashPerc(cv::InputArray input, cv::Size_<int> size) {
    using namespace cv;
    Mat Processing_img,Dct_img;
    double sum,avg;
    std::string hash_string = "";
    int height = size.height,width = size.width;
    resize(input,Processing_img,size);
    cvtColor(Processing_img,Processing_img,COLOR_RGB2GRAY);
    Processing_img = Mat_<double>(Processing_img);
    dct(Processing_img,Processing_img);
    for(int i = 0;i<width;i++) {
        for (int j = 0; j < height; j++) {
            sum += Processing_img.at<double>(i,j);
        }
    }
    avg = (double) sum / (double) height / (double) width ;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (Processing_img.at<double>(i, j) > avg) {
                hash_string += '1';
            } else {
                hash_string += '0';
            }
        }
    }
    return hash_string;
}

__attribute__((unused))
int TP_Matching::HashCmp(std::string img, std::string model) {
    int CmpValue = 0;
    if(img.length() != model.length())
    {
        return TP_Matching::HASH_LENGTH_NOT_MATCHED;
    }
    for (int i = 0; i < img.length(); i++) {
        if (img[i] != model[i]) {
            CmpValue++;
        }
    }
    return CmpValue;
}

__attribute__((unused))
std::vector<std::vector<cv::Point>> TP_Matching::ShapeDetect(cv::InputArray input,double epsilon)
{
    using namespace std;
    using namespace cv;
    vector<vector<Point>> Counters;
    vector<Vec4i> hierarchy;
    findContours(input,Counters,hierarchy,RETR_LIST,CHAIN_APPROX_SIMPLE,Point(0,0));
    vector<vector<Point>> CounterPoly(Counters.size());

    for(int i = 0; i < Counters.size();i++)
    {
        approxPolyDP(Counters[i],CounterPoly[i],epsilon,true);
    }
    return CounterPoly;
}