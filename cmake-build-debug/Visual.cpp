#include "Visual.h"

static CamaraAnalyse_t VisualData;

int line_following();
static void VisualInit();

using namespace cv;
using namespace std;

void OpenCV()
{
    int error;
    VideoCapture Cap(0);

    VisualData.Width    = 1280;
    VisualData.Height   = 720;
    VisualData.Fps      = 60;

    VisualInit();

    Cap.set(CAP_PROP_FRAME_WIDTH,VisualData.Width);
    Cap.set(CAP_PROP_FRAME_HEIGHT,VisualData.Height);
    Cap.set(CAP_PROP_FPS,VisualData.Fps);

    while (1)
    {
        Cap >> VisualData.InputFrame;

        switch (VisualData.CameraCmdData.mode) {

            case LineFollowingMode:
                error = line_following();
                break;
            case ItemIdentifyMode:

                break;

        }
        cout << error << endl;
        if(waitKey(1) == 27)
        {
            break;
        }
    }
}


const CameraCmd_t* Get_CameraData_Pointer() {
    return &(VisualData.CameraCmdData);
}



int line_following() {

    cvtColor(VisualData.InputFrame, VisualData.HSVFrame, COLOR_RGB2HSV_FULL);

    VisualData.ProcessFrame = VisualData.HSVFrame(Range(VisualData.LineFollowing.RowRange[0],
                                                          VisualData.LineFollowing.RowRange[1]),
                                                    Range(VisualData.LineFollowing.ColRange[0],
                                                          VisualData.LineFollowing.ColRange[1]));

    inRange(VisualData.ProcessFrame,Scalar(VisualData.HRange[VisualData.LineFollowing.Color].Low,
                                               VisualData.HRange[VisualData.LineFollowing.Color].Low,
                                               VisualData.HRange[VisualData.LineFollowing.Color].Low),
                                        Scalar(VisualData.HRange[VisualData.LineFollowing.Color].High,
                                               VisualData.HRange[VisualData.LineFollowing.Color].High,
                                               VisualData.HRange[VisualData.LineFollowing.Color].High),
                                               VisualData.ThreshFrame);

    VisualData.CameraCmdData.LineFollowingMsg.SpecialCondition = Normal;

    VisualData.LineFollowing.TotalNumSum = VisualData.LineFollowing.TotalWidthSum = 0;
    for (int i = 0; i < VisualData.LineFollowing.RowDelta; i++) {
        VisualData.LineFollowing.OneLineNumSum = 0;
        for (int j = 0; j < VisualData.LineFollowing.ColDelta; j++) {
            if (VisualData.ThreshFrame.at<uchar>(i, j) > 128) {
                VisualData.LineFollowing.TotalWidthSum += j;
                VisualData.LineFollowing.OneLineNumSum++;
            }
        }
        VisualData.LineFollowing.TotalNumSum += VisualData.LineFollowing.OneLineNumSum;

        if (VisualData.LineFollowing.OneLineNumSum >= 300) {
            VisualData.CameraCmdData.LineFollowingMsg.SpecialCondition = RightAngel;
        }
    }
    VisualData.LineFollowing.BlackPixelAverage = (float) VisualData.LineFollowing.TotalWidthSum /
                                                 VisualData.LineFollowing.TotalNumSum;

    VisualData.CameraCmdData.LineFollowingMsg.offset = (int) (VisualData.LineFollowing.BlackPixelAverage -
                                                              (VisualData.Width / 2));

}

static void VisualInit() {
    VisualData.LineFollowing.Color = Black;
    VisualData.CameraCmdData.LineFollowingMsg.SpecialCondition = Normal;
    VisualData.CameraCmdData.LineFollowingMsg.offset = 0;
    VisualData.CameraCmdData.LineFollowingMsg.speed = 0;
    VisualData.CameraCmdData.mode = LineFollowingMode;
    for (int i = 0; i < 7; i++) {
        VisualData.HRange[i].Low    = ColorHRange[i][0];
        VisualData.HRange[i].High   = ColorHRange[i][1];
        VisualData.SRange[i].Low    = ColorSRange[i][0];
        VisualData.SRange[i].High   = ColorSRange[i][1];
        VisualData.VRange[i].Low    = ColorVRange[i][0];
        VisualData.VRange[i].High   = ColorVRange[i][1];
    }
}

int SquareDetect()
{
    inRange(VisualData.HSVFrame,Scalar(VisualData.HRange[Pink].Low,
                                       VisualData.HRange[Pink].Low,
                                       VisualData.HRange[Pink].Low),
                                    Scalar(VisualData.HRange[Pink].High,
                                           VisualData.HRange[Pink].High,
                                           VisualData.HRange[Pink].High),
                                    VisualData.ThreshFrame);

}

int line_following_1() {

    VisualData.ProcessFrame = VisualData.InputFrame(Range(VisualData.LineFollowing.RowRange[0],
                                                          VisualData.LineFollowing.RowRange[1]),
                                                    Range(VisualData.LineFollowing.ColRange[0],
                                                          VisualData.LineFollowing.ColRange[1]));

    cvtColor(VisualData.ProcessFrame, VisualData.ProcessFrame, COLOR_RGB2GRAY);

    // threshold the video
    threshold(VisualData.ProcessFrame, VisualData.ProcessFrame, 114, 256, THRESH_BINARY_INV);


    VisualData.CameraCmdData.LineFollowingMsg.SpecialCondition = Normal;

    for (int i = 0; i < VisualData.LineFollowing.RowDelta; i++) {
        VisualData.LineFollowing.OneLineNumSum = 0;
        for (int j = 0; j < VisualData.LineFollowing.ColDelta; j++) {
            if (VisualData.ProcessFrame.at<uchar>(i, j) > 128) {
                VisualData.LineFollowing.TotalWidthSum += j;
                VisualData.LineFollowing.OneLineNumSum++;
            }
        }
        VisualData.LineFollowing.TotalNumSum += VisualData.LineFollowing.OneLineNumSum;

        if (VisualData.LineFollowing.OneLineNumSum >= 300) {
            VisualData.CameraCmdData.LineFollowingMsg.SpecialCondition = RightAngel;
        }
    }
    VisualData.LineFollowing.BlackPixelAverage = (float) VisualData.LineFollowing.TotalWidthSum /
                                                 VisualData.LineFollowing.TotalNumSum;

    VisualData.CameraCmdData.LineFollowingMsg.offset = (int) (VisualData.LineFollowing.BlackPixelAverage -
                                                              (VisualData.Width / 2));

}