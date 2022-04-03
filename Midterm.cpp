/* SKU CoE ITE - ParkSooYoung , 2021-02 ImageProcessing : Midterm*/

#define _CRT_SECURE_NO_WARNINGS // Warning C4996 Error

//전처리기 파트
#include <opencv2/opencv.hpp>    // OpenCV 지원 기능 헤더파일 포함
#include <opencv2/core.hpp>      // OpenCV 기본 구조체 및 산술 루틴 기능 헤더파일 포함
#include <opencv2/highgui.hpp>   // OpenCV 윈도우 화면 및 UI와 마우스 제어 기능 헤더파일 포함
#include <opencv2/video.hpp>     // OpenCV 비디오 추적 및 배경 세그멘테이션 기능 헤더파일 포함
#include <opencv2/imgcodecs.hpp> // OpenCV 기본 이미지 코덱 기능 헤더파일 포함
#include <opencv2/imgproc.hpp>   // OpenCV 영상처리 기능 헤더파일 포함
#include <stdio.h>               // 표준입출력 함수 라이브러리 헤더파일 포함
#include <math.h>                // 수학 함수 라이브러리 헤더파일 포함

typedef unsigned char uchar; // unsigned char 타입을 uchar로 재정의

using namespace std; // 표준 네임스페이스 사용
using namespace cv; // OpenCV 네임스페이스 사용

// 메모리 할당 함수 uc_alloc 선언
uchar** uc_alloc(int size_x, int size_y) // 매개변수는 x축(행)의 크기와 y축(열)의 크기
{
    uchar** m; // 메모리 할당 판별을 위한 이중 포인터 변수 m 선언
    int i;     // 반복문 사용을 위한 정수형 변수 i 선언

    if ((m = (uchar**)calloc(size_y, sizeof(uchar*))) == NULL) // y축에 대하여 메모리 할당이 실패하면
    {
        printf("d_alloc error 1\7\n"); // 에러 메시지 출력
        exit(0);                       // 프로그램 종료
    }

    for (i = 0; i < size_y; i++) // y축의 크기까지 아래 코드 반복
    {
        if ((m[i] = (uchar*)calloc(size_x, sizeof(uchar))) == NULL) // x축에 대하여 메모리 할당이 실패하면
        {
            printf("d_alloc error 2\7\n"); // 에러 메시지 출력
            exit(0);                       // 프로그램 종료
        }
    }

    return m; // 메모리 할당 판별을 마치고 결과 반환
}

// 영상 읽는 함수 read_ucmatrix 선언
// 매개변수는 행크기, 열크기, (입력)이미지 변수, 파일 이름(아규먼트로 받는 입력 이미지 이름)
void read_ucmatrix(int size_x, int size_y, uchar** ucmatrix, const char* filename)
{
    int i;   // 반복문 사용을 위한 정수형 변수 i 선언
    FILE* f; // 파일 기능 사용을 위한 파일 포인터 f 선언

    if ((f = fopen(filename, "rb")) == NULL) // 입력 이미지 파일에 대하여 이진 형식으로 읽는 옵션이 실패하면
    {
        printf("%s File open Error!\n", filename); // 에러 메시지 출력
        exit(0);                                   // 프로그램 종료
    }

    for (i = 0; i < size_y; i++) // y축의 크기까지 아래 코드 반복
    {
        if (fread(ucmatrix[i], sizeof(uchar), size_x, f) != size_x) // 행과 열의 크기만큼 입력 이미지 읽기가 실패하면
        {
            printf("Data Read Error!\n"); // 에러 메시지 출력
            exit(0);                      // 프로그램 종료
        }
    }

    fclose(f); // 파일 기능 종료
}

// 영상 쓰는 함수 write_ucmatrix 선언
// 매개변수는 행크기, 열크기, (출력)이미지 변수, 파일 이름(아규먼트로 받는 출력 이미지 이름)
void write_ucmatrix(int size_x, int size_y, uchar** ucmatrix, const char* filename)
{
    int i;   // 반복문 사용을 위한 정수형 변수 i 선언
    FILE* f; // 파일 기능 사용을 위한 파일 포인터 f 선언

    if ((f = fopen(filename, "wb")) == NULL) // 출력 이미지 파일에 대하여 이진 형식으로 쓰는 옵션이 실패하면
    {
        printf("%s File open Error!\n", filename); // 에러 미시지 출력
        exit(0);                                   // 프로그램 종료
    }

    for (i = 0; i < size_y; i++) // y축의 크기까지 아래 코드 반복
    {
        if (fwrite(ucmatrix[i], sizeof(uchar), size_x, f) != size_x) // 행과 열의 크기만큼 출력 이미지 쓰기가 실패하면
        {
            printf("Data Write Error!\n"); // 에러 메시지 출력
            exit(0);                       // 프로그램 종료
        }
    }

    fclose(f); // 파일 기능 종료
}

// 중간고사 함수 Midterm 선언 = 기존 수업시간에 실습했던 소스코드들을 하나의 함수에 조건문 형식(메뉴 선택 구조)으로 통합
// 매개변수는 입력 이미지, 출력 이미지, 행크기, 열크기, 블록사이즈, 행위치, 열위치, 지름, 메뉴 선택
void Midterm(uchar** inImage, uchar** outImage, int Row, int Col, int Block, int Row_pos, int Col_pos, int Diameter, int Menu)
{
    int i, j, sum, count, x, y, position;                   // 반복문 사용을 위한 정수형 변수와 각종 파트에서 사용할 정수형 변수 선언
    double tmp, xSquare, ySquare, bsum, bavg, gamma, gcpow; // 서클 생성을 위한 실수형 변수와 각종 파트에서 사용할 실수형 변수 선언

    if (Menu == 1) // Dispcv , 이후 함수들을 위한 기본 베이스 -> 이 파트에서 조금씩 변형
    {
        for (i = 0; i < Row; i++) // 행의 크기만큼 반복(이미지의 x축)
        {
            for (j = 0; j < Col; j++) // 열의 크기만큼 반복(이미지의 y축)
            {
                xSquare = (abs(Row_pos / 2 - j)) * (abs(Row_pos / 2 - j)); // (Row/2, Col/2) = 중심 좌표 -> position으로 변형
                ySquare = (abs(Col_pos / 2 - i)) * (abs(Col_pos / 2 - i)); // 즉, 포지션 값은 원래 이미지 크기에서 2배를 고려하여 입력 필요

                tmp = sqrt(xSquare + ySquare); // tmp는 중심으로부터 떨어진 거리 -> sqrt() : 제곱근 함수 + 피타고라스 정리 x^2 + y^2 = z^2

                if (tmp < Diameter) // 서클 내부 영역
                {
                    outImage[i][j] = inImage[i][j]; // 변화없음
                }
                else // 서클 외부 영역
                {
                    outImage[i][j] = inImage[i][j]; // 변화없음
                }
            }
        }
    }

    else if (Menu == 2) // Negative , 반전 효과
    {
        for (i = 0; i < Row; i++) // 행의 크기만큼 반복(이미지의 x축)
        {
            for (j = 0; j < Col; j++) // 열의 크기만큼 반복(이미지의 y축)
            {
                xSquare = (abs(Row_pos / 2 - j)) * (abs(Row_pos / 2 - j)); // (Row/2, Col/2) = 중심 좌표 -> position으로 변형
                ySquare = (abs(Col_pos / 2 - i)) * (abs(Col_pos / 2 - i)); // 즉, 포지션 값은 원래 이미지 크기에서 2배를 고려하여 입력 필요

                tmp = sqrt(xSquare + ySquare); // tmp는 중심으로부터 떨어진 거리 -> sqrt() : 제곱근 함수 + 피타고라스 정리 x^2 + y^2 = z^2

                if (tmp < Diameter) // 서클 내부 영역
                {
                    outImage[i][j] = 255 - inImage[i][j]; // 반전 효과
                }
                else // 서클 외부 영역
                {
                    outImage[i][j] = inImage[i][j]; // 변화 없음
                }
            }
        }
    }

    else if (Menu == 3) // Mosaic , 모자이크
    {
        for (i = 0; i < Row; i += Block) // 행의 크기까지 블록 사이즈만큼 반복
        {
            for (j = 0; j < Col; j += Block) // 열의 크기까지 블록 사이즈만큼 반복
            {
                sum = 0;   // 합계 0으로 초기화
                count = 0; // 총합 0으로 초기화

                for (y = 0; y < Block; y++) // 선택 부분의 열에 대하여 블록 사이즈까지 반복
                {
                    for (x = 0; x < Block; x++) // 선택 부분의 행에 대하여 블록 사이즈까지 반복
                    {
                        sum += inImage[i + y][j + x]; // 입력 이미지 선택 부분의 픽셀 합
                        count++;                      // 픽셀 총합 1 증가
                    }
                } //get average = 모자이크 처리를 위한 평균 도출

                sum /= count; // 합계(sum) / 픽셀 총합(count) = 블럭(Block) 내부 픽셀 평균

                for (y = 0; y < Block; y++) // 선택 부분의 열에 대하여 블록 사이즈까지 반복
                {
                    for (x = 0; x < Block; x++) // 선택 부분의 행에 대하여 블록 사이즈까지 반복
                    {
                        xSquare = (abs(Row_pos / 2 - j)) * (abs(Row_pos / 2 - j)); // (Row/2, Col/2) = 중심 좌표 -> position으로 변형
                        ySquare = (abs(Col_pos / 2 - i)) * (abs(Col_pos / 2 - i)); // 즉, 포지션 값은 원래 이미지 크기에서 2배를 고려하여 입력 필요

                        tmp = sqrt(xSquare + ySquare); // tmp는 중심으로부터 떨어진 거리 -> sqrt() : 제곱근 함수 + 피타고라스 정리 x^2 + y^2 = z^2

                        if (tmp < Diameter) // 서클 내부 영역
                        {
                            outImage[i + y][j + x] = sum; // 모자이크
                        }
                        else // 서클 외부 영역
                        {
                            outImage[i + y][j + x] = inImage[i + y][j + x]; // 변화 없음
                        }
                    }
                }
            }
        }
    }

    else if (Menu == 4) // Binary , 바이너리
    {
        bsum = 0; // 바이너리 평균 도출 위한 합계 0으로 초기화

        for (i = 0; i < Row; i++) // 행의 크기만큼 반복(이미지의 x축)
        {
            for (j = 0; j < Col; j++) // 열의 크기만큼 반복(이미지의 y축)
            {
                bsum += inImage[i][j]; // 입력 이미지 픽셀 합
            }
        }

        bavg = bsum / ((double)Row * Col); // 바이너리 처리를 위한 평균 도출
        printf("      Average of Image : %lf \n", bavg); // 평균 값 출력
        
        // 여기까지 바이너리 평균 도출
        // 아래부터 바이너리 영상 처리

        for (i = 0; i < Row; i++) // 행의 크기만큼 반복(이미지의 x축)
        {
            for (j = 0; j < Col; j++) // 열의 크기만큼 반복(이미지의 y축)
            {
                xSquare = (abs(Row_pos / 2 - j)) * (abs(Row_pos / 2 - j)); // (Row/2, Col/2) = 중심 좌표 -> position으로 변형
                ySquare = (abs(Col_pos / 2 - i)) * (abs(Col_pos / 2 - i)); // 즉, 포지션 값은 원래 이미지 크기에서 2배를 고려하여 입력 필요

                tmp = sqrt(xSquare + ySquare); // tmp는 중심으로부터 떨어진 거리 -> sqrt() : 제곱근 함수 + 피타고라스 정리 x^2 + y^2 = z^2

                if (tmp < Diameter) // 서클 내부 영역 -> 바이너리
                {
                    if (inImage[i][j] > bavg) // 입력 이미지 영역의 값이 평균 값보다 크면
                    {
                        outImage[i][j] = 255; // 출력 이미지 영역은 white
                    }
                    else // 입력 이미지 영역의 값이 평균 값보다 작으면
                    {
                        outImage[i][j] = 0; // 출력 이미지 영역은 black
                    }
                }
                else // 서클 외부 영역
                {
                    outImage[i][j] = inImage[i][j]; // 변화 없음
                }
            }
        }
    }

    else if (Menu == 5) // Gamma , 감마 보정
    {
        printf("        Gamma Correction (0.45 or 2.45 추천 , 다른 값도 가능) : "); // 감마 수치 입력 지시 메시지 출력
        scanf("%lf", &gamma); // 입력한 감마 수치 저장

        for (i = 0; i < Row; i++) // 행의 크기만큼 반복(이미지의 x축)
        {
            for (j = 0; j < Col; j++) // 열의 크기만큼 반복(이미지의 y축)
            {
                xSquare = (abs(Row_pos / 2 - j)) * (abs(Row_pos / 2 - j)); // (Row/2, Col/2) = 중심 좌표 -> position으로 변형
                ySquare = (abs(Col_pos / 2 - i)) * (abs(Col_pos / 2 - i)); // 즉, 포지션 값은 원래 이미지 크기에서 2배를 고려하여 입력 필요

                tmp = sqrt(xSquare + ySquare); // tmp는 중심으로부터 떨어진 거리 -> sqrt() : 제곱근 함수 + 피타고라스 정리 x^2 + y^2 = z^2

                if (tmp < Diameter) // 서클 내부 영역 -> 감마 보정
                {
                    gcpow = pow(inImage[i][j] / 255., 1 / gamma);
                    // 해당 픽셀의 값(inImage[i][j])을 255로 나눈 후(0~1), ^(1/매개변수 gamma) 연산 시행(pow)하여 gcpow에 저장
                    // gamma의 역수 승만큼 제곱하므로 gamma가 작을 수록 gcpow 값이 커짐. (0~1)^(1/gamma)

                    if (gcpow * 255 > 255) // gcpow 값이 1 초과일 경우
                    {
                        gcpow = 1; // gcpow 값에 1 저장
                    }
                    else if (gcpow * 255 < 0) // gcpow 값이 0 미만일 경우
                    {
                        gcpow = 0; // gcpow 값에 0으로 저장
                    }

                    outImage[i][j] = gcpow * 255; // 연산 시행 후의 결과를 해당 픽셀의 위치에 저장
                }
                else // 서클 외부 영역
                {
                    outImage[i][j] = inImage[i][j]; // 변화 없음
                }
            }
        }
    }

    else if (Menu == 6) // Bit Plane , 비트 슬라이싱
    {
        printf("        Position (0 ~ 7) : "); // 비트 포지션 입력 지시 메시지 출력
        scanf("%d", &position); // 입력한 비트 포지션 저장

        // 해당 position 값에 맞는 mask값을 통해 이미지를 구함
        uchar mask = 0x01;
        mask <<= position;

        for (i = 0; i < Row; i++) // 행의 크기만큼 반복(이미지의 x축)
        {
            for (j = 0; j < Col; j++) // 열의 크기만큼 반복(이미지의 y축)
            {
                xSquare = (abs(Row_pos / 2 - j)) * (abs(Row_pos / 2 - j)); // (Row/2, Col/2) = 중심 좌표 -> position으로 변형
                ySquare = (abs(Col_pos / 2 - i)) * (abs(Col_pos / 2 - i)); // 즉, 포지션 값은 원래 이미지 크기에서 2배를 고려하여 입력 필요

                tmp = sqrt(xSquare + ySquare); // tmp는 중심으로부터 떨어진 거리 -> sqrt() : 제곱근 함수 + 피타고라스 정리 x^2 + y^2 = z^2

                if (tmp < Diameter) // 서클 내부 영역 -> 비트 슬라이싱
                {
                    if ((mask & inImage[i][j]) == pow(2, position)) //pow(2, Position) = mask , position과 mask에 대한 판별 일치가 맞으면
                    {
                        outImage[i][j] = 255; // 출력 이미지 영역은 white
                    }
                    else // position과 mask에 대한 판별 일치가 틀리면
                    {
                        outImage[i][j] = 0; // 출력 이미지 영역은 black
                    }
                }
                else // 서클 외부 영역
                {
                    outImage[i][j] = inImage[i][j]; // 변화 없음
                }
            }
        }
    }
}

// 메인 함수 정수형 main 선언
int main(int argc, char* argv[])
{
    int i, j, Menu, Row, Col, Block, Row_pos, Col_pos, Diameter; // 필요한 정수형 변수들과 행렬 변수 선언
    uchar** inImage, ** outImage; // 입력 영상 변수와 출력 영상 변수 선언(이중 포인터 개념)

    while(1) // 편의성을 위한 무한루프
    {
        if (argc != 9) // 사용자 입력 시, 입력 데이터 갯수를 파악하여 메세지를 출력하는 파트, 9이므로 9개 데이터 입력 필요
        {
            printf("\n    입력 데이터 형식 : 프로그램명 입력이미지 출력이미지 행 열 블록사이즈 행위치 열위치 지름 \n");
            exit(0); // 프로그램 종료
        }

        Row = atoi(argv[3]); // 입력 데이터 중 3번째는 행
        Col = atoi(argv[4]); // 입력 데이터 중 4번째는 열
        Block = atoi(argv[5]); // 입력 데이터 중 5번째는 블록사이즈
        Row_pos = atoi(argv[6]); // 입력 데이터 중 6번째는 행위치
        Col_pos = atoi(argv[7]); // 입력 데이터 중 7번째는 열위치
        Diameter = atoi(argv[8]); // 입력 데이터 중 8번째는 지름
        inImage = uc_alloc(Row, Col);  // 입력 영상에 대하여 행렬 크기로 메모리 할당
        outImage = uc_alloc(Row, Col); // 출력 영상에 대하여 행렬 크기로 메모리 할당

        read_ucmatrix(Row, Col, inImage, argv[1]); // 입력 데이터 중 1번째는 입력 영상, 해당 영상을 읽기

        // 보여지는 화면 구성
        printf("\n----------------------------------------------------------------------------------------------------\n\n");
        printf("    성결대학교 공과대학 정보통신공학부 - 20170910 박수영 , 2021-2 영상처리 중간고사 \n\n");
        printf("    0. Exit \n");
        printf("    1. Dispcv \n");
        printf("    2. Negative \n");
        printf("    3. Mosaic \n");
        printf("    4. Binary \n");
        printf("    5. Gamma \n");
        printf("    6. Bit Plane \n");
        printf("\n");

        printf("    출력된 이미지를 지우면 메뉴 선택으로 돌아오는 무한루프 구조입니다. \n");
        printf("    메뉴를 선택해주세요. (숫자를 입력하시면 됩니다.) : ");
        scanf("%d", &Menu);
        printf("\n");

        if (Menu == 0) // Exit , 프로그램 종료
        {
            printf("    Menu 0 : Exit \n\n");
            printf("        By. SKU CoE ITE : 20170910 Park Soo Young , 2021-2 DIP Midterm , The End \n");
            break; // 무한루프 탈출
        }

        if (Menu == 1) // Dispcv , 기본 보기
        {
            printf("    Menu 1 : Dispcv \n\n");
            Midterm(inImage, outImage, Row, Col, Block, Row_pos, Col_pos, Diameter, Menu);
        }

        if (Menu == 2) // Negative , 반전 효과
        {
            printf("    Menu 2 : Negative \n\n");
            Midterm(inImage, outImage, Row, Col, Block, Row_pos, Col_pos, Diameter, Menu);
        }

        if (Menu == 3) // Mosaic , 모자이크
        {
            printf("    Menu 3 : Mosaic \n\n");
            Midterm(inImage, outImage, Row, Col, Block, Row_pos, Col_pos, Diameter, Menu);
        }

        if (Menu == 4) // Binary , 바이너리
        {
            printf("    Menu 4 : Binary \n\n");
            Midterm(inImage, outImage, Row, Col, Block, Row_pos, Col_pos, Diameter, Menu);
        }

        if (Menu == 5) // Gamma , 감마 보정
        {
            printf("    Menu 5 : Gamma \n\n");
            Midterm(inImage, outImage, Row, Col, Block, Row_pos, Col_pos, Diameter, Menu);
        }
        
        if (Menu == 6) // Bit Plane , 비트 슬라이싱
        {
            printf("    Menu 6 : Bit Plane \n\n");
            Midterm(inImage, outImage, Row, Col, Block, Row_pos, Col_pos, Diameter, Menu);
        }

        Mat cvImg(Row, Col, CV_8UC(1));
        // Mat = OpenCV 기본 데이터 타입, 행렬 구조체
        // CV_8UC(1) 위치 = 행렬 요소 데이터 타입 지정

        // 입력 영상을 작업을 완료한 출력 영상으로 변경하는 파트
        for (i = 0; i < Row; i++) // 행의 크기만큼 반복(이미지의 x축)
        {
            for (j = 0; j < Col; j++) // 열의 크기만큼 반복(이미지의 y축)
            {
                // (cvImg.data+cvImg.step*i)[j]=img[i][j] // Think This
                cvImg.at<uchar>(i, j) = outImage[i][j]; // 입력 영상(inImage)을 영상 처리하여 출력 영상(outImage)으로 변경
            }
        }

        write_ucmatrix(Row, Col, outImage, argv[2]); // 입력 데이터 중 2번째를 이름으로, 해당 영상을 쓰기

        namedWindow(argv[2], WINDOW_AUTOSIZE); // 출력 창 생성, 창 이름은 입력 데이터 두번째인 출력 영상의 이름, 크기는 자동
        imshow(argv[2], cvImg); // 결과 영상(cvImg) 출력, 창 이름은 위와 동일

        waitKey(0); // 키 입력 대기, 0이므로 무한 대기
    }
    
    return 0; // 프로그램 종료
}
