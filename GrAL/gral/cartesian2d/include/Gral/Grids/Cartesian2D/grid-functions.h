#ifndef NMWR_GB_REG2D_GRIDFUNCTIONS_H
#define NMWR_GB_REG2D_GRIDFUNCTIONS_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Grids/Cartesian2D/element-traits.h" 
#include "Gral/Base/grid-function-vector.h"

/*! \file
 */

namespace GrAL {

/*! \defgroup gridfunctionsreggrid2d Total grid functions for RegGrid2D

    \ingroup cartesian2dmodule

    \see $GrAL TotalGridFunction
 */


/*!  \brief Specialization of grid_function for RegGrid2D::Vertex

  Model of $GrAL TotalGridFunction
   \ingroup gridfunctionsreggrid2d
 */
template<class T>
class grid_function<cartesian2d::RegGrid2D::Vertex,T> 
  : public grid_function_vector<cartesian2d::RegGrid2D::Vertex,T>
{
  typedef grid_function<cartesian2d::RegGrid2D::Vertex,T>        self;
  typedef grid_function_vector<cartesian2d::RegGrid2D::Vertex,T> base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
};


/*! \brief Specialization of grid_function for RegGrid2D::Edge

  Model of $GrAL TotalGridFunction
  \ingroup gridfunctionsreggrid2d
 */
template<class T>
class grid_function<cartesian2d::RegGrid2D::Edge,T> 
  : public grid_function_vector<cartesian2d::RegGrid2D::Edge,T>
{
  typedef grid_function<cartesian2d::RegGrid2D::Edge,T>        self;
  typedef grid_function_vector<cartesian2d::RegGrid2D::Edge,T> base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
};


/*! \brief Specialization of grid_function for RegGrid2D::Cell

  Model of $GrAL TotalGridFunction
  \ingroup gridfunctionsreggrid2d
 */
template<class T>
class grid_function<cartesian2d::RegGrid2D::Cell,T> 
  : public grid_function_vector<cartesian2d::RegGrid2D::Cell,T>
{
  typedef grid_function<cartesian2d::RegGrid2D::Cell,T>        self;
  typedef grid_function_vector<cartesian2d::RegGrid2D::Cell,T> base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
};

} // namespace GrAL 

#endif
