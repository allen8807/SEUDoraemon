/*
 * File:   camera.c
 * Author: liuxx
 *
 * Created on 2012年7月21日, 下午9:04
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string.h>
#include <pthread.h>
#include <linux/videodev.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>

#include "camera.h"
#include "v4l2uvc.h"
#include "gui.h"
#include "utils.h"
#include "color.h"
/*
int piancha = 0;
int xielv = 0;
int last_piancha = 0;
int last_xielv = 0;
int stop_flag = 0;
int final_stop_flag = 0;
#define row_num  10
#define col_num  160
#define yuzhi 35                    //ÈüµÀºÚ°×²îÖµ

unsigned char image[row_num][col_num];
//////////////////////ºÚÏßÌáÈ¡////////////////////
int center_line[3][row_num]; //ºÚÏßÖÐµã×ø±ê
int fixed_center_line[3][row_num + 2]; //ÐÞÕýºóµÄºÚÏßÖÐµã×ø±ê
int stop_line[10];
unsigned char fall_edge, rise_edge; //ºÚ°×œ»œÓ±êÖŸ
unsigned char left_edge, right_edge; //ºÚÏßÁœ¶Ë×ø±ê
unsigned char exact = 0; //ÕýÈ·ÕÒµœºÚÏß


unsigned int count = 0;
unsigned int director = 0;
unsigned int tiaobian_line_0_0 = 0;
unsigned int tiaobian_line_1_0 = 0;
unsigned int tiaobian_line_0_1 = 0;
unsigned int tiaobian_line_1_1 = 0;
*/

struct point2D {
	int row;
	int column;
};

struct point2D yellow_Center;
double distance;
double angle;
double piancha;

#define row_num  240
#define col_num  160
#define sample_row_num 24
#define sample_col_num 32
int  image_Y1[row_num][col_num];
int  image_Y2[row_num][col_num];
int  image_U[row_num][col_num];
int  image_V[row_num][col_num];

int  image_R[row_num][col_num+col_num];
int  image_G[row_num][col_num+col_num];
int  image_B[row_num][col_num+col_num];

int  image_H[row_num][col_num+col_num];
int  image_S[row_num][col_num+col_num];
int  image_BV[row_num][col_num+col_num];

char image_Yellow_10[sample_row_num][sample_col_num];
char image_Yellow_10_2[sample_row_num][sample_col_num];

/*************************************************************/

int getMax(int Red , int Green , int Blue)
{
	if( Red >= Green )
	{
		if(Red >= Blue)return Red;
		else return Blue;
	}
	else
	{
		if(Green >= Blue)return Green;
		else return Blue;	
	}
}

int getMin(int Red , int Green , int Blue)
{
	if( Red <= Green )
	{
		if(Red <= Blue)return Red;
		else return Blue;
	}
	else
	{
		if(Green <= Blue)return Green;
		else return Blue;	
	}
}

int getUp(struct point2D yc)
{

int i=0;
int j=0;
int yc_row=yc.row*10;
int yc_col=yc.column*10;
int upDis=0;

if(yc_row == 0 && yc_col == 0)return 0;


		for(j = yc_row ;j >= 0 ;j--)
		{
			if( image_H[j][yc_col-1+i] > 200 && image_H[j][yc_col-1+i] < 300)
				{upDis+=(yc_row-j);break;}

			else if(j == 0 && image_H[j][yc_col-1+i] > 30 && image_H[j][yc_col-1+i] < 90)
				{upDis+=yc_row;printf("NOT FOUND");break;}
			else if(j == 0 && (image_H[j][yc_col-1+i] > 90 || image_H[j][yc_col-1+i] < 30) )
				{return 0;}
		}

return upDis;
}

int getDown(struct point2D yc)
{

int i=0;
int j=0;
int yc_row=yc.row*10;
int yc_col=yc.column*10;
int downDis=0;

if(yc_row == 0 && yc_col == 0)return 0;


		for(j = yc_row ;j < row_num ;j++)
		{
			if( image_H[j][yc_col-1+i] > 200 && image_H[j][yc_col-1+i] < 260)
				{downDis+=(j-yc_row);break;}

			else if(j == row_num-1 && image_H[j][yc_col-1+i] > 30 && image_H[j][yc_col-1+i] < 90)
				{downDis+=row_num-yc_row;printf("NOT FOUND");break;}
			else if(j == row_num-1 && (image_H[j][yc_col-1+i] > 90 || image_H[j][yc_col-1+i] < 30) )
				{return 0;}
		}

return downDis;
}


/*******************************************************************************/








int Get_xielv() {
    return distance;
}

int Get_piancha() {
    return piancha;
}

struct vdIn *videoIn;
int width = 320;
int height = 240;

int init_video() {
    const char *videodevice = NULL;
    //  int format = V4L2_PIX_FMT_MJPEG;
    int format = V4L2_PIX_FMT_YUYV;
    int i;
    int grabmethod = 1;

    int fps = 15;

    char *avifilename = NULL;
    if (videodevice == NULL || *videodevice == 0) {
        videodevice = "/dev/video0";
    }
    if (avifilename == NULL || *avifilename == 0) {
        avifilename = "video.avi";
    }
    videoIn = (struct vdIn *) calloc(1, sizeof (struct vdIn));

    if (init_videoIn
            (videoIn, (char *) videodevice, width, height, fps, format,
            grabmethod, avifilename) < 0) {
        return 0;
    }
    return 1;
}

int mainLoop() {
    if (!(videoIn->signalquit)) {
        return 0;
    }

/*********************************************************************************
    int ii, jj;
    int i = 0;
    int j = 0;
    for (ii = 0; ii < height; ii = ii + 24) {

        for (jj = 0; jj < width * 2; jj = jj + 4) {
            image[i][j] = videoIn->framebuffer[ii * width * 2 + jj];
            j++;
        }
        //printf("\n");
        //i++;
    }
    printf("end end end end end end end end end end\n");
    Pic_lvbo();
    Get_blackline();
    Get_stopline();
    Modify_centerline();
    Modify_blackline();
    Get_Info();

    for (i = 0; i < 3; i++) {
        printf("line%d\n", i);
        printf("%d\t", tiaobian_line_0_1);
        printf("%d\n", tiaobian_line_1_1);
        for (j = 0; j < row_num + 2; j++) {
            printf("%d\t", fixed_center_line[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("xielv:%d\n", xielv);
    printf("piancha:%d\n", piancha);
    printf("\n");
    printf("stop_line\n");
    for (i = 0; i < 10; i++)printf("%d\t", stop_line[i]);
    printf("\n");
    printf("final_stop_flag:%d\n", final_stop_flag);

    /*
            currtime = SDL_GetTicks();
            if (currtime - lasttime > 0) {
                frmrate = 1000 / (currtime - lasttime);
            }
            lasttime = currtime;
     
*************************************************************************/
 	int ii,jj;
         int i=0;
         int j=0;

   //height:240 width:360
   //seperate 
           
	 for (ii = 0,i=0; ii < height; ii++,i++) 
	{
   
            for(jj=0,j=0;jj<width*2;jj=jj+4,j++)
            {
            	image_Y1[i][j]=videoIn->framebuffer[ii*width*2+jj];
	    	image_Y2[i][j]=videoIn->framebuffer[ii*width*2+jj+2];
		image_U[i][j]=videoIn->framebuffer[ii*width*2+jj+1];
		image_V[i][j]=videoIn->framebuffer[ii*width*2+jj+3];
            }
            // printf("\n");     
        }
	

//YUYV to RGB
	 for (ii = 0, i = 0 ; i < height ; ii++,i++) 
	{
            for( jj =0 ,j = 0 ; j < width ; jj++,j=j+2)
            {
            	image_R[i][j]=image_Y1[ii][jj]+1.13983*(image_V[ii][jj]-128);
	    	image_G[i][j]=image_Y1[ii][jj]-0.39465*(image_U[ii][jj]-128)-0.58060*(image_V[ii][jj]-128);
		image_B[i][j]=image_Y1[ii][jj]+2.08211*(image_U[ii][jj]-128);

		image_R[i][j+1]=image_Y2[ii][jj]+1.13983*(image_V[ii][jj]-128);
	    	image_G[i][j+1]=image_Y2[ii][jj]-0.39465*(image_U[ii][jj]-128)-0.58060*(image_V[ii][jj]-128);
		image_B[i][j+1]=image_Y2[ii][jj]+2.08211*(image_U[ii][jj]-128);
            }   
        }
//RGB to HSB (JUST H and S)

	float max,min;
	for (ii = 0, i = 0 ; i < height ; ii++,i++) 
	{
            for( jj =0 ,j = 0 ; j < width ; jj++,j++)
            {
            	max = getMax(image_R[ii][jj],image_G[ii][jj],image_B[ii][jj]);
		min = getMin(image_R[ii][jj],image_G[ii][jj],image_B[ii][jj]);
				
	//Caculate H
		if(max == min)image_H[i][j]=0;
		else if (max == image_R[ii][jj] && image_G[ii][jj] >= image_B[ii][jj]) 
			image_H[i][j]=60* ( (image_G[ii][jj]-image_B[ii][jj])/(max-min) );
		else if (max == image_R[ii][jj] && image_G[ii][jj] < image_B[ii][jj]) 
			image_H[i][j]=60* ( (image_G[ii][jj]-image_B[ii][jj])/(max-min) )+360;
		else if (max == image_G[ii][jj]) 
			image_H[i][j]=60* ( (image_B[ii][jj]-image_R[ii][jj])/(max-min) )+120;
		else  if (max == image_B[ii][jj])
			image_H[i][j]=60* ( (image_R[ii][jj]-image_G[ii][jj])/(max-min) )+240;
		else  image_H[i][j]=999999;

	//Caculate S
		   if(max == 0) image_S[i][j] = 0;
		   else image_S[i][j]=100-(min*100/max);

	//Caculate BV
				
		   image_BV[i][j] = max;

		}   
        }
	

		for(ii=0,i=0;ii<height;ii=ii+10,i++)
         {
             //printf("%d\t",ii);
             for(jj=0,j=0;jj<width;jj=jj+10,j++)
             {
			float a=image_R[ii][jj];float b= image_G[ii][jj];
			float tempRG=a/b;//image_R[ii][jj]/image_G[ii][jj];
			//printf("%f\t",tempRG);
		if(image_H[ii][jj]>=40 && image_H[ii][jj]<=85 && image_S[ii][jj] > 30 &&tempRG < 1.1)
		{		
		image_Yellow_10[i][j]=1;
               // printf(" *");
		}
		else 
		{
		image_Yellow_10[i][j]=0;
		//printf("  ");
		}	
             }
               //printf("\n"); // printf("\n"); // printf("\n");
         }

	for(i=0;i<sample_row_num;i++)
         {
             printf("%d\t",i);
             for(j=0;j<sample_col_num;j++)
             {
			image_Yellow_10_2[i][j]=0;
			if(image_Yellow_10[i][j] > 0)
			{
					if(i >= 1 && j >=1 && i<=sample_row_num-2 && j<=sample_col_num-2)
					{
						image_Yellow_10_2[i][j]=image_Yellow_10[i-1][j-1]+
									image_Yellow_10[i-1][j]+
									image_Yellow_10[i-1][j+1]+
									image_Yellow_10[i][j-1]+
									image_Yellow_10[i][j+1]+
									image_Yellow_10[i+1][j-1]+
									image_Yellow_10[i+1][j]+
									image_Yellow_10[i+1][j+1];
					if(image_Yellow_10_2[i][j]<=3)image_Yellow_10_2[i][j]=0;
					}
					else image_Yellow_10_2[i][j]=0;
						
			}
			//else {image_Yellow_10_2[i][j]=0;}

			if(image_Yellow_10_2[i][j]>=1)
			printf(" %d",image_Yellow_10_2[i][j]);
			else printf("  ");
             }
               printf("\n"); // printf("\n"); // printf("\n");
         }

// GET CENTER OF YELLOW

yellow_Center.row=0;
yellow_Center.column=0;
int yellow_Max;//=0;
int a2,b2,countab;
for(i=0;i<sample_row_num;i++)
{
for(j=0;j<sample_col_num;j++)
	{
		if (image_Yellow_10_2[i][j]>yellow_Max)
			{
				yellow_Center.row=i;
				yellow_Center.column=j;		
			}
//		if(yellow_Max==8)break;
if(image_Yellow_10_2[i][j]>6)
			{
				a2+=i;
				b2+=j;
				countab++;	
			}

	}
}

if(countab > 5){
yellow_Center.row=a2/countab;
yellow_Center.column=b2/countab;
}
int a=getUp(yellow_Center);
printf("Updis : %d\t",a);
int b=getDown(yellow_Center);
printf("Downdis : %d\t",b);
distance = a+b;
angle = yellow_Center.column - 17;
//double temp1 = sin(angle / 16 *25 /57.3);
piancha = 3500 * sin(angle / 16 *25 /57.3) / distance;
printf("piancha : %f\n",piancha);


printf("%d\t%d\n",yellow_Center.row,yellow_Center.column);



    if (uvcGrab(videoIn) < 0) {
        printf("Error grabbing \n");
        return 0;
    }
    return 1;
    //  usleep(10000);
}

/*

int main(int argc, char** argv) {
    if (init_video() == 0) {
        printf("init error");
        return;
    }
    int i = 0;
    while (videoIn->signalquit) {
        mainLoop();
        i++;
        if (i > 2) {

        }
    }
    return (EXIT_SUCCESS);
}
 *
 */
