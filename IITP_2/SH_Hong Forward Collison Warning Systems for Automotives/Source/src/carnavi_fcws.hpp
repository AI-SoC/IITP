///////////////////////////////////////////////////////////////////////////////
//
// ���� �ߵ� �溸 �ý���(Forward Collision Warning System)
//
// Created by  shhong
//
// FCWS New
//
// Type of Code:  C
// 
//
// Started 2017.03.06
// Last Modified 2018.03.13
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __CASCADE_H__
#define __CASCADE_H__

#define __OPENCV_USE__
#define SIMD

#include <stdio.h>
#include <stdlib.h>	//malloc �������
#include <math.h>	//sqrt �������
#include <string.h>	//strlen, strcmp �������

#ifdef __OPENCV_USE__
#include <opencv2/opencv.hpp>
#endif //__OPENCV_USE__

#ifdef SIMD
#include <immintrin.h>
#define X   0
#define Y   1
#define W   2
#define H   3
#endif

#define TTC_OFFSET		0.6
#define true			1
#define false			0

#define CASCADE_SCALE	1.0	//1.0->25*25(min), 2.0->50*50(min)
#define CASCADE_WINDOW_SIZE_MAX 100
//#define CASCADE_WINDOW_SIZE_MIN 25.0
#define PERCENT_MAX 400.0 // 100.0

#define TRACKING_WINDOW_SIZE 10

#define IMAGE_WIDTH  640    //320
#define IMAGE_HEIGHT 480    //240

#define MAX_DISTANCE 70		//FCW max distance


//camera intrinsic parameter
//1280*720 		-> 	800*600 crop 	-> 	320*240 resize
//fx: fx		->  fx				->	fx/2.5
//fy: fy  		->	fy				->	fy/2.5
//cx: cx		->	cx - 240		->	(cx - 240)/2.5
//cy: cy		->	cy - 60			->	(cy - 60)/2.5

//camera intrinsic parameter
//1280*720 		-> 	640*480 crop 	-> 	320*240 resize
//fx: fx		->  fx				->	fx/2.0
//fy: fy  		->	fy				->	fy/2.0
//cx: cx		->	cx - 320		->	(cx - 320)/2.0
//cy: cy		->	cy - 120		->	(cy - 120)/2.0

//camera intrinsic parameter
//1280*720 		-> 	480*360 crop 	-> 	320*240 resize
//fx: fx		->  fx				->	fx/1.5
//fy: fy  		->	fy				->	fy/1.5
//cx: cx		->	cx - 400		->	(cx - 400)/1.5
//cy: cy		->	cy - 180		->	(cy - 180)/1.5


#define CAMERA_CX					IMAGE_WIDTH * 0.5
#define CAMERA_CY					IMAGE_HEIGHT * 0.5
#define CAMERA_FY					475

//#define CAMERA_CY					(CAMERA_ORIGINAL_CY - 180.0) / 1.5
//#define CAMERA_FY					CAMERA_ORIGINAL_FY / 1.5

#define CAMERA_HEIGHT				1.45									//1m
#define CAMERA_RADIAN				atan2(CAMERA_CY - 80.0, CAMERA_FY);		//Camera radian, angle = atan((camera_cy - vanishing_point.y) / camera_fy)


typedef unsigned char uchar;
#ifdef SIMD
typedef union SIMDFRect
{
    __m128 reg;
    float temp[4];
}SIMDFRect;
typedef union SIMDRect
{
    __m128i reg;
    int temp[4];
}SIMDRect;
#endif
typedef struct CPoint
{
	int x, y;
}CPoint;

typedef struct CMat
{
	int cols, rows;
	uchar *data;
}CMat;

typedef struct CSize
{
	int width, height;
}CSize;

typedef struct CRect
{
	int x, y, width, height, size;
#ifdef SIMD
    SIMDRect simd;
#endif
}CRect;

typedef struct CarviTreeClassifier
{
	int nodeidx;
	float threshold, l_val, r_val;
}CarviTreeClassifier;

typedef struct CarviStageClassifier
{
	float threshold;
	int tree_size;
	CarviTreeClassifier *tree;
}CarviStageClassifier;

typedef struct CarviClassifier
{
	int stage_size, width, height;
	CarviStageClassifier *stage;
}CarviClassifier;

typedef struct CarviFeature
{
	uchar tilted;
	int size;	//feature size
	int rect_size; //rectangle size
	struct
	{
		CRect r;
		float weight;
	} rect[3];

}CarviFeature;

int get_detect_result(void);
int get_detect_sum(void);
void set_detect_sum(int set);
void fcws_Init(void);
CRect fcws_Main(unsigned char* image, int mode);
void fcws_DeInit(void);
void SetFcwsCarSpeed(int Speed);

uchar Cascade_Read_txt(const char* input, CarviClassifier *cascade, CarviFeature **cascade_rect, CarviFeature **cascade_rect_scale);
uchar Vehicle_Tracking(CMat image_ori, CRect image_roi, float real_velocity, CRect *vehicle_roi, float *width_ttc, float *width_distance);
CRect Image_Preprocessing(CMat image_ori);

void Image_Resize(CMat *iimage, CMat *oimage, CSize size);
void Image_Integral(CMat *image, int *nsum, float *dsum, int *tsum);
void Casacde_Malloc_free(void);
void Image_Malloc_free(void);
void Cascade_Scale(CarviFeature *cascade_rect, CarviFeature *cascade_rect_temp, float factor, float inv_window_area);
void Rectangles_Group(CRect *rectList, int groupThreshold);
uchar Cascade_Detect(CMat image_gray, CRect *vehicle, float scale, int min_neighbors, CSize min_size);
uchar Cascade_Compute(CarviStageClassifier cascade, CarviFeature *cascade_rect_scale, CRect rect, float var);
uchar Image_Classify(CarviStageClassifier cascade, CarviFeature *cascade_rect_scale, CRect rect, float var);
uchar Vehicle_Detect(CMat image, CRect image_roi, CRect *vehicle);
uchar Vehicle_Forward_Detect(CRect image_roi, CRect *vehicle, CRect *vehicle_position_update);
uchar Vanishing_Line_Detect(CRect vehicle);
int Cascade_Read_txt_gets(FILE *file, char *buffer, const char* control);
float Image_Sum(CRect rect);
float Image_Tsum(CRect rect);
float Image_Dsum(CRect rect);
float Image_Compute(CarviFeature cascade, CRect rect);
float *Cascade_Read_txt_values(char *buffer, int start);

#endif  //__CASCADE_H__
