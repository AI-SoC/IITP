#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "rusage.h"
//#include "image_convert.h"

int width, height;
pthread_mutex_t mutex;

double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec*1000 + (double)time.tv_usec /1000;
}

struct pixel {
	unsigned char b;
	unsigned char g;
	unsigned char r;
};


///////////////////////////////////////////////////////////////////////
int m_abs(int n){
  if (n>0)
    return n = n;
  else
    return n = n*(-1);
}

int read_bmp(char* filename, unsigned char** header, struct pixel** data) {
	struct pixel * data_temp;
	unsigned char * header_temp;
	FILE* file = fopen(filename, "rb");

	if (!file) return -1;
   
	// read the 54-byte header
	header_temp = (unsigned char *) malloc(54*sizeof(unsigned char));
	if (fread(header_temp, sizeof(unsigned char), 54, file) != 54){
		printf("Error reading BMP header\n");
		return -1;
	}

	// get height and width of image
	width = *(int*)&header_temp[18];
	height = *(int*)&header_temp[22];

	// Read in the image
	int size = width * height;
	data_temp = (struct pixel *) malloc(size*sizeof(struct pixel)); 
	if (fread(data_temp, sizeof(struct pixel), size, file) != size){
		printf("Error reading BMP image\n");
		return -1;
	}
	fclose(file);

	*header = header_temp;
	*data = data_temp;

	return 0;
}

void write_bmp(char* filename, unsigned char* header, struct pixel* data) {
	FILE* file = fopen(filename, "wb");

	// write the 54-byte header
	fwrite(header, sizeof(unsigned char), 54, file); 

	int size = width * height;
	fwrite(data, sizeof(struct pixel), size, file); 
	fclose(file);
}

// Write the grayscale image to disk.
void write_grayscale_bmp(char* filename, unsigned char* header, unsigned char* data) {
	FILE* file = fopen(filename, "wb");

	int size = width * height;
	struct pixel * data_temp = (struct pixel *) malloc(size*sizeof(struct pixel)); 

	// write the 54-byte header
	fwrite(header, sizeof(unsigned char), 54, file); 
	int y, x;
   
	// the r field of the pixel has the grayscale value. copy to g and b.
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			(*(data_temp + y*width + x)).b = (*(data + y*width + x));
			(*(data_temp + y*width + x)).g = (*(data + y*width + x));
			(*(data_temp + y*width + x)).r = (*(data + y*width + x));
		}
	}
   
	size = width * height;
	fwrite(data_temp, sizeof(struct pixel), size, file); 

	free(data_temp);
	fclose(file);
}

// Determine the grayscale 8 bit value by averaging the r, g, and b channel values.
void convert_to_grayscale(struct pixel * data, unsigned char ** converted_data) {
	int i;
	*converted_data = (unsigned char*) malloc(width * height*sizeof(unsigned char));

	for (i = 0; i < width*height; i++) {
		(*converted_data)[i] = (data[i].r + data[i].g + data[i].b)/3;
	}
}

// Gaussian blur. 
void gaussian_blur(unsigned char ** data) {
	unsigned int gaussian_filter[5][5] = {
		{ 2, 4, 5, 4, 2 },
		{ 4, 9,12, 9, 4 },
		{ 5,12,15,12, 5 },
		{ 4, 9,12, 9, 4 },
		{ 2, 4, 5, 4, 2 }
	};
	int x, y, i, j;
	unsigned int numerator_r, denominator;
   
	unsigned char * temp_data = (unsigned char*) malloc(width * height*sizeof(unsigned char)); 
   
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			numerator_r = 0;
			denominator = 0;
			for (j = -2; j <= 2; j++) {
				for (i = -2; i <= 2; i++) {
					if ( (x+i) >= 0 && (x+i) < width && (y+j) >= 0 && (y+j) < height) {
						numerator_r += (*(*data + (y+j)*width + (x+i)))*gaussian_filter[i+2][j+2];
						denominator += gaussian_filter[i+2][j+2];
					}
				}
			}
			(*(temp_data + y*width + x)) = numerator_r/denominator;
		}
	}
	free(*data);
	*data = temp_data;
}

void sobel_filter(unsigned char ** data) {
	int sobel_x[3][3] = {
        { -1,  0,  1 },
        { -2,  0,  2 },
        { -1,  0,  1 }
    };
    int sobel_y[3][3] = {
        { -1, -2, -1 },     // y == 0 (bottom) row
        {  0,  0,  0 },     // y == 1 (middle) row
        {  1,  2,  1 }      // y == 2 (top) row
    };
    
	unsigned char * temp_data = malloc(width * height*sizeof(unsigned char)); 
     
    int x, y, i, j;
    unsigned int c_x, c_y;
    
    for (y = 0; y < height; y++) {
      for (x = 0; x < width; x++) {
         c_x=0;
         c_y=0;
         for (j = -1; j <= 1; j++) {
            for (i = -1; i <= 1; i++) {
               if ( (x+i) >= 0 && (x+i) < width && (y+j) >= 0 && (y+j) < height) {
                  c_x += (*(*data + (y+j)*width + (x+i)))*sobel_x[i+1][j+1];
                  c_y += (*(*data + (y+j)*width + (x+i)))*sobel_y[i+1][j+1];
               }
            }
         }
         (*(temp_data + y*width + x)) = 0.5 * m_abs(c_x) + 0.5 * m_abs(c_y);
      }
   }
    free (*data);   // the previous image data is no longer needed
    *data = temp_data;

}

#define VALUE_T 255
float atan_vals[VALUE_T] = {0 ,45 ,63.434948822922 ,71.565051177078 ,75.9637565320735 ,78.6900675259798 ,80.5376777919744 ,81.869897645844 ,82.8749836510982 ,83.6598082540901 ,84.2894068625004 ,84.8055710922652 ,85.2363583092738 ,85.6012946450045 ,85.9143832200251 ,86.1859251657096 ,86.4236656250027 ,86.6335393365702 ,86.8201698801358 ,86.9872124958166 ,87.1375947738883 ,87.2736890060937 ,87.3974377975002 ,87.5104470780009 ,87.6140559696112 ,87.7093899573615 ,87.7974018382342 ,87.8789036033385 ,87.9545915111128 ,88.025065989118 ,88.0908475670036 ,88.1523897340054 ,88.2100893917539 ,88.2642954110716 ,88.3153156821037 ,88.3634229583833 ,88.4088597288054 ,88.451842301022 ,88.492564241225 ,88.5311992856142 ,88.5679038158354 ,88.6028189727036 ,88.6360724683971 ,88.6677801461304 ,88.6980473274211 ,88.7269699799433 ,88.7546357332316 ,88.7811247648687 ,88.806510576018 ,88.8308606720926 ,88.8542371618249 ,88.8766972859246 ,88.8982938847936 ,88.9190758133393 ,88.9390883097358 ,88.95837332399 ,88.9769698113322 ,88.9949139947458 ,89.0122396003602 ,89.0289780689208 ,89.0451587461278 ,89.0608090542644 ,89.0759546472273 ,89.0906195508009 ,89.1048262897889 ,89.1185960034178 ,89.1319485502545 ,89.1449026037333 ,89.1574757392596 ,89.169684513742 ,89.1815445383114 ,89.1930705448976 ,89.2042764472607 ,89.2151753970081 ,89.2257798350719 ,89.23610153907 ,89.2461516669292 ,89.2559407971113 ,89.2654789657452 ,89.2747757009407 ,89.2838400545296 ,89.2926806314557 ,89.3013056170165 ,89.3097228021349 ,89.3179396068274 ,89.3259631020155 ,89.3338000298169 ,89.3414568224364 ,89.3489396197705 ,89.3562542858246 ,89.3634064240365 ,89.3704013915897 ,89.3772443127937 ,89.3839400916008 ,89.3904934233248 ,89.3969088056195 ,89.4031905487708 ,89.4093427853533 ,89.4153694792948 ,89.4212744343922 ,89.4270613023165 ,89.4327335901421 ,89.4382946674335 ,89.4437477729193 ,89.4490960207814 ,89.4543424065843 ,89.4594898128693 ,89.4645410144355 ,89.4694986833262 ,89.4743653935424 ,89.4791436254981 ,89.4838357702352 ,89.488444133413 ,89.4929709390852 ,89.497418333279 ,89.5017883873864 ,89.5060831013813 ,89.5103044068708 ,89.5144541699919 ,89.5185341941616 ,89.5225462226904 ,89.5264919412651 ,89.5303729803104 ,89.534190917235 ,89.5379472785692 ,89.5416435419996 ,89.5452811383069 ,89.5488614532127 ,89.5523858291395 ,89.5558555668881 ,89.559271927239 ,89.5626361324793 ,89.5659493678606 ,89.5692127829914 ,89.5724274931666 ,89.5755945806385 ,89.5787150958314 ,89.5817900585029 ,89.5848204588551 ,89.5878072585977 ,89.5907513919651 ,89.593653766691 ,89.5965152649413 ,89.5993367442078 ,89.6021190381654 ,89.6048629574925 ,89.6075692906591 ,89.610238804682 ,89.612872245849 ,89.6154703404155 ,89.618033795271 ,89.620563298581 ,89.6230595204029 ,89.6255231132772 ,89.6279547127967 ,89.630354938153 ,89.6327243926619 ,89.6350636642694 ,89.6373733260375 ,89.6396539366128 ,89.6419060406764 ,89.6441301693781 ,89.6463268407535 ,89.6484965601267 ,89.6506398204972 ,89.6527571029142 ,89.6548488768359 ,89.6569156004765 ,89.6589577211412 ,89.6609756755486 ,89.6629698901422 ,89.6649407813909 ,89.6668887560786 ,89.6688142115846 ,89.6707175361533 ,89.6725991091556 ,89.6744593013407 ,89.6762984750798 ,89.6781169846013 ,89.6799151762183 ,89.6816933885486 ,89.683451952727 ,89.6851911926109 ,89.6869114249794 ,89.6886129597252 ,89.6902961000412 ,89.6919611426001 ,89.6936083777293 ,89.6952380895789 ,89.6968505562859 ,89.6984460501317 ,89.7000248376961 ,89.7015871800051 ,89.7031333326753 ,89.704663546053 ,89.7061780653493 ,89.7076771307714 ,89.7091609776495 ,89.7106298365594 ,89.7120839334429 ,89.7135234897229 ,89.7149487224162 ,89.7163598442422 ,89.7177570637291 ,89.7191405853166 ,89.7205106094553 ,89.7218673327036 ,89.7232109478222 ,89.7245416438648 ,89.7258596062668 ,89.7271650169319 ,89.7284580543155 ,89.7297388935059 ,89.7310077063035 ,89.7322646612978 ,89.7335099239416 ,89.734743656624 ,89.7359660187409 ,89.7371771667635 ,89.7383772543052 ,89.7395664321866 ,89.7407448484986 ,89.7419126486638 ,89.7430699754965 ,89.7442169692607 ,89.7453537677272 ,89.746480506228 ,89.7475973177109 ,89.7487043327911 ,89.7498016798024 ,89.7508894848471 ,89.7519678718435 ,89.7530369625737 ,89.7540968767293 ,89.7551477319557 ,89.7561896438959 ,89.7572227262332 ,89.7582470907319 ,89.7592628472782 ,89.7602701039192 ,89.7612689669011 ,89.762259540707 ,89.7632419280929 ,89.7642162301234 ,89.7651825462064 ,89.7661409741267 ,89.7670916100795 ,89.7680345487018 ,89.7689698831045 ,89.7698977049028 ,89.7708181042459 ,89.7717311698466 ,89.7726369890094 ,89.7735356476588 ,89.774427230366 ,89.7753118203755};

void non_maximum_suppressor(unsigned char ** data) {
  unsigned char * temp_data = malloc(width * height*sizeof(unsigned char)); 
     
  int x, y, i, j;
  int direction, theta_t, index;
  unsigned int c_x, c_y;
  float theta;
    
  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      direction = 0;
            
      if (c_x != 0){
        index = (int)(c_y/c_x);
        printf(index);
        theta= atan_vals[index];
        theta_t = (int)theta;
              
        if (theta_t == 0 ){  direction = 1;}
        else if (theta_t == 45){  direction = 3;}
        else { direction = 5;}
      }
            
      else {direction = 3;}
            
      if(direction == 5 || direction == 0){ (*(temp_data + y*width + x)) = 0;}
                
      else{
        if (direction == 1) {
          if ((*(*data + y*width + x)) < (*(*data + y*width + (x-1)))) (*(temp_data + y*width + x)) =0;
          else if ((*(*data + y*width + x)) < (*(*data + y*width + (x+1)))) (*(temp_data + y*width + x)) =0;
          else (*(temp_data + y*width + x))= (*(*data + y*width + x));
        }
        else if (direction == 3){
          if ((*(*data + y*width + x)) < (*(*data + (y-1)*width + x))) (*(temp_data + y*width + x)) =0;
          else if ((*(*data + y*width + x)) < (*(*data + (y+1)*width + x))) (*(temp_data + y*width + x)) =0;
          else (*(temp_data + y*width + x)) = (*(*data + y*width + x));
        }
      }

    }
  }
    free (*data);   // the previous image data is no longer needed
    *data = temp_data;	
}

// Only keep pixels that are next to at least one strong pixel.
void hysteresis_filter(unsigned char ** data) {
  #define strong_pixel 32 // example value
    
  unsigned char * temp_data = malloc(width * height*sizeof(unsigned char));    
  int x, y;
  for(y=0; y<height; y++){
    for(x=0; x<width; x++){
      if ((*(*data + y*width + x)) > strong_pixel ) (*(temp_data + y*width + x)) = (*(*data + y*width + x));
      else (*(temp_data + y*width + x)) = 0; 
    }
  }
    
  free (*data);
  *data = temp_data;
		
}


// Hough transform constants for a 720x540 image
#define ROWS 540
#define COLS 720
// The x and y ranges for the Hough transform sweep
#define X_START -COLS/2
#define Y_START -ROWS/2
#define X_END COLS/2
#define Y_END 0 // Only sweep the bottom half of the image to ignore the horizon
// Accumulator dimensions
#define RHO_RESOLUTION 2
#define RHOS (900/RHO_RESOLUTION) // How many values of rho do we go through? Sqrt(ROWS^2 + COLS^2) = 900.
#define THETAS 180 // How many values of theta do we go through? 

// sin and cos lookup values for thetas 0 to 179
float sinvals[THETAS] = {0.0, 0.01745240643728351, 0.03489949670250097, 0.05233595624294383, 0.0697564737441253, 0.08715574274765817, 0.10452846326765346, 0.12186934340514748, 0.13917310096006544, 0.15643446504023087, 0.17364817766693033, 0.1908089953765448, 0.20791169081775931, 0.224951054343865, 0.24192189559966773, 0.25881904510252074, 0.27563735581699916, 0.29237170472273677, 0.3090169943749474, 0.32556815445715664, 0.3420201433256687, 0.35836794954530027, 0.374606593415912, 0.3907311284892737, 0.40673664307580015, 0.42261826174069944, 0.4383711467890774, 0.45399049973954675, 0.4694715627858908, 0.48480962024633706, 0.49999999999999994, 0.5150380749100542, 0.5299192642332049, 0.5446390350150271, 0.5591929034707469, 0.573576436351046, 0.5877852522924731, 0.6018150231520483, 0.6156614753256582, 0.6293203910498374, 0.6427876096865393, 0.6560590289905072, 0.6691306063588582, 0.6819983600624985, 0.6946583704589973, 0.7071067811865475, 0.7193398003386511, 0.7313537016191705, 0.7431448254773941, 0.754709580222772, 0.766044443118978, 0.7771459614569708, 0.788010753606722, 0.7986355100472928, 0.8090169943749475, 0.8191520442889918, 0.8290375725550417, 0.8386705679454239, 0.848048096156426, 0.8571673007021122, 0.8660254037844386, 0.8746197071393957, 0.8829475928589269, 0.8910065241883678, 0.898794046299167, 0.9063077870366499, 0.9135454576426009, 0.9205048534524403, 0.9271838545667874, 0.9335804264972017, 0.9396926207859083, 0.9455185755993167, 0.9510565162951535, 0.9563047559630354, 0.9612616959383189, 0.9659258262890683, 0.9702957262759965, 0.9743700647852352, 0.9781476007338056, 0.981627183447664, 0.984807753012208, 0.9876883405951378, 0.9902680687415703, 0.992546151641322, 0.9945218953682733, 0.9961946980917455, 0.9975640502598242, 0.9986295347545738, 0.9993908270190958, 0.9998476951563913, 1.0, 0.9998476951563913, 0.9993908270190958, 0.9986295347545738, 0.9975640502598242, 0.9961946980917455, 0.9945218953682734, 0.9925461516413221, 0.9902680687415704, 0.9876883405951377, 0.984807753012208, 0.981627183447664, 0.9781476007338057, 0.9743700647852352, 0.9702957262759965, 0.9659258262890683, 0.9612616959383189, 0.9563047559630355, 0.9510565162951536, 0.9455185755993168, 0.9396926207859084, 0.9335804264972017, 0.9271838545667874, 0.9205048534524404, 0.913545457642601, 0.90630778703665, 0.8987940462991669, 0.8910065241883679, 0.8829475928589271, 0.8746197071393959, 0.8660254037844387, 0.8571673007021123, 0.8480480961564261, 0.8386705679454239, 0.8290375725550417, 0.819152044288992, 0.8090169943749475, 0.7986355100472927, 0.788010753606722, 0.777145961456971, 0.766044443118978, 0.7547095802227718, 0.7431448254773942, 0.7313537016191706, 0.7193398003386514, 0.7071067811865476, 0.6946583704589971, 0.6819983600624986, 0.6691306063588583, 0.6560590289905073, 0.6427876096865395, 0.6293203910498377, 0.6156614753256584, 0.6018150231520482, 0.5877852522924732, 0.5735764363510464, 0.5591929034707469, 0.544639035015027, 0.5299192642332049, 0.5150380749100544, 0.49999999999999994, 0.48480962024633717, 0.4694715627858911, 0.45399049973954686, 0.4383711467890773, 0.4226182617406995, 0.40673664307580043, 0.39073112848927416, 0.37460659341591224, 0.3583679495453002, 0.3420201433256689, 0.32556815445715703, 0.3090169943749475, 0.29237170472273705, 0.27563735581699966, 0.258819045102521, 0.24192189559966773, 0.22495105434386478, 0.20791169081775931, 0.19080899537654497, 0.17364817766693028, 0.15643446504023098, 0.13917310096006574, 0.12186934340514755, 0.10452846326765373, 0.08715574274765864, 0.06975647374412552, 0.05233595624294381, 0.0348994967025007, 0.01745240643728344};
float cosvals[THETAS] = {1.0, 0.9998476951563913, 0.9993908270190958, 0.9986295347545738, 0.9975640502598242, 0.9961946980917455, 0.9945218953682733, 0.992546151641322, 0.9902680687415704, 0.9876883405951378, 0.984807753012208, 0.981627183447664, 0.9781476007338057, 0.9743700647852352, 0.9702957262759965, 0.9659258262890683, 0.9612616959383189, 0.9563047559630354, 0.9510565162951535, 0.9455185755993168, 0.9396926207859084, 0.9335804264972017, 0.9271838545667874, 0.9205048534524404, 0.9135454576426009, 0.9063077870366499, 0.898794046299167, 0.8910065241883679, 0.882947592858927, 0.8746197071393957, 0.8660254037844387, 0.8571673007021123, 0.848048096156426, 0.838670567945424, 0.8290375725550416, 0.8191520442889918, 0.8090169943749475, 0.7986355100472928, 0.788010753606722, 0.7771459614569709, 0.766044443118978, 0.7547095802227721, 0.7431448254773942, 0.7313537016191706, 0.7193398003386512, 0.7071067811865476, 0.6946583704589974, 0.6819983600624985, 0.6691306063588582, 0.6560590289905073, 0.6427876096865394, 0.6293203910498375, 0.6156614753256583, 0.6018150231520484, 0.5877852522924731, 0.5735764363510462, 0.5591929034707468, 0.5446390350150272, 0.5299192642332049, 0.5150380749100544, 0.5000000000000001, 0.4848096202463371, 0.46947156278589086, 0.4539904997395468, 0.43837114678907746, 0.42261826174069944, 0.4067366430758002, 0.39073112848927394, 0.37460659341591196, 0.3583679495453004, 0.3420201433256688, 0.32556815445715676, 0.30901699437494745, 0.29237170472273677, 0.27563735581699916, 0.25881904510252074, 0.2419218955996679, 0.22495105434386492, 0.20791169081775945, 0.19080899537654492, 0.17364817766693041, 0.15643446504023092, 0.1391731009600657, 0.12186934340514749, 0.10452846326765346, 0.08715574274765814, 0.06975647374412546, 0.052335956242943966, 0.03489949670250108, 0.017452406437283376, 6.123233995736766e-17, -0.017452406437283477, -0.03489949670250073, -0.05233595624294362, -0.06975647374412533, -0.08715574274765824, -0.10452846326765333, -0.12186934340514737, -0.13917310096006535, -0.15643446504023104, -0.1736481776669303, -0.1908089953765448, -0.20791169081775912, -0.2249510543438648, -0.24192189559966779, -0.25881904510252085, -0.27563735581699905, -0.29237170472273666, -0.30901699437494734, -0.3255681544571564, -0.3420201433256687, -0.35836794954530027, -0.37460659341591207, -0.3907311284892736, -0.40673664307580004, -0.42261826174069933, -0.4383711467890775, -0.4539904997395467, -0.46947156278589053, -0.484809620246337, -0.4999999999999998, -0.5150380749100543, -0.5299192642332048, -0.5446390350150271, -0.5591929034707467, -0.5735764363510458, -0.587785252292473, -0.6018150231520484, -0.6156614753256583, -0.6293203910498373, -0.6427876096865394, -0.6560590289905075, -0.6691306063588582, -0.6819983600624984, -0.694658370458997, -0.7071067811865475, -0.7193398003386512, -0.7313537016191705, -0.743144825477394, -0.754709580222772, -0.7660444431189779, -0.7771459614569707, -0.7880107536067219, -0.7986355100472929, -0.8090169943749473, -0.8191520442889916, -0.8290375725550416, -0.8386705679454242, -0.848048096156426, -0.8571673007021122, -0.8660254037844387, -0.8746197071393957, -0.8829475928589268, -0.8910065241883678, -0.898794046299167, -0.9063077870366499, -0.9135454576426008, -0.9205048534524402, -0.9271838545667873, -0.9335804264972017, -0.9396926207859083, -0.9455185755993167, -0.9510565162951535, -0.9563047559630354, -0.9612616959383187, -0.9659258262890682, -0.9702957262759965, -0.9743700647852352, -0.9781476007338057, -0.981627183447664, -0.984807753012208, -0.9876883405951377, -0.9902680687415703, -0.992546151641322, -0.9945218953682733, -0.9961946980917455, -0.9975640502598242, -0.9986295347545738, -0.9993908270190958, -0.9998476951563913};


// data from BMP is stored with the bottom-left pixel first (origin is at bottom left)
void hough_transform(unsigned char * data, unsigned short ** accum) {
	
	if (height != ROWS || width != COLS){
		printf("ERROR: Invalid image dimensions for the hough transform\n");
		return;
	}
	
	unsigned short * temp_accum = (unsigned short*) malloc(RHOS * THETAS * sizeof(unsigned short)); 
	
  int x, y, theta, rho;
  double hough_height = ((sqrt(2.0) * (double)(height>width ? height:width)) / 2.0);
  int centerX = width / 2.0;
  int centerY = height / 2.0;

  for(y = 0; y <= (int)(height/2); y++){
    for(x = 0; x < width; x++){
      if ((*(data + y*width + x)) > 0){
        for(theta=0; theta<THETAS; theta++){
          rho = x*cosvals[theta] + y*sinvals[theta];
          rho = (int)rho;
          (*(temp_accum + theta*THETAS + rho)) += 1;
        }
      }
    }
  }
	*accum = temp_accum;
}

// A line (rho,theta)
struct line {
	short rho;
	unsigned short theta;
};

//////////////////////////////////////////////////////

// Find num_lines lines with the highest accumulated values (local maxima)
void extract_lines(unsigned short * accum, struct line ** lines, int num_lines) {
	
	struct line * temp_lines = (struct line *) malloc(num_lines * sizeof(struct line));

  int rho, theta, i, j;
  int k = 0;
  int max_rho, max_theta;
  int max_rho_arr[(THETAS-2)*(RHOS-2)];
  int max_theta_arr[(THETAS-2)*(RHOS-2)];
  
  printf("accum value 2: %d\n", *(accum + 9*THETAS + 9));
  
  int count;
  int x, y;
  

  for (rho = 0; rho < RHOS; rho++){
    for(theta = 0; theta < THETAS; theta++){
      if ( (*(accum + theta*THETAS + rho)) > 800){
        int max = (*(accum + theta*THETAS + rho));
        for(y = -2; y <= 2; y++){
          for(x = -2; x <= 2; x++){
            int new_rho = rho + y;
            int new_theta = theta + x;
            
            if(new_theta < 0){
              new_theta = 180 + new_theta;
            }
            else if(new_theta >= 180){
              new_theta = new_theta - 180;
            }
            
            
            if (new_rho >= 0 && new_rho < RHOS){
              if ((*(accum + new_theta*THETAS + new_rho)) > max){
                max = (*(accum + new_theta*THETAS + new_rho));
                max_rho_arr[k] =  new_rho;
                max_theta_arr[k] =  new_theta;
                x=y=3;
              }
            }
          
           
          }
        }
        
        if (max > (*(accum + theta*THETAS + rho))){
          continue;
        }
      }
      k++;
    }
  }
  
  printf("k: %d\n", k);  

  //sort
  printf("before sorted max theta: %d\n",max_theta_arr[1]);
  printf("before sorted max rho: %d\n",max_rho_arr[1]);
  int n, m, temp1, temp2;
  
  for(n=0; n<(THETAS/5)*(RHOS/5); n++){
    for(m=0; m< (THETAS/5)*(RHOS/5); m++){
  //for(n=0; n< k-1; n++){
    //for(m=n+1; m< k; m++){
      if((*(accum + (max_theta_arr[m])*THETAS + (max_rho_arr[m]))) < (*(accum + (max_theta_arr[m+1])*THETAS + (max_rho_arr[m+1])))){
      //if((*(accum + (max_theta_arr[n])*THETAS + (max_rho_arr[n]))) < (*(accum + (max_theta_arr[m])*THETAS + (max_rho_arr[m])))){  
        
        temp1 = max_rho_arr[m];
        max_rho_arr[m] = max_rho_arr[n];
        max_rho_arr[n] = temp1;
        
        temp2 = max_theta_arr[m];
        max_theta_arr[m] = max_theta_arr[n];
        max_theta_arr[n] = temp2;
      
      }
    }
  }


 
  printf("max accum: %d\n", (*(accum + (max_theta_arr[0])*THETAS + (max_rho_arr[0]))));
  printf("sorted max theta: %d\n",max_theta_arr[1]);
  printf("sorted max rho: %d\n",max_rho_arr[1]);

  int a;

  for (a=0; a< num_lines; a++){
    printf("max rho %d: %d\n",a, max_rho_arr[a]);
    printf("max theta %d: %d\n",a, max_theta_arr[a]);
    (*(temp_lines+a)).rho = max_rho_arr[a];
    (*(temp_lines+a)).theta =  max_theta_arr[a];

    printf("changed line rho %d: %d\n",a, (*(temp_lines+a)).rho);
    printf("changed line theta %d: %d\n",a, (*(temp_lines+a)).theta);    
   }

	*lines = temp_lines;
}

// Functions for finding coordinates x,y coordinates from a rho,theta line. 
// These x,y coordinates correspond to the coordinate system of the original 720x540 image,
// which as its origin at the bottom left of the image.
int find_x(int y_bottomleft_origin, struct line l) {
	// Using the coordinate system with origin at center of image, rho = xcostheta + ysintheta
	int y_center_origin = y_bottomleft_origin - height/2;
	int x_center_origin = (l.rho*RHO_RESOLUTION - y_center_origin*sinvals[l.theta])/cosvals[l.theta];
	int x_bottomleft_origin = x_center_origin + width/2;
	return x_bottomleft_origin >= 0 && x_bottomleft_origin < width ? x_bottomleft_origin : -1;
}
int find_y(int x_bottomleft_origin, struct line l) {
	// Using the coordinate system with origin at center of image, rho = xcostheta + ysintheta
	int x_center_origin = x_bottomleft_origin - width/2;
	int y_center_origin = (l.rho*RHO_RESOLUTION - x_center_origin*cosvals[l.theta])/sinvals[l.theta];
	int y_bottomleft_origin = y_center_origin + height/2;
	return y_bottomleft_origin >= 0 && y_bottomleft_origin < height ? y_bottomleft_origin : -1;
}

// Overlay the lines onto the original image.
void overlay_lines(struct pixel * data, struct line * lines, int num_lines) {
	
	int i, y, x;
	
	// Overlay the lines
	for (i = 0; i < num_lines; i++){
		// Determine the points that exist on this line.
		// If the line is more vertical than horizontal, sweep across the y
   printf("line theta: %d\n", lines[i].theta);
   printf("line rho: %d\n", lines[i].rho);
   
		if (lines[i].theta < THETAS/4 || lines[i].theta >= 3*THETAS/4){
      for (y = 0; y < height; y++){
				x = find_x(y, lines[i]);
				if (x >= 0){
					data[y*width + x].r = 255;
					data[y*width + x].g = 0;
					data[y*width + x].b = 0;
				}
			}
		// If the line is more horizontal than vertical, sweep across the x
		} else if (lines[i].theta >= THETAS/4 && lines[i].theta < 3*THETAS/4){
			for (x = 0; x < width; x++){
				y = find_y(x, lines[i]);
				if (y >= 0){
					data[y*width + x].r = 255;
					data[y*width + x].g = 0;
					data[y*width + x].b = 0;
				}
			}
		}
	}
}

int main(int argc, char *argv[]){
	struct pixel * rgb_data;
	unsigned char * data;
	unsigned char * header;
	unsigned short * accum = NULL;
	struct line * lines;
	int debug = 0;
	double start, end, func_start, hough_runtime, edge_runtime;
	char outputfile [256];
	int num_lines;

	// Check inputs
	if (argc < 4) {
		printf("Usage: edgedetect <BMP filename> <debug> <num lines to find>\n\ndebug: true false\n");
		return 0;
	}

	// Determine debug mode
	if (argv[2][0] == 't') debug = 1;

	// Parse number of lines to find
	num_lines = atoi(argv[3]);

	// Open input image file (24-bit bitmap image)
	if (read_bmp(argv[1], &header, &rgb_data) < 0) {
		printf("Failed to read BMP\n");
		return 0;
	}
 
 printf("height: %d, width: %d\n", height, width);

	/********************************************
	*          IMAGE PROCESSING STAGES          *
	********************************************/

	// Start measuring time
	start = get_wall_time();
	
	/// Grayscale conversion
	convert_to_grayscale(rgb_data, &data);

	/// Gaussian filter
	gaussian_blur(&data);

	/// Sobel operator
	sobel_filter(&data);

	/// Non-maximum suppression
	non_maximum_suppressor(&data);

	/// Hysteresis
	hysteresis_filter(&data);
	if (debug) {
		snprintf(outputfile, 256, "%s_part1_edges.bmp",argv[1]);
		printf("Writing edge-detected image to %s\n",outputfile);
		write_grayscale_bmp(outputfile, header, data);
	}
	
	edge_runtime = get_wall_time() - start;

 

	/// Hough
	func_start = get_wall_time();
	hough_transform(data, &accum); 
	//hough_runtime = get_wall_time() - func_start;

	/// Find the strongest num_lines lines
	extract_lines(accum, &lines, num_lines);
	hough_runtime = get_wall_time() - func_start;
	// Stop measuring time.
	end = get_wall_time();
	
	printf("EDGE DETECTION TIME ELAPSED: %.2f ms\n", edge_runtime);
	printf("HOUGH TRANSFORM TIME ELAPSED: %.2f ms\n", hough_runtime);
	printf("TOTAL TIME ELAPSED: %.2f ms\n", end - start);
	
	/// Overlay the detected lines onto the original image
	overlay_lines(rgb_data, lines, num_lines);

	// Write out the original image with detected lines overlaid
	snprintf(outputfile, 256, "%s_part1_lines.bmp",argv[1]);
	printf("Writing line-detected image to %s\n",outputfile);
	write_bmp(outputfile, header, rgb_data);

  EntitySet usage = rusage_create(0);
 int x;
 for(x = 0; x < usage.size; ++x) {
 Entity e = usage.entities[x];
  printf("Entity[%d]\n", x);
  printf(" key: '%s'\n", e.key);
  printf(" key: '%s'\n", e.value);
   }
 char* key = "VmSize";
  printf("Value of '%s' is: '%s'\n", key, rusage_getstr(&usage, key, "nil"));
  rusage_destroy(usage);
 return EXIT_SUCCESS;
 
	return 0;
}

  


