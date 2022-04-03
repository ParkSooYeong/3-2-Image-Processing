/* SKU CoE ITE - ParkSooYoung , 2021-02 ImageProcessing : Histogram */

#define _CRT_SECURE_NO_WARNINGS // Warning C4996 Error

//��ó���� ��Ʈ
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define CV_RGB(r, g, b) cvScalar(b, g, r) // OpenCV ���� �Լ� �۾��� �ʿ��� ����

typedef unsigned char uchar; // unsigned char Ÿ���� uchar�� ������

// ������׷� �۾��� �ʿ��� ���� �迭 �� ���� ����
int histogram[256], cdfOfHisto[256], histogramEqual[256], tmpCDF[256], val=0, mytemp;

// �޸� �Ҵ� �Լ�
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

// ���� �д� �Լ�
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

// ���� ���� �Լ�
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

// ������׷� �Լ� , �̹��� opencv�� ���
void get_hist(uchar** img, int X_Size, int Y_Size)
{
	int i, j, tmp;
	double tmp1;
	int t, tp, range, hrange;
	CvSize histoSize, cdfSize;
	IplImage* imgHisto, * cdfImgHisto;

	// ������ �̹��� ������(���� �̹���, �������� ���� �̹���)
	histoSize.width = 256;
	histoSize.height = 256;

	cdfSize.width = 256;
	cdfSize.height = 256;

	imgHisto = cvCreateImage(histoSize, 8, 1);
	cdfImgHisto = cvCreateImage(cdfSize, 8, 1);

	// �̹��� �ʱ�ȭ
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

	// ������׷� �迭 �ʱ�ȭ
	for (i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}

	// img[i][j]�� ���� �ִ� �׷��� ������ �ش��ϴ� ������׷� ���� ++��
	for (i = 0; i < Y_Size; i++)
	{
		for (j = 0; j < X_Size; j++)
		{
			histogram[img[i][j]]++;
		}
	}

	// Find the maximum histogram value ������׷� �ִ� ã��(����ȭ�� ����)
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

	// ��� ������׷��� ���Ͽ� CDF �����
	cvShowImage("Histo Line ", imgHisto);
	cdfOfHisto[0] = histogram[0];

	for (i = 1; i < 256; i++)
	{
		cdfOfHisto[i] = cdfOfHisto[i - 1] + histogram[i];
	}

	// Draw the CDF of Histogram
	// �ִ� ������׷� ����(��ø�� ������ ��) tmp�� ����
	tmp1 = (double)cdfOfHisto[255];

	for (i = 0; i < 256; ++i)
	{
		tmp = (int)255 * (cdfOfHisto[i] / tmp1);
		cvLine(cdfImgHisto, cvPoint(i, 255), cvPoint(i, 255 - tmp), CV_RGB(255, 255, 255), 1, 8, 0);
	}

	cvShowImage("Original CDF of Histogram ", cdfImgHisto);
	range = cdfOfHisto[255] - cdfOfHisto[0];
	histogramEqual[0] = 0;

	// ����ȭ
	for (i = 0; i < 256; ++i)
	{
		t = (int)ceil(((cdfOfHisto[i] - cdfOfHisto[0]) * 255.0) / range);
		histogramEqual[i] = (t < 0) ? 0 : (t > 255) ? 255 : t;
	}

	cvReleaseImage(&imgHisto); // �޸� ����

	for (i = 0; i < Y_Size; ++i)
	{
		for (j = 0; j < X_Size; ++j)
		{
			img[i][j] = histogramEqual[img[i][j]];
		}
	}
}

// ������׷� �����������̼� �Լ� , �̹��� opencv�� ���
void get_hist1(uchar** img, int X_Size, int Y_Size, int mod)
{
	int i, j, tmp;
	double tmp1;
	int t, tp, range, hrange;
	CvSize histoSize, cdfSize;
	IplImage* imgHisto, * cdfImgHisto;

	// ������ �̹��� ������(���� �̹���, �������� ���� �̹���)
	histoSize.width = 256;
	histoSize.height = 256;

	cdfSize.width = 256;
	cdfSize.height = 256;

	imgHisto = cvCreateImage(histoSize, 8, 1);
	cdfImgHisto = cvCreateImage(cdfSize, 8, 1);

	// �̹��� �ʱ�ȭ
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

	// ������׷� �迭 �ʱ�ȭ
	for (i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}

	// img[i][j]�� ���� �ִ� �׷��� ������ �ش��ϴ� ������׷� ���� ++��
	for (i = 0; i < Y_Size; i++)
	{
		for (j = 0; j < X_Size; j++)
		{
			histogram[img[i][j]]++;
		}
	}

	// Find the maximum histogram value ������׷� �ִ� ã��(����ȭ�� ����)
	tmp1 = 0;

	for (i = 0; i < 256; ++i)
	{
		tmp1 = tmp1 > histogram[i] ? tmp1 : histogram[i];
	}

	// �ִ� ���� ����ؼ� ����ȭ (0~255)
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

	// ��� ������׷��� ���Ͽ� CDF �����
	cdfOfHisto[0] = histogram[0];
	printf("\n\ncdfHisto %d \n\n\n", mod);

	for (i = 1; i < 256; i++)
	{
		cdfOfHisto[i] = cdfOfHisto[i - 1] + histogram[i];
		printf("%d, ", cdfOfHisto[i]);
	}

	// Draw the CDF of Histogram
	// �ִ� ������׷� ����(��ø�� ������ ��) tmp�� ����
	tmp1 = (double)cdfOfHisto[255];

	// ������׷� ����ȭ(0~255) �� ���
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
	// range == ������׷��� ���� 

	printf("\n\nHistoEqual %d \n\n\n", mod);
	histogramEqual[0] = 0;

	// ����ȭ
	for (i = 0; i < 256; ++i)
	{
		t = (int)ceil(((cdfOfHisto[i] - cdfOfHisto[0]) * 255.0) / range);
		histogramEqual[i] = (t < 0) ? 0 : (t > 255) ? 255 : t;
		printf("%d, ", histogramEqual[i]);
	}

	// ������׷��� ��Ȱȭ �۾�
	for (i = 0; i < 256; i++)
	{
		if (histogramEqual[i] > 127)
		{
			val = i;
			break;
		}
	}

	//if(mod==0) cvShowImage("����ȯ", rvsHisto);

	cvReleaseImage(&imgHisto);	  // �޸� ����
	cvReleaseImage(&cdfImgHisto); // �޸� ����

	//mod 0�� �� cdf�� ����ȯ�Ͽ� ���� ��Ȱȭ ����(�������� �̹���)
	//�����ϴ� �� ��ü�� ����ȯ �ϴ� ������ ������
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

// ������׷� ����������̼� �Լ� , �̹��� opencv�� ���
void get_Match(uchar** img, int X_Size, int Y_Size, int histogramSpec[256])
{
	int i, j, tmp, matchz = 0;
	int histogramMatch[256];
	float diff;

	CvSize matchSize;
	IplImage* matchImg;

	// ������ �̹��� ������(��Ī ���� �̹���)
	matchSize.width = 256;
	matchSize.height = 256;
	
	matchImg = cvCreateImage(matchSize, 8, 1);

	// �̹��� �ʱ�ȭ
	for (i = 0; i < matchSize.height; i++)
	{
		for (j = 0; j < matchSize.width; j++)
		{
			((uchar*)(matchImg->imageData + matchImg->widthStep * i))[j] = 0;
		}
	}

	printf("Start HistoGram Specification \n");

	// ���������� CDF�� �ٽ� ����ȯ�ϴ� ����
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

	cvShowImage("����ȯ�� ������׷�", matchImg);

	//����ȯ�ϸ� �̹��� ����
	for (i = 0; i < Y_Size; ++i)
	{
		for (j = 0; j < X_Size; ++j)
		{
			img[i][j] = histogramMatch[img[i][j]];
		}
	}
}

// ���� �Լ�
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

	// ������׷�
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

		cvNamedWindow(argv[1], 1);	 // ������ ����	
		cvShowImage(argv[1], cvImg); // �̹��� ����

		get_hist(img, imgSize.width, imgSize.height);
	}

	// ������׷� ��Ȱȭ
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

		// ���� ������׷� ��� �� ��Ȱȭ
		get_hist1(img, imgSize.width, imgSize.height, 0);

		cvNamedWindow(argv[1], 1);	 // ������ ����
		cvShowImage(argv[1], cvImg); // �̹��� ����

		for (i = 0; i < imgSize.height; i++)
		{
			for (j = 0; j < imgSize.width; j++)
			{
				((uchar*)(cvImg->imageData + cvImg->widthStep * i))[j] = img[i][j];
			}
		}

		cvShowImage("histoequals...", cvImg); // �̹��� ����

		get_hist1(img, imgSize.width, imgSize.height, 2);
	}

	// ������׷� ��Ī
	else if (atoi(argv[4]) == 0)
	{
		printf("else...\n");

		imgSize2.width = atoi(argv[6]);
		imgSize2.height = atoi(argv[7]);
		img2 = uc_alloc(imgSize2.width, imgSize2.height);
		read_ucmatrix(imgSize2.width, imgSize2.height, img2, argv[5]);

		outimg = uc_alloc(imgSize.width, imgSize.height);

		cvImg = cvCreateImage(imgSize, 8, 1);

		// constrastStreching(img, outimg, imgSize.width, imgSize.height); // �̷� ������..

		for (i = 0; i < imgSize.height; i++)
		{
			for (j = 0; j < imgSize.width; j++)
			{
				((uchar*)(cvImg->imageData + cvImg->widthStep * i))[j] = img[i][j];
			}
		}

		// ���� ������׷� ��� �� ��Ȱȭ
		get_hist1(img, imgSize.width, imgSize.height, 0);

		cvNamedWindow(argv[1], 1);	 //������ ����
		cvShowImage(argv[1], cvImg); //�̹��� ����

		cvImg2 = cvCreateImage(imgSize2, 8, 1);	// �̹��� ����2

		// Ÿ�� �̹��� �Է�
		for (i = 0; i < imgSize2.height; i++)
		{
			for (j = 0; j < imgSize2.width; j++)
			{
				((uchar*)(cvImg2->imageData + cvImg2->widthStep * i))[j] = img2[i][j];
			}
		}

		// Ÿ�� �̹��� ���
		cvShowImage(argv[5], cvImg2);

		// Ÿ�� ������׷� ���
		get_hist1(img2, imgSize2.width, imgSize2.height, 1);

		// ����ȯ�� Ÿ�� ������׷� cdf�� ��Ȱȭ�� ���� �̹����� ����
		// get_Match(img, imgSize.width, imgSize.height, histogramEqual);
		get_Match(img, imgSize.width, imgSize.height, tmpCDF);

		for (i = 0; i < imgSize.height; i++)
		{
			for (j = 0; j < imgSize.width; j++)
			{
				((uchar*)(cvImg->imageData + cvImg->widthStep * i))[j] = img[i][j];
			}
		}

		cvShowImage("Histogram match...", cvImg); // �̹��� ����

		get_hist1(img, imgSize.width, imgSize.height, 2);
	}

	cvWaitKey(0);
	cvDestroyWindow(argv[1]);
	cvReleaseImage(&cvImg);

	return 0;
}
