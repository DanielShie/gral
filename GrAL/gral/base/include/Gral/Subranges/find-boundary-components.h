#ifndef GRAL_BASE_GB_GRID_FIND_BOUNDARY_COMPONENTS_H
#define GRAL_BASE_GB_GRID_FIND_BOUNDARY_COMPONENTS_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1999
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

/*! \defgroup boundaryalgorithms Algorithms for exploring Grid Boundaries
    \ingroup algorithms
    \ingroup boundary
    
    The algorithms in this module count and find boundary facets and connected components.

    \todo The algorithms could take an additional parameter \c IsOnBd
    which is a predicate deciding whether a $GrAL Facet is on the boundary.
    Then one could handle also internal boundaries, for example
    between two partitions.

    \see Module \ref boundaryrange
    \see Module \ref boundarycompiter
 */

/*! \ingroup boundaryalgorithms 
    \brief Count the number of boundary facets.

    Runtime is linear in the number of facets of \c G.
 */    
template<class GRID>
int count_boundary_facets(GRID const& G);

/*! \ingroup boundaryalgorithms
    \brief Count the number of boundary components

    Runtime is linear in the number of facets of \c G.
 */    
template<class GRID>
int count_boundary_components(GRID const& G);


/*! \ingroup boundaryalgorithms 
    \brief Find a facet for each component of the boundary of G
  
*/
template<class GRID, class FACETSET>
void find_boundary_component_germs(GRID const& G, FACETSET & C);

/*! \ingroup boundaryalgorithms 
    \brief Find a facet for each component of the boundary of G,
    and number of boundary components and facets.
  
*/
template<class GRID, class FACETSET>
void find_boundary_component_germs(GRID const& G, FACETSET & C,
				   int& num_comps, int& num_facets);


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Subranges/find-boundary-components.C"
#endif

#endif
