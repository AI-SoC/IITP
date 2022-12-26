#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "carnavi_fcws.hpp"
#include <ctime>
#include <stdlib.h>

using namespace cv;

void help_menu(void)
{
    printf("ex) ./fcws (0~3)\n");
    printf("0: original\n");
    printf("1: original_simd\n");
    printf("2: vanishing\n");
    printf("3: vanishing_simd\n");
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        help_menu();
        return -1;
    }
    int mode = atoi(argv[1]);
    if(mode < 0 || mode > 3)
    {
        help_menu();
        return -1;
    }
    char file_name[50];
    sprintf(file_name, "Result_%d.txt", mode);
    FILE *fp = fopen(file_name, "w");
    int frame_time = 0;

    VideoCapture capture("../video/FCWS.avi");
    if(!capture.isOpened())
    {
        printf("file can not open!\n");
        return 0;
    }

    int start_frame = 1000;

    capture.set(CAP_PROP_POS_FRAMES, start_frame);
    int w_capture = cvRound(capture.get(CAP_PROP_FRAME_WIDTH));
    int h_capture = cvRound(capture.get(CAP_PROP_FRAME_HEIGHT));
    float w_scale = (float)w_capture / IMAGE_WIDTH;
    float h_scale = (float)h_capture / IMAGE_HEIGHT;
    printf("video size: %d * %d\n",w_capture, h_capture);

    Mat frame, image;
    //namedWindow("original");
    fcws_Init();

    capture >> frame;

    int frame_id = 1000;
    int output_start = 0;
    clock_t time_check;
    int detect_sum = 0;
    int detect_result = 0;
    float time_sum = 0.0f;
    while(1)
    {
        capture >> frame;
        if(frame.empty())
        {
            printf("end!\n");
            break;
        }

        SetFcwsCarSpeed(60);
        cvtColor(frame, image, COLOR_RGB2GRAY);
        resize(image, image, Size(IMAGE_WIDTH, IMAGE_HEIGHT));

        time_check = clock();
        CRect vehicle_roi = fcws_Main(image.data, mode);
        time_check = clock() - time_check;

        Rect vehicle(vehicle_roi.x*w_scale, vehicle_roi.y*h_scale, vehicle_roi.width*w_scale, vehicle_roi.height*h_scale);
        rectangle(frame, vehicle, Scalar(0, 0, 255), 4, 8, 0);

        float time = time_check / 1000.0f;
//        printf("vehicle x, y, width, height %d, %d, %d, %d\n", vehicle_roi.x, vehicle_roi.y, vehicle_roi.width, vehicle_roi.height);
        if(output_start > 5)
        {
            detect_result = get_detect_result();
            detect_sum = get_detect_sum();
            //if((vehicle_roi.width == 0 && vehicle_roi.height == 0) || detect_result) time = 0; //for tracking
            time_sum += (time / 1000.0f);
            fprintf(fp, "frame_id: %4d\tdetect: %d\tdetect_count: %4d\tnormal_time: %.4f [ms]\ttotal_time: %.4f [sec]\n", frame_time++, detect_result, detect_sum, time, time_sum);
        }
        else
        {
            set_detect_sum(0);
            output_start++;
        }
        printf("frame_id: %4d\tdetect: %d\tdetect_count: %4d\tnormal_time[%d]: %.4f [ms]\ttotal_time: %.4f [sec]\n", frame_id++, detect_result, detect_sum, mode, time, time_sum);
        //imshow("original", frame);

        uchar key = waitKey(1);
        if(key == 27)   //ESC
        {
            break;
        }
        else if(key == ' ')
        {
            waitKey(0);
        }
    }
    fclose(fp);
    fcws_DeInit();

    return 0;
}