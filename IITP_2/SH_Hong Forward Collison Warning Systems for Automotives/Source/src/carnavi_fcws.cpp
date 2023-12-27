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
#include <stdio.h>
#include <stdlib.h>	//malloc �������
#include <math.h>	//sqrt �������
#include <string.h>	//strlen, strcmp �������
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <iostream>
#include "carnavi_fcws.hpp"

//=================================== Cascade Parameter ===================================//
CarviClassifier cascade;
CarviFeature *cascade_rect;
CarviFeature *cascade_rect_scale;

CarviClassifier cascade2;
CarviFeature *cascade_rect2;
CarviFeature *cascade_rect_scale2;

CarviClassifier cascade3;
CarviFeature *cascade_rect3;
CarviFeature *cascade_rect_scale3;
//=================================== Cascade Parameter ===================================//

//=================================== Camera Parameter ===================================//
float camera_cx;
float camera_cy;
float camera_fy;
float camera_height;
float camera_radian;

float real_width_fx;	//fx * width
//=================================== Camera Parameter ===================================//

//=================================== Main Parameter ===================================//
uchar vehicle_detect = false;				//���, ���� �ڵ��� ���� �� ���� ���
uchar vehicle_ttc_init1 = false;				//���, ���� �ڵ��� �ߵ� �˶� ��� ����
float vehicle_velocity = 0;						//�Է�, ���� �ڵ��� �ӵ�
float vehicle_distance;							//���, ����ϴ� ���� �ڵ��� �Ÿ�(m)
float vehicle_ttc;								//���, ����ϴ� ���� �ڵ��� TTC(sec)
CRect vehicle_roi;								//���, ����ϴ� ���� �ڵ��� Rect(x, y, width, height)
CRect image_roi;								//��ó�� ROI
CMat image_ori = { IMAGE_WIDTH, IMAGE_HEIGHT };	//�̹��� ���� ũ��
CMat vehicle_detect_image = { IMAGE_WIDTH, IMAGE_HEIGHT };		//���� �����ϱ� ���� ����� �̹���
CMat vehicle_detect_image_crop = { IMAGE_WIDTH, IMAGE_HEIGHT };	//���� �����ϱ� ���� ����� �̹���
//=================================== Main Parameter ===================================//

//=================================== Sub Parameter ===================================//
CMat _image[1];
int *_image_integral, *_image_tintegral;
float *_image_dintegral;
uchar forward_vehicle_init = false;		//���� �ڵ��� �Ÿ� �ʱ�ȭ
uchar vehicle_forward_tracking = false;	//���� �ڵ��� ���� �ʱ�ȭ
uchar scale_state_init = false;			//Haar scale �ʱ�ȭ
uchar vehicle_forward_cutin = false;		//������ ���� �Ǵ�
uchar fcws_on = false;					//FCWS �˰��� ���� ����
int msec_sampling_display = 0;				//msec_sampling ���� ����
int haar_mode = 0;
int scale_state = 4;
int detect_count;
float vehicle_ttc2_display = 0;
float vehicle_ttc2 = 100;
CPoint vanishing_point;
CPoint vanishing_point_l;
CPoint vanishing_point_r;
CMat vehicle_image;				//Template image
CMat vehicle_forward_image;		//Tracking Haar detection �˻�
//=================================== Sub Parameter ===================================//

uchar g_DayNight = false;

#ifdef __OPENCV_USE__
using namespace cv;
using namespace std;
int haar_detect = 0;
float opencv_percent_threshold;
Mat opencv_image = Mat(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);
#endif

int g_mode = 0;
int g_detect_sum = 0;
int g_detect_result = 0;

int get_detect_result(void)
{
    return g_detect_result;
}

int get_detect_sum(void)
{
    return g_detect_sum;
}

void set_detect_sum(int set)
{
    g_detect_sum = set;
}

void data_outputfile(void)
{
	static uchar data_init = false;
	static FILE *fp;
	//if(!data_init)
	{
		data_init = true;
		fp = fopen("/mnt/mmc/data.txt", "at+");
		//fp2 = fopen("/mnt/mmc/data2.txt", "at+");
	}
	if (!forward_vehicle_init) fprintf(fp, "\r\n");
	else fprintf(fp, "%f\t%f\t%d\r\n", vehicle_distance, vehicle_ttc2, msec_sampling_display);


	fclose(fp);
}

void SetFcwsCarSpeed(int Speed)
{
	vehicle_velocity = (float)Speed * 1000.0 / 3600.0;	//km/h	-> m/s, 110km/h	=	30.555m/s	(�ӽ� ����)
}

float fast_sqrt(float x)
{
	float xhalf = 0.5f * x;
	int i = 0x5f3759fd - ((*(int*)&x) >> 1);

	float y = *(float*)&i;
	y = y*(1.5f - y*y*xhalf);
	y = y*(1.5f - y*y*xhalf);

	return (x*y);
}

void fcws_Init(void)
{
	///////////////////// �ʱ�ȭ /////////////////////
	vehicle_forward_tracking = false;
	vehicle_image.cols = TRACKING_WINDOW_SIZE;		vehicle_image.rows = TRACKING_WINDOW_SIZE;
	vehicle_image.data = (uchar *)malloc(sizeof(uchar)* vehicle_image.rows * vehicle_image.cols);						//20*20���� �̹��� �Ҵ�

	_image->cols = IMAGE_WIDTH; 					_image->rows = IMAGE_HEIGHT;
	_image->data = (uchar *)malloc(sizeof(uchar)* _image->rows * _image->cols);
	_image_integral = (int *)malloc(sizeof(int)* (_image->rows + 1) * (_image->cols + 1));
	_image_dintegral = (float *)malloc(sizeof(float)* (_image->rows + 1) * (_image->cols + 1));
	_image_tintegral = (int *)malloc(sizeof(int)* (_image->rows + 2) * (_image->cols + 2));

	image_ori.data = (uchar *)malloc(sizeof(uchar)* image_ori.cols * image_ori.rows);									//�̹��� ���� ũ�⸸ŭ �޸� �Ҵ�
	vehicle_detect_image.data = (uchar *)malloc(sizeof(uchar)* vehicle_detect_image.cols * vehicle_detect_image.rows);	//�̹��� ���� ũ�⸸ŭ �޸� �Ҵ�
	vehicle_detect_image_crop.data = (uchar *)malloc(sizeof(uchar)* vehicle_detect_image_crop.cols * vehicle_detect_image_crop.rows);	//�̹��� ���� ũ�⸸ŭ �޸� �Ҵ�

	camera_cx = CAMERA_CX;
	camera_cy = CAMERA_CY;
	camera_fy = CAMERA_FY;
	camera_height = CAMERA_HEIGHT;
	camera_radian = CAMERA_RADIAN;


	vanishing_point.x = IMAGE_WIDTH/2 + 14;			vanishing_point.y = IMAGE_HEIGHT/3 + 30;
	vanishing_point_l.x = 0;						vanishing_point_l.y = IMAGE_HEIGHT - 1;
	vanishing_point_r.x = IMAGE_WIDTH - 1;			vanishing_point_r.y = IMAGE_HEIGHT - 1;

	Cascade_Read_txt("../learning_txt/20170809.txt", &cascade, &cascade_rect, &cascade_rect_scale);		//Cascade���� �о��, ���Ÿ���
	Cascade_Read_txt("../learning_txt/20170807.txt", &cascade2, &cascade_rect2, &cascade_rect_scale2);	//Cascade���� �о��, �ٰŸ���
	Cascade_Read_txt("../learning_txt/DL_20180403.txt", &cascade3, &cascade_rect3, &cascade_rect_scale3);	//Cascade���� �о��, �߰���

	vehicle_forward_image.cols = IMAGE_WIDTH; vehicle_forward_image.rows = IMAGE_HEIGHT;
	vehicle_forward_image.data = (uchar *)malloc(sizeof(uchar)* vehicle_forward_image.rows * vehicle_forward_image.cols);	//25*25���� �̹��� �Ҵ�
	///////////////////// �ʱ�ȭ /////////////////////
}

CRect fcws_Main(unsigned char* image, int mode)
{
    g_mode = mode;
	static int msec = 0, msec_pre = 0, msec_sampling = 0, msec_temp = 0;
	static double vehicle_velocity_pre = 0;
	static double forward_vehicle_m = 0.0, forward_vehicle_m_pre = 0.0;
	static double forward_vehicle_m_t[2] = {0.0};
	static double forward_vehicle_m_t_max = 0.0;
	double distance_diff = 0.0;

	CRect image_roi_temp;

	struct timeval tv;

	gettimeofday(&tv, NULL);
	msec = tv.tv_usec / 1000;	//1msec

	if((msec - msec_pre) > 0) msec_temp = msec - msec_pre;
	else msec_temp = msec + (1000 - msec_pre);

	//if(msec_temp >= 100)	//100msec, 10Hz
	{
		memcpy(image_ori.data, image, image_ori.cols * image_ori.rows);		//���� �̹��� ����(Gray scale, unsigned char ����)

		Mat temp_gray = Mat(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC1);
        memcpy(temp_gray.data, image, image_ori.cols * image_ori.rows);		//���� �̹��� ����(Gray scale, unsigned char ����)
        cvtColor(temp_gray, opencv_image, COLOR_GRAY2RGB);

		camera_radian = atan2(camera_cy - vanishing_point.y, camera_fy);

		image_roi_temp.x = 0; image_roi_temp.y = 0; image_roi_temp.width = image_ori.cols; image_roi_temp.height = image_ori.rows;

		vehicle_detect = Vehicle_Tracking(image_ori, image_roi_temp, vehicle_velocity, &vehicle_roi, &vehicle_ttc, &vehicle_distance);	//���� �ڵ��� ���� �� ���� �˰���

		if (vehicle_detect)
		{
			vehicle_ttc = ((int)(vehicle_ttc * 100)) / 100.0;
			vehicle_distance = ((int)(vehicle_distance * 100)) / 100.0;

			if((int)(vehicle_ttc * 10) >= (int)(1.0 * 10) && (int)(vehicle_ttc * 10) < (int)(1.2 * 10))			vehicle_ttc = 1.0;
			else if((int)(vehicle_ttc * 10) >= (int)(1.2 * 10) && (int)(vehicle_ttc * 10) < (int)(1.4 * 10))	vehicle_ttc = 1.2;
			else if((int)(vehicle_ttc * 10) >= (int)(1.4 * 10) && (int)(vehicle_ttc * 10) < (int)(1.6 * 10))	vehicle_ttc = 1.4;
			else if((int)(vehicle_ttc * 10) >= (int)(1.6 * 10) && (int)(vehicle_ttc * 10) < (int)(1.8 * 10))	vehicle_ttc = 1.6;
			else if((int)(vehicle_ttc * 10) >= (int)(1.8 * 10) && (int)(vehicle_ttc * 10) < (int)(2.0 * 10))	vehicle_ttc = 1.8;
			else if((int)(vehicle_ttc * 10) >= (int)(2.0 * 10) && (int)(vehicle_ttc * 10) < (int)(2.5 * 10))	vehicle_ttc = 2.0;
			else if((int)(vehicle_ttc * 10) >= (int)(2.5 * 10) && (int)(vehicle_ttc * 10) < (int)(3.0 * 10))	vehicle_ttc = 2.5;
			else if((int)(vehicle_ttc * 10) <= 0)	vehicle_ttc = 100;

			if (!forward_vehicle_init)
			{
				if ((int)vehicle_distance < MAX_DISTANCE) forward_vehicle_init = true;

				msec_sampling = 0;
				vehicle_ttc2 = 0;
				vehicle_ttc2_display = 0;
				forward_vehicle_m = forward_vehicle_m_pre = vehicle_distance;
				forward_vehicle_m_t[1] = forward_vehicle_m_t[0] = vehicle_distance;
				forward_vehicle_m_t_max = 0;
			}
			else
			{
				forward_vehicle_m = vehicle_distance;
				msec_sampling = msec_sampling + msec_temp;
			}

			msec_sampling_display = msec_sampling;

			if((int)(vehicle_distance) >= 40)	//down sampling (60, 65, 68, ... ) -> 60
			{
				if((int)(forward_vehicle_m_pre) < 40)	//init
				{
					msec_sampling = 0;
					vehicle_ttc2 = 0;
					vehicle_ttc2_display = 0;
					forward_vehicle_m = forward_vehicle_m_pre = vehicle_distance;
					forward_vehicle_m_t[1] = forward_vehicle_m_t[0] = vehicle_distance;
					forward_vehicle_m_t_max = 0;
				}

				if((int)(forward_vehicle_m * 0.1) == (int)(forward_vehicle_m_pre * 0.1))
				{
					if((int)(forward_vehicle_m_t_max) < (int)(forward_vehicle_m)) forward_vehicle_m_t_max = forward_vehicle_m;
					vehicle_ttc2 = 0;
				}
				else if (msec_sampling >= 500)	//500msec,  2Hz
				{
					forward_vehicle_m_t[1] = forward_vehicle_m_t[0];
					forward_vehicle_m_t[0] = forward_vehicle_m;

					distance_diff = (forward_vehicle_m_t[0] - forward_vehicle_m_t_max);
					forward_vehicle_m_t_max = forward_vehicle_m_t[0];

					vehicle_ttc2 = -forward_vehicle_m_t[0] / distance_diff * ((double)(msec_sampling) * 0.001);
					vehicle_ttc2 = ((int)(vehicle_ttc2 * 100)) / 100.0;

					msec_sampling = 0;
				}
			}
			else
			{
				if((int)(forward_vehicle_m_pre) >= 40)	//init
				{
					msec_sampling = 0;
					vehicle_ttc2 = 0;
					vehicle_ttc2_display = 0;
					forward_vehicle_m = forward_vehicle_m_pre = vehicle_distance;
					forward_vehicle_m_t[1] = forward_vehicle_m_t[0] = vehicle_distance;
					forward_vehicle_m_t_max = 0;
				}

				if((int)(forward_vehicle_m * 100) == (int)(forward_vehicle_m_pre * 100))
				{
					vehicle_ttc2 = 0;
				}
				else if (msec_sampling >= 500)	//500msec,  2Hz
				{
					forward_vehicle_m_t[1] = forward_vehicle_m_t[0];
					forward_vehicle_m_t[0] = forward_vehicle_m;

					distance_diff = (forward_vehicle_m_t[0] - forward_vehicle_m_t[1]);

					vehicle_ttc2 = -forward_vehicle_m_t[0] / distance_diff * ((double)(msec_sampling) * 0.001);
					vehicle_ttc2 = ((int)(vehicle_ttc2 * 100)) / 100.0;

					msec_sampling = 0;
				}
			}

			if ((int)(vehicle_ttc2 * 10) > 0 && (int)(vehicle_ttc2 * 10) <= 255)
			{
				vehicle_ttc2_display = vehicle_ttc2;
			}
			else vehicle_ttc2 = 100;

			forward_vehicle_m_pre = forward_vehicle_m;

			//data_outputfile();
		}
		else
		{
			msec_sampling = 0;
			forward_vehicle_m = 0.0;
			forward_vehicle_m_pre = 0.0;

			forward_vehicle_init = false;
			vehicle_ttc = 0;
			vehicle_ttc2 = 100;
			vehicle_ttc2_display = 0;
			vehicle_distance = 0;
            vehicle_roi.x = 0; vehicle_roi.y = 0; vehicle_roi.width = 0; vehicle_roi.height = 0;
		}
		msec_pre = msec;
		vehicle_velocity_pre = vehicle_velocity;
		fcws_on = false;
	}
	return vehicle_roi;
}

void fcws_DeInit(void)
{
	Image_Malloc_free();
	Casacde_Malloc_free();
}

void Image_Resize(CMat *iimage, CMat *oimage, CSize size)
{
	uchar pixel;
	int x, y;
	int x_position, y_position;
	float x_scale, y_scale;
	CMat image_temp = *iimage;

	oimage->cols = size.width;
	oimage->rows = size.height;

	x_scale = (float)(image_temp.cols) / size.width;
	y_scale = (float)(image_temp.rows) / size.height;

	for (y = 0; y < size.height; y++)
	{
		for (x = 0; x < size.width; x++)
		{
			x_position = (int)(x * x_scale);
			y_position = (int)(y * y_scale) * image_temp.cols;
			pixel = image_temp.data[x_position + y_position];
			oimage->data[y*size.width + x] = pixel;
		}
	}
}

void Casacde_Malloc_free(void)
{
	free(cascade.stage->tree);
	free(cascade.stage);
	free(cascade_rect);
	free(cascade_rect_scale);

	free(cascade2.stage->tree);
	free(cascade2.stage);
	free(cascade_rect2);
	free(cascade_rect_scale2);

	free(cascade3.stage->tree);
	free(cascade3.stage);
	free(cascade_rect3);
	free(cascade_rect_scale3);
}

void Image_Malloc_free(void)
{
	free(_image->data);
	free(_image_integral);
	free(_image_tintegral);
	free(_image_dintegral);

	free(image_ori.data);					//�޸� ����
	free(vehicle_image.data);				//�޸� ����
	free(vehicle_forward_image.data);		//�޸� ����
	free(vehicle_detect_image.data);		//�޸� ����
	free(vehicle_detect_image_crop.data);	//�޸� ����
}

uchar Cascade_Detect(CMat image_gray, CRect *vehicle, float scale, int min_neighbors, CSize min_size)
{
  uchar vehicle_detect_temp, vanishing_line_bool;
	int x, y, i, vehicle_detect_size = 0;
	float factor, inv_window_area;
	float mean_factor, variance_norm_factor;
	CSize maxSize = { CASCADE_WINDOW_SIZE_MAX, CASCADE_WINDOW_SIZE_MAX };
	CSize minSize = { cascade.width, cascade.height };
	CSize cascade_orig_window_size = min_size;
	CSize winSize, sz, sz1, sz2;
	CRect rect, rect_line;
	float f_scale = CASCADE_SCALE; //(image_gray.cols < 100) ? 1 : 2;//CASCADE_SCALE;	//1.0; //1.5;
	int point_width, increment;
	CRect point_roi;

	increment = 1;  //2;

	if (!scale_state_init)
	{
		scale_state = 6;
		scale_state_init = true;
	}

    if (scale_state == 6)
    {
        point_roi.y = vanishing_point.y - 20 * scale_state;
        point_width = 80 * scale_state;
        scale_state = 5;

        f_scale = 480.0 / 60;// 8.0;
        haar_mode = 1;	//haar �ٰŸ�
    }
    else if (scale_state == 5)
    {
        point_roi.y = vanishing_point.y - 20 * scale_state;
        point_width = 80 * scale_state;
        scale_state = 4;

        f_scale = 400.0 / 60;// 8.0;
        haar_mode = 1;	//haar �ٰŸ�
    }
	else if (scale_state == 4)
	{
		point_roi.y = vanishing_point.y - 20 * scale_state;
		point_width = 80 * scale_state;
		scale_state = 3;

		f_scale = 320.0 / 60;// 8.0;
		haar_mode = 1;	//haar �ٰŸ�
	}
	else if (scale_state == 3)
	{
		point_roi.y = vanishing_point.y - 20 * scale_state;
		point_width = 80 * scale_state;
		scale_state = 2;

		f_scale = 240.0 / 60;//6.0;
		haar_mode = 1;	//haar �ٰŸ�
	}
	else if (scale_state == 2)
	{
		point_roi.y = vanishing_point.y - 20 * scale_state;
		point_width = 80 * scale_state;
		scale_state = 1;

		f_scale = 160.0 / 60;//4.0;
		haar_mode = 1;	//haar �ٰŸ�
	}
	else if (scale_state == 1)
	{
		//point_roi.y = vanishing_point.y - 20 * scale_state;
		//point_width = 80 * scale_state;
		point_roi.y = vanishing_point.y - 15;
		point_width = 60;
		scale_state = 0;

		f_scale = 1.0;
		haar_mode = 1;	//haar �ٰŸ�
	}
	else if (scale_state == 0)
	{
		point_roi.y = vanishing_point.y - 5;
		point_width = 30;
		scale_state = 6;

		f_scale = 0.5;
		haar_mode = 0;	//haar ���Ÿ�
	}
//    scale_state = 6;
	point_roi.x = vanishing_point.x - (point_width / 2.0);
	point_roi.width = point_width;
	point_roi.height = point_width;

	if(g_mode == 0 || g_mode == 1)
    {
        point_roi.x = 0;
        point_roi.y = 0;
        point_roi.width = 640;
        point_roi.height = 480;
        f_scale = 1.0;
        haar_mode = 1;
    }

	if (point_roi.x <= 0) point_roi.x = 0;
	if (point_roi.y <= 0) point_roi.y = 0;

	if (point_roi.x + point_roi.width >= image_gray.cols) point_roi.width = image_gray.cols - point_roi.x;
	if (point_roi.y + point_roi.height >= image_gray.rows) point_roi.height = image_gray.rows - point_roi.y;

	vehicle_detect_image_crop.cols = point_roi.width;		//���� �����ϱ� ���� ����� �̹��� ũ��(���氡��)
	vehicle_detect_image_crop.rows = point_roi.height;		//���� �����ϱ� ���� ����� �̹��� ũ��(���氡��)

	for (y = 0; y < vehicle_detect_image_crop.rows; y++)
	{
		for (x = 0; x < vehicle_detect_image_crop.cols; x++)
		{
			vehicle_detect_image_crop.data[y*vehicle_detect_image_crop.cols + x] = image_gray.data[(y + point_roi.y)*image_gray.cols + (x + point_roi.x)];	//ROI �̹��� �ֱ�
		}
	}

	image_roi.x = point_roi.x;
	image_roi.y = point_roi.y;
	image_roi.width = point_roi.width;
	image_roi.height = point_roi.height;

	sz.width = (int)(vehicle_detect_image_crop.cols / f_scale + 0.5);
	sz.height = (int)(vehicle_detect_image_crop.rows / f_scale + 0.5);

	Image_Resize(&vehicle_detect_image_crop, _image, sz);
	Image_Integral(_image, _image_integral, _image_dintegral, _image_tintegral);

#ifdef __OPENCV_USE__
	Mat temp_dis = Mat(_image->rows, _image->cols, CV_8UC1);
    memcpy(temp_dis.data, _image->data, _image->rows*_image->cols);
	rectangle(temp_dis, Rect(point_roi.x, point_roi.y, point_roi.width, point_roi.height), Scalar(0, 255, 255), 1, 8, 0);
//	imshow("temp_dis", temp_dis);
#endif //__OPENCV_USE__

#ifdef SIMD
    //SIMDRect add_rect, scale_rect;
    //scale_rect.reg = _mm_set1_ps(f_scale);
    //add_rect.temp[X] = point_roi.x; add_rect.temp[Y] = point_roi.y; add_rect.temp[W] = 0; add_rect.temp[H] = 0;
#endif
    for (factor = 1;; factor *= scale)
	{
		winSize.width = (int)(cascade_orig_window_size.width * factor + 0.5);
		winSize.height = (int)(cascade_orig_window_size.height * factor + 0.5);
		sz1.width = sz.width - cascade_orig_window_size.width;
		sz1.height = sz.height - cascade_orig_window_size.height;
		sz2.width = sz.width - winSize.width;
		sz2.height = sz.height - winSize.height;

		if (sz1.width <= 0 || sz1.height <= 0)
			break;
		if (sz2.width <= 0 || sz2.height <= 0)
			break;
		if (winSize.width > maxSize.width || winSize.height > maxSize.height)
			break;
		if (winSize.width < minSize.width || winSize.height < minSize.height)
			continue;

		inv_window_area = 1.0 / ((winSize.width) * (winSize.height));

		switch(g_DayNight)
		{
			case 1:		//�߰�
				Cascade_Scale(cascade_rect3, cascade_rect_scale3, factor, inv_window_area);
				break;

			default:	//�ְ�
				if (haar_mode == 0) Cascade_Scale(cascade_rect, cascade_rect_scale, factor, inv_window_area);
				else if (haar_mode == 1) Cascade_Scale(cascade_rect2, cascade_rect_scale2, factor, inv_window_area);
				break;
		}

#ifdef SIMD
        if(g_mode == 1 || g_mode == 3)
        {
            rect.simd.temp[W] = winSize.width;
            rect.simd.temp[H] = winSize.height;
        }
        else
#endif
        {
            rect.width = winSize.width;
            rect.height = winSize.height;
        }

		for (y = 0; (y + winSize.height) < _image->rows; y += increment)
		{
			for (x = 0; (x + winSize.width) < _image->cols; x += increment)
			{
#ifdef SIMD
                if(g_mode == 1 || g_mode == 3)
                {
                    rect.simd.temp[X] = x;
                    rect.simd.temp[Y] = y;
                    //rect_line.simd.reg = _mm_add_ps(_mm_mul_ps(rect.simd.reg, scale_rect.reg), add_rect.reg);
                    rect_line.simd.temp[X] = rect.simd.temp[X]  * f_scale + point_roi.x;
                    rect_line.simd.temp[Y] = rect.simd.temp[Y]  * f_scale + point_roi.y;
                    rect_line.simd.temp[W] = rect.simd.temp[W]  * f_scale;
                    rect_line.simd.temp[H] = rect.simd.temp[H]  * f_scale;
                }
                else
#endif
                {
                    rect.x = x;
                    rect.y = y;
                    rect_line.x = rect.x * f_scale + point_roi.x;
                    rect_line.y = rect.y * f_scale + point_roi.y;
                    rect_line.width = rect.width * f_scale;
                    rect_line.height = rect.height * f_scale;
                }

				vanishing_line_bool = Vanishing_Line_Detect(rect_line);
				if (vanishing_line_bool == false)	continue;

				mean_factor = Image_Sum(rect) * inv_window_area;
				variance_norm_factor = Image_Dsum(rect) * inv_window_area;
				variance_norm_factor = variance_norm_factor - (mean_factor * mean_factor);
				variance_norm_factor = (variance_norm_factor >= 0.0) ? sqrt(variance_norm_factor) : 1;

				switch(g_DayNight)
				{
					case 1:		//�߰�
						for (i = 0; i < cascade3.stage_size; i++)  //stage size
						{
							vehicle_detect_temp = Cascade_Compute(cascade3.stage[i], cascade_rect_scale3, rect, variance_norm_factor);
							if (!vehicle_detect_temp) break;
						}
						break;

					default:	//�ְ�
						if (haar_mode == 0)		//haar 20 x 20 mode
						{
#ifdef __OPENCV_USE__
							haar_detect = 0;	//haar 20 x 20 mode
#endif //__OPENCV_USE__

							for (i = 0; i < cascade.stage_size; i++)  //stage size
							{
								vehicle_detect_temp = Cascade_Compute(cascade.stage[i], cascade_rect_scale, rect, variance_norm_factor);
								if (!vehicle_detect_temp) break;
							}
						}
						else if (haar_mode == 1)	//haar 30 x 30 mode
						{
#ifdef __OPENCV_USE__
							haar_detect = 1;	//haar 30 x 30 mode
#endif //__OPENCV_USE__

							for (i = 0; i < cascade2.stage_size; i++)  //stage size
							{
								vehicle_detect_temp = Cascade_Compute(cascade2.stage[i], cascade_rect_scale2, rect, variance_norm_factor);
								if (!vehicle_detect_temp) break;
							}
						}
						break;
				}
				if (!vehicle_detect_temp) continue;

#ifdef SIMD
                if(g_mode == 1 || g_mode == 3)
                {
                    rect_line.x = rect_line.simd.temp[X];
                    rect_line.y = rect_line.simd.temp[Y];
                    rect_line.width = rect_line.simd.temp[W];
                    rect_line.height = rect_line.simd.temp[H];
                }
#endif
				vehicle[vehicle_detect_size] = rect_line;
				vehicle_detect_size++;

				//if(g_mode == 0 || g_mode == 1)
                {
                    if (vehicle_detect_size >= 50) vehicle_detect_size = 49;
                }
			}
		}
	}

	if (vehicle_detect_size > 0)
	{
		vehicle->size = vehicle_detect_size;
		Rectangles_Group(vehicle, min_neighbors);

		if (vehicle->size == 0)	return false;
		else
		{
			detect_count = vehicle->size;

			image_roi.x = point_roi.x;
			image_roi.y = point_roi.y;
			image_roi.width = point_roi.width;
			image_roi.height = point_roi.height;

			return true;
		}
	}
	else return false;
}

void Cascade_Scale(CarviFeature *cascade_rect, CarviFeature *cascade_rect_temp, float factor, float inv_window_area)
{
	int i, k;
	float correction_ratio, area0, sum0;
	CarviFeature temp;

	for (i = 0; i < cascade_rect->size; i++)
	{
		temp = cascade_rect[i];

		correction_ratio = inv_window_area * (!temp.tilted ? 1 : 0.5);

		for (k = 0; k < temp.rect_size; k++)
		{
			temp.rect[k].r.x *= factor;
			temp.rect[k].r.y *= factor;
			temp.rect[k].r.width *= factor;
			temp.rect[k].r.height *= factor;
			temp.rect[k].weight *= correction_ratio;

#ifdef SIMD
            if(g_mode == 1 || g_mode == 3)
            {
                temp.rect[k].r.simd.temp[X] = temp.rect[k].r.x;
                temp.rect[k].r.simd.temp[Y] = temp.rect[k].r.y;
                temp.rect[k].r.simd.temp[W] = temp.rect[k].r.width;
                temp.rect[k].r.simd.temp[H] = temp.rect[k].r.height;
            }
#endif
		}

		area0 = temp.rect[0].r.width * temp.rect[0].r.height;
		sum0 = temp.rect[1].weight * temp.rect[1].r.width * temp.rect[1].r.height;

		if (temp.rect_size == 3)	sum0 += temp.rect[2].weight * temp.rect[2].r.width * temp.rect[2].r.height;

		temp.rect[0].weight = (-sum0 / area0);

		cascade_rect_temp[i] = temp;
	}
}

uchar Cascade_Compute(CarviStageClassifier cascade, CarviFeature *cascade_rect_scale, CRect rect, float var)
{
	if (Image_Classify(cascade, cascade_rect_scale, rect, var) == false)
	{
		return false; // The image has been rejected.
	}

	return true; // The image has been detected.
}

void Image_Integral(CMat *image, int *nsum, float *dsum, int *tsum)
{
	int x, y;
	int width = image->cols;
	int height = image->rows;
	int iwidth = image->cols + 1;
	int iheight = image->rows + 1;
	int twidth = image->cols + 2;
	int theight = image->rows + 2;
	int yy, y1, p1, p2, r, a, b, c;

	for (y = 0; y < theight; y++)
	{
		for (x = 0; x < twidth; x++)
		{
			if (y < iheight && x < iwidth)
			{
				nsum[y*iwidth + x] = 0;
				dsum[y*iwidth + x] = 0;
			}
			tsum[y*twidth + x] = 0;
		}
	}

	for (y = 1; y <= height; y++)
	{
		yy = iwidth * (y);
		y1 = iwidth * (y - 1);

		// for each pixel
		for (x = 1; x <= width; x++)
		{
			p1 = image->data[((y - 1)*width) + (x - 1)];
			p2 = p1 * p1;

			r = yy + (x);
			a = yy + (x - 1);
			b = y1 + (x);
			c = y1 + (x - 1);

			nsum[r] = p1 + nsum[a] + nsum[b] - nsum[c];
			dsum[r] = p2 + dsum[a] + dsum[b] - dsum[c];
		}
	}

	// Left-to-right, top-to-bottom pass
	for (y = 1; y <= height; y++)
	{
		yy = twidth * (y);
		y1 = twidth * (y - 1);

		for (x = 2; x < width + 2; x++)
		{
			a = y1 + (x - 1);
			b = yy + (x - 1);
			c = y1 + (x - 2);
			r = yy + (x);

			tsum[r] = image->data[((y - 1)*width) + (x - 2)] + tsum[a] + tsum[b] - tsum[c];
		}
	}

	{
		yy = twidth * (height);
		y1 = twidth * (height + 1);

		for (x = 2; x < width + 2; x++)
		{
			a = yy + (x - 1);
			c = yy + (x - 2);
			b = y1 + (x - 1);
			r = y1 + (x);

			tsum[r] = tsum[a] + tsum[b] - tsum[c];
		}
	}

	// Right-to-left, bottom-to-top pass
	for (y = height; y >= 0; y--)
	{
		yy = twidth * (y);
		y1 = twidth * (y + 1);

		for (x = width + 1; x >= 1; x--)
		{
			r = yy + (x);
			b = y1 + (x - 1);

			tsum[r] += tsum[b];
		}
	}

	for (y = height + 1; y >= 0; y--)
	{
		yy = twidth * (y);

		for (x = width + 1; x >= 2; x--)
		{
			r = yy + (x);
			b = yy + (x - 2);

			tsum[r] -= tsum[b];
		}
	}
}

float Image_Sum(CRect rect)
{
	unsigned int *ii1 = (unsigned int *)_image_integral;
	int width = _image->cols + 1;
	int height = _image->rows + 1;
	int a,b,c,d;
#ifdef SIMD
    if(g_mode == 1 || g_mode == 3)
    {
        a = rect.simd.temp[X] + (rect.simd.temp[Y] * width);
        b = (rect.simd.temp[X] + rect.simd.temp[W]) + (rect.simd.temp[Y] * width);
        c = rect.simd.temp[X] + ((rect.simd.temp[Y] + rect.simd.temp[H]) * width);
        d = (rect.simd.temp[X] + rect.simd.temp[W]) + (rect.simd.temp[Y] + rect.simd.temp[H]) * width;
    }
    else
#endif
    {
        a = rect.x + (rect.y * width);
        b = (rect.x + rect.width) + (rect.y * width);
        c = rect.x + ((rect.y + rect.height) * width);
        d = (rect.x + rect.width) + (rect.y + rect.height) * width;
	}

	float mx = ii1[a] + ii1[d] - ii1[b] - ii1[c];

	return mx;
}

float Image_Tsum(CRect rect)
{
	unsigned int *tii1 = (unsigned int *)_image_tintegral;
	int width = _image->cols + 2;
	int height = _image->rows + 2;
	int a,b,c,d;
#ifdef SIMD
    if(g_mode == 1 || g_mode == 3)
    {
        a = (rect.simd.temp[X] + rect.simd.temp[W]) + (rect.simd.temp[Y] + rect.simd.temp[W]) * width;
        b = rect.simd.temp[X] + rect.simd.temp[Y] * width;
        c = (rect.simd.temp[X] + rect.simd.temp[W] - rect.simd.temp[H]) +
                (rect.simd.temp[Y] + rect.simd.temp[W] + rect.simd.temp[H]) * width;
        d = (rect.simd.temp[X] - rect.simd.temp[H]) + (rect.simd.temp[Y] + rect.simd.temp[H]) * width;
    }
    else
#endif
    {
        a = (rect.x + rect.width) + (rect.y + rect.width) * width;
        b = rect.x + rect.y * width;
        c = (rect.x + rect.width - rect.height) + (rect.y + rect.width + rect.height) * width;
        d = (rect.x - rect.height) + (rect.y + rect.height) * width;
    }

	float tmx = tii1[a] + tii1[d] - tii1[b] - tii1[c];

	return tmx;
}

float Image_Dsum(CRect rect)
{
	float *ii2 = (float *)_image_dintegral;
	int width = _image->cols + 1;
	int height = _image->rows + 1;
    int a,b,c,d;
#ifdef SIMD
    if(g_mode == 1 || g_mode == 3)
    {
        a = rect.simd.temp[X] + (rect.simd.temp[Y] * width);
        b = (rect.simd.temp[X] + rect.simd.temp[W]) + (rect.simd.temp[Y] * width);
        c = rect.simd.temp[X] + ((rect.simd.temp[Y] + rect.simd.temp[H]) * width);
        d = (rect.simd.temp[X] + rect.simd.temp[W]) + (rect.simd.temp[Y] + rect.simd.temp[H]) * width;
    }
    else
#endif
    {
        a = rect.x + (rect.y * width);
        b = (rect.x + rect.width) + (rect.y * width);
        c = rect.x + ((rect.y + rect.height) * width);
        d = (rect.x + rect.width) + (rect.y + rect.height) * width;
    }

	float mx2 = ii2[a] + ii2[d] - ii2[b] - ii2[c];

	return mx2;
}

float Image_Compute(CarviFeature cascade, CRect rect)
{
	int i;
	float threshold = 0;
	CRect rect_temp;

#ifdef SIMD
    if(g_mode == 1 || g_mode == 3)
    {
        rect.simd.temp[W] = 0;
        rect.simd.temp[H] = 0;
    }
#endif
	for (i = 0; i < cascade.rect_size; i++)
	{
#ifdef SIMD
        if(g_mode == 1 || g_mode == 3)
        {
            rect_temp.simd.reg = _mm_add_epi32(rect.simd.reg, cascade.rect[i].r.simd.reg);
        }
        else
#endif
        {
            rect_temp.x = rect.x + cascade.rect[i].r.x;
            rect_temp.y = rect.y + cascade.rect[i].r.y;
            rect_temp.width = cascade.rect[i].r.width;
            rect_temp.height = cascade.rect[i].r.height;
        }

        if (cascade.tilted)	threshold += Image_Tsum(rect_temp) * cascade.rect[i].weight;
		else threshold += Image_Sum(rect_temp) * cascade.rect[i].weight;
	}
	return threshold;
}

CRect rect_t[200][3];

uchar Image_Classify(CarviStageClassifier cascade, CarviFeature *cascade_rect_scale, CRect rect, float var)
{
	int y, nodeidx;
	float value = 0, threshold;

#ifdef SIMD
    if(g_mode == 1 || g_mode == 3)
    {
        CRect rect_temp;
        rect.simd.temp[W] = 0;
        rect.simd.temp[H] = 0;

        for (y = 0; y < cascade.tree_size; y++) //tree size
        {
            nodeidx = cascade.tree[y].nodeidx;

            rect_t[y][0].simd.reg = _mm_add_epi32(rect.simd.reg,
                                                  cascade_rect_scale[cascade.tree[y].nodeidx].rect[0].r.simd.reg);
            rect_t[y][1].simd.reg = _mm_add_epi32(rect.simd.reg,
                                                  cascade_rect_scale[cascade.tree[y].nodeidx].rect[1].r.simd.reg);

            if (cascade_rect_scale[cascade.tree[y].nodeidx].tilted) threshold = Image_Tsum(rect_t[y][0]) *
                                                                                cascade_rect_scale[cascade.tree[y].nodeidx].rect[0].weight +
                                                                                Image_Tsum(rect_t[y][1]) *
                                                                                cascade_rect_scale[cascade.tree[y].nodeidx].rect[1].weight;
            else threshold = Image_Sum(rect_t[y][0]) * cascade_rect_scale[cascade.tree[y].nodeidx].rect[0].weight +
                             Image_Sum(rect_t[y][1]) * cascade_rect_scale[cascade.tree[y].nodeidx].rect[1].weight;

            if (cascade_rect_scale[cascade.tree[y].nodeidx].rect_size == 3) {
                rect_t[y][2].simd.reg = _mm_add_epi32(rect.simd.reg,
                                                      cascade_rect_scale[cascade.tree[y].nodeidx].rect[2].r.simd.reg);
                if (cascade_rect_scale[cascade.tree[y].nodeidx].tilted) threshold += Image_Tsum(rect_t[y][2]) *
                                                                                     cascade_rect_scale[cascade.tree[y].nodeidx].rect[2].weight;
                else threshold += Image_Sum(rect_t[y][2]) * cascade_rect_scale[cascade.tree[y].nodeidx].rect[2].weight;
            }

            if (threshold < cascade.tree[y].threshold * var) value += cascade.tree[y].l_val;    //tree threshold
            else value += cascade.tree[y].r_val;
        }

    }
//    for (y = 0; y < cascade.tree_size; y++) //tree size
//    {
//        threshold = 0;
//        nodeidx = cascade.tree[y].nodeidx;
//
//        for (int i = 0; i < cascade_rect_scale[nodeidx].rect_size; i++)
//        {
//            if (cascade_rect_scale[nodeidx].tilted)	threshold += Image_Tsum(rect_t[y][i]) * cascade_rect_scale[nodeidx].rect[i].weight;
//            else threshold += Image_Sum(rect_t[y][i]) * cascade_rect_scale[nodeidx].rect[i].weight;
//        }
//
//        if (threshold < cascade.tree[y].threshold * var) value += cascade.tree[y].l_val;	//tree threshold
//        else value += cascade.tree[y].r_val;
//    }
    else
#endif
    {
        for (y = 0; y < cascade.tree_size; y++) //tree size
        {
            nodeidx = cascade.tree[y].nodeidx;
            threshold = Image_Compute(cascade_rect_scale[nodeidx], rect);

            if (threshold < cascade.tree[y].threshold * var) value += cascade.tree[y].l_val;	//tree threshold
            else value += cascade.tree[y].r_val;
        }
    }

	if (value < cascade.threshold) return false;	//stage threshold
	else return true;
}

void Rectangles_Group(CRect *rectList, int groupThreshold)
{
	CRect rectGroup[50];
	CRect rect_big, rect_small;
	CRect rect_temp;
	//CPoint center;
	CPoint center_big, center_small;

	int i = 0, j = 0, group_size = 0, rectList_size;
	int distance_min = 0, temp = 0;

	float center_distance;

	rectList_size = rectList->size;

	for (i = 0; i < rectList_size; i++)
	{
		int rect_size = 0;
		if (rectList[i].width == 0 && rectList[i].height == 0) continue;
		for (j = i + 1; j < rectList_size; j++)
		{
			if (rectList[j].width == 0 && rectList[j].height == 0) continue;
			if (rectList[i].width >= rectList[j].width && rectList[i].height >= rectList[j].height)
			{
				rect_big = rectList[i];
				rect_small = rectList[j];
				//center.x = rect_small.x + rect_small.width / 2.0;
				//center.y = rect_small.y + rect_small.height / 2.0;
			}
			else
			{
				rect_big = rectList[j];
				rect_small = rectList[i];
				//center.x = rect_small.x + rect_small.width / 2.0;
				//center.y = rect_small.y + rect_small.height / 2.0;
			}
			center_big.x = rect_big.x + rect_big.width / 2.0;
			center_big.y = rect_big.y + rect_big.height / 2.0;

			center_small.x = rect_small.x + rect_small.width / 2.0;
			center_small.y = rect_small.y + rect_small.height / 2.0;

			center_distance = sqrt((center_big.x - center_small.x) * (center_big.x - center_small.x) + (center_big.y - center_small.y) * (center_big.y - center_small.y));

			if ((int)center_distance <= 5)
			{
				rectList[i] = rect_big;
				rectList[j].width = 0;
				rectList[j].height = 0;
				rect_size++;
			}
		}
		if (rect_size >= groupThreshold)
		{
			rectGroup[group_size] = rectList[i];
			group_size++;
		}
	}

	distance_min = rectGroup[0].y + rectGroup[0].height;
	for (j = 0; j < group_size; j++)
	{
		for (i = j + 1; i < group_size - 1; i++)
		{
			temp = rectGroup[i].y + rectGroup[i].height;
			if (temp > distance_min)
			{
				distance_min = temp;
				rect_temp = rectGroup[i];
				rectGroup[i] = rectGroup[j];
				rectGroup[j] = rect_temp;
			}
		}
	}

	rectGroup->size = group_size;
	*rectList = *rectGroup;

	for (i = 1; i < rectGroup->size; i++)
	{
		rectList[i] = rectGroup[i];
	}
}

int Cascade_Read_txt_gets(FILE *file, char *buffer, const char* control)
{
	int i;
	while (!feof(file))
	{
		char temp[20];
		int position = 0;
		fgets(buffer, 100, file);
		for (i = 0; i < strlen(buffer); i++)
		{
			if (buffer[i] != ' ' && buffer[i] != '\r' && buffer[i] != '\n')
			{
				temp[position] = buffer[i];
				position++;
			}
			else if (position != 0)
			{
				temp[position] = '\0';
				position = 0;
				if (!strcmp(temp, control)) return i;
			}
		}
	}
}

float *Cascade_Read_txt_values(char *buffer, int start)
{
	char temp[20];
	int i, position = 0, value_position = 0;
	float value[10];
	for (i = start; i < strlen(buffer); i++)
	{
		if (buffer[i] != ' ' && buffer[i] != '\r' && buffer[i] != '\n')
		{
			temp[position] = buffer[i];
			position++;
		}
		else if (position != 0)
		{
			temp[position] = '\0';
			position = 0;
			value[value_position] = atof(temp);
			value_position++;
		}
	}
	float *p_value = value;
	return p_value;
}

uchar Cascade_Read_txt(const char* input, CarviClassifier *cascade, CarviFeature **cascade_rect, CarviFeature **cascade_rect_scale)
{
	FILE *Cascade_File;
	char buffer[100];
	int stage, stage_state, tree, position = 0;
	int feature, k;
	int cascade_rect_size;
	float *value;

	if((Cascade_File = fopen(input, "r")) == NULL)
	{
		printf("Cascade_File open fail!: %s\n", input);
		return false;
	}
	else printf("Cascade_File open success!: %s\n", input);

	position = Cascade_Read_txt_gets(Cascade_File, buffer, "height");
	cascade->height = atoi(buffer + position);

	position = Cascade_Read_txt_gets(Cascade_File, buffer, "width");
	cascade->width = atoi(buffer + position);

	position = Cascade_Read_txt_gets(Cascade_File, buffer, "stagenum");
	cascade->stage_size = atoi(buffer + position);
	cascade->stage = (CarviStageClassifier *)malloc(sizeof(CarviStageClassifier)* cascade->stage_size);

	for (stage = 0; stage < cascade->stage_size; stage++)
	{
		position = Cascade_Read_txt_gets(Cascade_File, buffer, "stage");
		stage_state = atoi(buffer + position);
		if (stage_state != stage)	return false;
		position = Cascade_Read_txt_gets(Cascade_File, buffer, "treesize");
		cascade->stage[stage].tree_size = atoi(buffer + position);

		position = Cascade_Read_txt_gets(Cascade_File, buffer, "stagethreshold");
		cascade->stage[stage].threshold = atof(buffer + position);
		cascade->stage[stage].tree = (CarviTreeClassifier *)malloc(sizeof(CarviTreeClassifier)* cascade->stage[stage].tree_size);
//		std::cout << cascade.stage[stage].tree_size << endl;
//		std::cout << cascade.stage[stage].threshold << endl;
		for (tree = 0; tree < cascade->stage[stage].tree_size; tree++)
		{
			position = Cascade_Read_txt_gets(Cascade_File, buffer, "internal");
			value = Cascade_Read_txt_values(buffer, position);
			cascade->stage[stage].tree[tree].l_val = value[0];
			cascade->stage[stage].tree[tree].r_val = value[1];
			cascade->stage[stage].tree[tree].nodeidx = (int)(value[2]);
			cascade->stage[stage].tree[tree].threshold = value[3];

			//cout << cascade.stage[stage].tree[tree].l_val <<" ";
			//cout << cascade.stage[stage].tree[tree].r_val << " ";
			//cout << cascade.stage[stage].tree[tree].nodeidx << " ";
			//cout << cascade.stage[stage].tree[tree].threshold << endl;
		}
	}
	position = Cascade_Read_txt_gets(Cascade_File, buffer, "featurenum");
	cascade_rect_size = atoi(buffer + position);

	(*cascade_rect) = (CarviFeature *)malloc(sizeof(CarviFeature)* cascade_rect_size);
	(*cascade_rect_scale) = (CarviFeature *)malloc(sizeof(CarviFeature)* cascade_rect_size);
	(*cascade_rect)->size = cascade_rect_size;

	for (feature = 0; feature < (*cascade_rect)->size; feature++)
	{
		position = Cascade_Read_txt_gets(Cascade_File, buffer, "rectnum");
		(*cascade_rect)[feature].rect_size = atoi(buffer + position);
		position = Cascade_Read_txt_gets(Cascade_File, buffer, "rects");

		for (k = 0; k < (*cascade_rect)[feature].rect_size; k++)
		{
			fgets(buffer, sizeof(buffer), Cascade_File);
			value = Cascade_Read_txt_values(buffer, 0);
			(*cascade_rect)[feature].rect[k].r.x = (int)(value[0]);
			(*cascade_rect)[feature].rect[k].r.y = (int)(value[1]);
			(*cascade_rect)[feature].rect[k].r.width = (int)(value[2]);
			(*cascade_rect)[feature].rect[k].r.height = (int)(value[3]);
			(*cascade_rect)[feature].rect[k].weight = value[4];

#ifdef SIMD
            if(g_mode == 1 || g_mode == 3)
            {
                (*cascade_rect)[feature].rect[k].r.simd.temp[X] = (int) (value[0]);
                (*cascade_rect)[feature].rect[k].r.simd.temp[Y] = (int) (value[1]);
                (*cascade_rect)[feature].rect[k].r.simd.temp[W] = (int) (value[2]);
                (*cascade_rect)[feature].rect[k].r.simd.temp[H] = (int) (value[3]);
            }
#endif

			//cout << cascade_rect[feature].rect[k].r.x <<" ";
			//cout << cascade_rect[feature].rect[k].r.y << " ";
			//cout << cascade_rect[feature].rect[k].r.width << " ";
			//cout << cascade_rect[feature].rect[k].r.height << " ";
			//cout << cascade_rect[feature].rect[k].weight << endl;
		}
		position = Cascade_Read_txt_gets(Cascade_File, buffer, "tilted");
		(*cascade_rect)[feature].tilted = (atoi(buffer + position) != 0) ? true : false;
		//cout << cascade_rect[feature].tilted << " ";
		//cout << cascade_rect[feature].rect_size << endl;
	}
	//cout << cascade.height << endl;
	//cout << cascade.width << endl;
	//cout << cascade.stage_size << endl;

	fclose(Cascade_File);

	printf("Cascade_Read Success \n");
	return true;
}

uchar Vehicle_Detect(CMat image_ori, CRect image_roi, CRect *vehicle)
{
  uchar vehicle_detect_temp;
	int x = 0, y = 0;
	CSize HAARSIZE = { cascade.width, cascade.height }; 									//Cascade �н� �� ������ ũ��
/*
	vehicle_detect_image.cols = image_roi.width;											//���� �����ϱ� ���� ����� �̹��� ũ��(���氡��)
	vehicle_detect_image.rows = image_roi.height;											//���� �����ϱ� ���� ����� �̹��� ũ��(���氡��)

	for (y = 0; y < vehicle_detect_image.rows; y++)
	{
		for (x = 0; x < vehicle_detect_image.cols; x++)
		{
			vehicle_detect_image.data[y*vehicle_detect_image.cols + x] = image_ori.data[(y + image_roi.y)*image_ori.cols + (x + image_roi.x)];	//ROI �̹��� �ֱ�
		}
	}
*/
	//vehicle_detect = Cascade_Detect(vehicle_detect_image, vehicle, 1.21, 1, HAARSIZE);	//���� ���� �� true ��ȯ

	vehicle_detect_temp = Cascade_Detect(image_ori, vehicle, 1.21, 3, HAARSIZE);	//���� ���� �� true ��ȯ

	return vehicle_detect_temp;
}

uchar Vehicle_Forward_Detect(CRect image_roi, CRect *vehicle, CRect *vehicle_forward_position)
{
    uchar vanishing_line_bool = false;
    uchar vehicle_forward_bool = false;
	int i = 0;

	for (i = 0; i < vehicle->size; i++)
	{
		//vehicle[i].x += image_roi.x;
		//vehicle[i].y += image_roi.y;
#ifdef __OPENCV_USE__
		//rectangle(opencv_image, Rect(vehicle[i].x, vehicle[i].y, vehicle[i].width, vehicle[i].height), Scalar(255, 255, 0), 2, 8, 0);
#endif //__OPENCV_USE__
		vanishing_line_bool = Vanishing_Line_Detect(vehicle[i]);

		if (vanishing_line_bool)
		{
			*vehicle_forward_position = vehicle[i];
			vehicle_forward_bool = true;
		}
	}
	if (vehicle_forward_bool) return true;
	else return false;
}

uchar Vanishing_Line_Detect(CRect vehicle)
{
    uchar vanishing_point_bool = false;
	float xl, xr;
	CPoint point_l;
	CPoint point_r;
	CPoint point_c;

#ifdef SIMD
    if(g_mode == 1 || g_mode == 3)
    {
        point_l.x = vehicle.simd.temp[X] + (vehicle.simd.temp[W] * 1.0 / 3.0);
        point_l.y = vehicle.simd.temp[Y] + vehicle.simd.temp[H];
        point_r.x = vehicle.simd.temp[X] + (vehicle.simd.temp[W] * 2.0 / 3.0);
        point_r.y = vehicle.simd.temp[Y] + vehicle.simd.temp[H];
    }
    else
#endif
    {
        point_l.x = vehicle.x + (vehicle.width * 1.0 / 3.0);
        point_l.y = vehicle.y + vehicle.height;
        point_r.x = vehicle.x + (vehicle.width * 2.0 / 3.0);
        point_r.y = vehicle.y + vehicle.height;
    }

	point_c.x = (point_l.x + point_r.x) / 2.0;					point_c.y = (point_l.y + point_r.y) / 2.0;

	xl = (float)(vanishing_point.x - vanishing_point_l.x) / (vanishing_point.y - vanishing_point_l.y)*(point_l.y - vanishing_point.y) + vanishing_point.x;
	xr = (float)(vanishing_point.x - vanishing_point_r.x) / (vanishing_point.y - vanishing_point_r.y)*(point_l.y - vanishing_point.y) + vanishing_point.x;
	if (point_l.x >= xl && point_l.x <= xr) vanishing_point_bool = true;
	else if (point_r.x >= xl && point_r.x <= xr) vanishing_point_bool = true;
	else if (point_c.x >= xl && point_c.x <= xr) vanishing_point_bool = true;

	return vanishing_point_bool;
}

#ifdef SIMD
SIMDFRect frect, frect2;
SIMDRect irect, irect_t, irect2, irect3;
#endif

uchar Vehicle_Tracking(CMat image_ori, CRect image_roi_temp, float real_velocity, CRect *vehicle_roi, float *width_ttc, float *width_distance)
{
    uchar vehicle_detect_temp = false;				//vehicle detect boolean
    uchar vehicle_forward_detect = false;			//vehicle forward detect boolean
	uchar pixel = 0;

	static int vehicle_forward_percent = 0;				//tracking haar detection fail counter
	static int vehicle_tracking_counter = 0;			//tracking & haar detection implement cycle
	int i = 0, x = 0, y = 0;
	int x_position, y_position;

	static float residual_gain[TRACKING_WINDOW_SIZE][TRACKING_WINDOW_SIZE] = { 1.0 };	//tracking residual gain
	float percent_threshold = 0;						//tracking percent threshold
	float real_distance = 0, x_scale = 0, y_scale = 0, v = 0;

	static CRect vehicle_forward_position = {0};		//forward vehicle position
	static CRect vehicle_forward_position_pre = {0};	//forward vehicle previous position
	CRect vehicle_forward_update = {0};					//forward vehicle position update
	CRect vehicle[50] = {0};							//�ĺ����� ��
	CRect rect = {0};

	if (vehicle_forward_tracking) //���� ���� tracking
	{
        uchar percent_threshold_init = false;							//tracking percent threshold init boolean
		int w_x, w_y, w_s, w_s_t, size_range, increment;					//tracking x, y, scale, scale_temp, range, increment
		int residual[TRACKING_WINDOW_SIZE][TRACKING_WINDOW_SIZE] = { 0 };	//tracking residual
		float w_scale = 1.0;
		float cx = vehicle_forward_position.x + (vehicle_forward_position.width / 2.0);
		float cy = vehicle_forward_position.y + (vehicle_forward_position.height / 2.0);
		float residual_mean = 0, residual_std = 0, residual_d = 0;			//mean, standard deviation, deviation
		float x_w_alpha, y_w_alpha;											//optimization variable
		float x_ratio, y_ratio;												//optimization variable
		float x_r_alpha, y_r_alpha;											//optimization variable

		increment = 1;
		size_range = 4;

		for (w_s = -size_range; w_s <= size_range; w_s += increment)
		{
			if (w_s < 0)
			{
				w_scale = 1.0;
				for (w_s_t = w_s; w_s_t < 0; w_s_t++) w_scale /= 1.02;
			}
			else if (w_s > 0)
			{
				w_scale = 1.0;
				for (w_s_t = 0; w_s_t < w_s; w_s_t++) w_scale *= 1.02;
			}
			else w_scale = 1.0;

			rect.width = vehicle_forward_position.width * w_scale;
			rect.height = vehicle_forward_position.height * w_scale;

			if (rect.width < 10 || rect.height < 10) continue;

			x_r_alpha = w_scale * vehicle_forward_position.x;
			y_r_alpha = w_scale * vehicle_forward_position.y;

			x_w_alpha = (1.0 - w_scale) * cx;
			y_w_alpha = (1.0 - w_scale) * cy;

			x_ratio = (float)rect.width / vehicle_image.cols;
			y_ratio = (float)rect.height / vehicle_image.rows;

			for (w_y = -size_range; w_y <= size_range; w_y += increment)
			{
				for (w_x = -size_range; w_x <= size_range; w_x += increment)
				{
					float vehicle_per = 0;
					float x_alpha = x_w_alpha + w_x;
					float y_alpha = y_w_alpha + w_y;

					//x = s * p_x + (1 - s) * c_x + t_x,	p_x = r_x + x * ratio
					//y = s * p_y + (1 - s) * c_y + t_y,	p_y = r_y + y * ratio
					rect.x = x_r_alpha + x_alpha;
					rect.y = y_r_alpha + y_alpha;
					//rect.width = vehicle_forward_position.width * w_scale;
					//rect.height = vehicle_forward_position.height * w_scale;

					if (rect.x < 0 || rect.y < 0) continue;
					else if (rect.x + rect.width >= image_ori.cols || rect.y + rect.height >= image_ori.rows) break;

#ifdef SIMD
                    if(g_mode == 1 || g_mode == 3)
                    {
                        for (y = 0; y < vehicle_image.rows; y++)
                        {
                            y_position = (int)(rect.y + (y * y_ratio)) * image_ori.cols;
                            irect2.reg = _mm_set1_epi32(y_position);
                            int yy = y * vehicle_image.cols;

                            for (x = 0; x < vehicle_image.cols; x+=5)
                            {
                                //x_position = (int)(rect.x + (x * x_ratio));
                                //y_position = (int)(rect.y + (y * y_ratio));
                                irect.temp[0] = (int)(rect.x + ((x+0) * x_ratio));
                                irect.temp[1] = (int)(rect.x + ((x+1) * x_ratio));
                                irect.temp[2] = (int)(rect.x + ((x+2) * x_ratio));
                                irect.temp[3] = (int)(rect.x + ((x+3) * x_ratio));
                                x_position    = (int)(rect.x + ((x+4) * x_ratio));

                                irect_t.reg = _mm_add_epi32(irect.reg, irect2.reg);
                                irect.temp[0] = image_ori.data[irect_t.temp[0]];
                                irect.temp[1] = image_ori.data[irect_t.temp[1]];
                                irect.temp[2] = image_ori.data[irect_t.temp[2]];
                                irect.temp[3] = image_ori.data[irect_t.temp[3]];

                                irect3.temp[0] = vehicle_image.data[yy + x + 0];
                                irect3.temp[1] = vehicle_image.data[yy + x + 1];
                                irect3.temp[2] = vehicle_image.data[yy + x + 2];
                                irect3.temp[3] = vehicle_image.data[yy + x + 3];

                                irect.reg = _mm_sub_epi32(irect.reg, irect3.reg);

                                frect.temp[0] = residual_gain[y][x + 0];
                                frect.temp[1] = residual_gain[y][x + 1];
                                frect.temp[2] = residual_gain[y][x + 2];
                                frect.temp[3] = residual_gain[y][x + 3];

                                frect2.temp[0] = abs(irect.temp[0]);
                                frect2.temp[1] = abs(irect.temp[1]);
                                frect2.temp[2] = abs(irect.temp[2]);
                                frect2.temp[3] = abs(irect.temp[3]);

                                frect.reg = _mm_mul_ps(frect.reg, frect2.reg);

//                                residual[y][x + 0] = irect.temp[0];
//                                residual[y][x + 1] = irect.temp[1];
//                                residual[y][x + 2] = irect.temp[2];
//                                residual[y][x + 3] = irect.temp[3];
//                                residual[y][x + 4] = image_ori.data[y_position + x_position] - vehicle_image.data[yy + x + 4];

                                float temp = residual_gain[y][x + 4] * abs(image_ori.data[y_position + x_position] - vehicle_image.data[yy + x + 4]);
                                vehicle_per = vehicle_per + frect.temp[0] + frect.temp[1] + frect.temp[2] + frect.temp[3] + temp;
                                //residual[y][x] = image_ori.data[y_position * image_ori.cols + x_position] - vehicle_image.data[y * vehicle_image.cols + x];
                                //vehicle_per += (residual_gain[y][x] * abs(residual[y][x]));
                            }
                        }
					}
					else
#endif
                    {
                        for (y = 0; y < vehicle_image.rows; y++)
                        {
                            for (x = 0; x < vehicle_image.cols; x++)
                            {
                                //	p_x = r_x + x * ratio
                                //	p_y = r_y + y * ratio
                                //ratio = r_width / TRACKING_WINDOW_SIZE
                                x_position = (int)(rect.x + (x * x_ratio));
                                y_position = (int)(rect.y + (y * y_ratio));

                                residual[y][x] = image_ori.data[y_position * image_ori.cols + x_position] - vehicle_image.data[y * vehicle_image.cols + x];

                                vehicle_per += (residual_gain[y][x] * abs(residual[y][x]));
                            }
                        }
					}

					if (!percent_threshold_init) percent_threshold_init = true;
					else if ((int)(vehicle_per * 100) >= (int)(percent_threshold * 100)) continue;

					percent_threshold = vehicle_per;
#ifdef SIMD
                    if(g_mode == 1 || g_mode == 3)
                    {
                        rect.simd.temp[X] = rect.x;
                        rect.simd.temp[Y] = rect.y;
                        rect.simd.temp[W] = rect.width;
                        rect.simd.temp[H] = rect.height;
                    }
#endif
					vehicle_forward_update = rect;
				}
			}
		}

		x_ratio = (float)vehicle_forward_update.width / vehicle_image.cols;
		y_ratio = (float)vehicle_forward_update.height / vehicle_image.rows;

		for (y = 0; y < vehicle_image.rows; y++)
		{
			for (x = 0; x < vehicle_image.cols; x++)
			{
				x_position = (int)(vehicle_forward_update.x + (x * x_ratio));
				y_position = (int)(vehicle_forward_update.y + (y * y_ratio));

				residual[y][x] = image_ori.data[y_position * image_ori.cols + x_position] - vehicle_image.data[y * vehicle_image.cols + x];

				residual_gain[y][x] = 1.0 / (1 + 0.1 * abs(residual[y][x]));
			}
		}
	}

	if (!vehicle_forward_tracking || vehicle_tracking_counter > 0)
	{
		forward_vehicle_init = false;
		vehicle_detect_temp = Vehicle_Detect(image_ori, image_roi_temp, vehicle);	//��������
	}

	if (vehicle_detect_temp) vehicle_forward_detect = Vehicle_Forward_Detect(image_roi_temp, vehicle, &vehicle_forward_update);	//���� ���� �� ���� ���� Ȯ��

    g_detect_result = vehicle_forward_detect;

	if (vehicle_forward_detect)	//���� ���� ���� ��
	{
        g_detect_sum++;
		float xl, xr;
		CPoint point_l, point_r, center;
		CRect rect_big, rect_small;

		if (vehicle_forward_tracking)
		{
			if (vehicle_forward_update.width >= vehicle_forward_position.width && vehicle_forward_update.height >= vehicle_forward_position.height)
			{
				rect_big = vehicle_forward_update;
				rect_small = vehicle_forward_position;
				center.x = rect_small.x + rect_small.width / 2.0;
				center.y = rect_small.y + rect_small.height / 2.0;
			}
			else
			{
				rect_big = vehicle_forward_position;
				rect_small = vehicle_forward_update;
				center.x = rect_small.x + rect_small.width / 2.0;
				center.y = rect_small.y + rect_small.height / 2.0;
			}

			if (center.x >= rect_big.x && center.x <= rect_big.x + rect_big.width - 1 && center.y >= rect_big.y && center.y <= rect_big.y + rect_big.height - 1)
			{
				if (vehicle_forward_update.y + vehicle_forward_update.height >= vehicle_forward_position.y + vehicle_forward_position.height + 10 &&
					(vehicle_forward_update.height >= vehicle_forward_position.height + 20)) vehicle_forward_cutin = true;	//������ ���� O
				else vehicle_forward_cutin = false;	//������ ���� X
			}
			else
			{
				vehicle_forward_cutin = true;	//������ ���� O
				vehicle_ttc_init1 = false;
			}
		}
		else
		{
			//������ ���� �Ǵ�//
			point_l.x = vehicle_forward_update.x;
			point_l.y = vehicle_forward_update.y + vehicle_forward_update.height;
			point_r.x = vehicle_forward_update.x + vehicle_forward_update.width;
			point_r.y = vehicle_forward_update.y + vehicle_forward_update.height;

			xl = (float)(vanishing_point.x - vanishing_point_l.x) / (vanishing_point.y - vanishing_point_l.y)*(point_l.y - vanishing_point.y) + vanishing_point.x;
			xr = (float)(vanishing_point.x - vanishing_point_r.x) / (vanishing_point.y - vanishing_point_r.y)*(point_l.y - vanishing_point.y) + vanishing_point.x;
			if (point_l.x >= xl && point_r.x <= xr) vehicle_forward_cutin = false; //������ ���� X
			else vehicle_forward_cutin = true;	//������ ���� O
			//������ ���� �Ǵ�//

			vehicle_ttc_init1 = false;
		}

		vehicle_forward_position = vehicle_forward_update;

		x_scale = (float)(vehicle_forward_update.width) / TRACKING_WINDOW_SIZE;
		y_scale = (float)(vehicle_forward_update.height) / TRACKING_WINDOW_SIZE;

		for (y = 0; y < vehicle_image.rows; y++)
		{
			for (x = 0; x < vehicle_image.cols; x++)
			{
				x_position = (int)(x * x_scale) + vehicle_forward_update.x;
				y_position = ((int)(y * y_scale) + vehicle_forward_update.y) * image_ori.cols;
				pixel = image_ori.data[x_position + y_position];
				vehicle_image.data[y*vehicle_image.cols + x] = pixel;	//ROI�� �̹��� �ְ� 20*20���� resize
				residual_gain[y][x] = 1.0;
			}
		}

		if((vehicle_forward_update.y + vehicle_forward_update.height - 1) > vanishing_point.y)
		{
			v = ((vehicle_forward_update.y + vehicle_forward_update.height - 1) - camera_cy) / camera_fy;
			real_distance = ((cos(camera_radian) - v * sin(camera_radian)) / (v * cos(camera_radian) + sin(camera_radian))) * camera_height;
			real_width_fx = vehicle_forward_update.width * real_distance;// vehicle_image.rows * real_distance;
			vehicle_forward_tracking = true;
		}
		else vehicle_forward_tracking = false;

		forward_vehicle_init = false;
		vehicle_tracking_counter = 0;
		vehicle_forward_percent = 0;
		scale_state_init = false;
	}

	if (vehicle_forward_tracking)	//���� ���� tracking
	{
		int counter = 0, increment = 1;
		CSize winSize, cascade_orig_window_size = { cascade.width, cascade.height };
		CRect center_update;
		CRect vehicle_haar;
		CSize sz;
		float inv_window_area, factor;
		float mean_factor, variance_norm_factor;

		vehicle_forward_position_pre = vehicle_forward_position;
		vehicle_forward_position = vehicle_forward_update;

		//vehicle_haar = vehicle_forward_position;
		vehicle_haar.x = vehicle_forward_position.x - 3;
		vehicle_haar.y = vehicle_forward_position.y - 3;
		vehicle_haar.width = vehicle_forward_position.width + 6;
		vehicle_haar.height = vehicle_forward_position.height + 6;

		vehicle_forward_image.cols = vehicle_haar.width;
		vehicle_forward_image.rows = vehicle_haar.height;

		x_scale = 1.0;//(float)vehicle_haar.width / cascade.width;
		y_scale = 1.0;//(float)vehicle_haar.height / cascade.height;
		for (y = 0; y < vehicle_forward_image.rows; y++)
		{
			for (x = 0; x < vehicle_forward_image.cols; x++)
			{
				x_position = (int)(x * x_scale) + vehicle_haar.x;
				y_position = ((int)(y * y_scale) + vehicle_haar.y) * image_ori.cols;
				pixel = image_ori.data[x_position + y_position];
				vehicle_forward_image.data[y*vehicle_forward_image.cols + x] = pixel;	//ROI�� �̹��� �ְ� 25*25�� resize
			}
		}

		sz.width = 50;
		sz.height = 50;

		factor = (float)sz.width / cascade_orig_window_size.width;

		Image_Resize(&vehicle_forward_image, _image, sz);
		Image_Integral(_image, _image_integral, _image_dintegral, _image_tintegral);

		for (factor;; factor /= 1.1)
		{
			if (counter >= 3) break;
			counter++;

			winSize.width = (int)(cascade_orig_window_size.width * factor + 0.5);
			winSize.height = (int)(cascade_orig_window_size.height * factor + 0.5);

			if (winSize.width < cascade_orig_window_size.width || winSize.height < cascade_orig_window_size.height)
				break;

			inv_window_area = 1.0 / ((winSize.width) * (winSize.height));

			switch(g_DayNight)
			{
				case 1:		//�߰�
					Cascade_Scale(cascade_rect3, cascade_rect_scale3, factor, inv_window_area);
					break;

				default:	//�ְ�
					if (haar_mode == 0) Cascade_Scale(cascade_rect, cascade_rect_scale, factor, inv_window_area);
					else if (haar_mode == 1) Cascade_Scale(cascade_rect2, cascade_rect_scale2, factor, inv_window_area);
					break;
			}

			for (y = 0; (y + winSize.height) <= _image->rows; y += increment)
			{
				for (x = 0; (x + winSize.width) <= _image->cols; x += increment)
				{
					rect.x = x;
					rect.y = y;
					rect.width = winSize.width;
					rect.height = winSize.height;

#ifdef SIMD
                    if(g_mode == 1 || g_mode == 3)
                    {
                        rect.simd.temp[X] = rect.x;
                        rect.simd.temp[Y] = rect.y;
                        rect.simd.temp[W] = winSize.width;
                        rect.simd.temp[H] = winSize.height;
                    }
#endif

					mean_factor = Image_Sum(rect) * inv_window_area;
					variance_norm_factor = Image_Dsum(rect) * inv_window_area;
					variance_norm_factor = variance_norm_factor - (mean_factor * mean_factor);
					variance_norm_factor = (variance_norm_factor >= 0.0) ? sqrt(variance_norm_factor) : 1;

					switch(g_DayNight)
					{
						case 1: 	//�߰�
							for (i = 0; i < cascade3.stage_size; i++)  //stage size
							{
								vehicle_detect_temp = Cascade_Compute(cascade3.stage[i], cascade_rect_scale3, rect, variance_norm_factor);
								if (!vehicle_detect_temp) break;
							}
							break;

						default:	//�ְ�
							if (haar_mode == 0) 	//haar 20 x 20 mode
							{
								for (i = 0; i < cascade.stage_size; i++)  //stage size
								{
									vehicle_detect_temp = Cascade_Compute(cascade.stage[i], cascade_rect_scale, rect, variance_norm_factor);
									if (!vehicle_detect_temp) break;
								}

							}
							else if (haar_mode == 1)	//haar 30 x 30 mode
							{
								for (i = 0; i < cascade2.stage_size; i++)  //stage size
								{
									vehicle_detect_temp = Cascade_Compute(cascade2.stage[i], cascade_rect_scale2, rect, variance_norm_factor);
									if (!vehicle_detect_temp) break;
								}

							}
							break;
					}
					if (!vehicle_detect_temp) continue;
					goto DETECT_EXIT;
				}
			}
		}

		DETECT_EXIT:
		if (!vehicle_detect_temp) vehicle_forward_percent++;
		else
		{
			if (vehicle_forward_percent >= 1)
			{
				x_scale = (float)(vehicle_forward_position.width) / TRACKING_WINDOW_SIZE;
				y_scale = (float)(vehicle_forward_position.height) / TRACKING_WINDOW_SIZE;

				for (y = 0; y < vehicle_image.rows; y++)
				{
					for (x = 0; x < vehicle_image.cols; x++)
					{
						x_position = (int)(x * x_scale) + vehicle_forward_position.x;
						y_position = ((int)(y * y_scale) + vehicle_forward_position.y) * image_ori.cols;
						pixel = image_ori.data[x_position + y_position];
						vehicle_image.data[y*vehicle_image.cols + x] = pixel;	//ROI�� �̹��� �ְ� 20*20���� resize
						residual_gain[y][x] = 1.0;
					}
				}
				forward_vehicle_init = false;
			}
			vehicle_tracking_counter = 0;
			vehicle_forward_percent = 0;
			scale_state_init = false;
		}

		if (vehicle_forward_percent >= 1)
		{
		      //for tracking
//            forward_vehicle_init = false;
//            vehicle_forward_tracking = false;
//
//            vehicle_forward_position.x = 0;	vehicle_forward_position.y = 0;	vehicle_forward_position.width = 0;	vehicle_forward_position.height = 0;
//            real_width_fx = 0;
//            vehicle_tracking_counter = 0;
			vehicle_forward_position = vehicle_forward_position_pre;
			if (vehicle_tracking_counter == 0 || vehicle_tracking_counter > 10) vehicle_tracking_counter = 10;
		}

		if (vehicle_forward_position.x <= 1 || vehicle_forward_position.x + vehicle_forward_position.width >= image_ori.cols - 1) vehicle_forward_tracking = false;
		else if (!Vanishing_Line_Detect(vehicle_forward_position)) vehicle_forward_tracking = false;
	}

	if (vehicle_tracking_counter > 0)
	{
		vehicle_tracking_counter--;

		if (vehicle_tracking_counter <= 0)
		{
			forward_vehicle_init = false;
			vehicle_forward_tracking = false;

			vehicle_forward_position.x = 0;	vehicle_forward_position.y = 0;	vehicle_forward_position.width = 0;	vehicle_forward_position.height = 0;
			real_width_fx = 0;
			vehicle_tracking_counter = 0;
		}
	}

	real_distance = (vehicle_forward_position.width != 0) ? real_width_fx / vehicle_forward_position.width : 0;

	if ((int)(real_distance) >= MAX_DISTANCE)
	{
		forward_vehicle_init = false;

		if((vehicle_forward_position.y + vehicle_forward_position.height - 1) > vanishing_point.y)
		{
			v = ((vehicle_forward_position.y + vehicle_forward_position.height - 1) - camera_cy) / camera_fy;
			real_distance = ((cos(camera_radian) - v * sin(camera_radian)) / (v * cos(camera_radian) + sin(camera_radian))) * camera_height;
			real_width_fx = vehicle_forward_position.width * real_distance;// vehicle_image.rows * real_distance;
		}
	}

	*vehicle_roi = vehicle_forward_position;
	*width_distance = real_distance;
	*width_ttc = ((int)real_velocity != 0) ? real_distance / real_velocity : 0.0;

#ifdef __OPENCV_USE__
	///////////////////// OPENCV DISPLAY /////////////////////
	if (vehicle_forward_tracking)
	{
		Mat opencv_vehicle = Mat(TRACKING_WINDOW_SIZE, TRACKING_WINDOW_SIZE, CV_8UC1);
		for (int i = 0; i < opencv_vehicle.total(); i++)
		{
			opencv_vehicle.data[i] = vehicle_image.data[i];
		}
		opencv_percent_threshold = percent_threshold;
		if (haar_detect == 0)	//�ְ�
			rectangle(opencv_image, Rect(vehicle_forward_position.x, vehicle_forward_position.y, vehicle_forward_position.width, vehicle_forward_position.height), Scalar(255, 0, 0), 4, 8, 0);
		else if (haar_detect == 1)	//�߰�
			rectangle(opencv_image, Rect(vehicle_forward_position.x, vehicle_forward_position.y, vehicle_forward_position.width, vehicle_forward_position.height), Scalar(255, 0, 0), 4, 8, 0);

		//overlayImage(opencv_image, pinImage, Point(vehicle_forward_position.x - 25, vehicle_forward_position.y), Size(vehicle_forward_position.width + 50, vehicle_forward_position.height + 50));

		char myText_temp[100];
		string myText = myText_temp;
		sprintf(myText_temp, "%.2f%%", opencv_percent_threshold);
		myText = myText_temp;
		putText(opencv_image, myText, Point(0, 30), 2, 1, Scalar(0, 0, 255));

		sprintf(myText_temp, "%.2d", vehicle_forward_percent);
		myText = myText_temp;
		putText(opencv_image, myText, Point(0, 60), 2, 1, Scalar(0, 0, 255));

		sprintf(myText_temp, "%.2f", vehicle_ttc2_display);
		myText = myText_temp;
		putText(opencv_image, myText, Point(0, 90), 2, 1, Scalar(0, 0, 255));

		sprintf(myText_temp, "%.2d", vehicle_roi->width);
		myText = myText_temp;
		putText(opencv_image, myText, Point(0, 120), 2, 1, Scalar(0, 0, 255));

		//sprintf_s(myText_temp, "%.2fs", *width_ttc);
		//myText = myText_temp;
		//putText(image_640, myText, Point((vehicle_position->at(0).x + vehicle_position->at(0).width / 2) * 1 - 40, (vehicle_position->at(0).y + vehicle_position->at(0).height) * 1 + 30), 2, 1, Scalar(0, 0, 255));

		sprintf(myText_temp, "%.2fm", *width_distance);
		myText = myText_temp;
		putText(opencv_image, myText, Point((vehicle_forward_position.x + vehicle_forward_position.width / 2) * 1 - 40, (vehicle_forward_position.y + vehicle_forward_position.height) * 1 + 30), 2, 1, Scalar(0, 0, 255));

		sprintf(myText_temp, "%.2fs", *width_ttc);
		myText = myText_temp;
		putText(opencv_image, myText, Point((vehicle_forward_position.x + vehicle_forward_position.width / 2) * 1 - 40, (vehicle_forward_position.y + vehicle_forward_position.height) * 1 + 60), 2, 1, Scalar(0, 0, 255));

		//imshow("opencv_vehicle", opencv_vehicle);
		//imshow("opencv_vehicle2", opencv_vehicle2);
	}
	Rect opencv_rect;
	opencv_rect.x = image_roi.x;
	opencv_rect.y = image_roi.y;
	opencv_rect.width = image_roi.width;
	opencv_rect.height = image_roi.height;

	rectangle(opencv_image, opencv_rect, Scalar(0, 255, 255), 1, 8, 0);
	line(opencv_image, Point(vanishing_point.x, vanishing_point.y), Point(vanishing_point_l.x, vanishing_point_l.y), Scalar(255, 0, 0), 2, 8, 0);
	line(opencv_image, Point(vanishing_point.x, vanishing_point.y), Point(vanishing_point_r.x, vanishing_point_r.y), Scalar(255, 0, 0), 2, 8, 0);

	//Mat image_640;
	//resize(image, image_640, Size(IMAGE_WIDTH, IMAGE_WIDTH));
	std::string str = "image_" + std::to_string(g_mode);
	imshow(str, opencv_image);
	//imshow("image_temp", image_temp);
	//cvWaitKey(1);
	///////////////////// OPENCV DISPLAY /////////////////////
#endif //__OPENCV_USE__
//    forward_vehicle_init = false;
//    vehicle_forward_tracking = false;
	return vehicle_forward_tracking;
}
