#ifndef NMWR_GB_GRID_SELF_MAP_H
#define NMWR_GB_GRID_SELF_MAP_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Container/bijective-mapping.h"

namespace GrAL {

//----------------------------------------------------------------
//
// template<class Grid> class GridSelfMap;
// class for mapping the grid elements of a grid G onto themselves.
// This defaults to the identity mapping.
//
//----------------------------------------------------------------

template<class Grid>
class GridSelfMap {
public:
  typedef Grid             grid_type;
  typedef grid_types<Grid> gt;
  typedef typename gt::vertex_handle vertex_handle;
  typedef typename gt::Vertex        Vertex;
  typedef typename gt::facet_handle  facet_handle;
  typedef typename gt::Facet         Facet;
  typedef typename gt::cell_handle   cell_handle;
  typedef typename gt::Cell          Cell;
private:
  const grid_type* the_grid;

  typedef bijective_mapping<vertex_handle, vertex_handle> vmap_type;
  typedef bijective_mapping<facet_handle, facet_handle>   fmap_type;
  typedef bijective_mapping<cell_handle, cell_handle>     cmap_type;
  bijective_mapping<vertex_handle, vertex_handle> vertex_map;
  bijective_mapping<facet_handle,  facet_handle>  facet_map;
  bijective_mapping<cell_handle,   cell_handle>   cell_map;

public:
  GridSelfMap(const grid_type& g) : the_grid(&g) {}

  grid_type const& TheGrid() const { return (*the_grid);}

  vmap_type &  TheVertexMap() { return vertex_map;}
  fmap_type &  TheFacetMap()  { return facet_map;}
  cmap_type &  TheCellMap()   { return cell_map;}

  Vertex operator() (const Vertex& V) const 
    { 
      return ( vertex_map.defined(V.handle()) ? 
	       TheGrid().vertex(vertex_map(V.handle()))
	       : V);
	}
  Facet operator() (const Facet& F) const 
    { 
      return ( facet_map.defined(F.handle()) ? 
	       TheGrid().facet(facet_map(F.handle()))
	       : F);
	}
  Cell operator() (const Cell& C) const 
    { 
      return ( cell_map.defined(C.handle()) ? 
	       TheGrid().cell(cell_map(C.handle()))
	       : C);
	}
  
};

} // namespace GrAL 

#endif
