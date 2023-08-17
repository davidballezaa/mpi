#include <iostream>
#include <mpi.h>

int main(int argc, char* argv[]){
    MPI::Init(argc, argv);
    int tag = 30;
    if (MPI::COMM_WORLD.Get_rank() == 0){
        double send_buffer[2] = {100.0, 200.0};
        MPI::COMM_WORLD.Send(send_buffer, 2, MPI::DOUBLE, 1, tag);
        std::cout << "I'm process: " << MPI::COMM_WORLD.Get_rank() << '\n';
        std::cout << "Just sent the message" << '\n';
    }
    if (MPI::COMM_WORLD.Get_rank() == 1){
        double recv_buffer[2] = {0.0, 0.0};
        MPI::COMM_WORLD.Recv(recv_buffer, 2, MPI::DOUBLE, MPI::ANY_SOURCE, MPI::ANY_TAG);

        std::cout << "I'm process: " << MPI::COMM_WORLD.Get_rank() << '\n';
        std::cout << "Just received the message" << '\n';
        std::cout << recv_buffer[0] << '\n';
        std::cout << recv_buffer[1] << '\n';
    }
    MPI::Finalize();

    return 0;
}
