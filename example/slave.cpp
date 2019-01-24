#include "../MPMD.hpp"

int main(int argc, char *argv[])
{
   MPMDHelper MPMD(true);

   printf("DEBUG: SLAVE: Initializing MPI...\n");
   MPI_Init(&argc, &argv);
   printf("DEBUG: SLAVE: Initializing MPMD...\n");
   MPMD.Init(MPI_COMM_WORLD, "SLAVE");

   printf("DEBUG: SLAVE: Identify...\n");
   MPMD.Identify();

   printf("DEBUG: SLAVE: Finalize MPI...\n");
   MPI_Finalize();
   return 0;
}
