#ifndef NMWR_GB_MPI_TYPES_H
#define NMWR_GB_MPI_TYPES_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#include <mpi.h>

///////////////////////////////////////////////////
//
// traits classes to help calling MPI-routines
// from template functions.
//
///////////////////////////////////////////////////


template<class T> struct mpi_types {};

struct mpi_types<int> { 
  static MPI_Datatype tag() { return MPI_INTEGER; }
};

struct mpi_types<short> { 
  static MPI_Datatype tag() { return MPI_SHORT; }
};

struct mpi_types<float> { 
  static MPI_Datatype tag() { return MPI_FLOAT; }
};

struct mpi_types<double> { 
  static MPI_Datatype tag() { return MPI_DOUBLE; }
};

struct mpi_types<long double> { 
  static MPI_Datatype tag() { return MPI_LONG_DOUBLE; }
};


#endif
