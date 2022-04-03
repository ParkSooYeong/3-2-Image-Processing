/* SKU CoE ITE - ParkSooYoung , 2021-02 ImageProcessing : Histogram */

#define _CRT_SECURE_NO_WARNINGS // Warning C4996 Error

//전처리기 파트
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define CV_RGB(r, g, b) cvScalar(b, g, r) // OpenCV 내장 함수 작업에 필요한 정의

typedef unsigned char uchar; // unsigned char 타입을 uchar로 재정의

// 히스토그램 작업에 필요한 전역 배열 및 변수 선언
int histogram[256], cdfOfHisto[256], histogramEqual[256], tmpCDF[256], val=0, mytemp;

// 메모리 할당 함수
uchar** uc_alloc(int size_x, int size_y)
{
    uchar** m;
    int i;

    if ((m = (uchar**)calloc(size_y, sizeof(uchar*))) == NULL)
    {
        printf("d_alloc error 1\7\n");
        exit(0);
    }

    for (i = 0; i < size_y; i++)
    {
        if ((m[i] = (uchar*)calloc(size_x, sizeof(uchar))) == NULL)
        {
            printf("d_alloc error 2\7\n");
            exit(0);
        }
    }

    return m;
}

// 영상 읽는 함수
void read_ucmatrix(int size_x, int size_y, uchar** ucmatrix, const char* filename)
{
    int i;
    FILE* f;

    if ((f = fopen(filename, "rb")) == NULL)
    {
        printf("%s File open Error!\n", filename);
        exit(0);
    }

    for (i = 0; i < size_y; i++)
    {
        if (fread(ucmatrix[i], sizeof(uchar), size_x, f) != size_x)
        {
            printf("Data Read Error!\n");
            exit(0);
        }
    }

    fclose(f);
}

// 영상 쓰는 함수
void write_ucmatrix(int size_x, int size_y, uchar** ucmatrix, const char* filename)
{
    int i;
    FILE* f;

    if ((f = fopen(filename, "wb")) == NULL)
    {
        printf("%s File open Error!\n", filename);
        exit(0);
    }

    for (i = 0; i < size_y; i++)
    {
        if (fwrite(ucmatrix[i], sizeof(uchar), size_x, f) != size_x)
        {
            printf("Data Write Error!\n");
            exit(0);
        }
    }

    fclose(f);
}

// 히스토그램 함수 , 이미지 opencv로 출력
void get_hist(uchar** img, int X_Size, int Y_Size)
{
	int i, j, tmp;
	double tmp1;
	int t, tp, range, hrange;
	CvSize histoSize, cdfSize;
	IplImage* imgHisto, * cdfImgHisto;

	// 생성할 이미지 사이즈(원본 이미지, 누적분포 후의 이미지)
	histoSize.width = 256;
	histoSize.height = 256;

	cdfSize.width = 256;
	cdfSize.height = 256;

	imgHisto = cvCreateImage(histoSize, 8, 1);
	cdfImgHisto = cvCreateImage(cdfSize, 8, 1);

	// 이미지 초기화
	for (i = 0; i < histoSize.height; i++)
	{
		for (j = 0; j < histoSize.width; j++)
		{
			((uchar*)(imgHisto->imageData + cdfImgHisto->widthStep * i))[j] = 0;
		}
	}

	for (i = 0; i < cdfSize.height; i++)
	{
		for (j = 0; j < cdfSize.width; j++)
		{
			((uchar*)(cdfImgHisto->imageData + cdfImgHisto->widthStep * i))[j] = 0;
		}
	}

	tp = X_Size * Y_Size;

	// 히스토그램 배열 초기화
	for (i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}

	// img[i][j]가 갖고 있는 그레이 레벨에 해당하는 히스토그램 값을 ++함
	for (i = 0; i < Y_Size; i++)
	{
		for (j = 0; j < X_Size; j++)
		{
			histogram[img[i][j]]++;
		}
	}

	// Find the maximum histogram value 히스토그램 최댓값 찾기(정규화를 위해)
	tmp1 = 0;

	for (i = 0; i < 256; ++i)
	{
		tmp1 = tmp1 > histogram[i] ? tmp1 : histogram[i];
	}

	for (i = 0; i < 256; ++i)
	{
		tmp = (int)255 * (histogram[i] / tmp1);
		cvLine(imgHisto, cvPoint(i, 255), cvPoint(i, 255 - tmp), CV_RGB(255, 255, 255), 1, 8, 0);
	}

	// 모든 히스토그램을 더하여 CDF 만들기
	cvShowImage("Histo Line ", imgHisto);
	cdfOfHisto[0] = histogram[0];

	for (i = 1; i < 256; i++)
	{
		cdfOfHisto[i] = cdfOfHisto[i - 1] + histogram[i];
	}

	// Draw the CDF of Histogram
	// 최대 히스토그램 값을(중첩된 마지막 값) tmp로 지정
	tmp1 = (double)cdfOfHisto[255];

	for (i = 0; i < 256; ++i)
	{
		tmp = (int)255 * (cdfOfHisto[i] / tmp1);
		cvLine(cdfImgHisto, cvPoint(i, 255), cvPoint(i, 255 - tmp), CV_RGB(255, 255, 255), 1, 8, 0);
	}

	cvShowImage("Original CDF of Histogram ", cdfImgHisto);
	range = cdfOfHisto[255] - cdfOfHisto[0];
	histogramEqual[0] = 0;

	// 정규화
	for (i = 0; i < 256; ++i)
	{
		t = (int)ceil(((cdfOfHisto[i] - cdfOfHisto[0]) * 255.0) / range);
		histogramEqual[i] = (t < 0) ? 0 : (t > 255) ? 255 : t;
	}

	cvReleaseImage(&imgHisto); // 메모리 해제

	for (i = 0; i < Y_Size; ++i)
	{
		for (j = 0; j < X_Size; ++j)
		{
			img[i][j] = histogramEqual[img[i][j]];
		}
	}
}

// 히스토그램 이퀄라이제이션 함수 , 이미지 opencv로 출력
void get_hist1(uchar** img, int X_Size, int Y_Size, int mod)
{
	int i, j, tmp;
	double tmp1;
	int t, tp, range, hrange;
	CvSize histoSize, cdfSize;
	IplImage* imgHisto, * cdfImgHisto;

	// 생성할 이미지 사이즈(원본 이미지, 누적분포 후의 이미지)
	histoSize.width = 256;
	histoSize.height = 256;

	cdfSize.width = 256;
	cdfSize.height = 256;

	imgHisto = cvCreateImage(histoSize, 8, 1);
	cdfImgHisto = cvCreateImage(cdfSize, 8, 1);

	// 이미지 초기화
	for (i = 0; i < histoSize.height; i++)
	{
		for (j = 0; j < histoSize.width; j++)
		{
			((uchar*)(imgHisto->imageData + imgHisto->widthStep * i))[j] = 0;
		}
	}

	for (i = 0; i < cdfSize.height; i++)
	{
		for (j = 0; j < cdfSize.width; j++)
		{
			((uchar*)(cdfImgHisto->imageData + cdfImgHisto->widthStep * i))[j] = 0;
		}
	}

	tp = X_Size * Y_Size;

	// 히스토그램 배열 초기화
	for (i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}

	// img[i][j]가 갖고 있는 그레이 레벨에 해당하는 히스토그램 값을 ++함
	for (i = 0; i < Y_Size; i++)
	{
		for (j = 0; j < X_Size; j++)
		{
			histogram[img[i][j]]++;
		}
	}

	// Find the maximum histogram value 히스토그램 최댓값 찾기(정규화를 위해)
	tmp1 = 0;

	for (i = 0; i < 256; ++i)
	{
		tmp1 = tmp1 > histogram[i] ? tmp1 : histogram[i];
	}

	// 최대 값에 비례해서 정규화 (0~255)
	printf("\n\nHisto %d \n\n\n", mod);

	for (i = 0; i < 256; ++i)
	{
		tmp = (int)255 * (histogram[i] / tmp1);
		cvLine(imgHisto, cvPoint(i, 255), cvPoint(i, 255 - tmp), CV_RGB(255, 255, 255), 1, 8, 0);
	}

	switch (mod)
	{
		case 0:
			cvShowImage("Original Histogram", imgHisto);
			break;
		case 1:
			cvShowImage("Target Histogram", imgHisto);
			break;
		case 2:
			cvShowImage("Result Histogram", imgHisto);
			break;
	}

	// 모든 히스토그램을 더하여 CDF 만들기
	cdfOfHisto[0] = histogram[0];
	printf("\n\ncdfHisto %d \n\n\n", mod);

	for (i = 1; i < 256; i++)
	{
		cdfOfHisto[i] = cdfOfHisto[i - 1] + histogram[i];
		printf("%d, ", cdfOfHisto[i]);
	}

	// Draw the CDF of Histogram
	// 최대 히스토그램 값을(중첩된 마지막 값) tmp로 지정
	tmp1 = (double)cdfOfHisto[255];

	// 히스토그램 정규화(0~255) 및 출력
	printf("\n\ncdfHisto Temp%d \n\n\n", mod);

	for (i = 0; i < 256; ++i)
	{
		tmp = (int)255 * (cdfOfHisto[i] / tmp1);
		printf("%d ,", tmp);
		cvLine(cdfImgHisto, cvPoint(i, 255), cvPoint(i, 256 - tmp), CV_RGB(255, 255, 255), 1, 8, 0);
		tmpCDF[i] = tmp;
	}

	switch (mod)
	{
		case 0:
			cvShowImage("Original CDF of Histogram", cdfImgHisto);
			break;
		case 1:
			cvShowImage("Target CDF of Histogram", cdfImgHisto);
			break;
		case 2:
			cvShowImage("Result CDF of Histogram", cdfImgHisto);
			break;
	}

	range = cdfOfHisto[255] - cdfOfHisto[0];
	// printf("%d" %d \n", tp, range);
	// range == 히스토그램의 범위 

	printf("\n\nHistoEqual %d \n\n\n", mod);
	histogramEqual[0] = 0;

	// 정규화
	for (i = 0; i < 256; ++i)
	{
		t = (int)ceil(((cdfOfHisto[i] - cdfOfHisto[0]) * 255.0) / range);
		histogramEqual[i] = (t < 0) ? 0 : (t > 255) ? 255 : t;
		printf("%d, ", histogramEqual[i]);
	}

	// 히스토그램의 평활화 작업
	for (i = 0; i < 256; i++)
	{
		if (histogramEqual[i] > 127)
		{
			val = i;
			break;
		}
	}

	//if(mod==0) cvShowImage("역변환", rvsHisto);

	cvReleaseImage(&imgHisto);	  // 메모리 해제
	cvReleaseImage(&cdfImgHisto); // 메모리 해제

	//mod 0일 때 cdf를 역변환하여 대입 평활화 수행(오리지날 이미지)
	//대입하는 것 자체가 역변환 하는 동작을 수행함
	if (mod == 0)
	{
		for (i = 0; i < Y_Size; ++i)
		{
			for (j = 0; j < X_Size; ++j)
			{
				img[i][j] = histogramEqual[img[i][j]];
			}
		}
	}
}

// 히스토그램 스페시피케이션 함수 , 이미지 opencv로 출력
void get_Match(uchar** img, int X_Size, int Y_Size, int histogramSpec[256])
{
	int i, j, tmp, matchz = 0;
	int histogramMatch[256];
	float diff;

	CvSize matchSize;
	IplImage* matchImg;

	// 생성할 이미지 사이즈(매칭 후의 이미지)
	matchSize.width = 256;
	matchSize.height = 256;
	
	matchImg = cvCreateImage(matchSize, 8, 1);

	// 이미지 초기화
	for (i = 0; i < matchSize.height; i++)
	{
		for (j = 0; j < matchSize.width; j++)
		{
			((uchar*)(matchImg->imageData + matchImg->widthStep * i))[j] = 0;
		}
	}

	printf("Start HistoGram Specification \n");

	// 지정영상의 CDF를 다시 역변환하는 과정
	for (i = 0; i < 256; i++)
	{
		histogramMatch[i] = 0;

		for (j = 0; j < 256; j++)
		{
			if ((i - histogramSpec[j]) > 0)
			{
				histogramMatch[i] = j;
			}
		}
	}

	for (i = 0; i < 256; ++i)
	{
		cvLine(matchImg, cvPoint(i, 255), cvPoint(i, 255 - histogramMatch[i]), CV_RGB(255, 255, 255), 1, 8, 0);
	}

	cvShowImage("역변환된 히스토그램", matchImg);

	//역변환하며 이미지 대응
	for (i = 0; i < Y_Size; ++i)
	{
		for (j = 0; j < X_Size; ++j)
		{
			img[i][j] = histogramMatch[img[i][j]];
		}
	}
}

// 메인 함수
int main(int argc, char* argv[])
{
	int i, j;
	IplImage* cvImg, * cvImg2;
	CvSize imgSize, imgSize2;
	uchar** img, ** img2, ** outimg;

	if (argc != 8)
	{
		printf("Exe imgData x_size y_size (-2 or -1 or 0) target x_size y_size \n");
		exit(0);
	}

	imgSize.width = atoi(argv[2]);
	imgSize.height = atoi(argv[3]);
	img = uc_alloc(imgSize.width, imgSize.height);
	read_ucmatrix(imgSize.width, imgSize.height, img, argv[1]);

	// 히스토그램
	if (atoi(argv[4]) == -2)
	{
		cvImg = cvCreateImage(imgSize, 8, 1);	

		for (i = 0; i < imgSize.height; i++)
		{
			for (j = 0; j < imgSize.width; j++)
			{
				((uchar*)(cvImg->imageData + cvImg->widthStep * i))[j] = img[i][j];
			}
		}

		cvNamedWindow(argv[1], 1);	 // 윈도우 열기	
		cvShowImage(argv[1], cvImg); // 이미지 열기

		get_hist(img, imgSize.width, imgSize.height);
	}

	// 히스토그램 평활화
	else if (atoi(argv[4]) == -1)
	{
		printf("equal\n		");
		cvImg = cvCreateImage(imgSize, 8, 1);

		for (i = 0; i < imgSize.height; i++)
		{
			for (j = 0; j < imgSize.width; j++)
			{
				((uchar*)(cvImg->imageData + cvImg->widthStep * i))[j] = img[i][j];
			}
		}

		// 원본 히스토그램 출력 및 평활화
		get_hist1(img, imgSize.width, imgSize.height, 0);

		cvNamedWindow(argv[1], 1);	 // 윈도우 열기
		cvShowImage(argv[1], cvImg); // 이미지 열기

		for (i = 0; i < imgSize.height; i++)
		{
			for (j = 0; j < imgSize.width; j++)
			{
				((uchar*)(cvImg->imageData + cvImg->widthStep * i))[j] = img[i][j];
			}
		}

		cvShowImage("histoequals...", cvImg); // 이미지 열기

		get_hist1(img, imgSize.width, imgSize.height, 2);
	}

	// 히스토그램 매칭
	else if (atoi(argv[4]) == 0)
	{
		printf("else...\n");

		imgSize2.width = atoi(argv[6]);
		imgSize2.height = atoi(argv[7]);
		img2 = uc_alloc(imgSize2.width, imgSize2.height);
		read_ucmatrix(imgSize2.width, imgSize2.height, img2, argv[5]);

		outimg = uc_alloc(imgSize.width, imgSize.height);

		cvImg = cvCreateImage(imgSize, 8, 1);

		// constrastStreching(img, outimg, imgSize.width, imgSize.height); // 이런 식으로..

		for (i = 0; i < imgSize.height; i++)
		{
			for (j = 0; j < imgSize.width; j++)
			{
				((uchar*)(cvImg->imageData + cvImg->widthStep * i))[j] = img[i][j];
			}
		}

		// 원본 히스토그램 출력 및 평활화
		get_hist1(img, imgSize.width, imgSize.height, 0);

		cvNamedWindow(argv[1], 1);	 //윈도우 열기
		cvShowImage(argv[1], cvImg); //이미지 열기

		cvImg2 = cvCreateImage(imgSize2, 8, 1);	// 이미지 생성2

		// 타겟 이미지 입력
		for (i = 0; i < imgSize2.height; i++)
		{
			for (j = 0; j < imgSize2.width; j++)
			{
				((uchar*)(cvImg2->imageData + cvImg2->widthStep * i))[j] = img2[i][j];
			}
		}

		// 타겟 이미지 출력
		cvShowImage(argv[5], cvImg2);

		// 타겟 히스토그램 출력
		get_hist1(img2, imgSize2.width, imgSize2.height, 1);

		// 역변환된 타겟 히스토그램 cdf를 평활화된 원본 이미지에 대입
		// get_Match(img, imgSize.width, imgSize.height, histogramEqual);
		get_Match(img, imgSize.width, imgSize.height, tmpCDF);

		for (i = 0; i < imgSize.height; i++)
		{
			for (j = 0; j < imgSize.width; j++)
			{
				((uchar*)(cvImg->imageData + cvImg->widthStep * i))[j] = img[i][j];
			}
		}

		cvShowImage("Histogram match...", cvImg); // 이미지 열기

		get_hist1(img, imgSize.width, imgSize.height, 2);
	}

	cvWaitKey(0);
	cvDestroyWindow(argv[1]);
	cvReleaseImage(&cvImg);

	return 0;
}
