#include <mpi.h>
#include <cmath>
#include <cassert>

class MpiVector
{
    private:
        int mLo, mHi, mSize;
        double* mData;
        double* mGlobalData;
    public:
        MpiVector(int vecSize){
            int num_procs = MPI::COMM_WORLD.Get_size();
            int rank = MPI::COMM_WORLD.Get_rank();
            int ideal_local_size = vecSize / num_procs;

            assert(ideal_local_size > 0);
            mLo = ideal_local_size*rank;
            mHi = ideal_local_size*(rank + 1);

            if(rank == num_procs - 1){
                mHi = vecSize;
            }

            assert(mHi > mLo);
            mData = new double[mHi - mLo];
            mGlobalData = new double[vecSize];
            mSize = vecSize;
        }

        ~MpiVector() {
            delete[] mData;
            delete[] mGlobalData;
        }

        // local vector index conversion from global index
        double& operator[](int globalIndex){
            assert(mLo <= globalIndex && globalIndex < mHi);
            return mData[globalIndex-mLo];
        }
};