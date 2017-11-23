#include "../MPMD.hpp"

int main(int argc, char *argv[])
{
   MPMDHelper MPMD;

   MPI_Init(&argc, &argv);
   MPMD.Init(MPI_COMM_WORLD, "SLAVE");

   MPMD.Identify();

   MPI_Finalize();
   return 0;
}
