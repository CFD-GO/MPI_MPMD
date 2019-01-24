#include "../MPMD.hpp"

int main(int argc, char *argv[])
{
   MPMDHelper MPMD;
   
   printf("WORKER_SPAWN: Initializing MPI...\n");
   MPI_Init(&argc, &argv);
   
   std::vector<int> excl;
   excl.push_back(0); // Exclude process 0 from the working group.
   
   printf("WORKER_SPAWN: Initializing MPMD...\n");
   MPMD.Init(MPI_COMM_WORLD, "WORKER_SPAWN", excl);
   
   for (int i=1; i<argc; i++) {
      int n = atoi(argv[i]);
      i++;
      if (i >= argc) return -1;
      if (MPMD.in_work) {
         printf("WORKER_SPAWN: Spawning %dx%s...\n",n,argv[i]);
         MPMD.Spawn(argv[i], MPI_ARGV_NULL, n, MPI_INFO_NULL, true);
      }
   }

   printf("WORKER_SPAWN: Identify...\n");
   MPMD.Identify();

   printf("WORKER_SPAWN: Finalize MPI...\n");
   MPI_Finalize();
   return 0;
}
