#include <FreeRTOS_ARM.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_AMG88xx.h>
#include "Adafruit_TCS34725.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define data_NUM 5
#define SEALEVELPRESSURE_HPA (1013.25)

#define BME_CS 10

SemaphoreHandle_t sem0, sem1, sem2, sem3, sem4;

char *dtostrf2(double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}

byte len;
long tim,tims;
int i;
char buf[70], buf1[7000], buf2[7000];

extern TwoWire Wire1;
int idx;
int data[data_NUM] = {0,};
int total = 0;
int sound = 0;
int movement = 0;
float red, green, blue;
uint16_t r, g, b, c, lux;
const int MPU_addr = 0x68;
int16_t GyXf, GyYf, GyZf, GyX=0, GyY=0, GyZ=0;
int GyXd[data_NUM] = {0,};
int GyYd[data_NUM] = {0,};
int GyZd[data_NUM] = {0,};
float temp, pre, hum;
char tempc[5], prec[7], humc[5];

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Adafruit_AMG88xx amg;
Adafruit_BME280 bme(BME_CS); 

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];

static void Thread0(void* arg) {
  tims = millis();
  xSemaphoreGive(sem0);
  while(1)
  {
    tim = millis()-tims;
    vTaskDelay((10L*configTICK_RATE_HZ)/1000L);
  }
}

static void Thread1(void* arg) {
  xSemaphoreGive(sem3);
  while(1)
  {
    xSemaphoreTake(sem3, portMAX_DELAY);
    for(i=0;i<100;i++)
    {
      Wire1.beginTransmission(MPU_addr);
      Wire1.write(0x43); 
      Wire1.endTransmission(false);
      Wire1.requestFrom(MPU_addr,14,true);  
  
      GyX -=GyXd[idx];
      GyY -=GyYd[idx];
      GyZ -=GyZd[idx];
      GyXd[idx] = Wire1.read()<<8|Wire1.read();
      GyYd[idx] = Wire1.read()<<8|Wire1.read();  
      GyZd[idx] = Wire1.read()<<8|Wire1.read();  
      GyX +=GyXd[idx];
      GyY +=GyYd[idx];
      GyZ +=GyZd[idx];
    
      total -= data[idx];
      data[idx] = analogRead(A1);
      total += data[idx];
      idx++;
      if (idx >= data_NUM) idx = 0;
  
      if(digitalRead(7)==LOW) {movement = 1; xSemaphoreGive(sem4);}
      else movement = 0;
      sound = (total / data_NUM)-503;
      GyXf = (GyX/data_NUM)-690;
      GyYf = (GyY/data_NUM)-90;
      GyZf = (GyZ/data_NUM)-67;
      sprintf(buf, "%ld %d %d %d %d %d %d %d %d %d %s %s %s\n",tim,movement,sound,GyXf,GyYf,GyZf,int(red),int(blue),int(green),int(lux),tempc,prec,humc);
      strcat(buf1, buf);
      vTaskDelay((10L*configTICK_RATE_HZ)/1000L);
    }
    xSemaphoreGive(sem1);

    for(i=0;i<100;i++)
    {
      Wire1.beginTransmission(MPU_addr);
      Wire1.write(0x43); 
      Wire1.endTransmission(false);
      Wire1.requestFrom(MPU_addr,14,true);  
  
      GyX -=GyXd[idx];
      GyY -=GyYd[idx];
      GyZ -=GyZd[idx];
      GyXd[idx] = Wire1.read()<<8|Wire1.read();
      GyYd[idx] = Wire1.read()<<8|Wire1.read();  
      GyZd[idx] = Wire1.read()<<8|Wire1.read();  
      GyX +=GyXd[idx];
      GyY +=GyYd[idx];
      GyZ +=GyZd[idx];
    
      total -= data[idx];
      data[idx] = analogRead(A1);
      total += data[idx];
      idx++;
      if (idx >= data_NUM) idx = 0;
  
      if(digitalRead(7)==LOW) {movement = 1; xSemaphoreGive(sem4);}
      else movement = 0;
      sound = (total / data_NUM)-503;
      GyXf = (GyX/data_NUM)-690;
      GyYf = (GyY/data_NUM)-90;
      GyZf = (GyZ/data_NUM)-67;
      sprintf(buf, "%ld %d %d %d %d %d %d %d %d %d %s %s %s\n",tim,movement,sound,GyXf,GyYf,GyZf,int(red),int(blue),int(green),int(lux),tempc,prec,humc);
      strcat(buf2, buf);
      vTaskDelay((10L*configTICK_RATE_HZ)/1000L);
    }
    xSemaphoreGive(sem2);
    xSemaphoreGive(sem3);
    vTaskDelay((5L*configTICK_RATE_HZ)/1000L);
  }
}

static void Thread2(void* arg) {
  while(1)
  {
    xSemaphoreTake(sem1, portMAX_DELAY);
    xSemaphoreTake(sem0, portMAX_DELAY);
    digitalWrite(5,HIGH);
    digitalWrite(5,LOW);
    Serial1.print(buf1);
    strcpy(buf1, "");
    xSemaphoreGive(sem0);
  }
}

static void Thread3(void* arg) {
  while(1)
  {
    xSemaphoreTake(sem2, portMAX_DELAY);
    xSemaphoreTake(sem0, portMAX_DELAY);
    digitalWrite(5,HIGH);
    digitalWrite(5,LOW);
    Serial1.print(buf2);
    strcpy(buf2, "");
    xSemaphoreGive(sem0);
  }
}

static void Thread4(void* arg) {
  vTaskDelay((5000L*configTICK_RATE_HZ)/1000L);
  while(1)
  {
    xSemaphoreTake(sem3, portMAX_DELAY);
    xSemaphoreTake(sem0, portMAX_DELAY);
    
    tcs.getRGB(&red, &green, &blue);
    tcs.getRawData(&r, &g, &b, &c);
    lux = tcs.calculateLux(r, g, b);
    temp = bme.readTemperature();
    pre = bme.readPressure()/100.0F;
    hum = bme.readHumidity();

    dtostrf2(temp,5,2,tempc);
    dtostrf2(pre,7,2,prec);
    dtostrf2(hum,5,2,humc);
    
    xSemaphoreGive(sem0);
    xSemaphoreGive(sem3);
    vTaskDelay((5000L*configTICK_RATE_HZ)/1000L);
  }
}

static void Thread5(void* arg) {
  int li;
  vTaskDelay((5000L*configTICK_RATE_HZ)/1000L);
  while(1)
  {
    xSemaphoreTake(sem4, portMAX_DELAY);
    xSemaphoreTake(sem3, portMAX_DELAY);
    amg.readPixels(pixels);
    xSemaphoreTake(sem0, portMAX_DELAY);
    for (int li = AMG88xx_PIXEL_ARRAY_SIZE-1; li >= 0; li--) 
    {
      if(pixels[li]<(temp-5)) pixels[li] = temp - 5;
      if(pixels[li]>27) Serial.print("O ");
      else Serial.print(". ");
      if((li%8)==0) Serial.print("\n");
    }
    Serial.println();
    xSemaphoreGive(sem0);
    xSemaphoreGive(sem3);
    vTaskDelay((1000L*configTICK_RATE_HZ)/1000L);
  }
}

void setup() {

  portBASE_TYPE s0, s1, s2, s3, s4, s5;
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(5,OUTPUT); //INTERRUPT
  pinMode(6,OUTPUT); //INTERRUPT
  pinMode(A1,INPUT); //SOUND
  pinMode(7,INPUT); //MOTION
  
  Wire1.begin();
  Wire1.beginTransmission(MPU_addr);
  Wire1.write(0x6B);  
  Wire1.write(0);     
  Wire1.endTransmission(true);
  
  if (tcs.begin()) {} 
  else 
  {
    Serial.println("Could not find a valid TCS34725 sensor!");
    while (1); 
  }
  delay(100);
  if (amg.begin()) {}
  else
  {
    Serial.println("Could not find a valid AMG88xx sensor!");
    while (1);
  }
  delay(100);
  if (bme.begin()) {}
  else
  {
    Serial.println("Could not find a valid BME280 sensor!");
    while (1);
  }

  strcpy(tempc, "0");
  strcpy(prec, "0");
  strcpy(humc, "0");
  
  sem0 = xSemaphoreCreateCounting(1, 0);
  sem1 = xSemaphoreCreateCounting(1, 0);
  sem2 = xSemaphoreCreateCounting(1, 0);
  sem3 = xSemaphoreCreateCounting(1, 0);
  sem4 = xSemaphoreCreateCounting(1, 0);
  s0 = xTaskCreate(Thread0, NULL, 2000, NULL, 2, NULL);
  s1 = xTaskCreate(Thread1, NULL, 2000, NULL, 1, NULL);
  s2 = xTaskCreate(Thread2, NULL, 2000, NULL, 1, NULL);
  s3 = xTaskCreate(Thread3, NULL, 2000, NULL, 1, NULL);
  s4 = xTaskCreate(Thread4, NULL, 2000, NULL, 1, NULL);
  s5 = xTaskCreate(Thread5, NULL, 2000, NULL, 1, NULL);

  if (sem0==NULL||sem1==NULL||sem2==NULL||sem3==NULL||s0!=pdPASS||s1!=pdPASS||s2!=pdPASS||s3!=pdPASS||s4!=pdPASS){
    Serial.println(F("Creation problem"));
    while(1);
  }

  vTaskStartScheduler();
  Serial.println(F("Insufficient RAM"));
  while(1);
}

void loop() {
}
