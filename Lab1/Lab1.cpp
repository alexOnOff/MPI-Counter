#include <cstdlib>
#include <iostream>
#include <ctime>
#include "mpi.h"

using namespace std;

int main(int argc, char** argv) {
    srand(time(0));
    int rank; // ранг процесса
    int size; // количество процессов
    int* arr = (int*)malloc(sizeof(int) * 1); // массив (из одного элемента) отправляемый приемнику
    int* res = (int*)malloc(sizeof(int) * 1); // результат приема
    int count = 0; // счетчик принятых сообщений
    int* mode = (int*)malloc(sizeof(int)*1); // режим работы процессов 1 - посылаем/принимаем, 0 - заканчиваем

    MPI_Init(&argc, &argv); // инициализируем процессы

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    mode[0] = 1; // устанавливаем режим

    while(mode[0])
    {
        arr[0] = rand() % 6 - 1; // случайное целочисленное сообщение от -1 до 3
        if (rank == 0) { 

            MPI_Reduce(arr, res, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);  // для нулевого процесса устанавливаем прием от других, из принятых берем минимальное значения
            cout << "process: " << rank << " recv value: ";
            cout << res[0] << " , count - " << count << " ";   // пришло сообщение - печатаем его и счетчик
            cout << endl;
            if (res[0] == -1) { // если сообщение -1
                mode[0] = 0; // выходим из цикла
            }
            count++;
            MPI_Bcast(mode, 1, MPI_INT, 0, MPI_COMM_WORLD); // отправляем сообщение всем потокам о возможности продолжения работы
        }                                                                       
        else
        {
            MPI_Reduce(arr, res, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD); // отправка сообщений от всех потоков одному
            MPI_Bcast(mode, 1, MPI_INT, 0, MPI_COMM_WORLD); // прием ответа от получателя, можно ли продолжать работу
        }
    }

    free(arr); // очищаем
    free(res);

    MPI_Finalize(); // завершаем
    return 0;
}


