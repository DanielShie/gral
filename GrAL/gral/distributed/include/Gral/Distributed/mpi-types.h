#ifndef NMWR_GB_MPI_TYPES_H
#define NMWR_GB_MPI_TYPES_H




/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include <mpi.h>


namespace GrAL {

/*! \brief traits class to help calling MPI-routines
    from template functions.
    \ingroup mpidistributedgrids   

   Specializations exist for int, short, double, float, long double.
*/

template<class T> struct mpi_types {};

template<>
struct mpi_types<int> { 
  static MPI_Datatype tag() { return MPI_INTEGER; }
};

template<>
struct mpi_types<short> { 
  static MPI_Datatype tag() { return MPI_SHORT; }
};


template<>
struct mpi_types<float> { 
  static MPI_Datatype tag() { return MPI_FLOAT; }
};

template<>
struct mpi_types<double> { 
  static MPI_Datatype tag() { return MPI_DOUBLE; }
};

template<>
struct mpi_types<long double> { 
  static MPI_Datatype tag() { return MPI_LONG_DOUBLE; }
};

} // namespace GrAL 

#endif
