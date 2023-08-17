#include <iostream>
#include <mpi.h>
#include <cmath>

// Using Leibniz series sum to aproximate pi
int main(int argc, char* argv[]){
    MPI::Init(argc, argv);

    int max_n = 5000; 
    double local_sum = 0;

    int num_procs = MPI::COMM_WORLD.Get_size();
    int rank = MPI::COMM_WORLD.Get_rank();

    for(int n=rank; n<max_n; n+=num_procs){
        double temp = 1 / (2 * ((double)(n)) + 1);
        if(n % 2 == 0){
            local_sum += temp;
        } else {
            local_sum -= temp;
        }
    }


    double global_sum;
    MPI::COMM_WORLD.Reduce(&local_sum, &global_sum, 1, MPI::DOUBLE, MPI::SUM, 0);
    if(rank == 0){
        std::cout << "Aproximation of Pi is : " << 4*global_sum << '\n';
    }

    MPI::Finalize();
    return 0;
}
