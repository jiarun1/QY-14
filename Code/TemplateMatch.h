//
// Created by jiarun on 3/14/22.
//

#ifndef INC_2022_3_6_TEMPLATEMATCH_H
#define INC_2022_3_6_TEMPLATEMATCH_H

#include <opencv2/opencv.hpp>

int TemplateMatching_task();


namespace TP_Matching {
    using namespace cv;
    using namespace std;

    struct MatchTemplate_t {
        Mat *StartMat;
        string *Pic_name;
        int Total_num;
    };

    enum Error_Code {
        HASH_LENGTH_NOT_MATCHED = -1,
        MATCH_OK = 0,
        MATCH_SIZE_LARGE = 1,
    };

    static MatchTemplate_t MatchTemplate;

    __attribute__((unused)) extern int Init(const std::string path,const std::string* name, int total_num);

    __attribute__((unused)) extern int ShowMatchImg(int number);
    __attribute__((unused)) extern int ShowMatchImg(const std::string Windows_name,int number);
    __attribute__((unused)) extern int ShowMatchImg(int* array,int length);

    __attribute__((unused)) extern int TemplateMatch(cv::InputArray img,cv::OutputArray matched,int Matched_num,int method);

    __attribute__((unused)) extern std::string HashAvg(cv::InputArray input, cv::Size_<int> size = cv::Size(8,8));
    __attribute__((unused)) extern std::string HashDiff(cv::InputArray input, cv::Size_<int> size = cv::Size(9,8));
    __attribute__((unused)) extern std::string HashPerc(cv::InputArray input, cv::Size_<int> size = cv::Size(64,64));
    __attribute__((unused)) extern int HashCmp(std::string img,std::string model);

    __attribute__((unused)) extern std::vector<std::vector<cv::Point>> ShapeDetect(cv::InputArray input,double epsilon = 3);

    extern int Release() ;
}


#endif //INC_2022_3_6_TEMPLATEMATCH_H
