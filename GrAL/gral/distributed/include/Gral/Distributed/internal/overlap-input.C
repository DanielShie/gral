#ifndef NMWR_GB_OVERLAP_INPUT_C
#define NMWR_GB_OVERLAP_INPUT_C

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Distributed/overlap-input.h"
#include <fstream>

namespace GrAL {

template<
  class CoarseGrid, 
  class FineGrid,
  template<class U> class OVLP_RANGES 
  >
void overlap_input<CoarseGrid, FineGrid, OVLP_RANGES>::set_neighbours()
{
  std::ifstream nbin( (basenm + ".neighbors").c_str());
  typename cgt::cell_handle c;
  while(nbin >> c) {
    neighbours.push_back(c);
  }
  nbin.close();
  
}

template<
  class CoarseGrid, 
  class FineGrid,
  template<class U> class OVLP_RANGES 
  >
void overlap_input<CoarseGrid, FineGrid, OVLP_RANGES>
::init(typename overlap_input<CoarseGrid, FineGrid, OVLP_RANGES>::coarse_grid_type const& cg,
       typename overlap_input<CoarseGrid, FineGrid, OVLP_RANGES>::  fine_grid_type const& fg,
       std::string const& basenm_)
{
  basenm = basenm_;
  set_coarse_grid(cg);
  set_fine_grid  (fg);

  total_ranges_v.init(basenm + ".total.vertices");
  total_ranges_c.init(basenm + ".total.cells");

  set_neighbours();

  for(NbIterator N = neighbours.FirstCell(); ! N.IsDone(); ++N) {
    neighbour_ranges_v[*N].init(basenm + "." + as_string(number(*N)) + ".vertices");
    neighbour_ranges_c[*N].init(basenm + "." + as_string(number(*N)) + ".cells");
  }
}

} // namespace GrAL 

#endif
