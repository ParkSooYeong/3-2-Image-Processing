/* SKU CoE ITE - ParkSooYoung , 2021-02 ImageProcessing : Final(2) */

// Warning C4996 Error 프로젝트 속성에서 전처리 완료

// 전처리기 파트
#include <stdio.h> // 표준입출력 함수 라이브러리 헤더파일 포함
#include <stdlib.h> // 표준 함수 라이브러리 헤더파일 포함

#define _USE_MATH_DEFINES //  math.h 호출 전 및 정확한 Pi 값 사용을 위한 코드
#include <math.h> // 수학 함수 라이브러리 헤더파일 포함

typedef unsigned char uchar; // unsigned char 타입을 uchar로 재정의

int Row, Col; // 정수형 전역 변수 행, 열 선언

// 메모리 할당 함수 uc_alloc 선언
uchar** uc_alloc(int size_x, int size_y) // 매개변수는 x축(행)의 크기와 y축(열)의 크기
{
	uchar** m; // 메모리 할당 판별을 위한 이중 포인터 변수 m 선언
	int i; // 반복문 사용을 위한 정수형 변수 i 선언

	if ((m = (uchar**)calloc(size_y, sizeof(uchar*))) == NULL) // y축에 대하여 메모리 할당이 실패하면
	{
		printf("d_alloc error 1\7\n"); // 에러 메시지 출력
		exit(0);					   // 프로그램 종료
	}

	for (i = 0; i < size_y; i++) // y축의 크기까지 아래 코드 반복
	{
		if ((m[i] = (uchar*)calloc(size_x, sizeof(uchar))) == NULL) // x축에 대하여 메모리 할당이 실패하면
		{
			printf("d_alloc error 2\7\n"); // 에러 메시지 출력
			exit(0);					   // 프로그램 종료
		}
	}

	return m; // 메모리 할당 판별을 마치고 결과 반환
}

// 정수형 메모리 할당 함수 i_alloc 선언 
int** i_alloc(int size_x, int size_y) // 매개변수는 x축(행)의 크기와 y축(열)의 크기
{
	int** m; // 메모리 할당 판별을 위한 이중 포인터 변수 m 선언
	int i; // 반복문 사용을 위한 정수형 변수 i 선언

	if ((m = (int**)calloc(size_y, sizeof(int*))) == NULL) // y축에 대하여 메모리 할당이 실패하면
	{
		printf("i_alloc error 1\7\n"); // 에러 메시지 출력
		exit(0);					   // 프로그램 종료
	}

	for (i = 0; i < size_y; i++) // y축의 크기까지 아래 코드 반복
	{
		if ((m[i] = (int*)calloc(size_x, sizeof(int))) == NULL) // x축에 대하여 메모리 할당이 실패하면
		{
			printf("i_alloc error 2\7\n"); // 에러 메시지 출력
			exit(0);					   // 프로그램 종료
		}
	}

	return m; // 메모리 할당 판별을 마치고 결과 반환
}

// 영상 읽는 함수 read_ucmatrix 선언
// 매개변수는 행크기, 열크기, (입력)이미지 변수, 파일 이름(아규먼트로 받는 입력 이미지 이름)
void read_ucmartrix(int size_x, int size_y, uchar** ucmatrix, const char* filename)
{
	int i;	 // 반복문 사용을 위한 정수형 변수 i 선언
	FILE* f; // 파일 기능 사용을 위한 파일 포인터 f 선언

	if ((fopen_s(&f, filename, "rb")) != NULL) // 입력 이미지 파일에 대하여 이진 형식으로 읽는 옵션이 실패하면
	{
		printf("%s File open Error!\n", filename); // 에러 메시지 출력
		exit(0);								   // 프로그램 종료
	}

	for (i = 0; i < size_y; i++) // y축의 크기까지 아래 코드 반복
	{
		if (fread(ucmatrix[i], sizeof(uchar), size_x, f) != size_x) // 행과 열의 크기만큼 입력 이미지 읽기가 실패하면
		{
			printf("Data Read Error!\n"); // 에러 메시지 출력
			exit(0);					  // 프로그램 종료
		}
	}

	fclose(f); // 파일 기능 종료
}

// 영상 쓰는 함수 write_ucmatrix 선언
// 매개변수는 행크기, 열크기, (출력)이미지 변수, 파일 이름(아규먼트로 받는 출력 이미지 이름)
void write_ucmatrix(int size_x, int size_y, uchar** ucmatrix, const char* filename)
{
	int i; // 반복문 사용을 위한 정수형 변수 i 선언
	FILE* f; // 파일 기능 사용을 위한 파일 포인터 f 선언

	if ((fopen_s(&f, filename, "wb")) != NULL) // 출력 이미지 파일에 대하여 이진 형식으로 쓰는 옵션이 실패하면
	{
		printf("%s File open Error!\n", filename); // 에러 메시지 출력
		exit(0);								   // 프로그램 종료
	}

	for (i = 0; i < size_y; i++) // y축의 크기까지 아래 코드 반복
	{
		if (fwrite(ucmatrix[i], sizeof(uchar), size_x, f) != size_x) // 행과 열의 크기만큼 출력 이미지 쓰기가 실패하면
		{
			printf("Data Write Error!\n"); // 에러 메시지 출력
			exit(0);					   // 프로그램 종료
		}
	}

	fclose(f); // 파일 기능 종료
}

// Forward Discrete Cosine Transform 함수 선언
// 매개변수 PEL은 Inverse 개념 , Coeff는 Forward 개념으로 취급
void FDCT(int** PEL, int** Coeff)
{
	int x, y, u, v; // 반복문 사용 및 수식 사용을 위한 정수형 변수 선언
	double cv, cu;	// 조건 충족을 위한 실수형 변수 선언
	double N = 8.;  // 실수형 변수 N 선언 , 정수 자리는 8
	double dd = 0.;	// 실수형 변수 dd 선언 , 정수 자리 및 전체 0으로 초기화 , 시그마 역할

	// 4중 반복문 구조 , 전체 사각형 내부의 각 셀들에 대한 구조라고 생각하면 편함(필터 개념)
	for (u = 0; u < 8; u++) // 필터 가로 크기에 대한 반복
	{
		for (v = 0; v < 8; v++) // 필터 세로 크기에 대한 반복
		{
			dd = 0; // 시그마 계산 0으로 초기화

			if (u == 0) // u가 0일 때의 조건 충족
			{
				cu = 1. / sqrt(2); // cu는 루트2분의 1
			}
			else // u가 0보다 클 때의 조건 충족
			{
				cu = 1.; // cu는 1
			}

			if (v == 0) // v가 0일 때의 조건 충족
			{
				cv = 1. / sqrt(2); // cv는 루트2분의 1
			}
			else // v가 0보다 클 때의 조건 충족
			{
				cv = 1.; // cv는 1
			}

			for (x = 0; x < 8; x++) // 필터 셀 가로 개수에 대한 반복
			{
				for (y = 0; y < 8; y++) // 필터 셀 세로 개수에 대한 반복
				{
					//printf("PEL[%d][%d] = %d\n", x, y, PEL[x][y]); // 확인용
					//dd += PEL[x][y] * cos(((2. * (double)x + 1.) * (double)u * M_PI) / 2. * N) * cos(((2. * (double)y + 1.) * (double)v * M_PI) / 2. * N); // 교재 테스트
					dd += PEL[x][y] * cos(((2. * x + 1.) * u * M_PI) / (2. * N))
						* cos(((2. * y + 1.) * v * M_PI) / (2. * N)); // 주어진 fdct 수식을 코드로 나타냄
					//printf("fdct dd[%d][%d] = %lf\n", u, v, dd); // 확인용
				}
			}

			Coeff[u][v] = dd * ((4. * cv * cu) / (2. * N)); // 수식 최종 결과(Forward여서 2N분의 4cvcu를 시그마 계산에 곱함)
			//Coeff[u][v] = dd * (4 * cv * cu) / pow(N, 0.5); // 교재 테스트
		}
	}
}

// Inverse Discrete Cosine Transform 함수 선언
// 매개변수 Coeff는 Forward 개념 , PEL은 Inverse 개념으로 취급
void IDCT(int** Coeff, int** PEL)
{
	int x, y, u, v; // 반복문 사용 및 수식 사용을 위한 정수형 변수 선언
	double cv, cu;	// 조건 충족을 위한 실수형 변수 선언
	double N = 8.;	// 실수형 변수 N 선언 , 정수 자리는 8
	double dd = 0.;	// 실수형 변수 dd 선언 , 정수 자리 및 전체 0으로 초기화 , 시그마 역할

	for (x = 0; x < 8; x++) // 필터 셀 가로 개수에 대한 반복
	{
		for (y = 0; y < 8; y++) // 필터 셀 세로 개수에 대한 반복
		{
			dd = 0; // 시그마 계산 0으로 초기화

			for (u = 0; u < 8; u++) // 필터 가로 크기에 대한 반복
			{
				for (v = 0; v < 8; v++) // 필터 세로 크기에 대한 반복
				{
					if (u == 0) // u가 0일 때의 조건 충족
					{
						cu = 1. / sqrt(2); // cu는 루트2분의 1
					}
					else // u가 0보다 클 때의 조건 충족
					{
						cu = 1.; // cu는 1
					}

					if (v == 0) // v가 0일 때의 조건 충족
					{
						cv = 1. / sqrt(2); // cv는 루트2분의 1
					}
					else // v가 0보다 클 때의 조건 충족
					{
						cv = 1.; // cv는 1
					}

					dd += cv * cu * Coeff[u][v] * cos(((2. * (double)x + 1) * (double)u * M_PI) / (2 * N))
						* cos(((2. * (double)y + 1.) * (double)v * M_PI) / (2. * N)); // 주어진 idct 수식을 코드로 나타냄
				}
			}

			PEL[x][y] = dd * (4. / (2. * N)); // 수식 최종 결과(Inverse여서 2N분의 4를 시그마 계산에 곱함)
			//PEL[x][y] = dd * 4 / pow(N, 0.5); // 교재 테스트
		}
	}
}

// 지그재그 행렬 파악을 위한 함수
int* getZigZagMatrix(int** arr, int n, int m)
{
	// 행렬 파악을 위한 변수 선언
	int row = 0, col = 0;
	int row_inc = 0;
	int array_size = m * n;
	int* w;
	w = (int*)malloc(array_size * sizeof(int));
	int array_counter = 0;

	// 하단 절반 지그재그 패턴의 행렬 출력 파트
	int mn = (m < n) ? m : n;

	for (int len = 1; len <= mn; ++len)
	{
		for (int i = 0; i < len; ++i)
		{
			w[array_counter] = arr[row][col];
			array_counter++;

			if (i + 1 == len)
			{
				break;
			}
			
			if (row_inc) // 만약 row_increment 값이 참인 경우, 
			{
				++row, --col; // 행은 증가, 열은 감소
			}
			else // 그렇지 않은 경우,
			{
				--row, ++col; // 행은 감소, 열은 증가
			}
		}

		if (len == mn)
		{
			break;
		}

		// 마지막 증가치에 따라 행 또는 열 값 갱신
		if (row_inc)
		{
			++row, row_inc = 0;
		}
		else
		{
			++col, row_inc = 1;
		}
	}

	// 행과 열 변수의 인덱스 갱신
	if (row == 0)
	{
		if (col == m - 1)
		{
			++row;
		}
		else
		{
			++col;
		}

		row_inc = 1;
	}
	else
	{
		if (row == n - 1)
		{
			++col;
		}
		else
		{
			++row;
		}

		row_inc = 0;
	}

	// 남은 상단 절반 지그재그 패턴의 행렬 출력 파트
	int MAX = ((m > n) ? m : n) - 1;

	for (int len, diag = MAX; diag > 0; --diag)
	{
		if (diag > mn)
		{
			len = mn;
		}
		else
		{
			len = diag;
		}

		for (int i = 0; i < len; ++i)
		{
			w[array_counter] = arr[row][col];
			array_counter++;

			if (i + 1 == len)
			{
				break;
			}

			if (row_inc) // 만약 row_increment 값이 참인 경우, 
			{
				++row, --col; // 행은 증가, 열은 감소
			}
			else // 그렇지 않은 경우,
			{
				--row, ++col; // 행은 감소, 열은 증가
			}
		}

		// 행과 열 변수의 인덱스 갱신
		if (row == 0 || col == m - 1)
		{
			if (col == m - 1)
			{
				++row;
			}
			else
			{
				++col;
			}

			row_inc = 1;
		}
		else if (col == 0 || row == n - 1)
		{
			if (row == n - 1)
			{
				++col;
			}
			else
			{
				++row;
			}

			row_inc = 0;
		}
	}

	return w;
}

// 지그재그 행렬에서 경로, 순서를 위한 함수
int* getZigZagOrder(int n, int m)
{
	// 행렬 파악을 위한 변수 선언
	int row = 0, col = 0;
	int row_inc = 0;
	int array_size = m * n;
	int* order;
	order = (int*)malloc(array_size * sizeof(int));
	int array_counter = 0;

	// 하단 절반 지그재그 패턴의 행렬 출력 파트
	int mn = (m < n) ? m : n;

	for (int len = 1; len <= mn; ++len)
	{
		for (int i = 0; i < len; ++i)
		{
			order[array_counter] = row * m + col;
			array_counter++;

			if (i + 1 == len)
			{
				break;
			}

			if (row_inc) // 만약 row_increment 값이 참인 경우, 
			{
				++row, --col; // 행은 증가, 열은 감소
			}
			else // 그렇지 않은 경우,
			{
				--row, ++col; // 행은 감소, 열은 증가
			}
		}

		if (len == mn)
		{
			break;
		}

		// 마지막 증가치에 따라 행 또는 열 값 갱신
		if (row_inc)
		{
			++row, row_inc = 0;
		}
		else
		{
			++col, row_inc = 1;
		}
	}

	// 행과 열 변수의 인덱스 갱신
	if (row == 0)
	{
		if (col == m - 1)
		{
			++row;
		}
		else
		{
			++col;
		}
		row_inc = 1;
	}
	else
	{
		if (row == n - 1)
		{
			++col;
		}
		else
		{
			++row;
		}
		row_inc = 0;
	}

	// 남은 상단 절반 지그재그 패턴의 행렬 출력 파트
	int MAX = ((m > n) ? m : n) - 1;

	for (int len, diag = MAX; diag > 0; --diag)
	{

		if (diag > mn)
		{
			len = mn;
		}
		else
		{
			len = diag;
		}

		for (int i = 0; i < len; ++i)
		{
			order[array_counter] = row * m + col;
			array_counter++;

			if (i + 1 == len)
			{
				break;
			}

			if (row_inc) // 만약 row_increment 값이 참인 경우, 
			{
				++row, --col; // 행은 증가, 열은 감소
			}
			else // 그렇지 않은 경우,
			{
				--row, ++col; // 행은 감소, 열은 증가
			}
		}

		// 행과 열 변수의 인덱스 갱신
		if (row == 0 || col == m - 1)
		{
			if (col == m - 1)
			{
				++row;
			}
			else
			{
				++col;
			}

			row_inc = 1;
		}
		else if (col == 0 || row == n - 1)
		{
			if (row == n - 1)
			{
				++col;
			}
			else
			{
				++row;
			}

			row_inc = 0;
		}
	}

	return order;
}

// 메인 함수 정수형 main 선언
int main(int argc, char* argv[])
{
	int i, j, k, l; // 필요한 정수형 변수들과 행렬 변수 선언
	uchar** img, ** outdct; // 입력 영상 변수와 출력 영상 변수 선언(이중 포인터 개념)
	int** i_img, ** i_outimg, ** img8, ** outimg8; // 영상 처리를 위한 변수 선언(이중 포인터 개념)
	int* zig, * scan; // 지그재그를 위한 변수 선언(포인터 개념)

	if (argc != 5) // 사용자 입력 시, 입력 데이터 갯수를 파악하여 메세지를 출력하는 파트, 5이므로 5개 데이터 입력 필요
	{
		printf("\n    입력 데이터 형식 : 프로그램명 입력이미지 행 열 DCT출력이름 \n");
		exit(0); // 프로그램 종료
	}

	Row = atoi(argv[2]); // 입력 데이터 중 2번째는 행
	Col = atoi(argv[3]); // 입력 데이터 중 3번째는 열
	img = uc_alloc(Row, Col); // 입력 영상에 대하여 행렬 크기로 메모리 할당
	outdct = uc_alloc(Row, Col); // DCT 출력 영상에 대하여 행렬 크기로 메모리 할당

	// 영상 처리를 위하여 행렬 크기로 정수 메모리 각각 할당
	i_img = i_alloc(Row, Col);
	i_outimg = i_alloc(Row, Col);
	img8 = i_alloc(Row, Col);
	outimg8 = i_alloc(Row, Col);

	read_ucmartrix(Row, Col, img, argv[1]); // 입력 데이터 중 1번째는 입력 영상, 해당 영상을 읽기

	for (i = 0; i < Row; i++) // 행의 크기만큼 반복(이미지의 x축)
	{
		for (j = 0; j < Col; j++) // 열의 크기만큼 반복(이미지의 y축)
		{
			i_img[i][j] = img[i][j]; // 입력 이미지 저장
		}
	}

	// FDCT
	for (i = 0; i < Row; i += 8) // 행의 크기까지 8만큼 반복
	{
		for (j = 0; j < Col; j += 8) // 열의 크기까지 8만큼 반복
		{
			printf("FDCT img[%d][%d] \n", i, j); // 처리 확인용 메시지 출력

			// 8조각 내기
			for (k = 0; k < 8; k++)
			{
				for (l = 0; l < 8; l++)
				{
					img8[k][l] = i_img[i + k][j + l];
					//printf("img8[%d][%d] = %d \n", k, l, img8[k][l]); // 확인용
				}
			}

			FDCT(img8, outimg8); // FDCT 처리

			// 결과 저장
			for (k = 0; k < 8; k++)
			{
				for (l = 0; l < 8; l++)
				{
					i_outimg[i + k][j + l] = outimg8[k][l];
				}
			}
		}
	}

	zig = (int*)calloc(9, sizeof(int)); // 지그재그 행렬을 저장할 변수 선언
	zig = getZigZagMatrix(img8, 8, 8);	// 지그재그 행렬 파악하여 저장
	scan = getZigZagOrder(8, 8);		// 지그재그 행렬 스캔하여 순서

	// IDCT
	for (i = 0; i < Row; i += 8) // 행의 크기까지 8만큼 반복
	{
		for (j = 0; j < Col; j += 8) // 열의 크기까지 8만큼 반복
		{
			printf("IDCT img[%d][%d] \n", i, j); // 처리 확인용 메시지 출력

			// 8조각 처리
			for (k = 0; k < 8; k++)
			{
				for (l = 0; l < 8; l++)
				{
					img8[k][l] = i_outimg[i + k][j + l];
				}
			}

			IDCT(img8, outimg8); // IDCT 처리

			// 결과 저장
			for (k = 0; k < 8; k++)
			{
				for (l = 0; l < 8; l++)
				{
					i_outimg[i + k][j + l] = outimg8[k][l];
				}
			}
		}
	}

	for (i = 0; i < Row; i++) // 행의 크기만큼 반복(이미지의 x축)
	{
		for (j = 0; j < Col; j++) // 열의 크기만큼 반복(이미지의 y축)
		{
			outdct[i][j] = i_outimg[i][j]; // 출력 이미지 저장
		}
	}

	write_ucmatrix(Col, Row, outdct, argv[4]); // 입력 데이터 중 4번째를 이름으로, DCT 처리 영상을 쓰기

	// 지그재그 파악 결과 = 영상의 지그재그를 위한 값 파악
	printf("\n\nzigzag\n\n");

	for (i = 0; i < 66; i++) // 512 -> 134? 256 -> 66?
	{
		printf("%d ", zig[i]); // 아무리 생각해도 여기 값은 뭔가 이상한거 같다.. ㅠ_ㅠ
	}

	// 지그재그 스캔 결과 = 지그재그 순서를 위하여 스캔
	printf("\n\nscan\n\n");

	for (j = 0; j < 64; j++)
	{
		printf("%d ", scan[j]);
	}

	printf("\n");

	return 0; // 프로그램 종료
}
