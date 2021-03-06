#ifndef GRAL_GB_COMPLEX2D_INTERNAL_COPY_CELLS_H
#define GRAL_GB_COMPLEX2D_INTERNAL_COPY_CELLS_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Grids/Complex2D/complex2d.h"

//----------------------------------------------------------------
//  !! intended for use in other functions only, 
//  !! not directly by user! 
//  !! This leaves Complex2D G in an inconsistent state !!
//  
//  purpose: append the cells from G_src to G, translating the
//  vertices from G_src to G via vtx_corr : V_h(G_src) --> V_h(G).
//  * Preconditions:
//    -------------
//    The vertices in ran(vtx_corr) are already  created in G,
//    and vtx_corr is the correspondance map.
//  * Postconditions:
//    --------------
//    cells will be copied from G-src to G, 
//      with correct vertex references
//      with neighbour references set to <boundary>
//    cell_corr is the correspondance map C_h(G_src) --> C_h(G)
//----------------------------------------------------------------




#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Grids/Complex2D/internal/copy.C"
#endif


namespace GrAL {

template<int WITHARCH>
struct dispatch_complex2d_copy {
  
  template<class GSRC, class VtxCorr, class CellCorr>
  static void copy_cells(Complex2D      & G_dst, 
		  GSRC      const& G_src,
		  VtxCorr&         vtx_corr,
		  CellCorr&        cell_corr)
  { copy_cells_archetypes(G_dst, G_src, vtx_corr, cell_corr);}
};


template<>
struct dispatch_complex2d_copy<0> {
  
  template<class GSRC, class VtxCorr, class CellCorr>
  static void copy_cells(Complex2D      & G_dst, 
		  GSRC      const& G_src,
		  VtxCorr&         vtx_corr,
		  CellCorr&        cell_corr)
  { copy_cells_no_archetypes(G_dst, G_src, vtx_corr, cell_corr);}
};

} // namespace GrAL 

#endif
