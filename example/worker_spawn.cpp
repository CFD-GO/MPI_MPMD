#include "../MPMD.hpp"

int main(int argc, char *argv[])
{
   MPMDHelper MPMD;
   
   MPI_Init(&argc, &argv);
   
   std::vector<int> excl;
   excl.push_back(0); // Exclude process 0 from the working group.
   
   MPMD.Init(MPI_COMM_WORLD, "WORKER_SPAWN", excl);
   
   for (int i=1; i<argc; i++) {
      int n = atoi(argv[i]);
      i++;
      if (i >= argc) return -1;
      if (MPMD.in_work) {
         MPMD.Spawn(argv[i], MPI_ARGV_NULL, n, MPI_INFO_NULL, true);
      }
   }

   MPMD.Identify();

   MPI_Finalize();
   return 0;
}
