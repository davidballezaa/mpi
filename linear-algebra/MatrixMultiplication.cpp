#include <iostream>
#include <cmath>
#include <cassert>
#include <mpi.h>
#include <vector>

using namespace std;

// function to initialize matrix with random values
void initializeMatrix(vector<vector<double>> &matrix, int rows, int cols){
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            matrix[i][j] = rand()%10; // random values between 0-9
        }
    }
}


// function to print matrix
void printMatrix(vector<vector<double>> &matrix, int rows, int cols){
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            cout << matrix[i][j] << " ";
        }
        cout << '\n';
    }
}

int main(int argc, char* argv[]){
    /*
     A = [1 2 3   => dim(A) = 2x3 [MxN]
          4 5 6] 
     B = [10 11
          20 21   => dim(B) = 3x2 [NxP]
          30 31]
    --

    */
    const int M = 2; // number of rows matrix A
    const int N = 3; // number of columns matrix A & rows matrix B
    const int P = 2; // number of columns matrix B

    MPI::Init(argc, argv);
    int num_procs = MPI::COMM_WORLD.Get_size();
    int rank = MPI::COMM_WORLD.Get_rank();

    /*
    program will execute by dividing A in M rows
    that will require M number of processes
    */
    assert(num_procs == M);

    // Initialize matrix A, B and C (result)
    //  A   *  B 
    // MxN    NxP
    // dim(C) = MxP

    vector<vector<double>> A(M, vector<double>(N));
    vector<vector<double>> B(N, vector<double>(P));
    vector<vector<double>> C(M, vector<double>(P));
    // double A[M][N], B[N][P], C[M][P];

    if(rank == 0){
        initializeMatrix(A, M, N);
        initializeMatrix(B, N, P);
    }

    // Send B matrix to all processes
    // Bcast(void* buffer, int count, const MPI::Datatype datatype, int root)
    MPI::COMM_WORLD.Bcast(&B[0][0], N*P, MPI::DOUBLE, 0);

    // Scatter matrix A rows through processes
    double myRow[N];
    // Scatter(void* sendbuf, int sendcount, MPI::Datatype sendtype, void* recvbuf, int recvcount, MPI::Datatype recvtype, int root)
    MPI::COMM_WORLD.Scatter(&A[0][0], N, MPI::DOUBLE, myRow, N, MPI::DOUBLE, 0);
    /*
        BCAST AND SCATTER DONT WORK, THEY JUST SEND FIRST PART 
    */

    // Perform local matrix multiplication
    vector<double> myResult(P);
    for(int i=0; i<P; i++){
        for(int j=0; j<N; j++){
            myResult[i] += myRow[j]*B[j][i];
        }
    }

    // Gather all results
    MPI::COMM_WORLD.Gather(&myResult[0], P, MPI::DOUBLE, &C[0][0], P, MPI::DOUBLE, 0);

    if(rank == 0){
        cout << "Matrix A: " << '\n';
        printMatrix(A, M, N);

        cout << "Matrix B: " << '\n';
        printMatrix(B, N, P);

        cout << "Matrix C: " << '\n';
        printMatrix(C, M, P);
    }


    MPI::Finalize();

    return 0;
}