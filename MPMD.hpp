#include <stdlib.h>
#include <vector>
#include <string>
#include <mpi.h>

struct MPMDIntercomm {
   MPI_Comm local;
   MPI_Comm work;
   int local_size;
   int work_size;
   bool in_world;
   std::string name;
};

class MPMDHelper {
   static char * char_vec_from_string( std::string str, int size) {
      char * ret = new char[size];
      int i=0;
      for (; i<str.size(); i++) ret[i] = str[i];
      for (; i<size; i++) ret[i] = '\0';
      return ret;
   }


public:
   MPI_Comm world;
   MPI_Comm local;
   MPI_Group local_group;
   MPI_Comm work;
   MPI_Group work_group;
   
   int world_rank, world_size;
   int local_rank, local_size;
   int work_rank,  work_size;
   bool in_work;
   typedef MPMDIntercomm intercomm_t;
   typedef std::map< std::string, MPMDIntercomm > intercomm_map;
   intercomm_map intercomm;

   int leader;
   int local_leader;
   
   std::string name;
   std::string host_name;

   inline MPMDHelper() {
      world = MPI_COMM_NULL;
      local = MPI_COMM_NULL;
      local_rank = -1;
   };

   inline ~MPMDHelper() {
   };
   
   inline void Init(const MPI_Comm& world_, const std::string& name_, const std::vector<int>& excl = std::vector<int>()) {
      int mylen,maxlen;
      
      world = world_;
      
      std::vector<std::string> names;
      std::vector<int> leaders;
      int color = -1;
      int colors = 0;
      
      name = name_;
      MPI_Comm_rank(world, &world_rank);
      MPI_Comm_size(world, &world_size);
      {
         char host_name_c[MPI_MAX_PROCESSOR_NAME];
         int host_len;
         MPI_Get_processor_name( host_name_c, &host_len );
         host_name = host_name_c;
      }

      mylen = name.size() + 1;
      MPI_Allreduce( &mylen, &maxlen, 1, MPI_INT, MPI_MAX, world);
      MPI_Barrier(world);
      char* my_name = char_vec_from_string(name, maxlen);
      char* other_name = char_vec_from_string("", maxlen);

      
      color = -1;
      colors = 0;
      int rank, who;
      rank = world_rank;
      int end_value = world_size;
      while (colors <= world_size) {
         MPI_Allreduce( &rank, &who, 1, MPI_INT, MPI_MIN, world);
         if (who == end_value) break;
         if (world_rank == who) {
            for (int i=0; i<maxlen; i++) other_name[i] = my_name[i];
         }
         MPI_Bcast(other_name, maxlen, MPI_CHAR, who, world);
         names.push_back(other_name);
         leaders.push_back(who);
         if (strcmp(my_name, other_name) == 0) {
            rank = end_value;
            color = colors;
            leader = who;
         }
         colors++;
      }
      if (colors > world_size) {
         fprintf(stderr, "MPMD: more colors then ranks. This doesn't make sens. Fatal error.\n");
         exit(-1);
      }
      delete[] my_name;
      delete[] other_name;
      MPI_Comm_split(world, color, rank, &local);
      MPI_Comm_rank(local, &local_rank);
      MPI_Comm_size(local, &local_size);

      MPI_Comm_group(local,&local_group);
      if (excl.size() > 0) {
         MPI_Group_excl(local_group,excl.size(),&excl[0],&work_group);
         MPI_Comm_create(local,work_group,&work);
         MPI_Group_rank(work_group, &work_rank);
         MPI_Group_size(work_group, &work_size);
         in_work = (work_rank != MPI_UNDEFINED);
      } else {
         work_group=local_group;
         work = local;
         work_rank = local_rank;
         work_size = local_size;
      }
      
      for (int c=0; c<colors; c++) {
         rank = local_rank;
         MPI_Bcast(&rank, 1, MPI_INT, leaders[c], world);
         if (color == c) local_leader = rank;
      }
      for (int c=0; c<colors; c++) {
         MPI_Comm inter = MPI_COMM_NULL;
         if (c != color) {
            int tag;
            if (c < color) {
               tag = c*colors + color;
            } else {
               tag = color*colors + c;
            }
            MPI_Intercomm_create(local, local_leader, world, leaders[c], tag, &inter);
            ConnectIntercomm(inter, true);
         }
      }
      
      MPI_Comm parent;
      MPI_Comm_get_parent(&parent);
      if (parent != MPI_COMM_NULL) {
         ConnectIntercomm(parent, false);
      }
      
   }

   inline void MPI_Exchange(const void * out, int out_count, void * in, int in_count, MPI_Datatype datatype, MPI_Comm intercomm, MPI_Comm comm) {
      MPI_Request request;
      MPI_Status status;
      int rank;
      MPI_Comm_rank(intercomm, &rank);
      fflush(stdout);
      if (rank == 0) {
         MPI_Isend(out, out_count, datatype, 0, 123, intercomm, &request);
         MPI_Recv(in, in_count, datatype, 0, 123, intercomm, &status);
         MPI_Wait(&request,  &status);
      }
      MPI_Bcast(in, in_count, datatype, 0, comm);
   }

   inline void ConnectIntercomm(MPI_Comm local_inter, bool world_) {
      intercomm_t inter;
      inter.local = local_inter;
      MPI_Comm_remote_size(inter.local, &inter.local_size);
      inter.in_world = world_;
      MPI_Comm_create(inter.local, work_group, &inter.work);
      if (in_work) MPI_Comm_remote_size(inter.work, &inter.work_size);
      
      int other_name_size;
      int my_name_size = name.size() + 1;
      MPI_Exchange(&my_name_size, 1, &other_name_size, 1, MPI_INT, inter.local, local);
      {
         char* my_name = char_vec_from_string(name, my_name_size);
         char* other_name = char_vec_from_string("", other_name_size);
         MPI_Exchange(my_name, my_name_size, other_name, other_name_size, MPI_CHAR, inter.local, local);
         inter.name = other_name;
         delete[] my_name;
         delete[] other_name;
      }
      intercomm.insert(make_pair(inter.name, inter));
   }

   inline void Spawn( const char *command, char *argv[], int maxprocs, MPI_Info info, bool world_ = false) {
      MPI_Comm comm, inter;
      std::vector<int> err(maxprocs);
      int root;
      if (world_) {
         comm = world;
         root = 0;
      } else {
         comm = local;
         root = local_leader;
      }
      MPI_Comm_spawn( command, argv, maxprocs, info, root, comm, &inter, &err[0]);
      if (inter != MPI_COMM_NULL) {
         ConnectIntercomm(inter, world_);
      } else {
         // could not spawn?
      }
   }

   inline void Identify(bool world_ = false) {
//      printf("MPMD: %03d.%03d world:%d/%d local:%d/%d color:%d/%d leader:%d/%d --- %s @ %s\n", color, local_rank, world_rank, world_size, local_rank, local_size,  color, colors, local_leader, leader, my_name, host_name);
      char iden_c[400];
      std::string iden = "MPMD: ";
      iden += name;
      iden += ":";
      if (world_) {
         sprintf(iden_c, " world:%d/%d", world_rank, world_size); iden += iden_c;
      }
      sprintf(iden_c, " local:%d/%d", local_rank, local_size); iden += iden_c;
      if (in_work) {
         sprintf(iden_c, " work:%d/%d", work_rank, work_size);
      } else {
         sprintf(iden_c, " work:NA/%d", work_size);
      }      
      iden += iden_c;
      iden += " --- ";
      iden += " connected to:";
      for (intercomm_map::iterator c = intercomm.begin(); c != intercomm.end(); c++) {
         intercomm_t inter = c->second;
         iden += " ";
         iden += inter.name;
         if (in_work) {
            sprintf(iden_c, "(%d/%d)", inter.work_size, inter.local_size); iden += iden_c;
         } else {
            sprintf(iden_c, "(NA/%d)", inter.local_size); iden += iden_c;
         }
      }
      
      printf("%s\n",iden.c_str());
   }
};

