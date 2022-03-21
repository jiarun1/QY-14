#ifndef __VISUAL_H__
#define __VISUAL_H__

#include "opencv2/opencv.hpp"

enum VehicleMode {
    LineFollowingMode	= 0,
    ItemIdentifyMode	= 1,
    PlayingSoundMode	= 2,

};

enum LineFollowingSpecial
{
    Normal      = 0,
    RightAngel  = 1,
};

enum HSVColor
{
    Red     = 0,
    Green   = 1,
    Blue    = 2,
    Yellow  = 3,
    Pink    = 4,
    Black   = 5,
    White   = 6
};


struct CameraCmd_t{
    VehicleMode mode;

    struct	{
        int offset;
        int speed;
        LineFollowingSpecial SpecialCondition;
    }LineFollowingMsg;
};

struct ColorRange_t
{
    int Low;
    int High;
};

struct CamaraAnalyse_t{
    CameraCmd_t CameraCmdData;
    cv::Mat InputFrame;
    cv::Mat HSVFrame;
    cv::Mat ProcessFrame;
    cv::Mat ThreshFrame;
    cv::Mat HSVImage[3];

    int Width;
    int Height;
    double Fps;

    ColorRange_t HRange[7];
    ColorRange_t SRange[7];
    ColorRange_t VRange[7];

    struct	{
        HSVColor Color;
        int RowRange[2];
        int ColRange[2];
        int RowDelta;
        int ColDelta;
        float BlackPixelAverage;
        int OneLineNumSum;
        long TotalNumSum;
        float TotalWidthSum;
    }LineFollowing;
};




const int ColorHRange[7][2] = {
        {0 , 0},    //Red
        {0 , 0},    //Green
        {0 , 0},    //Blue
        {0 , 0},    //Yellow
        {0 , 0},    //Pink
        {0 , 0},    //Black
        {0 , 0},    //White
};

const int ColorSRange[7][2] = {
        {0 , 0},    //Red
        {0 , 0},    //Green
        {0 , 0},    //Blue
        {0 , 0},    //Yellow
        {0 , 0},    //Pink
        {0 , 0},    //Black
        {0 , 0},    //White
};

const int ColorVRange[7][2] = {
        {0 , 0},    //Red
        {0 , 0},    //Green
        {0 , 0},    //Blue
        {0 , 0},    //Yellow
        {0 , 0},    //Pink
        {0 , 0},    //Black
        {0 , 0},    //White
};





void OpenCV();
const CameraCmd_t* Get_CameraData_Pointer();





#endif