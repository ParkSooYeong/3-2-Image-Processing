/* SKU CoE ITE - ParkSooYoung , 2021-02 ImageProcessing : Midterm*/

#define _CRT_SECURE_NO_WARNINGS // Warning C4996 Error

//��ó���� ��Ʈ
#include <opencv2/opencv.hpp>    // OpenCV ���� ��� ������� ����
#include <opencv2/core.hpp>      // OpenCV �⺻ ����ü �� ��� ��ƾ ��� ������� ����
#include <opencv2/highgui.hpp>   // OpenCV ������ ȭ�� �� UI�� ���콺 ���� ��� ������� ����
#include <opencv2/video.hpp>     // OpenCV ���� ���� �� ��� ���׸����̼� ��� ������� ����
#include <opencv2/imgcodecs.hpp> // OpenCV �⺻ �̹��� �ڵ� ��� ������� ����
#include <opencv2/imgproc.hpp>   // OpenCV ����ó�� ��� ������� ����
#include <stdio.h>               // ǥ������� �Լ� ���̺귯�� ������� ����
#include <math.h>                // ���� �Լ� ���̺귯�� ������� ����

typedef unsigned char uchar; // unsigned char Ÿ���� uchar�� ������

using namespace std; // ǥ�� ���ӽ����̽� ���
using namespace cv; // OpenCV ���ӽ����̽� ���

// �޸� �Ҵ� �Լ� uc_alloc ����
uchar** uc_alloc(int size_x, int size_y) // �Ű������� x��(��)�� ũ��� y��(��)�� ũ��
{
    uchar** m; // �޸� �Ҵ� �Ǻ��� ���� ���� ������ ���� m ����
    int i;     // �ݺ��� ����� ���� ������ ���� i ����

    if ((m = (uchar**)calloc(size_y, sizeof(uchar*))) == NULL) // y�࿡ ���Ͽ� �޸� �Ҵ��� �����ϸ�
    {
        printf("d_alloc error 1\7\n"); // ���� �޽��� ���
        exit(0);                       // ���α׷� ����
    }

    for (i = 0; i < size_y; i++) // y���� ũ����� �Ʒ� �ڵ� �ݺ�
    {
        if ((m[i] = (uchar*)calloc(size_x, sizeof(uchar))) == NULL) // x�࿡ ���Ͽ� �޸� �Ҵ��� �����ϸ�
        {
            printf("d_alloc error 2\7\n"); // ���� �޽��� ���
            exit(0);                       // ���α׷� ����
        }
    }

    return m; // �޸� �Ҵ� �Ǻ��� ��ġ�� ��� ��ȯ
}

// ���� �д� �Լ� read_ucmatrix ����
// �Ű������� ��ũ��, ��ũ��, (�Է�)�̹��� ����, ���� �̸�(�ƱԸ�Ʈ�� �޴� �Է� �̹��� �̸�)
void read_ucmatrix(int size_x, int size_y, uchar** ucmatrix, const char* filename)
{
    int i;   // �ݺ��� ����� ���� ������ ���� i ����
    FILE* f; // ���� ��� ����� ���� ���� ������ f ����

    if ((f = fopen(filename, "rb")) == NULL) // �Է� �̹��� ���Ͽ� ���Ͽ� ���� �������� �д� �ɼ��� �����ϸ�
    {
        printf("%s File open Error!\n", filename); // ���� �޽��� ���
        exit(0);                                   // ���α׷� ����
    }

    for (i = 0; i < size_y; i++) // y���� ũ����� �Ʒ� �ڵ� �ݺ�
    {
        if (fread(ucmatrix[i], sizeof(uchar), size_x, f) != size_x) // ��� ���� ũ�⸸ŭ �Է� �̹��� �бⰡ �����ϸ�
        {
            printf("Data Read Error!\n"); // ���� �޽��� ���
            exit(0);                      // ���α׷� ����
        }
    }

    fclose(f); // ���� ��� ����
}

// ���� ���� �Լ� write_ucmatrix ����
// �Ű������� ��ũ��, ��ũ��, (���)�̹��� ����, ���� �̸�(�ƱԸ�Ʈ�� �޴� ��� �̹��� �̸�)
void write_ucmatrix(int size_x, int size_y, uchar** ucmatrix, const char* filename)
{
    int i;   // �ݺ��� ����� ���� ������ ���� i ����
    FILE* f; // ���� ��� ����� ���� ���� ������ f ����

    if ((f = fopen(filename, "wb")) == NULL) // ��� �̹��� ���Ͽ� ���Ͽ� ���� �������� ���� �ɼ��� �����ϸ�
    {
        printf("%s File open Error!\n", filename); // ���� �̽��� ���
        exit(0);                                   // ���α׷� ����
    }

    for (i = 0; i < size_y; i++) // y���� ũ����� �Ʒ� �ڵ� �ݺ�
    {
        if (fwrite(ucmatrix[i], sizeof(uchar), size_x, f) != size_x) // ��� ���� ũ�⸸ŭ ��� �̹��� ���Ⱑ �����ϸ�
        {
            printf("Data Write Error!\n"); // ���� �޽��� ���
            exit(0);                       // ���α׷� ����
        }
    }

    fclose(f); // ���� ��� ����
}

// �߰���� �Լ� Midterm ���� = ���� �����ð��� �ǽ��ߴ� �ҽ��ڵ���� �ϳ��� �Լ��� ���ǹ� ����(�޴� ���� ����)���� ����
// �Ű������� �Է� �̹���, ��� �̹���, ��ũ��, ��ũ��, ��ϻ�����, ����ġ, ����ġ, ����, �޴� ����
void Midterm(uchar** inImage, uchar** outImage, int Row, int Col, int Block, int Row_pos, int Col_pos, int Diameter, int Menu)
{
    int i, j, sum, count, x, y, position;                   // �ݺ��� ����� ���� ������ ������ ���� ��Ʈ���� ����� ������ ���� ����
    double tmp, xSquare, ySquare, bsum, bavg, gamma, gcpow; // ��Ŭ ������ ���� �Ǽ��� ������ ���� ��Ʈ���� ����� �Ǽ��� ���� ����

    if (Menu == 1) // Dispcv , ���� �Լ����� ���� �⺻ ���̽� -> �� ��Ʈ���� ���ݾ� ����
    {
        for (i = 0; i < Row; i++) // ���� ũ�⸸ŭ �ݺ�(�̹����� x��)
        {
            for (j = 0; j < Col; j++) // ���� ũ�⸸ŭ �ݺ�(�̹����� y��)
            {
                xSquare = (abs(Row_pos / 2 - j)) * (abs(Row_pos / 2 - j)); // (Row/2, Col/2) = �߽� ��ǥ -> position���� ����
                ySquare = (abs(Col_pos / 2 - i)) * (abs(Col_pos / 2 - i)); // ��, ������ ���� ���� �̹��� ũ�⿡�� 2�踦 ����Ͽ� �Է� �ʿ�

                tmp = sqrt(xSquare + ySquare); // tmp�� �߽����κ��� ������ �Ÿ� -> sqrt() : ������ �Լ� + ��Ÿ��� ���� x^2 + y^2 = z^2

                if (tmp < Diameter) // ��Ŭ ���� ����
                {
                    outImage[i][j] = inImage[i][j]; // ��ȭ����
                }
                else // ��Ŭ �ܺ� ����
                {
                    outImage[i][j] = inImage[i][j]; // ��ȭ����
                }
            }
        }
    }

    else if (Menu == 2) // Negative , ���� ȿ��
    {
        for (i = 0; i < Row; i++) // ���� ũ�⸸ŭ �ݺ�(�̹����� x��)
        {
            for (j = 0; j < Col; j++) // ���� ũ�⸸ŭ �ݺ�(�̹����� y��)
            {
                xSquare = (abs(Row_pos / 2 - j)) * (abs(Row_pos / 2 - j)); // (Row/2, Col/2) = �߽� ��ǥ -> position���� ����
                ySquare = (abs(Col_pos / 2 - i)) * (abs(Col_pos / 2 - i)); // ��, ������ ���� ���� �̹��� ũ�⿡�� 2�踦 ����Ͽ� �Է� �ʿ�

                tmp = sqrt(xSquare + ySquare); // tmp�� �߽����κ��� ������ �Ÿ� -> sqrt() : ������ �Լ� + ��Ÿ��� ���� x^2 + y^2 = z^2

                if (tmp < Diameter) // ��Ŭ ���� ����
                {
                    outImage[i][j] = 255 - inImage[i][j]; // ���� ȿ��
                }
                else // ��Ŭ �ܺ� ����
                {
                    outImage[i][j] = inImage[i][j]; // ��ȭ ����
                }
            }
        }
    }

    else if (Menu == 3) // Mosaic , ������ũ
    {
        for (i = 0; i < Row; i += Block) // ���� ũ����� ��� �����ŭ �ݺ�
        {
            for (j = 0; j < Col; j += Block) // ���� ũ����� ��� �����ŭ �ݺ�
            {
                sum = 0;   // �հ� 0���� �ʱ�ȭ
                count = 0; // ���� 0���� �ʱ�ȭ

                for (y = 0; y < Block; y++) // ���� �κ��� ���� ���Ͽ� ��� ��������� �ݺ�
                {
                    for (x = 0; x < Block; x++) // ���� �κ��� �࿡ ���Ͽ� ��� ��������� �ݺ�
                    {
                        sum += inImage[i + y][j + x]; // �Է� �̹��� ���� �κ��� �ȼ� ��
                        count++;                      // �ȼ� ���� 1 ����
                    }
                } //get average = ������ũ ó���� ���� ��� ����

                sum /= count; // �հ�(sum) / �ȼ� ����(count) = ��(Block) ���� �ȼ� ���

                for (y = 0; y < Block; y++) // ���� �κ��� ���� ���Ͽ� ��� ��������� �ݺ�
                {
                    for (x = 0; x < Block; x++) // ���� �κ��� �࿡ ���Ͽ� ��� ��������� �ݺ�
                    {
                        xSquare = (abs(Row_pos / 2 - j)) * (abs(Row_pos / 2 - j)); // (Row/2, Col/2) = �߽� ��ǥ -> position���� ����
                        ySquare = (abs(Col_pos / 2 - i)) * (abs(Col_pos / 2 - i)); // ��, ������ ���� ���� �̹��� ũ�⿡�� 2�踦 ����Ͽ� �Է� �ʿ�

                        tmp = sqrt(xSquare + ySquare); // tmp�� �߽����κ��� ������ �Ÿ� -> sqrt() : ������ �Լ� + ��Ÿ��� ���� x^2 + y^2 = z^2

                        if (tmp < Diameter) // ��Ŭ ���� ����
                        {
                            outImage[i + y][j + x] = sum; // ������ũ
                        }
                        else // ��Ŭ �ܺ� ����
                        {
                            outImage[i + y][j + x] = inImage[i + y][j + x]; // ��ȭ ����
                        }
                    }
                }
            }
        }
    }

    else if (Menu == 4) // Binary , ���̳ʸ�
    {
        bsum = 0; // ���̳ʸ� ��� ���� ���� �հ� 0���� �ʱ�ȭ

        for (i = 0; i < Row; i++) // ���� ũ�⸸ŭ �ݺ�(�̹����� x��)
        {
            for (j = 0; j < Col; j++) // ���� ũ�⸸ŭ �ݺ�(�̹����� y��)
            {
                bsum += inImage[i][j]; // �Է� �̹��� �ȼ� ��
            }
        }

        bavg = bsum / ((double)Row * Col); // ���̳ʸ� ó���� ���� ��� ����
        printf("      Average of Image : %lf \n", bavg); // ��� �� ���
        
        // ������� ���̳ʸ� ��� ����
        // �Ʒ����� ���̳ʸ� ���� ó��

        for (i = 0; i < Row; i++) // ���� ũ�⸸ŭ �ݺ�(�̹����� x��)
        {
            for (j = 0; j < Col; j++) // ���� ũ�⸸ŭ �ݺ�(�̹����� y��)
            {
                xSquare = (abs(Row_pos / 2 - j)) * (abs(Row_pos / 2 - j)); // (Row/2, Col/2) = �߽� ��ǥ -> position���� ����
                ySquare = (abs(Col_pos / 2 - i)) * (abs(Col_pos / 2 - i)); // ��, ������ ���� ���� �̹��� ũ�⿡�� 2�踦 ����Ͽ� �Է� �ʿ�

                tmp = sqrt(xSquare + ySquare); // tmp�� �߽����κ��� ������ �Ÿ� -> sqrt() : ������ �Լ� + ��Ÿ��� ���� x^2 + y^2 = z^2

                if (tmp < Diameter) // ��Ŭ ���� ���� -> ���̳ʸ�
                {
                    if (inImage[i][j] > bavg) // �Է� �̹��� ������ ���� ��� ������ ũ��
                    {
                        outImage[i][j] = 255; // ��� �̹��� ������ white
                    }
                    else // �Է� �̹��� ������ ���� ��� ������ ������
                    {
                        outImage[i][j] = 0; // ��� �̹��� ������ black
                    }
                }
                else // ��Ŭ �ܺ� ����
                {
                    outImage[i][j] = inImage[i][j]; // ��ȭ ����
                }
            }
        }
    }

    else if (Menu == 5) // Gamma , ���� ����
    {
        printf("        Gamma Correction (0.45 or 2.45 ��õ , �ٸ� ���� ����) : "); // ���� ��ġ �Է� ���� �޽��� ���
        scanf("%lf", &gamma); // �Է��� ���� ��ġ ����

        for (i = 0; i < Row; i++) // ���� ũ�⸸ŭ �ݺ�(�̹����� x��)
        {
            for (j = 0; j < Col; j++) // ���� ũ�⸸ŭ �ݺ�(�̹����� y��)
            {
                xSquare = (abs(Row_pos / 2 - j)) * (abs(Row_pos / 2 - j)); // (Row/2, Col/2) = �߽� ��ǥ -> position���� ����
                ySquare = (abs(Col_pos / 2 - i)) * (abs(Col_pos / 2 - i)); // ��, ������ ���� ���� �̹��� ũ�⿡�� 2�踦 ����Ͽ� �Է� �ʿ�

                tmp = sqrt(xSquare + ySquare); // tmp�� �߽����κ��� ������ �Ÿ� -> sqrt() : ������ �Լ� + ��Ÿ��� ���� x^2 + y^2 = z^2

                if (tmp < Diameter) // ��Ŭ ���� ���� -> ���� ����
                {
                    gcpow = pow(inImage[i][j] / 255., 1 / gamma);
                    // �ش� �ȼ��� ��(inImage[i][j])�� 255�� ���� ��(0~1), ^(1/�Ű����� gamma) ���� ����(pow)�Ͽ� gcpow�� ����
                    // gamma�� ���� �¸�ŭ �����ϹǷ� gamma�� ���� ���� gcpow ���� Ŀ��. (0~1)^(1/gamma)

                    if (gcpow * 255 > 255) // gcpow ���� 1 �ʰ��� ���
                    {
                        gcpow = 1; // gcpow ���� 1 ����
                    }
                    else if (gcpow * 255 < 0) // gcpow ���� 0 �̸��� ���
                    {
                        gcpow = 0; // gcpow ���� 0���� ����
                    }

                    outImage[i][j] = gcpow * 255; // ���� ���� ���� ����� �ش� �ȼ��� ��ġ�� ����
                }
                else // ��Ŭ �ܺ� ����
                {
                    outImage[i][j] = inImage[i][j]; // ��ȭ ����
                }
            }
        }
    }

    else if (Menu == 6) // Bit Plane , ��Ʈ �����̽�
    {
        printf("        Position (0 ~ 7) : "); // ��Ʈ ������ �Է� ���� �޽��� ���
        scanf("%d", &position); // �Է��� ��Ʈ ������ ����

        // �ش� position ���� �´� mask���� ���� �̹����� ����
        uchar mask = 0x01;
        mask <<= position;

        for (i = 0; i < Row; i++) // ���� ũ�⸸ŭ �ݺ�(�̹����� x��)
        {
            for (j = 0; j < Col; j++) // ���� ũ�⸸ŭ �ݺ�(�̹����� y��)
            {
                xSquare = (abs(Row_pos / 2 - j)) * (abs(Row_pos / 2 - j)); // (Row/2, Col/2) = �߽� ��ǥ -> position���� ����
                ySquare = (abs(Col_pos / 2 - i)) * (abs(Col_pos / 2 - i)); // ��, ������ ���� ���� �̹��� ũ�⿡�� 2�踦 ����Ͽ� �Է� �ʿ�

                tmp = sqrt(xSquare + ySquare); // tmp�� �߽����κ��� ������ �Ÿ� -> sqrt() : ������ �Լ� + ��Ÿ��� ���� x^2 + y^2 = z^2

                if (tmp < Diameter) // ��Ŭ ���� ���� -> ��Ʈ �����̽�
                {
                    if ((mask & inImage[i][j]) == pow(2, position)) //pow(2, Position) = mask , position�� mask�� ���� �Ǻ� ��ġ�� ������
                    {
                        outImage[i][j] = 255; // ��� �̹��� ������ white
                    }
                    else // position�� mask�� ���� �Ǻ� ��ġ�� Ʋ����
                    {
                        outImage[i][j] = 0; // ��� �̹��� ������ black
                    }
                }
                else // ��Ŭ �ܺ� ����
                {
                    outImage[i][j] = inImage[i][j]; // ��ȭ ����
                }
            }
        }
    }
}

// ���� �Լ� ������ main ����
int main(int argc, char* argv[])
{
    int i, j, Menu, Row, Col, Block, Row_pos, Col_pos, Diameter; // �ʿ��� ������ ������� ��� ���� ����
    uchar** inImage, ** outImage; // �Է� ���� ������ ��� ���� ���� ����(���� ������ ����)

    while(1) // ���Ǽ��� ���� ���ѷ���
    {
        if (argc != 9) // ����� �Է� ��, �Է� ������ ������ �ľ��Ͽ� �޼����� ����ϴ� ��Ʈ, 9�̹Ƿ� 9�� ������ �Է� �ʿ�
        {
            printf("\n    �Է� ������ ���� : ���α׷��� �Է��̹��� ����̹��� �� �� ��ϻ����� ����ġ ����ġ ���� \n");
            exit(0); // ���α׷� ����
        }

        Row = atoi(argv[3]); // �Է� ������ �� 3��°�� ��
        Col = atoi(argv[4]); // �Է� ������ �� 4��°�� ��
        Block = atoi(argv[5]); // �Է� ������ �� 5��°�� ��ϻ�����
        Row_pos = atoi(argv[6]); // �Է� ������ �� 6��°�� ����ġ
        Col_pos = atoi(argv[7]); // �Է� ������ �� 7��°�� ����ġ
        Diameter = atoi(argv[8]); // �Է� ������ �� 8��°�� ����
        inImage = uc_alloc(Row, Col);  // �Է� ���� ���Ͽ� ��� ũ��� �޸� �Ҵ�
        outImage = uc_alloc(Row, Col); // ��� ���� ���Ͽ� ��� ũ��� �޸� �Ҵ�

        read_ucmatrix(Row, Col, inImage, argv[1]); // �Է� ������ �� 1��°�� �Է� ����, �ش� ������ �б�

        // �������� ȭ�� ����
        printf("\n----------------------------------------------------------------------------------------------------\n\n");
        printf("    ������б� �������� ������Ű��к� - 20170910 �ڼ��� , 2021-2 ����ó�� �߰���� \n\n");
        printf("    0. Exit \n");
        printf("    1. Dispcv \n");
        printf("    2. Negative \n");
        printf("    3. Mosaic \n");
        printf("    4. Binary \n");
        printf("    5. Gamma \n");
        printf("    6. Bit Plane \n");
        printf("\n");

        printf("    ��µ� �̹����� ����� �޴� �������� ���ƿ��� ���ѷ��� �����Դϴ�. \n");
        printf("    �޴��� �������ּ���. (���ڸ� �Է��Ͻø� �˴ϴ�.) : ");
        scanf("%d", &Menu);
        printf("\n");

        if (Menu == 0) // Exit , ���α׷� ����
        {
            printf("    Menu 0 : Exit \n\n");
            printf("        By. SKU CoE ITE : 20170910 Park Soo Young , 2021-2 DIP Midterm , The End \n");
            break; // ���ѷ��� Ż��
        }

        if (Menu == 1) // Dispcv , �⺻ ����
        {
            printf("    Menu 1 : Dispcv \n\n");
            Midterm(inImage, outImage, Row, Col, Block, Row_pos, Col_pos, Diameter, Menu);
        }

        if (Menu == 2) // Negative , ���� ȿ��
        {
            printf("    Menu 2 : Negative \n\n");
            Midterm(inImage, outImage, Row, Col, Block, Row_pos, Col_pos, Diameter, Menu);
        }

        if (Menu == 3) // Mosaic , ������ũ
        {
            printf("    Menu 3 : Mosaic \n\n");
            Midterm(inImage, outImage, Row, Col, Block, Row_pos, Col_pos, Diameter, Menu);
        }

        if (Menu == 4) // Binary , ���̳ʸ�
        {
            printf("    Menu 4 : Binary \n\n");
            Midterm(inImage, outImage, Row, Col, Block, Row_pos, Col_pos, Diameter, Menu);
        }

        if (Menu == 5) // Gamma , ���� ����
        {
            printf("    Menu 5 : Gamma \n\n");
            Midterm(inImage, outImage, Row, Col, Block, Row_pos, Col_pos, Diameter, Menu);
        }
        
        if (Menu == 6) // Bit Plane , ��Ʈ �����̽�
        {
            printf("    Menu 6 : Bit Plane \n\n");
            Midterm(inImage, outImage, Row, Col, Block, Row_pos, Col_pos, Diameter, Menu);
        }

        Mat cvImg(Row, Col, CV_8UC(1));
        // Mat = OpenCV �⺻ ������ Ÿ��, ��� ����ü
        // CV_8UC(1) ��ġ = ��� ��� ������ Ÿ�� ����

        // �Է� ������ �۾��� �Ϸ��� ��� �������� �����ϴ� ��Ʈ
        for (i = 0; i < Row; i++) // ���� ũ�⸸ŭ �ݺ�(�̹����� x��)
        {
            for (j = 0; j < Col; j++) // ���� ũ�⸸ŭ �ݺ�(�̹����� y��)
            {
                // (cvImg.data+cvImg.step*i)[j]=img[i][j] // Think This
                cvImg.at<uchar>(i, j) = outImage[i][j]; // �Է� ����(inImage)�� ���� ó���Ͽ� ��� ����(outImage)���� ����
            }
        }

        write_ucmatrix(Row, Col, outImage, argv[2]); // �Է� ������ �� 2��°�� �̸�����, �ش� ������ ����

        namedWindow(argv[2], WINDOW_AUTOSIZE); // ��� â ����, â �̸��� �Է� ������ �ι�°�� ��� ������ �̸�, ũ��� �ڵ�
        imshow(argv[2], cvImg); // ��� ����(cvImg) ���, â �̸��� ���� ����

        waitKey(0); // Ű �Է� ���, 0�̹Ƿ� ���� ���
    }
    
    return 0; // ���α׷� ����
}
