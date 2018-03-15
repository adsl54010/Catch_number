#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>



using namespace cv;
using namespace std;
int main()
{
	IplImage *InImage, *MidImage,*OutImage,*OutImage1,*OutImage1x,*OutImage1y,*OutImage2,*OutImage3,*OutImage4,*OutImage5;
	int label=1,**Imagelabel,x,y,max,flag=0,i,j,*Imagegray,**Imagelabelx,**Imagelabely,big=0,small=0,maxx=0,minn=0,sum=0,sumn=0,num=0,maxT=0,threshold=1,rangebottom=0,*lo,rg=0,gb=0,rb=0,error=4,left=0,right=0,top=0,bottom=0,xx=0,yy=0,findbottom=0,pointx=0,pointy=0,qq=0,mmaxx=0,mmaxy=0,mmmaxx=0,mmmaxy=0,range=20,rangetop=40,minx=10000,miny=10000,zzx=0,saveerrorw=0,saveerrorh=0;
	double u0=0,u1=0,ur=0,pie0=0,pie1=0,lob=0,lot=0,maxlo=0;
	char InputFileName[50]="",InputFileNameout[50],InputFileNameblack[50],InputF[50];

	strcat(InputFileNameout,"IMG_0");
	printf("Enter file name.\n");
	scanf("%s", &InputF);
	strcat(InputFileNameout,InputF);
	strcat(InputFileNameout,".jpg");
	
	InImage = cvLoadImage(InputFileName, CV_LOAD_IMAGE_COLOR);
	if(!InImage)
	{
		printf("Error: Couldn't open the image file.\n");
		system("pause");
		return -1;
	}


	
	MidImage = cvCreateImage(cvSize(InImage->width, InImage->height), 8, 3);
	OutImage = cvCreateImage(cvSize(InImage->width, InImage->height), 8, 3);
	OutImage1 = cvCreateImage(cvSize(InImage->width, InImage->height), 8, 3);
	OutImage2 = cvCreateImage(cvSize(InImage->width, InImage->height), 8, 3);
	
	cvNamedWindow("initial", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("灰階化", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("灰階化直方圖", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("sigma直方圖", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("結果", CV_WINDOW_AUTOSIZE);
	
	
	y=InImage -> width;
	x=InImage -> height;
	miny=InImage -> width;
	minx=InImage -> height;
	
	Imagelabel = (int**)malloc(x*sizeof(int *)); 
	for (i=0;i<x;i++)
	{
		Imagelabel[i]=(int*)malloc(y*sizeof(int));
	}


	//讀取圖 歸零
	for (i=0;i<x;i++)
	{
		for(j=0;j<y;j++)
		{
			Imagelabel[i][j]=0;
		}
	}



	//灰階化  +  num
	cvShowImage("initial", InImage);

	for(i=0; i<InImage->height; i++){
		for(j=0; j<InImage->width; j++){
			CvScalar x = cvGet2D(InImage, i, j);
		    int y = 0.299*x.val[2]+0.587*x.val[1]+0.114*x.val[0];
			if (i==((InImage->height)/2))
				cvSet2D(MidImage, i, j, CV_RGB(0,128,255) );
			else
				cvSet2D(MidImage, i, j, CV_RGB(y,y,y) );
			Imagelabel[i][j]=y;
			num=num+1;
			rg=rg+abs((x.val[0]-x.val[1]));
			rb=rb+abs((x.val[1]-x.val[2]));
			gb=gb+abs((x.val[0]-x.val[2]));
		}
	}
		
	
	
	for (i=0;i<256;i++)
	{
		Imagegray=(int*)malloc(num*sizeof(int));
		lo=(int*)malloc(256*sizeof(int));
	}
	for (i=0;i<256;i++)
	{
		Imagegray[i]=0;
		lo[i]=0;
	}
	
	


	cvShowImage("灰階化", MidImage);




	//ni(Imagegray)
	for(i=0; i<(MidImage->height); i++)
	{
		for(j=0; j<(MidImage->width)-1; j++)
		{
			Imagegray[Imagelabel[i][j]]=Imagegray[Imagelabel[i][j]]+1;
		}
	}
	
	
	for (threshold=1;threshold<255;threshold++)
	{
		pie0=0;
		u0=0;
		u1=0;
		ur=0;
		lot=0;
		lob=0;
		//pi=Imagegray/num

		//pie0 pie1
		for (i=1;i<=threshold;i++)
		{
			pie0=pie0+((double)Imagegray[i]/(double)num);
		}
		pie1=1-pie0;
	

		//u0 u1 ur
		for (i=1;i<=threshold;i++)
		{
			u0=u0+i*Imagegray[i]/num/pie0;
		}
		for (i=threshold+1;i<256;i++)
		{
			u1=u1+i*Imagegray[i]/num/pie1;
		}
		for (i=1;i<256;i++)
		{
			ur=ur+i*Imagegray[i]/num;
		}

		//lob lot
		lob=(double)pie0*(double)pie1*(double)pow((u1-u0),2);
	
		

		lo[threshold]=lob;
		if (lob>maxlo)
		{
			maxlo=lob;
			maxT=threshold;
		}
	}
	printf("%f\n",maxlo);
	printf("%d\n",maxT);

	//結果


	/*
	for(i=0; i<(MidImage->height); i++)
	{
		for(j=0; (j<MidImage->width); j++)
		{
			if (Imagelabel[i][j]>maxT)
			{
				cvSet2D(OutImage, i, j, CV_RGB(255,255,255) );
			}
			else
			{
				cvSet2D(OutImage, i, j, CV_RGB(0,0,0) );
			}
		}
	}
	*/


	
	for(i=0; i<(MidImage->height); i++)
	{
		for(j=0; (j<MidImage->width); j++)
		{
			CvScalar x = cvGet2D(InImage, i, j);
			if ((float)abs((x.val[0]-x.val[1]))<((rb/num)+error) && (float)abs((x.val[1]-x.val[2])<((rb/num))+error) && (float)abs((x.val[0]-x.val[2])<((gb/num)+error)))
			{
				cvSet2D(OutImage, i, j, CV_RGB(255,255,255) );
			}
			else
			{
				cvSet2D(OutImage, i, j, CV_RGB(0,0,0) );
			}
		}
	}
	
		cvShowImage("結果", OutImage);
			
	left=(OutImage->width)-1;
	right=0;
	top=(OutImage->height)-1;
	bottom=0;
	for(i=0;i<(OutImage->height);i++)
	{
		for(j=0; (j<OutImage->width); j++)
		{
			CvScalar x = cvGet2D(OutImage, i, j);
			cvSet2D(OutImage2, i, j, CV_RGB(x.val[0],x.val[0],x.val[0]) );
		}
	}
	i=((OutImage->height)/2);
		for(j=0; (j<OutImage->width); j++)
		{
			CvScalar x = cvGet2D(OutImage2, i, j);
			if (x.val[0]==255)
			{
				zzx=zzx+1;
				cvSet2D(OutImage2, i, j, CV_RGB(128+zzx,128,128) );
				pointx=j;
				pointy=i;
				mmmaxy=i;
				mmmaxx=j;
				if (j>((OutImage->width)/range))
					xx=j-((OutImage->width)/range);
				else
					xx=0;
				if(xx<minx)
					minx=xx;
				if (i>((OutImage->height)/rangetop))
					yy=i-((OutImage->height)/rangetop);
				else
					yy=0;
				if(yy<miny)
					miny=yy;
				mmaxy=(pointy+((OutImage->height)/rangetop));
				mmaxx=(pointx+((OutImage->width)/range));
				if (mmaxx>((OutImage->width)))
					mmaxx=(OutImage->width)-1;
				if (mmaxy>((OutImage->height)/rangetop))
					mmaxy=(OutImage->height)-1;
				while(yy<mmaxy)
				{
					xx=pointx-((OutImage->width)/range);
					if (xx<0)
						xx=0;/*
					if(xx<minx)
						minx=xx;
					else
						xx=minx;*/
					while(xx<mmaxx)
					{
						CvScalar x = cvGet2D(OutImage2, yy, xx);
						if (x.val[0]==255)
						{
							if (xx<left)
								left=xx;
							if(xx>right)
								right=xx;
							if(yy<top)
								top=yy;
							if(yy>bottom)
								bottom=yy;
							cvSet2D(OutImage2, yy, xx, CV_RGB(128,128,128) );
							pointx=xx;
							pointy=yy;
							xx=xx-((OutImage->width)/range);
							yy=yy-((OutImage->height)/rangetop);
							if (xx<0)
								xx=0;
							if(xx<minx)
								minx=xx;
							if (yy<0)
								yy=0;
							if(yy<miny)
								miny=yy;
							
							mmaxy=(pointy+((OutImage->height)/rangetop));
							mmaxx=(pointx+((OutImage->width)/range));
							if(mmaxy>mmmaxy)
							{
								mmmaxy=mmaxy;
							}
							else if (mmmaxy>mmaxy)
							{
								mmaxy=mmmaxy;
							}

							/*
							mmmaxy=(pointy+((OutImage->height)/rangetop));
							mmmaxx=(pointx+((OutImage->width)/range));
							
							if(mmmaxy>mmaxy)
							{
								mmaxy=mmmaxy;
								printf("pro  %d   %d\n",mmaxy,mmmaxy);
								
							}*/
							if(mmmaxx>mmaxx)
								mmaxx=mmmaxx;
							else
								mmmaxx=mmaxx;


							if (mmaxx>=((OutImage->width)))
								mmaxx=(OutImage->width)-1;
							if (mmaxy>=((OutImage->height)))
								mmaxy=(OutImage->height)-1;
							qq=qq+1;
						}
						xx=xx+1;
					}
					yy=yy+1;
				}
			}
		}

	
	cvShowImage("sigma直方圖", OutImage2);

	
	saveerrorw=(right-left)/10;
	saveerrorh=(bottom-top)/20;
	//saveerrorw=0;
	//saveerrorh=0;
	int qnn,qqnn;
	//qnn=
	OutImage4 = cvCreateImage(cvSize((right-left+1+saveerrorw+saveerrorw), (bottom-top+1+saveerrorh+saveerrorh)), 8, 3);
	OutImage5 = cvCreateImage(cvSize((right-left+1+saveerrorw+saveerrorw), (bottom-top+1+saveerrorh+saveerrorh)), 8, 3);
	for(i=0; i<InImage->height; i++){
		for(j=0; j<InImage->width; j++){
			CvScalar x = cvGet2D(InImage, i, j);
			CvScalar y = cvGet2D(OutImage, i, j);
			if (i>=(top-saveerrorh) && i<=(bottom+saveerrorh) && j>=(left-saveerrorw) && j<=(right+saveerrorw))
			{
				cvSet2D(OutImage1, i, j, CV_RGB(x.val[0],x.val[1],x.val[2]) );
				cvSet2D(OutImage4, i-top+saveerrorh, j-left+saveerrorw, CV_RGB(x.val[2],x.val[1],x.val[0]) );
				cvSet2D(OutImage5, i-top+saveerrorh, j-left+saveerrorw, CV_RGB(y.val[2],y.val[1],y.val[0]) );
			}
			else
				cvSet2D(OutImage1, i, j, CV_RGB(128,128,255) );
		}
	}





	strcpy(InputFileNameblack,InputFileName);
	cvShowImage("灰階化直方圖", OutImage1);
	printf("gogo");
	strcat(InputFileName,"num.jpg");
	cvSaveImage(InputFileName,OutImage4);
	strcat(InputFileNameblack,"blacknum.jpg");
	cvSaveImage(InputFileNameblack,OutImage5);
	while(1)
	{
		char c;
		c = cvWaitKey(1);
		if(c == 'q') break;
	}
	cvReleaseImage(&InImage);
	cvReleaseImage(&MidImage);
	cvReleaseImage(&OutImage);
	cvReleaseImage(&OutImage1);
	cvReleaseImage(&OutImage2);
	cvDestroyWindow("initial");    // 關閉視窗
	cvDestroyWindow("灰階化");
	cvDestroyWindow("灰階化直方圖");
	cvDestroyWindow("sigma直方圖");
	cvDestroyWindow("結果");
	
	for (i=0;i<x;i++)
	{
		free(Imagelabel[i]);
	}
	
	free(Imagelabel);
	

	
 Rect bounding_rect;
 
 printf("%s\n",InputFileNameblack);

 Mat src = imread(InputFileNameblack); //Load source image
 IplImage* img;
 img = &IplImage(src);
 Mat src1 = imread(InputFileName);
 IplImage* colorimg;
 colorimg = &IplImage(src1);
 Mat thr(src.rows,src.cols,CV_8UC1); 
 Mat dst(src.rows,src.cols,CV_8UC1,Scalar::all(0));
 cvtColor(src,thr,CV_BGR2GRAY); //Convert to gray
 //threshold(thr, thr, 100 , 255,THRESH_BINARY); //Threshold the gray
 
  //saveerrorh=(bottom-top)/20;
  saveerrorh=3;
  Mat erodeStruct = getStructuringElement(MORPH_RECT,Size(saveerrorh,saveerrorh));
  erode(thr,thr,erodeStruct);
  Mat dilateStruct = getStructuringElement(MORPH_RECT,Size(saveerrorh,saveerrorh));
  dilate(thr,thr,dilateStruct);
  //saveerrorh=(bottom-top)/6;
  saveerrorh=11;
  Mat dilate2Struct = getStructuringElement(MORPH_RECT,Size(saveerrorh,saveerrorh));
  dilate(thr,thr,dilate2Struct);
 imshow( "Output", thr );

  vector<vector<Point>> contours; // Vector for storing contour
  vector<Vec4i> hierarchy;
 
  findContours( thr, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); // Find the contours in the image
  
	saveerrorw=(right-left)/10;
	saveerrorh=(bottom-top)/5;
    int b=0;
     for( int i = 0; i< contours.size(); i++ ) // iterate through each contour. 
      {
       double a=contourArea( contours[i],false);  //  Find the area of contour
       bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour 
	   if(bounding_rect.height>saveerrorh && bounding_rect.width>saveerrorw)
	   {
		 rectangle(src, bounding_rect, Scalar(0,255,0),1, 8,0);  
	     cvSetImageROI(colorimg,cvRect(bounding_rect.x,bounding_rect.y,bounding_rect.width,bounding_rect.height));
		 if(b==0){strcpy(InputFileNameout,InputFileName);strcat(InputFileNameout,"01.jpg");cvShowImage(InputFileNameout,colorimg); cvSaveImage( InputFileNameout, colorimg );}
		 if(b==1){strcpy(InputFileNameout,InputFileName);strcat(InputFileNameout,"02.jpg");cvShowImage(InputFileNameout,colorimg); cvSaveImage( InputFileNameout, colorimg );}
		 if(b==2){strcpy(InputFileNameout,InputFileName);strcat(InputFileNameout,"03.jpg");cvShowImage(InputFileNameout,colorimg); cvSaveImage( InputFileNameout, colorimg );}
		 if(b==3){strcpy(InputFileNameout,InputFileName);strcat(InputFileNameout,"04.jpg");cvShowImage(InputFileNameout,colorimg); cvSaveImage( InputFileNameout, colorimg );}
		 if(b==4){strcpy(InputFileNameout,InputFileName);strcat(InputFileNameout,"05.jpg");cvShowImage(InputFileNameout,colorimg); cvSaveImage( InputFileNameout, colorimg );}
		 if(b==5){strcpy(InputFileNameout,InputFileName);strcat(InputFileNameout,"06.jpg");cvShowImage(InputFileNameout,colorimg); cvSaveImage( InputFileNameout, colorimg );}
		 if(b==6){strcpy(InputFileNameout,InputFileName);strcat(InputFileNameout,"07.jpg");cvShowImage(InputFileNameout,colorimg); cvSaveImage( InputFileNameout, colorimg );}
		 if(b==7){strcpy(InputFileNameout,InputFileName);strcat(InputFileNameout,"08.jpg");cvShowImage(InputFileNameout,colorimg); cvSaveImage( InputFileNameout, colorimg );}
		 if(b==8){strcpy(InputFileNameout,InputFileName);strcat(InputFileNameout,"09.jpg");cvShowImage(InputFileNameout,colorimg); cvSaveImage( InputFileNameout, colorimg );}
		 if(b==9){strcpy(InputFileNameout,InputFileName);strcat(InputFileNameout,"10.jpg");cvShowImage(InputFileNameout,colorimg); cvSaveImage( InputFileNameout, colorimg );}
		 b++;
	   }
	  }
 imshow( "Output00", src );
 waitKey(0);
	return 0;
}
