#include <iostream>
#include <mpi.h>

int main(int argc, char* argv[]){
    MPI::Init(argc, argv);

    int tag = 30;
    int rank = MPI::COMM_WORLD.Get_rank();
    int num_procs = MPI::COMM_WORLD.Get_size();

    int left_rank = (rank-1 + num_procs) % num_procs;
    int right_rank = (rank+1) % num_procs;
    int recv_data;

    /*
    Sendrecv prototype: 

    void Comm::Sendrecv(const void *sendbuf, int sendcount,
            const Datatype& sendtype,
            int dest, int sendtag,
            void *recvbuf, int recvcount,
            const Datatype& recvtype,
            int source, int recvtag) const
    */
    
    // Ring communication ...->0->1->2...
    MPI::COMM_WORLD.Sendrecv(&rank, 1, MPI::INT, right_rank, tag, &recv_data, 1, MPI::INT, left_rank, tag); 
    std::cout << "Process " << rank << " received from " << recv_data << '\n';

    MPI::Finalize();
    return 0;
}
