#ifndef NMWR_GB_ELEMENT_SEQ_REFERENCES_H
#define NMWR_GB_ELEMENT_SEQ_REFERENCES_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------



//-----------------------------------------------------------------------
//  
//  Here adapters for grids are defined, that let a grid be viewed as
//  a container of vertices, edges and so on.
// 
//  Contents:
//  ---------
//  [1] template<class Grid> class vertex_seq_ref;
//      template<class Grid> vertex_seq_ref<Grid> VertexSet(const Grid&);
//  [2] template<class Grid> class edge_seq_ref;
//      template<class Grid> edge_seq_ref<Grid>   EdgeSet(const Grid&);
//  [3] template<class Grid> class cell_seq_ref;
//      template<class Grid> cell_seq_ref<Grid>   CellSet(const Grid&);
//  [4] template<class FacetRange> class vertex_set_of_facets
//  [5] template<class FacetRange> class vertex_set_of_cells
// 
//  Description:
//  ------------
//
//  Three different usage levels are supported:
//
//  1) explicitely use the element category:
//      FirstVertex(), NumOfVertices()
//      Vertex, VertexIterator
//  2) use only the grid-context:
//      FirstElement(), NumOfElements(), 
//      TheGrid()
//      grid_type, Element, ElementIterator
//  3) fully STL conforming generic container:
//       begin() / end(),
//       size(), empty()
//       const_iterator, value_type
//
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
//                   [1]  vertex_seq_ref
//-----------------------------------------------------------------------

template<class Grid>
class vertex_seq_ref {
public:
  typedef grid_types<Grid>          gt;
  typedef Grid                      grid_type;

  typedef typename gt::Vertex         Vertex;
  typedef typename gt::VertexIterator VertexIterator;

  typedef typename gt::Vertex         Element;
  typedef typename gt::VertexIterator ElementIterator;

  typedef Element                   value_type;
  typedef ElementIterator           const_iterator;
private:
  const Grid* g;
public:
  vertex_seq_ref(const Grid& gg) : g(&gg) {}

  const grid_type& TheGrid() const { return *g;}


  //------------------ size queries -----------------------

  unsigned NumOfVertices()    const { return size();}

  unsigned NumOfElements() const { return size();}

  unsigned size() const  { return g->NumOfVertices();}
  bool     empty() const { return (size() == 0);}


  //------------------ iteration --------------------------

  ElementIterator FirstVertex()    const { return g->FirstVertex();}

  ElementIterator FirstElement() const { return g->FirstVertex();}

  const_iterator begin() const { return g->FirstVertex();}
  const_iterator end()   const { return g->EndVertex();  }
};

template<class Grid>
inline
vertex_seq_ref<Grid> VertexSet(const Grid& G) { return vertex_seq_ref<Grid>(G) ;}


//-----------------------------------------------------------------------
//                     [2] edge_seq_ref
//-----------------------------------------------------------------------


template<class Grid>
class edge_seq_ref {
public:
  typedef grid_types<Grid>          gt;
  typedef Grid                      grid_type;

  typedef typename gt::Edge         Edge;
  typedef typename gt::EdgeIterator EdgeIterator;

  typedef typename gt::Edge         Element;
  typedef typename gt::EdgeIterator ElementIterator;

  typedef Element                   value_type;
  typedef ElementIterator           const_iterator;
private:
  const Grid* g;
public:
  edge_seq_ref(const Grid& gg) : g(&gg) {}

  const grid_type& TheGrid() const { return *g;}


  //------------------ size queries -----------------------

  unsigned NumOfEdges()    const { return size();}

  unsigned NumOfElements() const { return size();}

  unsigned size() const  { return g->NumOfEdges();}
  bool     empty() const { return (size() == 0);}


  //------------------ iteration --------------------------

  ElementIterator FirstEdge()    const { return g->FirstEdge();}

  ElementIterator FirstElement() const { return g->FirstEdge();}

  const_iterator begin() const { return g->FirstEdge();}
  const_iterator end()   const { return g->EndEdge();  }
};

template<class Grid>
inline
edge_seq_ref<Grid> EdgeSet(const Grid& G) { return edge_seq_ref<Grid>(G) ;}


//-----------------------------------------------------------------------
//                    [3] cell_seq_ref
//-----------------------------------------------------------------------

template<class Grid>
class cell_seq_ref {
public:
  typedef grid_types<Grid>          gt;
  typedef Grid                      grid_type;

  typedef typename gt::Cell         Cell;
  typedef typename gt::CellIterator CellIterator;

  typedef typename gt::Cell         Element;
  typedef typename gt::CellIterator ElementIterator;

  typedef Element                   value_type;
  typedef ElementIterator           const_iterator;
private:
  const Grid* g;
public:
  cell_seq_ref(const Grid& gg) : g(&gg) {}

  const grid_type& TheGrid() const { return *g;}


  //------------------ size queries -----------------------

  unsigned NumOfCells()    const { return size();}

  unsigned NumOfElements() const { return size();}

  unsigned size() const  { return g->NumOfCells();}
  bool     empty() const { return (size() == 0);}


  //------------------ iteration --------------------------

  ElementIterator FirstCell()    const { return g->FirstCell();}

  ElementIterator FirstElement() const { return g->FirstCell();}

  const_iterator begin() const { return g->FirstCell();}
  //  const_iterator end()   const { return g->EndCell();  }
};


template<class Grid>
inline
cell_seq_ref<Grid> CellSet(const Grid& G) { return cell_seq_ref<Grid>(G) ;}


//-----------------------------------------------------------------------
//                     [4] vertex_set_of_facets 
//-----------------------------------------------------------------------

// this is a "fat" iterator due to the table keeping track of already
// visited vertices, so either we must be careful with copying or
// there should be some shallow copy semantics (copy-on-write ?)
// Only on construction time this iterator should be rather "lean".
template<class FacetRange>
class vertex_set_of_facets_iterator {
public:
  typedef typename FacetRange::grid_type grid_type;
  typedef grid_types<grid_type>          gt;
  typedef typename gt::Vertex            Element;
  typedef Element                        Vertex;
  typedef Element                        value_type;

private:
  //typedef typename FacetRange::FacetIterator      facet_iter;
  typedef typename FacetRange::ElementIterator      facet_iter;
  typedef typename gt::VertexOnFacetIterator        vertex_on_facet_iter;
  typedef vertex_set_of_facets_iterator<FacetRange> self;

  const FacetRange*                  facets;
  facet_iter                         f;
  vertex_on_facet_iter               vf;
  partial_grid_function<Vertex,bool> visited;

public:
  //--------------- construction --------------------

  vertex_set_of_facets_iterator(const FacetRange& fr) 
    : facets(&fr),
      //      f(fr.FirstFacet()),
      f(fr.FirstElement()),
      visited(fr.TheGrid(), false) 
    { if(! f.IsDone()) vf = (*f).FirstVertex(); }
  
  //--------------- component access ----------------

  grid_type const & TheGrid() const { return facets->TheGrid();}

  //---------------- iteration ----------------------

  Element operator*() const { return  value(); }
  bool    IsDone()    const { return (f.IsDone());}
  self&   operator++() {
    advance();
    while(! IsDone() && visited(value()))
      advance();
    return *this;
  }
  
private:
  Element value() const {  return (*vf); }
  void advance() { 
    visited[value()] = true;
    ++vf; 
    if(vf.IsDone()) {
      ++f;
      if(! f.IsDone())
	vf = (*f).FirstVertex();
    } 
  }
};



template<class FacetRange>
class vertex_set_of_facets {
public:
  typedef typename FacetRange::grid_type            grid_type;
  typedef grid_types<grid_type>                     gt;
  typedef typename gt::Vertex                       Element;
  typedef vertex_set_of_facets_iterator<FacetRange> ElementIterator;
  typedef Element                                   Vertex;
  typedef ElementIterator                           VertexIterator;
private:
  const FacetRange* facets;
public:
  vertex_set_of_facets(const FacetRange& f) : facets(&f) {}
  VertexIterator FirstVertex() const { return VertexIterator(*facets);}
  //  VertexIterator EndVertex()   const { return VertexIterator(???);}
};

template<class FacetRange>
inline vertex_set_of_facets<FacetRange> 
VerticesOfFacets(const FacetRange& FR) 
{ return  vertex_set_of_facets<FacetRange>(FR); }


//-----------------------------------------------------------------------
//                     [5] vertex_set_of_cells
//-----------------------------------------------------------------------

template<class CellRange>
class vertex_set_of_cells_iterator {
public:
  typedef typename CellRange::grid_type  grid_type;
  typedef grid_types<grid_type>          gt;
  typedef typename gt::Vertex            Element;
  typedef Element                        Vertex;
  typedef Element                        value_type;

private:
  typedef typename CellRange::ElementIterator      cell_iter;
  typedef typename gt::VertexOnCellIterator        vertex_on_cell_iter;
  typedef vertex_set_of_cells_iterator<CellRange>  self;

  //-------------- DATA  ------------------
  //-- referenced ---
  const CellRange*                  cells;
  //----- owned -----
  cell_iter                          c;
  vertex_on_cell_iter                vc;
  partial_grid_function<Vertex,bool> visited;

public:
  //--------------- construction --------------------

  vertex_set_of_cells_iterator(const CellRange& cr) 
    : cells(&cr),
      //      c(cr.FirstCell()),
      c(cr.FirstElement()),
      visited(cr.TheGrid(), false) 
    { if(! c.IsDone()) vc = (*c).FirstVertex(); }
  
  //--------------- component access ----------------

  grid_type const & TheGrid() const { return cells->TheGrid();}

  //---------------- iteration ----------------------

  Element operator*() const { return  value(); }
  bool    IsDone()    const { return (c.IsDone());}
  self&   operator++() {
    advance();
    while(! IsDone() && visited(value()))
      advance();
    return *this;
  }
  
private:
  Element value() const {  return (*vc); }
  void advance() { 
    visited[value()] = true;
    ++vc; 
    if(vc.IsDone()) {
      ++c;
      if(! c.IsDone())
	vc = (*c).FirstVertex();
    } 
  }

};


template<class CellRange>
class vertex_set_of_cells {
public:
  typedef typename CellRange::grid_type           grid_type;
  typedef grid_types<grid_type>                   gt;
  typedef typename gt::Vertex                     Element;
  typedef vertex_set_of_cells_iterator<CellRange> ElementIterator;
  typedef Element                                 Vertex;
  typedef ElementIterator                         VertexIterator;
private:
  const CellRange* cells;
public:
  vertex_set_of_cells(const CellRange& cr) : cells(&cr) {}
  VertexIterator FirstVertex() const { return VertexIterator(*cells);}
  //  VertexIterator EndVertex()   const { return VertexIterator(???);}
};

template<class CellRange>
inline vertex_set_of_cells<CellRange> 
VerticesOfCells(const CellRange& CR) 
{ return  vertex_set_of_cells<CellRange>(CR); }


//-----------------------------------------------------------------------
//                     [6] facets_span_of_vertices
//-----------------------------------------------------------------------

template<class VertexRange>
class facet_span_of_vertices_iterator {

public:
  typedef typename VertexRange::grid_type  grid_type;
  typedef grid_types<grid_type>            gt;
  typedef typename gt::Vertex              Vertex;
  typedef typename gt::Facet               Facet;
  typedef Facet                            Element;
  typedef Element                          value_type;

  typedef typename gt::CellOnVertexIterator  CellOnVertexIterator;
  typedef typename gt::FacetOnCellIterator   FacetOnCellIterator;
  typedef typename gt::VertexOnFacetIterator VertexOnFacetIterator;

  typedef typename VertexRange::ElementIterator RgeVertexIterator;
private:
  typedef facet_span_of_vertices_iterator<VertexRange> self;

  //---------- DATA --------------
  const VertexRange* vertices;

  RgeVertexIterator     v;
  CellOnVertexIterator  cv;
  FacetOnCellIterator   fcv;

  partial_grid_function<Facet,bool>        visited;
  partial_grid_function<Vertex,bool>       in_vertex_set;

public:
   facet_span_of_vertices_iterator(const VertexRange& vr)
     : vertices(&vr),
       visited(vr.TheGrid(), false),
       in_vertex_set(vr.TheGrid(), false)
    {
     v = vr.FirstElement();
     if(!v.IsDone()) {
        cv  = (*v).FirstCell();
        fcv = (*cv).FirstFacet();
     }
     range_assign(in_vertex_set, vertices->FirstElement(), true); 
     while(! IsDone() && ! valid())
       advance();
    }

  grid_type const& TheGrid() const { return vertices->TheGrid();}

  bool IsDone() const { return v.IsDone();}
  Facet operator*() const { return *fcv;}
  self& operator++() {
    do {
      advance();
    } while(!IsDone() && ! valid());
    return *this;
  }

private:
  bool valid() const { return (! visited(*fcv) && vertices_ok());}
  bool vertices_ok() const { 
    for(VertexOnFacetIterator vf = (*fcv).FirstVertex(); ! vf.IsDone(); ++vf)
      if( ! in_vertex_set(*vf))
	return false;
    return true;
  }

  void advance() {
    visited[fcv] = true;
    ++fcv;
    if(fcv.IsDone()) {
      ++cv;
      if(! cv.IsDone())
	fcv = (*cv).FirstFacet();
      else {
       ++v;
       if( ! v.IsDone()) {
         cv  = (*v) .FirstCell();
	 fcv = (*cv).FirstFacet();
       }
      }

    }
  }
};  

template<class VertexRange>
class facet_span_of_vertices {
public:
  typedef typename VertexRange::grid_type  grid_type;
  typedef grid_types<grid_type>            gt;
  typedef typename gt::Vertex              Vertex;
  typedef typename gt::Facet               Facet;
  typedef Facet                            Element;
  typedef facet_span_of_vertices_iterator<VertexRange> FacetIterator;
  typedef FacetIterator                    ElementIterator;
  typedef Element                          value_type;

private:
  VertexRange const* vertices;
public:
  facet_span_of_vertices(VertexRange const& vr) : vertices(&vr) {}

  // attention, fat iterator!
  FacetIterator FirstFacet() const { return FacetIterator(*vertices);} 
  FacetIterator FirstElement() const { return FacetIterator(*vertices);}
 
  grid_type const& TheGrid() const { return vertices->TheGrid();}
};

template<class VertexRange>
inline facet_span_of_vertices<VertexRange>
FacetSpanOfVertices(VertexRange const& vr)
{ return facet_span_of_vertices<VertexRange>(vr);}


#endif
