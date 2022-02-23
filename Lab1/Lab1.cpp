#include <cstdlib>
#include <iostream>
#include <ctime>
#include "mpi.h"

using namespace std;

int main(int argc, char** argv) {
    srand(time(0));
    int rank; // ���� ��������
    int size; // ���������� ���������
    int* arr = (int*)malloc(sizeof(int) * 1); // ������ (�� ������ ��������) ������������ ���������
    int* res = (int*)malloc(sizeof(int) * 1); // ��������� ������
    int count = 0; // ������� �������� ���������
    int* mode = (int*)malloc(sizeof(int)*1); // ����� ������ ��������� 1 - ��������/���������, 0 - �����������

    MPI_Init(&argc, &argv); // �������������� ��������

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    mode[0] = 1; // ������������� �����

    while(mode[0])
    {
        arr[0] = rand() % 6 - 1; // ��������� ������������� ��������� �� -1 �� 3
        if (rank == 0) { 

            MPI_Reduce(arr, res, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);  // ��� �������� �������� ������������� ����� �� ������, �� �������� ����� ����������� ��������
            cout << "process: " << rank << " recv value: ";
            cout << res[0] << " , count - " << count << " ";   // ������ ��������� - �������� ��� � �������
            cout << endl;
            if (res[0] == -1) { // ���� ��������� -1
                mode[0] = 0; // ������� �� �����
            }
            count++;
            MPI_Bcast(mode, 1, MPI_INT, 0, MPI_COMM_WORLD); // ���������� ��������� ���� ������� � ����������� ����������� ������
        }                                                                       
        else
        {
            MPI_Reduce(arr, res, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD); // �������� ��������� �� ���� ������� ������
            MPI_Bcast(mode, 1, MPI_INT, 0, MPI_COMM_WORLD); // ����� ������ �� ����������, ����� �� ���������� ������
        }
    }

    free(arr); // �������
    free(res);

    MPI_Finalize(); // ���������
    return 0;
}


