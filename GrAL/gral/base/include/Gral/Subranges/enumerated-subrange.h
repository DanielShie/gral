#ifndef NMWR_GB_ENUMERATED_SUBRANGE_H
#define NMWR_GB_ENUMERATED_SUBRANGE_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "vector.h"   // STL

#include "compiler-config.h"
#include "Utility/pre-post-conditions.h"

#include "Grids/mapped-iterators.h"
#include "Grids/grid-functors.h"
#include "Grids/Iterators/facet-by-cell-it.h"

//----------------------------------------------------------------
//
//  This file contains template classes for storage of grid
//  subranges that work by enumerating the elements.
//
// CONTENTS:
// --------
//  
// [1]  template<class E, class HIt> class element_range_ref;
// [1a]  template<class Grid, class HIt> class vertex_range_ref;
// [1b]  template<class Grid, class HIt> class facet_range_ref;
// [1c]  template<class Grid, class HIt> class cell_range_ref;
// [2]  template<class E> class enumerated_element_range;
// [2a]  template<class Grid> class enumerated_vertex_range;
// [2b]  template<class Grid> class enumerated_facet_range;
// [2c]  template<class Grid> class enumerated_cell_range;
// [3] template<class Grid> class enumerated_subrange;
// [4] template<class Grid, class vit, class cit>
//     class enumerated_subrange_ref;
// [5] template<class Grid, class CellIt>
//     void ConstructSubrangeFromCells(...);
//
// Description:
// -----------
//
// [1] - [1c]:
//   references to (enumerated) element ranges.
//   HIt is an (random access) iterator with value type 
//   element_handle, as is provided e.g. by classes [2] - [2c]
//   The range is referenced by the half-open range [b,e) (b,e : HIt)
// [2] - [2c]
//   classes that store sequences of element handles and give
//   iteration access to the corresponding elements.
//
// Aliases 'Element' and 'ElementIterator' are provided to ease the
// definition of template functions that are independent of the
// concrete element category (i.e. Vertex, Edge etc.)
//
//----------------------------------------------------------------


template<class E> class enumerated_element_range;
template<class Grid> class enumerated_vertex_range;
template<class Grid> class enumerated_facet_range;
template<class Grid> class enumerated_cell_range;



//----------------------------------------------------------------
//----------------------------------------------------------------
//
//             Classes  with REFERENCE semantics
//
//----------------------------------------------------------------
//----------------------------------------------------------------




//----------------------------------------------------------------
//                       element_range_ref
//----------------------------------------------------------------

template<class E, class R>
class element_range_ref {
  typedef element_range_ref<E,R>      self;
public:
  typedef element_traits<E>            et;
  typedef typename et::grid_type       grid_type;
  typedef grid_types<grid_type>        gt;
  typedef typename et::handle_type     elt_handle;
  typedef E                            Element;

  typedef R                                   range_type;
  typedef typename range_type::const_iterator const_iterator;
  typedef handle2element_map<E>        handle_map;

  typedef mapped_element_ra_seq_iterator<range_type, handle_map > ElementIterator;

  //  typedef HIt const_iterator; // iterator over handles
private:
  //---- DATA -----
  int ebeg, eend;
  // HIt ebeg;
  // HIt eend;
  const range_type* the_range;
  const grid_type * the_grid;
public:
  //---------------------- construction -------------------------

  //element_range_ref(HIt b, HIt e, const grid_type& g) : ebeg(b), eend(e), the_grid(&g) {}
  element_range_ref() : ebeg(0), eend(0), the_range(0), the_grid(0) {}

  // should this be the empty range or the whole range ??
  element_range_ref(const grid_type& g) 
    : ebeg(0), eend(0), the_range(0), the_grid(&g) {}
  element_range_ref(const range_type& r, const grid_type& g) 
    : ebeg(0), eend(0), the_range(&r), the_grid(&g) {}

  element_range_ref(int b, int e, const range_type& r,  const grid_type& g) 
    : ebeg(b), eend(e), the_range(&r), the_grid(&g) {}

  element_range_ref(const enumerated_element_range<E>& er);

  //---------------------- component access ---------------------

  //  const grid_type& TheGrid() const { return the_range->TheGrid();}
  grid_type const& TheGrid() const { return (*the_grid);}

  //---------------------- size queries -------------------------

  unsigned NumOfElements() const { return size();}

  // STL-like
  unsigned size()  const { return (eend - ebeg);} // random access!
  bool     empty() const { return (eend == ebeg);}

  //---------------------- iteration ----------------------------

  ElementIterator FirstElement() const 
    { return ElementIterator(ebeg,eend,
			     *the_range,handle_map(TheGrid()));}
  ElementIterator EndElement() const 
    { return ElementIterator(eend,eend,
			     *the_range,handle_map(TheGrid()));}

  // STL-like iteration over handles
  // WARNING: THESE ITERATORS GET INVALID IF THE RANGE IS EXTENDED !!
  const_iterator begin() const { return (the_range->begin() + ebeg);}
  const_iterator end()   const { return (the_range->begin() + eend);}

  MAKE_DEFAULT_OPS_SELF
};


//----------------------------------------------------------------
//     vertex_range_ref / facet_range_ref / cell_range_ref
//                    (reference semantics)
//----------------------------------------------------------------

template<class Grid, class R>
class vertex_range_ref
  : public element_range_ref<typename grid_types<Grid>::Vertex, R>
{
  typedef element_range_ref<typename grid_types<Grid>::Vertex, R> base;
public:
  typedef typename base::Element         Vertex;
  typedef typename base::ElementIterator VertexIterator;

  vertex_range_ref() {}
  vertex_range_ref(const grid_type& g) : base(g) {}
  vertex_range_ref(const range_type& r, const grid_type& g) : base(r,g) {}
  vertex_range_ref(int b, int e, const range_type& r, const grid_type& g) 
    : base(b,e,r,g) {}
  vertex_range_ref(const base& b) : base(b) {}

  
  unsigned       NumOfVertices() const { return size();}
  VertexIterator FirstVertex()   const { return FirstElement();}
  VertexIterator EndVertex  ()   const { return EndElement();}

};


template<class Grid, class R>
class facet_range_ref
  : public element_range_ref<typename grid_types<Grid>::Facet, R>
{
  typedef element_range_ref<typename grid_types<Grid>::Facet, R> base;
public:
  typedef typename base::Element         Facet;
  typedef typename base::ElementIterator FacetIterator;

  facet_range_ref() {}
  facet_range_ref(const grid_type& g) : base(g) {}
  facet_range_ref(const range_type& r, const grid_type& g) : base(r,g) {}
  facet_range_ref(int b, int e, const range_type& r, const grid_type& g) 
    : base(b,e,r,g) {}
  facet_range_ref(const base& b) : base(b) {}
  
  unsigned       NumOfFacets() const { return size();}
  FacetIterator  FirstFacet()  const { return FirstElement();}
  FacetIterator  EndFacet()    const { return EndElement();}

};


template<class Grid, class R>
class cell_range_ref
  : public element_range_ref<typename grid_types<Grid>::Cell, R>
{
  typedef element_range_ref<typename grid_types<Grid>::Cell, R> base;
public:
  typedef typename base::Element         Cell;
  typedef typename base::ElementIterator CellIterator;


  cell_range_ref() {}
  cell_range_ref(const grid_type& g) : base(g) {}
  cell_range_ref(const range_type& r, const grid_type& g) : base(r,g) {}
  cell_range_ref(int b, int e, const range_type& r, const grid_type& g) 
    : base(b,e,r,g) {}
  cell_range_ref(const base& b) : base(b) {}

  
  unsigned       NumOfCells() const { return size();}
  CellIterator   FirstCell()  const { return FirstElement();}
  CellIterator   EndCell()    const { return EndElement();}

};


//----------------------------------------------------------------
//----------------------------------------------------------------
//
//             Classes  with VALUE semantics
//
//----------------------------------------------------------------
//----------------------------------------------------------------


//----------------------------------------------------------------
//              enumerated_element_range (value semantics)
//----------------------------------------------------------------


template<class E>
class enumerated_element_range {
  typedef enumerated_element_range<E> self;
  MAKE_DEFAULT_OPS_SELF
public:
  //------- referenced types ------------------
  typedef element_traits<E>            et;
  typedef typename et::grid_type       grid_type;
  typedef grid_types<grid_type>        gt;
  typedef typename et::handle_type     elt_handle;
  typedef E                            Element;

  //---------- own types ----------------------

  typedef vector<elt_handle>                    elt_sequence;
  typedef elt_sequence                          container_type;
  typedef typename elt_sequence::const_iterator seq_iterator;
  typedef typename elt_sequence::const_iterator const_iterator;
  typedef typename elt_sequence::iterator       iterator;

  typedef handle2element_map<E>        handle_map;
  typedef mapped_element_ra_seq_iterator<container_type,
                                         handle_map > ElementIterator;


protected:
  const grid_type* the_grid;
  elt_sequence     elements;

public:
  //-------------------- construction --------------------------

  enumerated_element_range() : the_grid(0) {}
  enumerated_element_range(const grid_type& g) : the_grid(&g) {}

  void set_grid(const grid_type& g) {
    REQUIRE(elements.empty(), "set_grid(): range is not empty!",1);
    the_grid = &g;
  }

  void append(elt_handle v) { elements.push_back(v);}
  void append(Element v)    { elements.push_back(TheGrid().handle(v));}
  void push_back(elt_handle v) { elements.push_back(v);}
  void push_back(Element v)    { elements.push_back(TheGrid().handle(v));}

  //-------------------- component access ----------------------

  const grid_type&    TheGrid() const {
    REQUIRE((the_grid!=0),"no grid!\n",1);
    return *the_grid;
  }

  const elt_sequence& TheContainer() const { return elements;}
  elt_sequence&       TheContainer()       { return elements;}

  //-------------------- size queries --------------------------

  unsigned NumOfElements() const { return elements.size();}

  bool     empty() const { return elements.empty();}
  unsigned size()  const { return elements.size();}

  //-------------------- iteration  ----------------------------

  ElementIterator FirstElement() const 
    { return ElementIterator(0,elements.size(),elements, handle_map(*the_grid));}
  ElementIterator EndElement() const 
    { return ElementIterator(elements.size(),elements.size(),
			     elements, handle_map(*the_grid));}

  const_iterator begin() const { return elements.begin();}
  const_iterator end  () const { return elements.end  ();}
  iterator       begin()       { return elements.begin();}
  iterator       end  ()       { return elements.end  ();}
};


//----------------------------------------------------------------
//     vertex_range / facet_range / cell_range (value semantics)
//----------------------------------------------------------------


template<class Grid>
class enumerated_vertex_range 
  : public enumerated_element_range<typename grid_types<Grid>::Vertex>
{
  typedef enumerated_element_range<typename grid_types<Grid>::Vertex> base;
public:
  typedef typename base::Element         Vertex;
  typedef typename base::ElementIterator VertexIterator;

  typedef vertex_range_ref<grid_type, elt_sequence>  range_type_ref;

  enumerated_vertex_range() {}
  enumerated_vertex_range(const grid_type& g) : base(g) {}
  enumerated_vertex_range(const base& b) : base(b) {}

  
  unsigned NumOfVertices()     const { return size();}
  VertexIterator FirstVertex() const { return FirstElement();}
  VertexIterator EndVertex()   const { return EndElement();}

  range_type_ref range() const 
    { return range_type_ref(0,size(),elements,*the_grid);}
};


template<class Grid>
class enumerated_facet_range 
  : public enumerated_element_range<typename grid_types<Grid>::Facet>
{
  typedef enumerated_element_range<typename grid_types<Grid>::Facet> base;
public:
  typedef typename base::Element         Facet;
  typedef typename base::ElementIterator FacetIterator;
  
  typedef facet_range_ref<grid_type, elt_sequence>  range_type_ref;

  enumerated_facet_range() {}
  enumerated_facet_range(const grid_type& g) : base(g) {}
  enumerated_facet_range(const base& b) : base(b) {}
  
  unsigned NumOfFacets()     const { return size();}
  FacetIterator FirstFacet() const { return FirstElement();}
  FacetIterator EndFacet()   const { return EndElement();}

  range_type_ref range() const 
    { return range_type_ref(0,size(),elements,*the_grid);}

};

template<class Grid>
class enumerated_cell_range 
  : public enumerated_element_range<typename grid_types<Grid>::Cell>
{
  typedef enumerated_element_range<typename grid_types<Grid>::Cell> base;
public:
  typedef typename base::Element         Cell;
  typedef typename base::ElementIterator CellIterator;

  typedef cell_range_ref<grid_type, elt_sequence>  range_type_ref;

  enumerated_cell_range() {}
  enumerated_cell_range(const grid_type& g) : base(g) {}
  enumerated_cell_range(const base& b) : base(b) {}
  
  unsigned NumOfCells()     const { return size();}
  CellIterator FirstCell()  const { return FirstElement();}
  CellIterator EndCell()    const { return EndElement();}

  range_type_ref range() const 
    { return range_type_ref(0,size(),elements,*the_grid);}
};


//----------------------------------------------------------------
//             inline functions of range-references
//----------------------------------------------------------------


/*
template<class E, class R>
inline  
element_range_ref<E,R>:: element_range_ref(const enumerated_element_range<E>& er)
  : ebeg(er.begin()), eend(er.end()), the_grid(& er.TheGrid()) {}
*/


//----------------------------------------------------------------
//
// template<class Grid> class enumerated_subrange;
//
// A grid subrange that is a simple enumeration of its elements.
// This class has value-semantics, i.e. really stores
// the enumerated sequence.
// There is no check if the different element ranges are consistent,
// i.e. if the sequence of vertices is exactly the set of adjacent
// vertices to the sequence of cells. This has to be ensured at
// construction time, see ConstructSubrange(...) below.
//
//----------------------------------------------------------------

template<class Grid>
class enumerated_subrange {
  typedef enumerated_subrange<Grid> self;
  MAKE_DEFAULT_OPS_SELF
public:
  //------- referenced types ------------------
  typedef Grid grid_type;
  typedef grid_types<Grid> gt;
  typedef typename gt::vertex_handle vertex_handle;
  typedef typename gt::cell_handle   cell_handle;
  typedef typename gt::Cell                 Cell;
  typedef typename gt::Vertex               Vertex;
  typedef typename gt::VertexOnCellIterator VertexOnCellIterator;
  typedef tp<Vertex> tpV;
  typedef tp<Cell>   tpC;

  //---------- own types ----------------------
  typedef vector<vertex_handle> vertex_sequence;
  typedef vector<cell_handle>   cell_sequence;
  typedef typename vertex_sequence::const_iterator v_iterator;
  typedef typename cell_sequence  ::const_iterator c_iterator;

  //  typedef vertex_range_ref<grid_type,v_iterator>  vertex_range_type;
  //  typedef cell_range_ref  <grid_type,c_iterator>  cell_range_type;
  typedef vertex_range_ref<grid_type, vertex_sequence>  vertex_range_type;
  typedef cell_range_ref  <grid_type, cell_sequence  >  cell_range_type;

private:
  const grid_type* the_grid;

  vertex_sequence vertices;
  //  edge_sequence   edges;
  cell_sequence   cells;
  // note : consistency in the sence that vertices = V(cells)
  // (set of adj. vertices of cells) cannot be ensured here!

public:
  //-------------------- construction --------------------------

  enumerated_subrange()                   : the_grid(0)  {}
  enumerated_subrange(const grid_type& g) : the_grid(&g) {}


  void append_vertex(vertex_handle v) { vertices.push_back(v);}
  void append_cell(cell_handle v)     { cells.push_back(v);}
		     

  //-------------------- iteration  ----------------------------

  typedef mapped_element_ra_seq_iterator<vertex_sequence,
                                      handle2element_map<Vertex> > VertexIterator;
  typedef mapped_element_ra_seq_iterator<cell_sequence,   
                                      handle2element_map<Cell> >   CellIterator;
  typedef facet_set_of_cells_iterator<CellIterator>                FacetIterator;

  //                                    handle2element_map<Cell> >   CellIterator;
  // 2D only!
  typedef FacetIterator                                            EdgeIterator;

  unsigned NumOfCells()    const { return cells.size();}
  unsigned NumOfVertices() const { return vertices.size();}
  // valid only if homeomorphic to a disk!
  unsigned NumOfEdges()    const 
    { return NumOfCells() + NumOfVertices() - 2 + 1; /* 1 == NumOfBoundaryComponents */}

  CellIterator   FirstCell()   const 
    { 
      return CellIterator(0,cells.size(),
			  cells,
			  handle2element_map<Cell>(*the_grid));
    }
  CellIterator   EndCell()   const 
    { 
      return CellIterator(cells.size(),cells.size(),
			  cells,
			  handle2element_map<Cell>(*the_grid));
    }
  VertexIterator FirstVertex() const 
    {
      return VertexIterator(0, vertices.size(),
			    vertices,
			    handle2element_map<Vertex>(*the_grid));
    }
  VertexIterator EndVertex() const 
    {
      return VertexIterator(vertices.size(), vertices.size(),
			    vertices,
			    handle2element_map<Vertex>(*the_grid));
    }
  FacetIterator  FirstFacet() const { return FacetIterator(FirstCell());}  
  FacetIterator  FirstEdge()  const { return FacetIterator(FirstCell());}  
  FacetIterator  EndFacet() const { return FacetIterator(EndCell());}  
  FacetIterator  EndEdge()  const { return FacetIterator(EndCell());}  

  vertex_range_type range(tpV) const 
    { return vertex_range_type(0, vertices.size(), vertices, *the_grid);}
  vertex_range_type vertex_range() const 
    { return vertex_range_type(0, vertices.size(), vertices, *the_grid);}
  vertex_range_type Vertices() const 
    { return vertex_range_type(0, vertices.size(), vertices, *the_grid);}

  cell_range_type   range(tpC) const 
    { return cell_range_type  (0,(int) cells.size(),    cells,    *the_grid);}
  cell_range_type   cell_range() const 
    { return cell_range_type  (0,(int) cells.size(),    cells,    *the_grid);}
  cell_range_type   Cells() const 
    { return cell_range_type  (0,(int) cells.size(),    cells,    *the_grid);}

  const grid_type& TheGrid() const { return *the_grid;}
  cell_handle   handle(const Cell& C)   const { return TheGrid().handle(C);}
  vertex_handle handle(const Vertex& V) const { return TheGrid().handle(V);}
};
  

template<class Grid>
class enumerated_subrange_ref {
  typedef enumerated_subrange_ref<Grid> self;
  MAKE_DEFAULT_OPS_SELF
public:
  //------- referenced types ------------------
  typedef Grid grid_type;
  typedef grid_types<Grid> gt;
  typedef typename gt::vertex_handle vertex_handle;
  typedef typename gt::cell_handle   cell_handle;
  typedef typename gt::Cell                 Cell;
  typedef typename gt::Vertex               Vertex;
  typedef typename gt::VertexOnCellIterator VertexOnCellIterator;
  typedef tp<Vertex> tpV;
  typedef tp<Cell>   tpC;

  //---------- own types ----------------------

  typedef enumerated_subrange<Grid> range_type;
  typedef typename range_type::vertex_range_type vertex_range_ref_t;
  typedef typename range_type::cell_range_type   cell_range_ref_t;

private:
  //---- DATA ---------
  vertex_range_ref_t vertices;
  //  edge_range_ref   edges;
  cell_range_ref_t   cells;
  // note : consistency in the sence that vertices = V(cells)
  // (set of adj. vertices of cells) cannot be ensured here!

public:
  //-------------------- construction --------------------------

  enumerated_subrange_ref()  {}
  enumerated_subrange_ref(const range_type& R)
    : vertices(R.vertex_range()), cells(R.cell_range()) {}
  enumerated_subrange_ref(const vertex_range_ref_t& V,
			  const cell_range_ref_t  & C)
    : vertices(V), cells(C) {}


  //-------------------- iteration  ----------------------------

  typedef typename vertex_range_ref_t::VertexIterator  VertexIterator;
  typedef typename cell_range_ref_t  ::CellIterator   CellIterator;
  typedef facet_set_of_cells_iterator<CellIterator>   FacetIterator;
  // 2D only!
  typedef FacetIterator                                EdgeIterator;

  unsigned NumOfCells()    const { return cells.size();}
  unsigned NumOfVertices() const { return vertices.size();}
  // valid only if homeomorphic to a disk!
  unsigned NumOfEdges()    const 
    { return NumOfCells() + NumOfVertices() - 2 + 1; /* 1 == NumOfBoundaryComponents */}

  CellIterator   FirstCell()   const  { return cells.FirstCell();} 
  VertexIterator FirstVertex() const  { return vertices.FirstVertex();}
  FacetIterator  FirstFacet()  const  { return FacetIterator(FirstCell());}  
  FacetIterator  FirstEdge()   const  { return FacetIterator(FirstCell());}  

  CellIterator   EndCell()   const  { return cells.EndCell();} 
  VertexIterator EndVertex() const  { return vertices.EndVertex();}
  FacetIterator  EndFacet()  const  { return FacetIterator(EndCell());}  
  FacetIterator  EndEdge()   const  { return FacetIterator(EndCell());}  
  
  vertex_range_ref_t range(tpV) const { return vertices; }
  cell_range_ref_t   range(tpC) const { return cells; }

  grid_type const& TheGrid() const { return vertices.TheGrid();}
  cell_handle   handle(const Cell& C)   const { return TheGrid().handle(C);}
  vertex_handle handle(const Vertex& V) const { return TheGrid().handle(V);}
};
  

// with partial specialization, we could
// take this directly as definition for grid_types.
// now do:
// grid_types<enumerated_subrange<Complex2D> > 
//  : public grid_types_esr<Complex2D> {};

template<class Grid>
struct grid_types_esr  {
  typedef grid_types<Grid> bgt;
  typedef typename bgt::Cell   Cell;
  typedef typename bgt::Vertex Vertex;
  typedef typename bgt::vertex_handle         vertex_handle;
  typedef typename bgt::cell_handle           cell_handle;

  typedef enumerated_subrange<Grid>           range_type;
  typedef typename range_type::VertexIterator VertexIterator;
  typedef typename range_type::EdgeIterator   EdgeIterator;
  typedef typename range_type::FacetIterator  FacetIterator;
  typedef typename range_type::CellIterator   CellIterator;

  typedef typename bgt::VertexOnCellIterator  VertexOnCellIterator;
  typedef typename bgt::EdgeOnCellIterator    EdgeOnCellIterator;
  typedef typename bgt::FacetOnCellIterator   FacetOnCellIterator;
  typedef typename bgt::CellOnCellIterator    CellOnCellIterator;
};


template<class Grid>
struct grid_types_esr_ref  {
  typedef grid_types<Grid> bgt;
  typedef typename bgt::Cell   Cell;
  typedef typename bgt::Vertex Vertex;
  typedef typename bgt::vertex_handle         vertex_handle;
  typedef typename bgt::cell_handle           cell_handle;

  typedef enumerated_subrange_ref<Grid>           range_type;
  typedef typename range_type::VertexIterator VertexIterator;
  typedef typename range_type::EdgeIterator   EdgeIterator;
  typedef typename range_type::FacetIterator  FacetIterator;
  typedef typename range_type::CellIterator   CellIterator;

  typedef typename bgt::VertexOnCellIterator  VertexOnCellIterator;
  typedef typename bgt::EdgeOnCellIterator    EdgeOnCellIterator;
  typedef typename bgt::FacetOnCellIterator   FacetOnCellIterator;
  typedef typename bgt::CellOnCellIterator    CellOnCellIterator;
};

/*
enable this when partial specialization is available:
*/

#ifdef NMWR_PARTIAL_SPEC

template<class Grid>
struct grid_types<enumerated_subrange<Grid> >
 :  public grid_types_esr<Grid> {};

template<class Grid>
struct grid_types<enumerated_subrange_ref<Grid> >
 :  public grid_types_esr_ref<Grid> {};

#endif


  


//----------------------------------------------------------------
//  ConstructSubrangeFromCells
//
//  This template function constructs an enumerated subrange 
//  over a Grid G from  a set of cells described by the range of
//  CellIt Cit.
//----------------------------------------------------------------


  
template<class Range, class CellIt>
void ConstructSubrangeFromCells
 (Range& R,   // out: the subrange to be constructed
  //  const Grid&         G,          // in : base  grid
  CellIt              Cit);        // in : range(Cit) is the cell subrange 


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Grids/generic/enumerated-subrange.C"
#endif


#endif
