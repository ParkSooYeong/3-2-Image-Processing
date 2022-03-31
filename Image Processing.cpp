/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 0 , Number 0 */

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

typedef unsigned char uchar; // unsigned char 타입을 uchar로 재정의

using namespace std; // 표준 네임스페이스 사용
using namespace cv; // OpenCV 네임스페이스 사용

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

// 기본 함수
void Dispcv(uchar** img, uchar** Result, int Row, int Col)
{
    int i, j;

    for (i = 0; i < Row; i++)
    {
        for (j = 0; j < Col; j++)
        {
            Result[i][j] = img[i][j];
        }
    }
}

// 반전 효과 함수
void Negative(uchar** img, uchar** Result, int Row, int Col)
{
    int i, j;

    for (i = 0; i < Row; i++)
    {
        for (j = 0; j < Col; j++)
        {
            Result[i][j] = 255 - img[i][j];
        }
    }
}

// 모자이크 함수
void Mosaic(uchar** img, uchar** Result, int Row, int Col, int Block)
{
    int i, j, x, y, tmp, count;

    for (i = 0; i < Row; i += Block)
    {
        for (j = 0; j < Col; j += Block)
        {
            tmp = 0;
            count = 0;

            for (y = 0; y < Block; y++)
            {
                for (x = 0; x < Block; x++)
                {
                    tmp += img[i + y][j + x];
                    count++;
                }
            } //get average

            tmp /= count;

            for (y = 0; y < Block; y++)
            {
                for (x = 0; x < Block; x++)
                {
                    Result[i + y][j + x] = tmp;
                }
            }
        }
    }
}

// 바이너리 평균 함수
double Average(uchar** img, int Row, int Col)
{
    double sum = 0, Avg;
    int i, j;

    for (i = 0; i < Row; i++)
    {
        for (j = 0; j < Col; j++)
        {
            sum += img[i][j];
        }
    }

    Avg = sum / ((double)Row * Col);
    printf("\n    Average of Image : %lf \n", Avg);
    return Avg;
}

// 바이너리 함수
void MakeBinary(uchar** img, uchar** Result, int Row, int Col, double Avg)
{
    int i, j;

    for (i = 0; i < Row; i++)
    {
        for (j = 0; j < Col; j++)
        {
            if (img[i][j] > Avg)
            {
                Result[i][j] = 255; // Change value for book with avg-30
            }
            else
            {
                Result[i][j] = 0;
            }
        }
    }
}

// 바이너리 밝기 함수 2
void AdaptiveBinary2(uchar** img, uchar** Result, int Row, int Col)
{
    int i, j;

    for (i = 0; i < Row; i++) {
        for (j = 0; j < Col; j++) {
            if (img[i][j] > 50 && img[i][j] < 120) Result[i][j] = img[i][j];
            else Result[i][j] = 0;
        }
    }
}

// 바이너리 밝기 함수 1
void AdaptiveBinary1(uchar** img, uchar** Result, int Row, int Col)
{
    int i, j;

    for (i = 0; i < Row; i++) {
        for (j = 0; j < Col; j++) {
            if (img[i][j] > 50 && img[i][j] < 120) Result[i][j] = 200;
            else Result[i][j] = img[i][j];
        }
    }
}

// 바이너리 밝기 함수 0
void AdaptiveBinary0(uchar** img, uchar** Result, int Row, int Col)
{
    int i, j;

    for (i = 0; i < Row; i++) {
        for (j = 0; j < Col; j++) {
            if (img[i][j] > 50 && img[i][j] < 120) Result[i][j] = 200;
            else Result[i][j] = 0;
        }
    }
}

// 감마 보정 함수
void PowImg(uchar** img, uchar** Result, int Row, int Col, double Gamma)
{
    int i, j;
    double tmp;

    for (i = 0; i < Row; i++)
    {
        for (j = 0; j < Col; j++)
        {
            tmp = pow(img[i][j] / 255., 1 / Gamma);
            // 해당 픽셀의 값(img[i][j])을 255로 나눈 후(0~1), ^(1/매개변수 Gamma) 연산 시행(pow)하여 tmp에 저장
            // Gamma의 역수 승만큼 제곱하므로 Gamma가 작을 수록 tmp 값이 커짐. (0~1)^(1/Gamma)

            if (tmp * 255 > 255)
            {
                tmp = 1;
            }
            else if (tmp * 255 < 0)
            {
                tmp = 0;
            }
            // tmp 값이 1 초과 또는 0 미만일 경우 각각 1과 0으로 저장

            Result[i][j] = tmp * 255; // 연산 시행 후의 결과를 해당 픽셀의 위치에 저장
        }
    }
}

// 비트 슬라이싱 함수
void BitSlicing(uchar** img, uchar** Result, int Row, int Col, int Position)
{
    // 해당 position 값에 맞는 mask값을 통해 이미지를 구함
    int i, j;
    uchar mask = 0x01;
    mask <<= Position;

    for (i = 0;i < Row;i++)
    {
        for (j = 0; j < Col; j++)
        {
            if ((mask & img[i][j]) == pow(2, Position)) //pow(2, Position) = mask
            {
                Result[i][j] = 255;
            }
            else
            {
                Result[i][j] = 0;
            }
        }
    }
}

// 노이즈 함수
double uniform()
{
    return((double)(rand() & RAND_MAX) / RAND_MAX - 0.5);
}

// 가우시안 난수 생성 함수
double gaussian()
{
    static int ready = 0;
    static double gstore;
    double v1, v2, r, fac, gaus;
    double uniform();

    if (ready == 0)
    {
        do
        {
            v1 = 2. * uniform();
            v2 = 2. * uniform();
            r = v1 * v1 + v2 * v2;
        }
        while(r > 1.0);

        fac = sqrt(-2. * log(r) / r);
        gstore = v1 * fac;
        gaus = v2 * fac;
        ready = 1;
    }
    else
    {
        ready = 0;
        gaus = gstore;
    }

    return(gaus);
}

// 가우시안 난수 추가 함수
void AddNoise(uchar** img, uchar** Result, int Row, int Col)
{
    int i, j;

    for (i = 0; i < Row; i++)
    {
        for (j = 0; j < Col; j++)
        {
            Result[i][j] = img[i][j] + gaussian() * 50;
        }
    }
}

// 서클 함수
void Circle(uchar** Result, int Row, int Col, double Diameter) // 원하는 반지름의 크기로 원 생성
{
    int i, j;
    double tmp, xSquare, ySquare;

    for (i = 0; i < Row; i++)
    {
        for (j = 0; j < Col; j++)
        {
            ySquare = (abs(Row / 2 - i)) * (abs(Row / 2 - i)); // (Row/2, Col/2) = 중심 좌표
            xSquare = (abs(Col / 2 - j)) * (abs(Col / 2 - j));

            tmp = sqrt(ySquare + xSquare); // tmp는 현재 위치에서 중심과의 거리 -> 피타고라스 정리 x^2 + y^2 = z^2 , sqrt() : 제곱근을 구하는 함수

            if (tmp < Diameter)
            {
                Result[i][j] = 255;
            }
            else
            {
                Result[i][j] = 0;
            }
        }
    }
}

// AND 논리연산 함수
void MaskAnd(uchar** in1Img, uchar** in2Img, uchar** outImg, int Row, int Col)
{
    int i, j;

    for (i = 0; i < Row; i++)
    {
        for (j = 0; j < Col; j++)
        {
            outImg[i][j] = in1Img[i][j] & in2Img[i][j];
        }
    }
}

// OR 논리연산 함수
void MaskOr(uchar** in1Img, uchar** in2Img, uchar** outImg, int Row, int Col)
{
    int i, j;

    for (i = 0; i < Row; i++)
    {
        for (j = 0; j < Col; j++)
        {
            outImg[i][j] = in1Img[i][j] | in2Img[i][j];
        }
    }
}

// 중간고사 함수
// 프로젝트 중간고사 소스코드 midterm.cpp 참고

// 히스토그램 함수
// 히스토그램 이퀄라이제이션 함수
// 히스토그램 스페시피케이션 함수
// 프로젝트 히스토그램 소스코드 histogram.cpp 참고

// 기말고사 함수
// 프로젝트 기말고사 소스코드 final.cpp 참고

// 메인 함수
int main(int argc, char* argv[])
{
    int i, j, Row, Col, Menu; // 필요한 정수형 변수들과 행렬 변수 선언
    uchar** Input, ** Output; // 입력 영상 변수와 출력 영상 변수 선언(이중 포인터 개념) + 히스토그램용

    if (argc != 5) // 사용자 입력 시, 입력 데이터 갯수를 파악하여 메세지를 출력하는 파트, 5이므로 5개 데이터 입력 필요
    {
        printf("\n    입력 데이터 형식 : 프로그램명 입력이미지 행 열 저장이름 \n");
        exit(0);
    }

    Row = atoi(argv[2]); // 입력 데이터 중 2번째는 행
    Col = atoi(argv[3]); // 입력 데이터 중 3번째는 열
    Input = uc_alloc(Row, Col);  // 입력 영상에 대하여 행렬 크기로 메모리 할당
    Output = uc_alloc(Row, Col); // 출력 영상에 대하여 행렬 크기로 메모리 할당
    read_ucmatrix(Row, Col, Input, argv[1]); // 입력 데이터 중 1번째는 입력 영상, 해당 영상을 읽기

    printf("\n    성결대학교 공과대학 정보통신공학부 - 박수영 , 3-2 영상처리 \n\n");
    printf("    0. Exit \n");
    printf("    1. Dispcv \n");
    printf("    2. Negative \n");
    printf("    3. Mosaic \n");
    printf("    4. Binary \n");
    printf("    5. Gamma \n");
    printf("    6. Bit Plane \n");
    printf("    7. Noise \n");
    printf("    8. Circle \n");
    printf("    9. Logical \n");
    printf("    10. Midterm \n");
    printf("    11. Histogram \n");
    printf("    12. Filtering \n");
    printf("    13. Frequency Domain \n");
    printf("    14. Standard Encoding \n");
    printf("    15. Final \n");
    printf("\n");

    printf("    메뉴를 선택해주세요. (숫자를 입력하시면 됩니다.) : ");
    scanf("%d", &Menu);
    printf("\n");

    if (Menu == 0) // Exit , 프로그램 종료
    {
        printf("    Menu 0 : Exit \n\n");
    }

    if (Menu == 1) // Dispcv , 기본 읽기 or 쓰기
    {
        printf("    Menu 1 : Dispcv \n\n");
        Dispcv(Input, Output, Row, Col);
    }

    if (Menu == 2) // Negative , 반전 효과
    {
        printf("    Menu 2 : Negative \n\n");
        Negative(Input, Output, Row, Col);
    }

    if (Menu == 3) // Mosaic , 모자이크
    {
        int Block;
        printf("    Menu 3 : Mosaic \n\n");
        printf("        Block Size : ");
        scanf("%d", &Block);
        Mosaic(Input, Output, Row, Col, Block);
    }

    if (Menu == 4) // Binary , 바이너리
    {
        int Binary;
        printf("    Menu 4 : Binary \n\n");
        printf("        Option 1 - Average \n");
        printf("        Option 2 - Adaptive \n");
        printf("\n    Option : ");
        scanf("%d", &Binary);

        if (Binary == 1) // Average , 바이너리 평균
        {
            double Avg = Average(Input, Row, Col);
            printf("\n    Menu 4 : Binary , Option 1 - Average \n\n");
            MakeBinary(Input, Output, Row, Col, Avg);
        }
        else if (Binary == 2) // Adaptive , 바이너리 밝기
        {
            int Adaptive;
            printf("\n        Menu 4 : Binary , Option 2 - Adaptive \n\n");
            printf("            Select 2 \n");
            printf("            Select 1 \n");
            printf("            Select 0 \n");
            printf("\n        Select : ");
            scanf("%d", &Adaptive);

            if (Adaptive == 2) // 바이너리 밝기 2
            {
                printf("\n            Menu 4 : Binary , Option 2 - Adaptive , Select 2 \n\n");
                AdaptiveBinary2(Input, Output, Row, Col);
            }
            else if (Adaptive == 1) // 바이너리 밝기 1
            {
                printf("\n            Menu 4 : Binary , Option 2 - Adaptive , Select 1 \n\n");
                AdaptiveBinary1(Input, Output, Row, Col);
            }
            else if (Adaptive == 0) // 바이너리 밝기 0
            {
                printf("\n            Menu 4 : Binary , Option 2 - Adaptive , Select 0 \n\n");
                AdaptiveBinary0(Input, Output, Row, Col);
            }
        }
    }

    if (Menu == 5) // Gamma , 감마 보정
    {
        double Gamma;
        printf("    Menu 5 : Gamma \n\n");
        printf("        Gamma Correction : ");
        scanf("%lf", &Gamma);
        PowImg(Input, Output, Row, Col, Gamma);
    }

    if (Menu == 6) // Bit Plane , 비트 슬라이싱
    {
        int Position;
        printf("    Menu 6 : Bit Plane \n\n");
        printf("        Position : ");
        scanf("%d", &Position);
        BitSlicing(Input, Output, Row, Col, Position);
    }

    if (Menu == 7) // Noise , 노이즈
    {
        printf("    Menu 7 : Noise \n\n");
        AddNoise(Input, Output, Row, Col);
    }

    if (Menu == 8) // Circle , 서클
    {
        int Diameter;
        printf("    Menu 8 : Circle \n\n");
        printf("        Diameter : ");
        scanf("%d", &Diameter);
        Circle(Output, Row, Col, Diameter);
    }

    if (Menu == 9) // Logical , 논리연산
    {
        int Logical;
        int LDiameter;
        uchar** CirImg = uc_alloc(Row, Col);
        printf("    Menu 9 : Logical \n\n");
        printf("        Option 1 - AND \n");
        printf("        Option 2 - OR \n");
        printf("\n    Option : ");
        scanf("%d", &Logical);

        if (Logical == 1) // AND 연산
        {
            printf("\n    Menu 9 : Logical , Option 1 - AND \n\n");
            printf("        Diameter : ");
            scanf("%d", &LDiameter);
            Circle(CirImg, Row, Col, LDiameter);
            MaskAnd(Input, CirImg, Output, Row, Col);
        }
        else if (Logical == 2) // OR 연산
        {
            printf("\n    Menu 9 : Logical , Option 2 - OR \n\n");
            printf("        Diameter : ");
            scanf("%d", &LDiameter);
            Circle(CirImg, Row, Col, LDiameter);
            MaskOr(Input, CirImg, Output, Row, Col);
        }
    }

    if (Menu == 10) // Midterm , 중간고사
    {
        printf("    Menu 10 : Midterm \n\n");
        printf("    중간고사 소스코드 참고 \n\n");
    }

    if (Menu == 11) // Histogram , 히스토그램
    {
        printf("    Menu 11 : Histogram \n\n");
        printf("    히스토그램 소스코드 참고 \n\n");
    }

    if (Menu == 12) // Filtering , 필터링
    {
        printf("    Menu 12 : Filtering \n\n");
        printf("    필터링 소스코드 미구현 \n\n");
    }

    if (Menu == 13) // Frequency Domain , 주파수 영역
    {
        printf("    Menu 13 : Frequency Domain \n\n");
        printf("    주파수 영역 소스코드 미구현 \n\n");
    }

    if (Menu == 14) // Standard Encoding , 표준 부호화
    {
        printf("    Menu 14 : Standard Encoding \n\n");
        printf("    표준 부호화 소스코드 미구현 \n\n");
    }

    if (Menu == 15) // Final , 기말고사
    {
        printf("    Menu 15 : Final \n\n");
        printf("    기말고사 소스코드 참고 \n\n");
    }

    Mat cvImg(Row, Col, CV_8UC(1));
    // Mat = OpenCV 기본 데이터 타입, 행렬 구조체
    // CV_8UC(1) 위치 = 행렬 요소 데이터 타입 지정

    // 입력 영상을 작업을 완료한 출력 영상으로 변경하는 파트
    for (i = 0; i < Row; i++)
    {
        for (j = 0; j < Col; j++)
        {
            // (cvImg.data+cvImg.step*i)[j]=img[i][j] // Think This
            cvImg.at<uchar>(i, j) = Output[i][j]; // 입력 영상(Input)을 영상 처리하여 출력 영상(Output)으로 변경
        }
    }

    write_ucmatrix(Row, Col, Output, argv[4]); // 입력 데이터 중 4번째를 이름으로, 해당 영상을 쓰기

    namedWindow(argv[4], WINDOW_AUTOSIZE); // 출력 창 생성, 창 이름은 입력 데이터 첫번째인 입력 영상의 이름, 크기는 자동
    imshow(argv[4], cvImg); // 결과 영상(cvImg) 출력, 창 이름은 위와 동일

    waitKey(0); // 키 입력 대기, 0이므로 무한 대기
    return 0; // 프로그램 종료
}
