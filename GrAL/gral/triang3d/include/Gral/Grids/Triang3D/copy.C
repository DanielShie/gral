#ifndef GRAL_GB_GRIDS_TRIANG3D_COPY_CELLS_C
#define GRAL_GB_GRIDS_TRIANG3D_COPY_CELLS_C

// $LICENSE

#include "Gral/Grids/Triang3D/copy.h"

template<class Conn, class VtxCorr, class CellCorr>
void copy_cells(Triang3D       & G, 
		Conn      const&  G_src,
		VtxCorr        &  vtx_corr,
		CellCorr       &  cell_corr)
{
  const int d = Triang3D::dim;
  typedef grid_types<Conn>                      src_gt;
  typedef typename src_gt::CellIterator         src_cell_it;
  typedef typename src_gt::VertexOnCellIterator src_vertex_on_cell_it;

  typedef grid_types<Triang3D>      gt;
  typedef typename  gt::Cell        Cell;
  typedef typename  gt::cell_handle cell_handle;

  // first unused cell handle, = 0 if new grid
  cell_handle hc = G.NumOfCells();

  int * cells = new int[(d+1)*(G_src.NumOfCells()+hc)];

  for(src_cell_it src_c = G_src.FirstCell(); ! src_c.IsDone(); ++src_c, ++hc){
    REQUIRE(((*src_c).NumOfVertices() == d+1),
	    "Can handle only tetrahedra, but  NumOfVertices() was "
	    << (*src_c).NumOfVertices() << "!\n",1);

    cell_corr[src_c.handle()] = hc;
    
    int lv = 0;
    src_vertex_on_cell_it src_lv((*src_c).FirstVertex());
    for(; ! src_lv.IsDone(); ++src_lv, ++lv) {
       cells[(d+1)*hc+lv] = vtx_corr(src_lv.handle());
    }
   
  }
  G.Steal(cells, G_src.NumOfCells(), G_src.NumOfVertices());
}


#endif