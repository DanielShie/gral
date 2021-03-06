#ifndef NMWR_GB_COLLECT_ELEMENT_PARTITIONS_C
#define NMWR_GB_COLLECT_ELEMENT_PARTITIONS_C



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Partitioning/collect-element-partitions.h"

namespace GrAL {

template<class Partition, class Vtx2PartMap>
void collect_vertex_partitions(const Partition& P,      // in
			       Vtx2PartMap    & p_of_v, // out 
			       bool mark_on_boundary)   // int
{
  typedef typename Partition::PartBdVertexIterator PartBdVertexIterator;
  typedef typename Partition::grid_type   grid_type;
  typedef grid_types<grid_type>           gt;
  typedef typename gt::CellIterator             CellIterator;
  typedef typename gt::VertexOnCellIterator     VertexOnCellIterator;

  typedef BoundaryRange<grid_type>              BdRange;
  typedef typename BdRange::VertexIterator      BdVertexIterator;

  BdRange Bd(P.TheGrid());

  // add mark for vertices on grid boundary
  if(mark_on_boundary) {
    for(BdVertexIterator bv = Bd.FirstVertex(); ! bv.IsDone(); ++bv)
      p_of_v[*bv].push_back(-1);
  }
  for(CellIterator c = P.TheGrid().FirstCell(); ! c.IsDone(); ++c)
    for(VertexOnCellIterator vc = (*c).FirstVertex(); ! vc.IsDone(); ++vc) {
      int pt = P.partition(*c);
      if(std::find(p_of_v[*vc].begin(),p_of_v[*vc].end(),pt) == p_of_v[*vc].end())
	p_of_v[*vc].push_back(pt);
    }

  /*
  // add mark for vertices on boundary of partitions
  for(int pt = 0; pt < (int)P.NumOfPartitions(); ++pt) {
    for(PartBdVertexIterator pbv = P.FirstBdVertex(pt); ! pbv.IsDone(); ++pbv)
      partitions_of_vertex[*pbv].push_back(pt);
  }
  */
}


//----------------------------------------------------------------
// produce mapping Facet --> { pnum } where
// partitions_of_facet(f) = { p, q } <=> f separates p and q
//----------------------------------------------------------------

template<class Partition, class Fac2PartMap>
void collect_facet_partitions(const Partition& P,                   // in
			      Fac2PartMap    & partitions_of_facet, // out 
			      bool mark_on_boundary)                // in
{
  typedef typename Partition::PartBdFacetIterator PartBdFacetIterator;
  typedef typename Partition::grid_type   grid_type;
  typedef grid_types<grid_type>           gt;
  typedef typename gt::BdFacetIterator    BdFacetIterator;

  // add mark for facets on grid boundary
  if(mark_on_boundary) {
    for(BdFacetIterator bf = P.TheGrid().FirstBdFacet(); ! bf.IsDone(); ++bf)
      partitions_of_facet[*bf].push_back(-1);
  }

  // add mark for facets on boundary of partitions
  for(int pt = 0; pt < P.NumOfPartitions(); ++pt) {
    for(PartBdFacetIterator pbf = P.FirstPartBdFacet(pt); ! pbf.IsDone(); ++pbf)
      partitions_of_facet[*pbf].push_back(pt);
  }
}


} // namespace GrAL 

#endif
