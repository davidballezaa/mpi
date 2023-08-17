#include <iostream>
#include <mpi.h>

int main(int argc, char* argv[]){
    MPI::Init(argc, argv);
    int rank = MPI::COMM_WORLD.Get_rank();
    int num_procs = MPI::COMM_WORLD.Get_size();
    int tag = 30;
    int right_rank = rank + 1;
    if(rank == num_procs - 1){
        right_rank = MPI::PROC_NULL;
    }
    int left_rank = rank - 1;
    if(rank == 0){
        left_rank = MPI::PROC_NULL;
    }
    int recv_data = 99;

    MPI::COMM_WORLD.Sendrecv(&rank, 1, MPI::INT, right_rank, tag, &recv_data, 1, MPI::INT, left_rank, MPI::ANY_TAG);

    std::cout << "Process " << rank << " received from " << recv_data << '\n';

    MPI::Finalize();
    return 0;
}
