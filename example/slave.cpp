#include "../MPMD.hpp"

int main(int argc, char *argv[])
{
   MPMDHelper MPMD;

   printf("SLAVE: Initializing MPI...\n");
   MPI_Init(&argc, &argv);
   printf("SLAVE: Initializing MPMD...\n");
   MPMD.Init(MPI_COMM_WORLD, "SLAVE");

   printf("SLAVE: Identify...\n");
   MPMD.Identify();

   printf("SLAVE: Finalize MPI...\n");
   MPI_Finalize();
   return 0;
}
