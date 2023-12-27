#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define USE_OPENCV
//#define USE_SIMD

#ifdef USE_OPENCV
#include <opencv2/opencv.hpp>
#endif

#ifdef USE_SIMD
#include <immintrin.h>
#endif

#define IMAGE_WIDTH  1280
#define IMAGE_HEIGHT 720

#ifdef USE_OPENCV
using namespace cv;
#endif

using namespace std;
typedef unsigned char uchar;

typedef struct Camera_int
{
	float fx, fy, cx, cy;
}Camera_int;

#ifdef USE_SIMD
typedef union SIMDFloat
{
	__m128 reg;
	float temp[4];
}SIMDFloat;
#endif

typedef struct Image
{
	int width;
	int height;
	uchar *data;
	int *xy;
}Image;

void image_undistortion(Image image_i, Image image_o)
{
	for(int y = 0; y < IMAGE_HEIGHT; y++)
	{
		for(int x = 0; x < IMAGE_WIDTH; x+=4)
		{
			int position = y * IMAGE_WIDTH + x;

			if(image_i.xy[position] != -1)	
			{
                image_o.data[image_o.xy[position] + 0] = image_i.data[image_i.xy[position] + 0];
                image_o.data[image_o.xy[position] + 1] = image_i.data[image_i.xy[position] + 1];
                image_o.data[image_o.xy[position] + 2] = image_i.data[image_i.xy[position] + 2];
            }

			position++;
			
			if(image_i.xy[position] != -1)	
			{
                image_o.data[image_o.xy[position] + 0] = image_i.data[image_i.xy[position] + 0];
                image_o.data[image_o.xy[position] + 1] = image_i.data[image_i.xy[position] + 1];
                image_o.data[image_o.xy[position] + 2] = image_i.data[image_i.xy[position] + 2];
            }

			position++;
			
			if(image_i.xy[position] != -1)	
			{
                image_o.data[image_o.xy[position] + 0] = image_i.data[image_i.xy[position] + 0];
                image_o.data[image_o.xy[position] + 1] = image_i.data[image_i.xy[position] + 1];
                image_o.data[image_o.xy[position] + 2] = image_i.data[image_i.xy[position] + 2];
            }

			position++;
			
			if(image_i.xy[position] != -1)	
			{
                image_o.data[image_o.xy[position] + 0] = image_i.data[image_i.xy[position] + 0];
                image_o.data[image_o.xy[position] + 1] = image_i.data[image_i.xy[position] + 1];
                image_o.data[image_o.xy[position] + 2] = image_i.data[image_i.xy[position] + 2];
            }
		}
	}
}

void image_init(Image image_i, Image image_o, Camera_int camera)
{
	// distortion = -0.1103539x^3 + 0.08552575x^2 + 1.24129x + 0.001559812
	// x: radian
    float a = -0.1103539f;
    float b = 0.08552575f;
    float c = 1.24129f;
    float d = 0.001559812f;

#ifdef USE_SIMD
	SIMDFloat u, v, cx, cy, fx, fy; 
	SIMDFloat r, rd, theta, factor, temp, temp1, temp2;
	SIMDFloat xx, yy, uu, vv, uv;
	SIMDFloat aa, bb, cc, dd;

	cx.reg = _mm_load1_ps(&camera.cx);
	cy.reg = _mm_load1_ps(&camera.cy);
	fx.reg = _mm_load1_ps(&camera.fx);
	fy.reg = _mm_load1_ps(&camera.fy);
	
	aa.reg = _mm_load1_ps(&a);
	bb.reg = _mm_load1_ps(&b);
	cc.reg = _mm_load1_ps(&c);
	dd.reg = _mm_load1_ps(&d);

	int x_t = 0; 
    int y_t = 0;

	for(int y = 0; y < IMAGE_HEIGHT; y++)
	{
		yy.reg = _mm_set1_ps((float)y);
		v.reg = _mm_div_ps(_mm_sub_ps(yy.reg, cy.reg), fy.reg);
        
		for(int x = 0; x < IMAGE_WIDTH; x+=4)
		{
            // cartesian coordinates of the destination point (relative to the centre of the image)
            xx.temp[0] = (float)(x+0); 
            xx.temp[1] = (float)(x+1); 
            xx.temp[2] = (float)(x+2); 
            xx.temp[3] = (float)(x+3); 

			u.reg = _mm_div_ps(_mm_sub_ps(xx.reg, cx.reg), fx.reg);

			uv.reg = _mm_add_ps(_mm_mul_ps(u.reg, u.reg), _mm_mul_ps(v.reg, v.reg));
            // distance or radius of destination image
			r.reg = _mm_sqrt_ps(uv.reg);
			//theta.reg = _mm_atan_ps(r.reg);
			theta.temp[0] = atan(r.temp[0]);
			theta.temp[1] = atan(r.temp[1]);
			theta.temp[2] = atan(r.temp[2]);
			theta.temp[3] = atan(r.temp[3]);

			temp.reg = _mm_mul_ps(aa.reg, _mm_mul_ps(_mm_mul_ps(theta.reg, theta.reg), theta.reg));
			temp1.reg = _mm_mul_ps(bb.reg, _mm_mul_ps(theta.reg, theta.reg));
			temp2.reg = _mm_mul_ps(cc.reg, theta.reg);

			rd.reg = _mm_add_ps(_mm_add_ps(_mm_add_ps(temp.reg, temp1.reg), temp2.reg), dd.reg);
			
            // comparing old and new distance to get factor
            factor.reg = _mm_div_ps(rd.reg, r.reg);
			uu.reg = _mm_add_ps(_mm_mul_ps(_mm_mul_ps(u.reg, factor.reg), fx.reg), cx.reg);
			vv.reg = _mm_add_ps(_mm_mul_ps(_mm_mul_ps(v.reg, factor.reg), fy.reg), cy.reg);
		
			x_t = (int)uu.temp[0]; 
            y_t = (int)vv.temp[0];
			image_o.xy[y * IMAGE_WIDTH + x + 0] = 3 * y * IMAGE_WIDTH + 3 * (x+0);

            if(x_t >= 0 && y_t >= 0 && x_t < IMAGE_WIDTH && y_t < IMAGE_HEIGHT)
			{
                image_i.xy[y * IMAGE_WIDTH + x + 0] = 3 * y_t * IMAGE_WIDTH + 3 * x_t;
							}
			else
			{
				image_i.xy[y * IMAGE_WIDTH + x + 0] = -1;
			}
			
			x_t = (int)uu.temp[1]; 
            y_t = (int)vv.temp[1];
			image_o.xy[y * IMAGE_WIDTH + x + 1] = 3 * y * IMAGE_WIDTH + 3 * (x+1);
            
			if(x_t >= 0 && y_t >= 0 && x_t < IMAGE_WIDTH && y_t < IMAGE_HEIGHT)
			{
                image_i.xy[y * IMAGE_WIDTH + x + 1] = 3 * y_t * IMAGE_WIDTH + 3 * x_t;
			}
			else
			{
				image_i.xy[y * IMAGE_WIDTH + x + 1] = -1;
			}

			x_t = (int)uu.temp[2]; 
            y_t = (int)vv.temp[2];
			image_o.xy[y * IMAGE_WIDTH + x + 2] = 3 * y * IMAGE_WIDTH + 3 * (x+2);
            
			if(x_t >= 0 && y_t >= 0 && x_t < IMAGE_WIDTH && y_t < IMAGE_HEIGHT)
			{
                image_i.xy[y * IMAGE_WIDTH + x + 2] = 3 * y_t * IMAGE_WIDTH + 3 * x_t;
			}
			else
			{
				image_i.xy[y * IMAGE_WIDTH + x + 2] = -1;
			}

			x_t = (int)uu.temp[3]; 
            y_t = (int)vv.temp[3];
			image_o.xy[y * IMAGE_WIDTH + x + 3] = 3 * y * IMAGE_WIDTH + 3 * (x+3);
            
			if(x_t >= 0 && y_t >= 0 && x_t < IMAGE_WIDTH && y_t < IMAGE_HEIGHT)
			{
                image_i.xy[y * IMAGE_WIDTH + x + 3] = 3 * y_t * IMAGE_WIDTH + 3 * x_t;
			}
			else
			{
				image_i.xy[y * IMAGE_WIDTH + x + 3] = -1;
			}
        }
    }
#else
	float u, v, r, rd, theta, factor;
	
	for(int y = 0; y < IMAGE_HEIGHT; y++)
	{
        for(int x = 0; x < IMAGE_WIDTH; x++)
		{
            // cartesian coordinates of the destination point (relative to the centre of the image)
            u = (x - camera.cx) / camera.fx;
            v = (y - camera.cy) / camera.fy;

            // distance or radius of destination image
			r = sqrt(u*u + v*v);
			theta = atan(r);
            
			rd = (a * theta*theta*theta + b * theta*theta + c * theta + d);

            // comparing old and new distance to get factor
            factor = rd/r;

            int x_t = (int)(camera.cx + u * factor * camera.fx);
            int y_t = (int)(camera.cy + v * factor * camera.fy);

			image_o.xy[y * IMAGE_WIDTH + x] = 3 * y * IMAGE_WIDTH + 3 * x;
            
			if(x_t >= 0 && y_t >= 0 && x_t < IMAGE_WIDTH && y_t < IMAGE_HEIGHT)
			{
                image_i.xy[y * IMAGE_WIDTH + x] = 3 * y_t * IMAGE_WIDTH + 3 * x_t;
			}
			else
			{
				image_i.xy[y * IMAGE_WIDTH + x] = -1;
			}
        }
    }
#endif
}

int main(void)
{
    //ImageThread *image_thread = new ImageThread();

    // camera intrinsic parameter
    Camera_int camera;
    camera.fx = 330.0f;
    camera.fy = 330.0f;    //500.0;
    camera.cx = IMAGE_WIDTH / 2.0f;
    camera.cy = IMAGE_HEIGHT / 2.0f;

    // image of input, output
    Image image_i, image_o;
    image_i.width = IMAGE_WIDTH;
    image_i.height = IMAGE_HEIGHT;
    image_o.width = IMAGE_WIDTH;
    image_o.height = IMAGE_HEIGHT;

    image_i.data = (uchar *) malloc(sizeof(uchar) * IMAGE_WIDTH * IMAGE_HEIGHT * 3);
    image_o.data = (uchar *) malloc(sizeof(uchar) * IMAGE_WIDTH * IMAGE_HEIGHT * 3);

    image_i.xy = (int *) malloc(sizeof(int) * IMAGE_WIDTH * IMAGE_HEIGHT);
    image_o.xy = (int *) malloc(sizeof(int) * IMAGE_WIDTH * IMAGE_HEIGHT);

    for (int y = 0; y < IMAGE_HEIGHT; y++) {
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            image_i.data[3 * y * IMAGE_WIDTH + 3 * x + 0] = 0;
            image_i.data[3 * y * IMAGE_WIDTH + 3 * x + 1] = 0;
            image_i.data[3 * y * IMAGE_WIDTH + 3 * x + 2] = 0;

            image_o.data[3 * y * IMAGE_WIDTH + 3 * x + 0] = 0;
            image_o.data[3 * y * IMAGE_WIDTH + 3 * x + 1] = 0;
            image_o.data[3 * y * IMAGE_WIDTH + 3 * x + 2] = 0;
        }
    }

    image_init(image_i, image_o, camera);

#ifdef USE_OPENCV
    char files[50];
    int count = 0;
    namedWindow("original");
#endif

    while(1){
#ifdef USE_OPENCV
        if (count++ >= 4) count = 1;

        sprintf(files, "../images/IMG0000%d.png", count);
        Mat image = imread(files);

        if (image.empty()) {
            printf("file can not open!\n");
            return 0;
        }
#endif
        clock_t time;
        time = clock();

#ifdef USE_OPENCV
        image_i.data = image.data;
#endif
        image_undistortion(image_i, image_o);

        time = clock() - time;
        printf("processing time: %f\n", (float) time / CLOCKS_PER_SEC);

#ifdef USE_OPENCV
        Mat image_undistort = Mat::zeros(720, 1280, CV_8UC3);
        image_undistort.data = image_o.data;

        //imwrite("undistortion.png", image_undistort);
        imshow("original", image);
        imshow("undistort", image_undistort);

        unsigned char key = waitKey(5000);
        if(key == 'q' || key == 27)
        {
            break;
        }
#endif
    }

	//delete image_thread;
	free(image_i.data);
	free(image_i.xy);
	free(image_o.data);
	free(image_o.xy);

	return 0;
}