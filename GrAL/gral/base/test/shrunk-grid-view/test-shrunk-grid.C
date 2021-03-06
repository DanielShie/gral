

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 
*/

#include "Gral/Views/shrunk-grid-view.h"

#include "Gral/Grids/Complex3D/all.h"
#include "Gral/Grids/Cartesian3D/all.h"
#include "Gral/Geometries/simple-geometry.h"

#include "Gral/IO/gmv-format-output3d.h"

#include "Gral/Test/all.h"

#include "Container/functions.h" // std extensions: identity
#include "Utility/pre-post-conditions.h"

#include <algorithm>


int main() {
  using namespace GrAL;
  using namespace std;
  using namespace cartesian3d;
 
  // create Cartesian grid & geometry
  typedef CartesianGrid3D                           reg_grid_type;
  typedef grid_types<reg_grid_type>                 rgt; 
  typedef coordN<3>                                 coord_type;
  typedef stdext::identity<coord_type>              mapping_type;
  typedef mapped_geometry<mapping_type>             reg_geom_type;

  reg_grid_type R(3,2,2);
  mapping_type  id; 
  reg_geom_type GeomR(R, id);


  // create disjoint union view of above grids and geometries
  namespace sgv = shrink_grid_view;
  typedef sgv::grid_view<reg_grid_type>       shrunk_grid_type;
  typedef grid_types<shrunk_grid_type>        sgt;
  sgv::grid_view<reg_grid_type>               ShrunkG(R);
  sgv::geom_view<reg_grid_type,reg_geom_type> ShrunkGeom(ShrunkG, GeomR);
  ShrunkGeom.set_shrink_factor(0.5);


  // test functionality of union view
  test_vertex_iterator        (ShrunkG,cout);
  test_cell_iterator          (ShrunkG,cout);
  test_vertex_on_cell_iterator(ShrunkG,cout);

  test_sequence_iterator <sgt::Vertex>          (ShrunkG, cout);
  test_sequence_iterator <sgt::Cell  >          (ShrunkG, cout);
  test_incidence_iterator<sgt::Vertex,sgt::Cell>(ShrunkG, cout);

  // copy ShrunkG into unstructured grid
  
  Complex3D UG;
  simple_geometry<Complex3D, coord_type> GeomUG; //(UG);
  ConstructGrid(UG, GeomUG, ShrunkG, ShrunkGeom);

  // instantiate grid function adapters for vertex and cell
  typedef grid_function<rgt::Vertex, int> gfR_v_int_type;
  gfR_v_int_type gfRv(R,0);
  sgv::grid_function_view<gfR_v_int_type> gf_v_Shrink(ShrunkG,gfRv);
  typedef grid_function<rgt::Cell, int> gfR_c_int_type;
  gfR_c_int_type gfRc(R,0);
  sgv::grid_function_view<gfR_c_int_type> gf_c_Shrink(ShrunkG,gfRc);

  // can use this only for gf's which can be instantiated
  // test_grid_functions<sgt::Cell>(gf_c_Shrink,cout);
  int cnt = std::count(gf_c_Shrink.begin(), gf_c_Shrink.end(), 0);
  REQUIRE_ALWAYS(cnt == gf_c_Shrink.size(), "  cnt= " << cnt << " size()=" << gf_c_Shrink.size(), 1);

  
  // output ShrunkG to files

  {
    OstreamGMV3DFmt Out("shrunk1.gmv.out");
    ConstructGrid(Out, UG, GeomUG);
  }

  {
    OstreamGMV3DFmt Out("shrunk2.gmv.out");
    ConstructGrid(Out, ShrunkG, ShrunkGeom);
  }

  {
    namespace hl = heterogeneous_list;
    hl::BEGIN B;
    OstreamGMV3DFmt Out("shrunk3.gmv.out");
    ConstructGrid_GF(Out, ShrunkG, ShrunkGeom, 
		     (B, 
		      Out.pair("gfShrinkV",gf_v_Shrink),
		      Out.pair("gfShrinkC",gf_c_Shrink)));
  }


}
