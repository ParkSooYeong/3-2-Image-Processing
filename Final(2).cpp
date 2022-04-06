/* SKU CoE ITE - ParkSooYoung , 2021-02 ImageProcessing : Final(2) */

// Warning C4996 Error ������Ʈ �Ӽ����� ��ó�� �Ϸ�

// ��ó���� ��Ʈ
#include <stdio.h> // ǥ������� �Լ� ���̺귯�� ������� ����
#include <stdlib.h> // ǥ�� �Լ� ���̺귯�� ������� ����

#define _USE_MATH_DEFINES //  math.h ȣ�� �� �� ��Ȯ�� Pi �� ����� ���� �ڵ�
#include <math.h> // ���� �Լ� ���̺귯�� ������� ����

typedef unsigned char uchar; // unsigned char Ÿ���� uchar�� ������

int Row, Col; // ������ ���� ���� ��, �� ����

// �޸� �Ҵ� �Լ� uc_alloc ����
uchar** uc_alloc(int size_x, int size_y) // �Ű������� x��(��)�� ũ��� y��(��)�� ũ��
{
	uchar** m; // �޸� �Ҵ� �Ǻ��� ���� ���� ������ ���� m ����
	int i; // �ݺ��� ����� ���� ������ ���� i ����

	if ((m = (uchar**)calloc(size_y, sizeof(uchar*))) == NULL) // y�࿡ ���Ͽ� �޸� �Ҵ��� �����ϸ�
	{
		printf("d_alloc error 1\7\n"); // ���� �޽��� ���
		exit(0);					   // ���α׷� ����
	}

	for (i = 0; i < size_y; i++) // y���� ũ����� �Ʒ� �ڵ� �ݺ�
	{
		if ((m[i] = (uchar*)calloc(size_x, sizeof(uchar))) == NULL) // x�࿡ ���Ͽ� �޸� �Ҵ��� �����ϸ�
		{
			printf("d_alloc error 2\7\n"); // ���� �޽��� ���
			exit(0);					   // ���α׷� ����
		}
	}

	return m; // �޸� �Ҵ� �Ǻ��� ��ġ�� ��� ��ȯ
}

// ������ �޸� �Ҵ� �Լ� i_alloc ���� 
int** i_alloc(int size_x, int size_y) // �Ű������� x��(��)�� ũ��� y��(��)�� ũ��
{
	int** m; // �޸� �Ҵ� �Ǻ��� ���� ���� ������ ���� m ����
	int i; // �ݺ��� ����� ���� ������ ���� i ����

	if ((m = (int**)calloc(size_y, sizeof(int*))) == NULL) // y�࿡ ���Ͽ� �޸� �Ҵ��� �����ϸ�
	{
		printf("i_alloc error 1\7\n"); // ���� �޽��� ���
		exit(0);					   // ���α׷� ����
	}

	for (i = 0; i < size_y; i++) // y���� ũ����� �Ʒ� �ڵ� �ݺ�
	{
		if ((m[i] = (int*)calloc(size_x, sizeof(int))) == NULL) // x�࿡ ���Ͽ� �޸� �Ҵ��� �����ϸ�
		{
			printf("i_alloc error 2\7\n"); // ���� �޽��� ���
			exit(0);					   // ���α׷� ����
		}
	}

	return m; // �޸� �Ҵ� �Ǻ��� ��ġ�� ��� ��ȯ
}

// ���� �д� �Լ� read_ucmatrix ����
// �Ű������� ��ũ��, ��ũ��, (�Է�)�̹��� ����, ���� �̸�(�ƱԸ�Ʈ�� �޴� �Է� �̹��� �̸�)
void read_ucmartrix(int size_x, int size_y, uchar** ucmatrix, const char* filename)
{
	int i;	 // �ݺ��� ����� ���� ������ ���� i ����
	FILE* f; // ���� ��� ����� ���� ���� ������ f ����

	if ((fopen_s(&f, filename, "rb")) != NULL) // �Է� �̹��� ���Ͽ� ���Ͽ� ���� �������� �д� �ɼ��� �����ϸ�
	{
		printf("%s File open Error!\n", filename); // ���� �޽��� ���
		exit(0);								   // ���α׷� ����
	}

	for (i = 0; i < size_y; i++) // y���� ũ����� �Ʒ� �ڵ� �ݺ�
	{
		if (fread(ucmatrix[i], sizeof(uchar), size_x, f) != size_x) // ��� ���� ũ�⸸ŭ �Է� �̹��� �бⰡ �����ϸ�
		{
			printf("Data Read Error!\n"); // ���� �޽��� ���
			exit(0);					  // ���α׷� ����
		}
	}

	fclose(f); // ���� ��� ����
}

// ���� ���� �Լ� write_ucmatrix ����
// �Ű������� ��ũ��, ��ũ��, (���)�̹��� ����, ���� �̸�(�ƱԸ�Ʈ�� �޴� ��� �̹��� �̸�)
void write_ucmatrix(int size_x, int size_y, uchar** ucmatrix, const char* filename)
{
	int i; // �ݺ��� ����� ���� ������ ���� i ����
	FILE* f; // ���� ��� ����� ���� ���� ������ f ����

	if ((fopen_s(&f, filename, "wb")) != NULL) // ��� �̹��� ���Ͽ� ���Ͽ� ���� �������� ���� �ɼ��� �����ϸ�
	{
		printf("%s File open Error!\n", filename); // ���� �޽��� ���
		exit(0);								   // ���α׷� ����
	}

	for (i = 0; i < size_y; i++) // y���� ũ����� �Ʒ� �ڵ� �ݺ�
	{
		if (fwrite(ucmatrix[i], sizeof(uchar), size_x, f) != size_x) // ��� ���� ũ�⸸ŭ ��� �̹��� ���Ⱑ �����ϸ�
		{
			printf("Data Write Error!\n"); // ���� �޽��� ���
			exit(0);					   // ���α׷� ����
		}
	}

	fclose(f); // ���� ��� ����
}

// Forward Discrete Cosine Transform �Լ� ����
// �Ű����� PEL�� Inverse ���� , Coeff�� Forward �������� ���
void FDCT(int** PEL, int** Coeff)
{
	int x, y, u, v; // �ݺ��� ��� �� ���� ����� ���� ������ ���� ����
	double cv, cu;	// ���� ������ ���� �Ǽ��� ���� ����
	double N = 8.;  // �Ǽ��� ���� N ���� , ���� �ڸ��� 8
	double dd = 0.;	// �Ǽ��� ���� dd ���� , ���� �ڸ� �� ��ü 0���� �ʱ�ȭ , �ñ׸� ����

	// 4�� �ݺ��� ���� , ��ü �簢�� ������ �� ���鿡 ���� ������� �����ϸ� ����(���� ����)
	for (u = 0; u < 8; u++) // ���� ���� ũ�⿡ ���� �ݺ�
	{
		for (v = 0; v < 8; v++) // ���� ���� ũ�⿡ ���� �ݺ�
		{
			dd = 0; // �ñ׸� ��� 0���� �ʱ�ȭ

			if (u == 0) // u�� 0�� ���� ���� ����
			{
				cu = 1. / sqrt(2); // cu�� ��Ʈ2���� 1
			}
			else // u�� 0���� Ŭ ���� ���� ����
			{
				cu = 1.; // cu�� 1
			}

			if (v == 0) // v�� 0�� ���� ���� ����
			{
				cv = 1. / sqrt(2); // cv�� ��Ʈ2���� 1
			}
			else // v�� 0���� Ŭ ���� ���� ����
			{
				cv = 1.; // cv�� 1
			}

			for (x = 0; x < 8; x++) // ���� �� ���� ������ ���� �ݺ�
			{
				for (y = 0; y < 8; y++) // ���� �� ���� ������ ���� �ݺ�
				{
					//printf("PEL[%d][%d] = %d\n", x, y, PEL[x][y]); // Ȯ�ο�
					//dd += PEL[x][y] * cos(((2. * (double)x + 1.) * (double)u * M_PI) / 2. * N) * cos(((2. * (double)y + 1.) * (double)v * M_PI) / 2. * N); // ���� �׽�Ʈ
					dd += PEL[x][y] * cos(((2. * x + 1.) * u * M_PI) / (2. * N))
						* cos(((2. * y + 1.) * v * M_PI) / (2. * N)); // �־��� fdct ������ �ڵ�� ��Ÿ��
					//printf("fdct dd[%d][%d] = %lf\n", u, v, dd); // Ȯ�ο�
				}
			}

			Coeff[u][v] = dd * ((4. * cv * cu) / (2. * N)); // ���� ���� ���(Forward���� 2N���� 4cvcu�� �ñ׸� ��꿡 ����)
			//Coeff[u][v] = dd * (4 * cv * cu) / pow(N, 0.5); // ���� �׽�Ʈ
		}
	}
}

// Inverse Discrete Cosine Transform �Լ� ����
// �Ű����� Coeff�� Forward ���� , PEL�� Inverse �������� ���
void IDCT(int** Coeff, int** PEL)
{
	int x, y, u, v; // �ݺ��� ��� �� ���� ����� ���� ������ ���� ����
	double cv, cu;	// ���� ������ ���� �Ǽ��� ���� ����
	double N = 8.;	// �Ǽ��� ���� N ���� , ���� �ڸ��� 8
	double dd = 0.;	// �Ǽ��� ���� dd ���� , ���� �ڸ� �� ��ü 0���� �ʱ�ȭ , �ñ׸� ����

	for (x = 0; x < 8; x++) // ���� �� ���� ������ ���� �ݺ�
	{
		for (y = 0; y < 8; y++) // ���� �� ���� ������ ���� �ݺ�
		{
			dd = 0; // �ñ׸� ��� 0���� �ʱ�ȭ

			for (u = 0; u < 8; u++) // ���� ���� ũ�⿡ ���� �ݺ�
			{
				for (v = 0; v < 8; v++) // ���� ���� ũ�⿡ ���� �ݺ�
				{
					if (u == 0) // u�� 0�� ���� ���� ����
					{
						cu = 1. / sqrt(2); // cu�� ��Ʈ2���� 1
					}
					else // u�� 0���� Ŭ ���� ���� ����
					{
						cu = 1.; // cu�� 1
					}

					if (v == 0) // v�� 0�� ���� ���� ����
					{
						cv = 1. / sqrt(2); // cv�� ��Ʈ2���� 1
					}
					else // v�� 0���� Ŭ ���� ���� ����
					{
						cv = 1.; // cv�� 1
					}

					dd += cv * cu * Coeff[u][v] * cos(((2. * (double)x + 1) * (double)u * M_PI) / (2 * N))
						* cos(((2. * (double)y + 1.) * (double)v * M_PI) / (2. * N)); // �־��� idct ������ �ڵ�� ��Ÿ��
				}
			}

			PEL[x][y] = dd * (4. / (2. * N)); // ���� ���� ���(Inverse���� 2N���� 4�� �ñ׸� ��꿡 ����)
			//PEL[x][y] = dd * 4 / pow(N, 0.5); // ���� �׽�Ʈ
		}
	}
}

// ������� ��� �ľ��� ���� �Լ�
int* getZigZagMatrix(int** arr, int n, int m)
{
	// ��� �ľ��� ���� ���� ����
	int row = 0, col = 0;
	int row_inc = 0;
	int array_size = m * n;
	int* w;
	w = (int*)malloc(array_size * sizeof(int));
	int array_counter = 0;

	// �ϴ� ���� ������� ������ ��� ��� ��Ʈ
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
			
			if (row_inc) // ���� row_increment ���� ���� ���, 
			{
				++row, --col; // ���� ����, ���� ����
			}
			else // �׷��� ���� ���,
			{
				--row, ++col; // ���� ����, ���� ����
			}
		}

		if (len == mn)
		{
			break;
		}

		// ������ ����ġ�� ���� �� �Ǵ� �� �� ����
		if (row_inc)
		{
			++row, row_inc = 0;
		}
		else
		{
			++col, row_inc = 1;
		}
	}

	// ��� �� ������ �ε��� ����
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

	// ���� ��� ���� ������� ������ ��� ��� ��Ʈ
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

			if (row_inc) // ���� row_increment ���� ���� ���, 
			{
				++row, --col; // ���� ����, ���� ����
			}
			else // �׷��� ���� ���,
			{
				--row, ++col; // ���� ����, ���� ����
			}
		}

		// ��� �� ������ �ε��� ����
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

// ������� ��Ŀ��� ���, ������ ���� �Լ�
int* getZigZagOrder(int n, int m)
{
	// ��� �ľ��� ���� ���� ����
	int row = 0, col = 0;
	int row_inc = 0;
	int array_size = m * n;
	int* order;
	order = (int*)malloc(array_size * sizeof(int));
	int array_counter = 0;

	// �ϴ� ���� ������� ������ ��� ��� ��Ʈ
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

			if (row_inc) // ���� row_increment ���� ���� ���, 
			{
				++row, --col; // ���� ����, ���� ����
			}
			else // �׷��� ���� ���,
			{
				--row, ++col; // ���� ����, ���� ����
			}
		}

		if (len == mn)
		{
			break;
		}

		// ������ ����ġ�� ���� �� �Ǵ� �� �� ����
		if (row_inc)
		{
			++row, row_inc = 0;
		}
		else
		{
			++col, row_inc = 1;
		}
	}

	// ��� �� ������ �ε��� ����
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

	// ���� ��� ���� ������� ������ ��� ��� ��Ʈ
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

			if (row_inc) // ���� row_increment ���� ���� ���, 
			{
				++row, --col; // ���� ����, ���� ����
			}
			else // �׷��� ���� ���,
			{
				--row, ++col; // ���� ����, ���� ����
			}
		}

		// ��� �� ������ �ε��� ����
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

// ���� �Լ� ������ main ����
int main(int argc, char* argv[])
{
	int i, j, k, l; // �ʿ��� ������ ������� ��� ���� ����
	uchar** img, ** outdct; // �Է� ���� ������ ��� ���� ���� ����(���� ������ ����)
	int** i_img, ** i_outimg, ** img8, ** outimg8; // ���� ó���� ���� ���� ����(���� ������ ����)
	int* zig, * scan; // ������׸� ���� ���� ����(������ ����)

	if (argc != 5) // ����� �Է� ��, �Է� ������ ������ �ľ��Ͽ� �޼����� ����ϴ� ��Ʈ, 5�̹Ƿ� 5�� ������ �Է� �ʿ�
	{
		printf("\n    �Է� ������ ���� : ���α׷��� �Է��̹��� �� �� DCT����̸� \n");
		exit(0); // ���α׷� ����
	}

	Row = atoi(argv[2]); // �Է� ������ �� 2��°�� ��
	Col = atoi(argv[3]); // �Է� ������ �� 3��°�� ��
	img = uc_alloc(Row, Col); // �Է� ���� ���Ͽ� ��� ũ��� �޸� �Ҵ�
	outdct = uc_alloc(Row, Col); // DCT ��� ���� ���Ͽ� ��� ũ��� �޸� �Ҵ�

	// ���� ó���� ���Ͽ� ��� ũ��� ���� �޸� ���� �Ҵ�
	i_img = i_alloc(Row, Col);
	i_outimg = i_alloc(Row, Col);
	img8 = i_alloc(Row, Col);
	outimg8 = i_alloc(Row, Col);

	read_ucmartrix(Row, Col, img, argv[1]); // �Է� ������ �� 1��°�� �Է� ����, �ش� ������ �б�

	for (i = 0; i < Row; i++) // ���� ũ�⸸ŭ �ݺ�(�̹����� x��)
	{
		for (j = 0; j < Col; j++) // ���� ũ�⸸ŭ �ݺ�(�̹����� y��)
		{
			i_img[i][j] = img[i][j]; // �Է� �̹��� ����
		}
	}

	// FDCT
	for (i = 0; i < Row; i += 8) // ���� ũ����� 8��ŭ �ݺ�
	{
		for (j = 0; j < Col; j += 8) // ���� ũ����� 8��ŭ �ݺ�
		{
			printf("FDCT img[%d][%d] \n", i, j); // ó�� Ȯ�ο� �޽��� ���

			// 8���� ����
			for (k = 0; k < 8; k++)
			{
				for (l = 0; l < 8; l++)
				{
					img8[k][l] = i_img[i + k][j + l];
					//printf("img8[%d][%d] = %d \n", k, l, img8[k][l]); // Ȯ�ο�
				}
			}

			FDCT(img8, outimg8); // FDCT ó��

			// ��� ����
			for (k = 0; k < 8; k++)
			{
				for (l = 0; l < 8; l++)
				{
					i_outimg[i + k][j + l] = outimg8[k][l];
				}
			}
		}
	}

	zig = (int*)calloc(9, sizeof(int)); // ������� ����� ������ ���� ����
	zig = getZigZagMatrix(img8, 8, 8);	// ������� ��� �ľ��Ͽ� ����
	scan = getZigZagOrder(8, 8);		// ������� ��� ��ĵ�Ͽ� ����

	// IDCT
	for (i = 0; i < Row; i += 8) // ���� ũ����� 8��ŭ �ݺ�
	{
		for (j = 0; j < Col; j += 8) // ���� ũ����� 8��ŭ �ݺ�
		{
			printf("IDCT img[%d][%d] \n", i, j); // ó�� Ȯ�ο� �޽��� ���

			// 8���� ó��
			for (k = 0; k < 8; k++)
			{
				for (l = 0; l < 8; l++)
				{
					img8[k][l] = i_outimg[i + k][j + l];
				}
			}

			IDCT(img8, outimg8); // IDCT ó��

			// ��� ����
			for (k = 0; k < 8; k++)
			{
				for (l = 0; l < 8; l++)
				{
					i_outimg[i + k][j + l] = outimg8[k][l];
				}
			}
		}
	}

	for (i = 0; i < Row; i++) // ���� ũ�⸸ŭ �ݺ�(�̹����� x��)
	{
		for (j = 0; j < Col; j++) // ���� ũ�⸸ŭ �ݺ�(�̹����� y��)
		{
			outdct[i][j] = i_outimg[i][j]; // ��� �̹��� ����
		}
	}

	write_ucmatrix(Col, Row, outdct, argv[4]); // �Է� ������ �� 4��°�� �̸�����, DCT ó�� ������ ����

	// ������� �ľ� ��� = ������ ������׸� ���� �� �ľ�
	printf("\n\nzigzag\n\n");

	for (i = 0; i < 66; i++) // 512 -> 134? 256 -> 66?
	{
		printf("%d ", zig[i]); // �ƹ��� �����ص� ���� ���� ���� �̻��Ѱ� ����.. ��_��
	}

	// ������� ��ĵ ��� = ������� ������ ���Ͽ� ��ĵ
	printf("\n\nscan\n\n");

	for (j = 0; j < 64; j++)
	{
		printf("%d ", scan[j]);
	}

	printf("\n");

	return 0; // ���α׷� ����
}
