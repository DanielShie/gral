#ifndef NMWR_GB_COMPLEX2D_GRIDFUNCTIONS_H
#define NMWR_GB_COMPLEX2D_GRIDFUNCTIONS_H

#include "Grids/Complex2D/complex2d.h"
#include "Grids/grid-function-vector.h"
#include "Grids/grid-function-hash.h"

template<class T>
class grid_function<Vertex2D,T> 
  : public grid_function_vector<Vertex2D,T>
{
  typedef grid_function<Vertex2D,T>        self;
  typedef grid_function_vector<Vertex2D,T> base;
public:
  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
};

template<class T>
class grid_function<Cell2D,T> 
  : public grid_function_vector<Cell2D,T>
{
  typedef grid_function<Cell2D,T>         self;
  typedef grid_function_vector<Cell2D,T>  base;
public:
  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
};

template<class T>
class grid_function<Edge2D,T> 
  : public grid_function_hash<Edge2D,T>
{
  typedef grid_function<Edge2D,T>       self;
  typedef grid_function_hash<Edge2D,T>  base;
public:
  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
};


#define SPECIALIZATION_GRIDFUNCTION_COMPLEX2D(E,T) 


#endif
