#ifndef GRAL_GB_CARTESIANND_CARTESIANND_H
#define GRAL_GB_CARTESIANND_CARTESIANND_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"

#include "Container/index-map-nd.h"
#include "Container/range.h"
#include "Container/combinatorial.h"

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/partial-grid-function-hash.h"
#include "Gral/Base/archetype-0D.h"

#include "Gral/Grids/CartesianND/direction-maps.h"
#include "Gral/Grids/CartesianND/archetypes.h"

#include <vector>
#include <iostream>

#include <cmath>


namespace GrAL {

namespace cartesiannd {
  
  //-------------- forward declarations ---------------

  template<class CARTGRID, unsigned K>
  class element_handle_t;
  template<class CARTGRID, unsigned K>
  class sequence_iterator_t;
  template<class CARTGRID, unsigned DA, unsigned DE>
  class downward_inc_iterator_t;
  template<class CARTGRID, unsigned DA, unsigned DE>
  class upward_inc_iterator_t;
  template<unsigned DIM>
  class subrange;
  template<unsigned DIM>
  class grid;



 template<class CARTGRID, unsigned K>
 inline bool operator==(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs);
 template<class CARTGRID, unsigned K>
 inline bool operator<(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs);

#ifndef GRAL_INCLUDES_RELOPS
 template<class CARTGRID, unsigned K>
 inline bool operator!=(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs);
 template<class CARTGRID, unsigned K>
 inline bool operator>(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs);
 template<class CARTGRID, unsigned K>
 inline bool operator<=(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs);
 template<class CARTGRID, unsigned K>
 inline bool operator>=(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs);
#endif



 template<class CARTGRID, unsigned DA, unsigned DE>
 inline bool operator==(downward_inc_iterator_t<CARTGRID,DA, DE> lhs, 
			downward_inc_iterator_t<CARTGRID,DA, DE> rhs);
  //#ifndef GRAL_INCLUDES_RELOPS
 template<class CARTGRID, unsigned DA, unsigned DE>
 inline bool operator!=(downward_inc_iterator_t<CARTGRID,DA, DE> lhs, 
			downward_inc_iterator_t<CARTGRID,DA, DE> rhs);
  // #endif



 template<class CARTGRID, unsigned DA, unsigned DE>
 inline bool operator==(upward_inc_iterator_t<CARTGRID,DA, DE> lhs, 
			upward_inc_iterator_t<CARTGRID,DA, DE> rhs);
  //#ifndef GRAL_INCLUDES_RELOPS
 template<class CARTGRID, unsigned DA, unsigned DE>
 inline bool operator!=(upward_inc_iterator_t<CARTGRID,DA, DE> lhs, 
			upward_inc_iterator_t<CARTGRID,DA, DE> rhs);
  //#endif

  

  template<class GRID,   unsigned D, unsigned CD>  struct element_type 
  { typedef sequence_iterator_t<GRID, D> type; };

  /*  
  template<unsigned DIM, unsigned D, unsigned CD>  struct element_type<subrange<DIM>, D, CD> 
  { typedef sequence_iterator_t<grid<DIM>, D> type; };
  */

  template<class CARTGRID, unsigned K> struct element_handle_type
  { typedef element_handle_t<CARTGRID, K>  type; };
  
  /*
  template<unsigned DIM, unsigned K> struct element_handle_type<subrange<DIM>, K>
  { typedef element_handle_t<grid<DIM>, K> type;};
  */

  template<unsigned K, unsigned M> struct less_then { enum { result = ( K <= M ? 1 : 0) }; };

  template<class CARTGRID, unsigned DA, unsigned DE, unsigned LESS>
  struct incidence_iterator_type_aux {};

  // DA <= DE => <DE>On<DA> is upward
  template<class CARTGRID, unsigned DA, unsigned DE>
  struct incidence_iterator_type_aux<CARTGRID,DA,DE,1> 
  { typedef upward_inc_iterator_t<CARTGRID,DA,DE> type; };
  
  
  // DA > DE => <DE>On<DA> is downward
  template<class CARTGRID, unsigned DA, unsigned DE>
  struct incidence_iterator_type_aux<CARTGRID,DA,DE,0>
  { typedef downward_inc_iterator_t<CARTGRID,DA,DE> type; };

  template<class CARTGRID, unsigned DA, unsigned DE>
  struct incidence_iterator_type : public incidence_iterator_type_aux<CARTGRID,DA,DE, less_then<DA,DE>::result> {};

  template<class CARTGRID, unsigned DIM>
  struct archetype_of_grid_aux {
    typedef archetype_t<CARTGRID> type;
  };
  /*
  template<class CARTGRID, 2>
  struct archetype_of_grid_aux {
    typedef polygon1d::polygon type;
  };
  */

  template<class CARTGRID>
  struct archetype_of_grid_aux<CARTGRID,1> {
    typedef archetype_0D::archetype_t type;
  };

  template<class CARTGRID>
  struct archetype_of_grid_aux<CARTGRID,0> {
    // not quite correct ... could define a type empty_grid with dimension -1.
    typedef grid<0> type;
  };

  template<class CARTGRID>
  struct archetype_of_grid : public archetype_of_grid_aux<CARTGRID, CARTGRID::dim> {};


  template<class CARTGRID, unsigned DIM>
  struct grid_types_base : public grid_types_detail::grid_types_root 
  {
    enum { dim = DIM};
    typedef grid_dim_tag<dim> dimension_tag;
    typedef CARTGRID grid_type;

    typedef typename element_handle_type<CARTGRID, 0>       ::type vertex_handle;
    typedef          sequence_iterator_t<CARTGRID, 0>              VertexIterator;
    typedef typename element_type       <CARTGRID, 0, DIM>  ::type Vertex;

    typedef typename element_handle_type<CARTGRID, 1>       ::type edge_handle;
    typedef          sequence_iterator_t<CARTGRID, 1>              EdgeIterator;
    typedef typename element_type       <CARTGRID, 1, DIM-1>::type Edge;

    typedef typename element_handle_type<CARTGRID, 2>       ::type face_handle;
    typedef          sequence_iterator_t<CARTGRID, 2>              FaceIterator;
    typedef typename element_type       <CARTGRID, 2, DIM-2>::type Face;

    typedef typename element_handle_type<CARTGRID, DIM-1>   ::type facet_handle;
    typedef          sequence_iterator_t<CARTGRID, DIM-1>          FacetIterator;
    typedef typename element_type       <CARTGRID, DIM-1, 1>::type Facet;

    typedef typename element_handle_type<CARTGRID, DIM>     ::type cell_handle;
    typedef          sequence_iterator_t<CARTGRID, DIM>            CellIterator;
    typedef typename element_type       <CARTGRID, DIM, 0>  ::type Cell;

    typedef typename incidence_iterator_type<CARTGRID, 0, 0>      ::type VertexOnVertexIterator;
    typedef typename incidence_iterator_type<CARTGRID, 0, 1>      ::type EdgeOnVertexIterator;
    typedef typename incidence_iterator_type<CARTGRID, 0, 2>      ::type FaceOnVertexIterator;
    typedef typename incidence_iterator_type<CARTGRID, 0, DIM-1>  ::type FacetOnVertexIterator;
    typedef typename incidence_iterator_type<CARTGRID, 0, DIM>    ::type CellOnVertexIterator;

    typedef typename incidence_iterator_type<CARTGRID, 1, 0>      ::type VertexOnEdgeIterator;
    typedef typename incidence_iterator_type<CARTGRID, 1, 2>      ::type FaceOnEdgeIterator;
    typedef typename incidence_iterator_type<CARTGRID, 1, DIM-1>  ::type FacetOnEdgeIterator;
    typedef typename incidence_iterator_type<CARTGRID, 1, DIM>    ::type CellOnEdgeIterator;

    typedef typename incidence_iterator_type<CARTGRID, 2, 0>      ::type VertexOnFaceIterator;
    typedef typename incidence_iterator_type<CARTGRID, 2, 1>      ::type EdgeOnFaceIterator;
    typedef typename incidence_iterator_type<CARTGRID, 2, DIM-1>  ::type FacetOnFaceIterator;
    typedef typename incidence_iterator_type<CARTGRID, 2, DIM>    ::type CellOnFaceIterator;

    typedef typename incidence_iterator_type<CARTGRID, DIM-1, 0>  ::type VertexOnFacetIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM-1, 1>  ::type EdgeOnFacetIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM-1, 2>  ::type FaceOnFacetIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM-1, DIM>::type CellOnFacetIterator;

    typedef typename incidence_iterator_type<CARTGRID, DIM, 0>    ::type VertexOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, 1>    ::type EdgeOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, 2>    ::type FaceOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, DIM-1>::type FacetOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, DIM>  ::type CellOnCellIterator;



    // We could distinguish vertex-  and cell_index_type.
    // This would allow for clearer construction semantics
    typedef tuple<int, DIM> index_type;
    /*
    // simple integer wrappers
    template<class TAG>
    struct wrap_int { 
      typedef wrap_int<TAG> self;

      int i;
      operator int() const { return i;}
      wrap_int() {}

      template<class U>
      wrap_int(U u) : i(int(u)) {}
      self& operator++() { ++i; return *this;}
      self& operator--() { --i; return *this;}
    
      template<class U>
      self& operator+=(U r) { i += r; return *this;}
     
      template<class U>
      self& operator-=(U r) { i -= r; return *this;}
      
    };


    typedef tuple<wrap_int<vertex_type_tag>,dim> vertex_index_type;
    typedef tuple<wrap_int<cell_type_tag>  ,dim> cell_index_type;
    */  
    typedef tuple<int, DIM> vertex_index_type;
    typedef tuple<int, DIM> cell_index_type;


      /*
    class  vertex_index_type : public index_type 
    {
      typedef index_type base;
    public:
      vertex_index_type() {}
      explicit vertex_index_type(int i) : base(i) {}
      explicit vertex_index_type(base b) : base(b) {}
    };

    class  cell_index_type : public index_type 
    {
      typedef index_type base;
    public:
      cell_index_type() {}
      explicit cell_index_type(int i)  : base(i) {}
      explicit cell_index_type(base b) : base(b) {}
    };
      */



    typedef typename archetype_of_grid_aux<CARTGRID,DIM>::type archetype_type;
    typedef unsigned                                   archetype_handle;
    typedef archetype_type const*                      archetype_iterator;
    typedef grid_types<archetype_type>                 archgt;
  }; // struct grid_types_base<unsigned DIM>
 


  //--------------------- element handles -------------------

  template<class CARTGRID, unsigned K>
  class element_handle_t {
    typedef element_handle_t<CARTGRID,K> self;
    int h_;
  public:
    typedef grid_element_handle_category_d<K> category;

    element_handle_t() {}
    explicit element_handle_t(int hh) : h_(hh) {}
    int h() const { return h_;}
    operator int() const { return h_;}
    self& operator++() {++h_; return *this;}
    //operator int () const { return h;}
  };

  template<class CARTGRID, unsigned K>
  inline bool operator==(element_handle_t<CARTGRID,K> lhs, element_handle_t<CARTGRID,K> rhs) { return lhs.h() == rhs.h();}
  template<class CARTGRID, unsigned K>
  inline bool operator< (element_handle_t<CARTGRID,K> lhs, element_handle_t<CARTGRID,K> rhs) { return lhs.h() <  rhs.h();}
  //#ifndef GRAL_INCLUDES_RELOPS
  template<class CARTGRID, unsigned K>
  inline bool operator!=(element_handle_t<CARTGRID,K> lhs, element_handle_t<CARTGRID,K> rhs) { return lhs.h() != rhs.h();}
  template<class CARTGRID, unsigned K>
  inline bool operator> (element_handle_t<CARTGRID,K> lhs, element_handle_t<CARTGRID,K> rhs) { return lhs.h() >  rhs.h();}
  template<class CARTGRID, unsigned K>
  inline bool operator<=(element_handle_t<CARTGRID,K> lhs, element_handle_t<CARTGRID,K> rhs) { return lhs.h() <= rhs.h();}
  template<class CARTGRID, unsigned K>
  inline bool operator>=(element_handle_t<CARTGRID,K> lhs, element_handle_t<CARTGRID,K> rhs) { return lhs.h() >= rhs.h();}
  //#endif

  template<class CARTGRID, unsigned K>
  inline  ::std::ostream& operator<<( ::std::ostream& out, element_handle_t<CARTGRID,K> lhs)
  { return (out << lhs.h());}
  template<class CARTGRID, unsigned K>
  inline  ::std::istream& operator>>( ::std::istream& in,  element_handle_t<CARTGRID,K> lhs)
  { return (in >> lhs.h());}

} // namespace cartesiannd

// global namespace 
template<class R, class CARTGRID, unsigned K>
inline R convert(cartesiannd::element_handle_t<CARTGRID,K> const& h) { return h.h();}

} // namespace GrAL

namespace STDEXT {
  template<class T> class hash;

  template<class CARTGRID, unsigned K>
  class hash<GrAL::cartesiannd::element_handle_t<CARTGRID,K> > {
  public:
    typedef GrAL::cartesiannd::element_handle_t<CARTGRID,K>  key_type;
    typedef GrAL::cartesiannd::element_handle_t<CARTGRID,K>  argument_type;
    typedef size_t                        result_type;
    result_type operator()(key_type h) const { return h.h();}
  }; 
} // namespace STDEXT 


namespace GrAL {
namespace cartesiannd {


  //---------------------------- grid types ----------------------------

  /*! \brief Base class for n-dimensional Cartesian grids
      \ingroup cartesianndmodule
   */
  template<class CARTGRID, unsigned DIM>
  class grid_base : public grid_types_base<CARTGRID, DIM> {
    typedef grid_types_base<CARTGRID, DIM> base;
  public:
    typedef typename base::size_type         size_type;
    typedef typename base::vertex_index_type vertex_index_type;
    typedef typename base::cell_index_type   cell_index_type;
    typedef typename base::index_type        index_type;
    typedef typename base::VertexIterator    VertexIterator;
    typedef typename base::EdgeIterator      EdgeIterator;
    typedef typename base::FaceIterator      FaceIterator;
    typedef typename base::FacetIterator     FacetIterator;
    typedef typename base::CellIterator      CellIterator;
    typedef typename base::Vertex    Vertex;
    typedef typename base::Edge      Edge;
    typedef typename base::Face      Face;
    typedef typename base::Facet     Facet;
    typedef typename base::Cell      Cell;

    typedef typename base::VertexOnEdgeIterator VertexOnEdgeIterator;
    typedef typename base::CellOnFacetIterator  CellOnFacetIterator;

    enum { dim = DIM };
    // protected:
  public:
    map_table<DIM>    maps   [DIM+1]; // [0, ..., DIM]
    offset_table<DIM> offsets[DIM+1];

    void init(vertex_index_type low, vertex_index_type beyond);
  public:
    //! Combinatorial dimension
    unsigned dimension() const { return dim;}

    //@{
    /*! \name Size information and bounds
     */
    //! \brief Lowest vertex index, valid vertex indices are \Ge \c low_vertex_index() 
    vertex_index_type low_vertex_index()    const { return maps[0][0].min_tuple();}
    //! \brief Highest vertex index, valid vertex indices are \Le \c high_vertex_index() 
    vertex_index_type high_vertex_index()   const { return maps[0][0].max_tuple();}
    //! \brief Past-the-end vertex index, valid vertex indices are \Lt \c high_vertex_index() 
    vertex_index_type beyond_vertex_index() const { return maps[0][0].beyond_tuple();}
    //! \brief Number of vertices in each direction
    vertex_index_type vertex_size()         const { return beyond_vertex_index() - low_vertex_index();}


    //! \brief Lowest cell index, valid cell indices are \Ge \c low_cell_index() 
    cell_index_type low_cell_index()    const { return maps[DIM][0].min_tuple();}
    //! \brief Highest cell index, valid cell indices are \Le \c high_cell_index() 
    cell_index_type high_cell_index()   const { return maps[DIM][0].max_tuple();}
    //! \brief Past-the-end cell index, valid cell indices are \Lt \c high_cell_index() 
    cell_index_type beyond_cell_index() const { return maps[DIM][0].beyond_tuple();}

    //! \brief Number of cells in each direction
    cell_index_type cell_size() const { return beyond_cell_index() - low_cell_index();}
    //@}

    /*! \brief Number of possible element direction in dimension \c k
  
        For dimensions <tt> k == 0 </tt> and <tt> k == DIM </tt>, there is only one direction.
	In 3D, there are 3 directions (x,y, and z) for edges (<tt> k == 1 </tt>) ,
	and 3 directions (xy, xz, yz) for facets (<tt> k == 2</tt>).

        In general, there are \f$ k \choose  \mbox{DIM} \f$  directions.
     */
    unsigned NumOfDirections(unsigned k) const { 
      cvdim(k);
      return maps[k].num_of_directions();
    }
    
    template<unsigned K>
    void switch_element(sequence_iterator_t<CARTGRID,K-1> const& e_k_1,
			sequence_iterator_t<CARTGRID,K  >      & e_k,
			sequence_iterator_t<CARTGRID,K+1> const& e_k1) const
    {
      index_type i_k = e_k.index();
      unsigned   m_k = e_k.direction();
      delta_map<DIM>::switch_index(e_k_1.index(), e_k_1.direction(),
				   i_k, m_k,
				   e_k1 .index(), e_k1 .direction(),
				   K);
      e_k = sequence_iterator_t<CARTGRID,K>(e_k.TheGrid(), i_k, m_k);
    }
    void switch_element(sequence_iterator_t<CARTGRID,0>      & e_0,
			sequence_iterator_t<CARTGRID,1> const& e_1) const

    { VertexOnEdgeIterator v(e_1);  e_0 = (e_0 == *v ? *(++v) : *v);}

    void switch_element(sequence_iterator_t<CARTGRID,dim-1> const& f,
			sequence_iterator_t<CARTGRID,dim>        & c) const

    { CellOnFacetIterator cf(f); c = ( c == *cf ? *(++cf) : *cf); }


    void switch_vertex(Vertex& v, Edge const& e) const { switch_element(v,e);}
    void switch_edge  (Vertex const& v, Edge& e, Face const& f) const { switch_element(v,e,f);}
    void switch_cell  (Facet const& f, Cell& c)  const { switch_element(f,c);}
    typedef sequence_iterator_t<CARTGRID,dim-2> Ridge;
    void switch_facet (Ridge const& r, Facet& f, Cell const& c) const { switch_element(r,f,c);}

    //@{
    /*! \name Sequence iteration (Dimension-independent interface)
     */
    //! \brief Number of elements of dimension \c K (compile-time dimension)
    template<unsigned K>
    size_type NumOfElements()           const { cvdim(K); return offsets[K][maps[K].num_of_maps()];}
    //! \brief Number of elements of dimension \c k (run-time dimension)
    size_type NumOfElements(unsigned k) const { cvdim(k); return offsets[k][maps[k].num_of_maps()];}
    
    //! \brief First element of dimension \c K
    template<unsigned K>
    sequence_iterator_t<CARTGRID, K> FirstElement() const;
    //! \brief Past-the-end element of dimension \c K
    template<unsigned K>
    sequence_iterator_t<CARTGRID, K> EndElement()   const;
    //@}

    //@{
    /*! \name Sequence iteration
        \brief STL-style (begin/end) and GrAL-style (IsDone) iteration  
    */
    size_type NumOfVertices() const { return NumOfElements(0);}
    size_type NumOfEdges()    const { return NumOfElements(1);}
    size_type NumOfFaces()    const { return NumOfElements(2);}
    size_type NumOfFacets()   const { return NumOfElements(DIM-1);}
    size_type NumOfCells()    const { return NumOfElements(DIM);}

    VertexIterator FirstVertex()  const { return FirstElement<0>();}
    EdgeIterator   FirstEdge()    const { return FirstElement<1>();}
    FaceIterator   FirstFace()    const { return FirstElement<2>();}
    FacetIterator  FirstFacet()   const { return FirstElement<DIM-1>();}
    CellIterator   FirstCell()    const { return FirstElement<DIM>();}

    VertexIterator EndVertex()    const { return EndElement<0>();}
    EdgeIterator   EndEdge()      const { return EndElement<1>();}
    FaceIterator   EndFace()      const { return EndElement<2>();}
    FacetIterator  EndFacet()     const { return EndElement<DIM-1>();}
    CellIterator   EndCell()      const { return EndElement<DIM>();}
    //@}

    CARTGRID      & to_derived()       { return static_cast<CARTGRID      &>(*this);}
    CARTGRID const& to_derived() const { return static_cast<CARTGRID const&>(*this);}

    //@{
    //! \name Indices, directions and handles

    //! \brief Convert (index, direction) to handle
    template<unsigned K>
    element_handle_t<CARTGRID, K> index2handle(index_type idx, unsigned dir) const 
    { return element_handle_t<CARTGRID, K>(maps[K][dir](idx) + offsets[K][dir]); }

    //! \brief Convert vertex index to handle
    typename base::vertex_handle get_vertex_handle(index_type idx) const { return index2handle<0>(idx,0);}
    //! \brief Convert cell index to handle
    typename base::cell_handle   get_cell_handle  (index_type idx) const { return index2handle<DIM>(idx,0);}
 
   //! \brief Convert (handle,direction) to index
    template<unsigned K>
    index_type handle2index(element_handle_t<CARTGRID, K> h, unsigned dir) const
    { return  (maps[K][dir](h.h() - offsets[K][dir])); }

    //! \brief Check handle for validity
    template<unsigned K>
    bool valid_handle(element_handle_t<CARTGRID, K> h)  const { return 0 <= h.h() && h.h() < (int) offsets[K].beyond();}
    //! \brief Check (direction, index) for validity
    template<unsigned K>
    bool valid_index(int dir, index_type idx) const { return maps[K][dir].valid(idx);}
    //! \brief Check vertex index for validity
    bool valid_vertex_index(vertex_index_type idx) const { return valid_index<0  >(0,idx);}
    //! \brief Check cell index for validity
    bool valid_cell_index  (cell_index_type   idx) const { return valid_index<dim>(0,idx);}
    //@}

    bool valid_dim(unsigned k) const { return k <= DIM;}
    void cvdim    (unsigned k) const { REQUIRE(valid_dim(k), "k=" << k << " DIM=" << DIM, 1);}

    void print(std::ostream& out) const;


    //@{
    /*! \name Archetype handling
        \brief See $GrAL ArchetypedGrid
     */
    typedef typename base::archetype_type     archetype_type;
    typedef typename base::archetype_handle   archetype_handle;
    typedef typename base::archetype_iterator archetype_iterator;

    static archetype_type *       archetype_array;
    static int *                  archetype_initialized;

    static archetype_iterator    BeginArchetype()  { init_archetypes(); return archetype_array;}
    static archetype_iterator    EndArchetype()    { return BeginArchetype() + 1;}
    static unsigned              NumOfArchetypes() { return 1;}
    static archetype_type const& TheArchetype()  { return * BeginArchetype();} // archetype_array[0];}
    static archetype_handle      the_archetype() { return 0;}
    static archetype_type const& ArchetypeOf(typename base::Cell const&) { return TheArchetype();}
    static archetype_type const& ArchetypeOf(typename base::cell_handle) { return TheArchetype();}
    static archetype_type const& Archetype  (archetype_handle) { return TheArchetype();}
    static archetype_handle      handle(archetype_iterator ait) 
    { REQUIRE(ait == BeginArchetype(), "", 1); return (ait - BeginArchetype());} 
    static archetype_handle        archetype_of(typename base::cell_handle) { return 0;}
    static archetype_handle        archetype_of(typename base::Cell const&) { return 0;}
    //@}

    static void init_archetypes() { 
      if(! archetype_initialized ) {
	archetype_initialized = new int(1);
	archetype_array = new archetype_type[1];
	// base_dim_1::init_archetypes();
      }
    }

  }; // class grid_base<unsigned DIM>


  template<class CARTGRID, unsigned DIM>
  typename grid_base<CARTGRID, DIM>::archetype_type *
  grid_base<CARTGRID, DIM>::archetype_array = 0;
  template<class CARTGRID, unsigned DIM>
  int * grid_base<CARTGRID, DIM>::archetype_initialized = 0;




  /*! \brief Subrange of a Cartesian grid

      \ingroup cartesianndmodule
      \see Test in  \ref test-cartesiannd-construct.C
   */
  template<unsigned DIM>
  class subrange : public grid_base<subrange<DIM>, DIM> {
    typedef grid_base<subrange<DIM>, DIM> base;
    typedef subrange<DIM>                 self;

  public:
    typedef GrAL::cartesian_grid_view_category_d<DIM> category;

    typedef self grid_type;
    typedef grid_types_base<self, DIM> gt;
    typedef grid<DIM>   base_grid_type;
    typedef grid<DIM-1> facet_grid_type;

    typedef typename base::index_type        index_type;
    typedef typename base::vertex_index_type vertex_index_type;
    typedef typename base::Vertex            Vertex;
    typedef typename base::Cell              Cell;

    typedef grid_types_base<base_grid_type, DIM> bgt;
    typedef typename bgt::Vertex  BaseVertex;
    typedef typename bgt::Cell    BaseCell;


  private:
    ref_ptr<base_grid_type const> g;
    ref_ptr<self const>           this_c;
  public:
    //@{
    //! \name Constructors

    //! \brief Empty subrange
    subrange() {}

    subrange(self const& rhs) { init(rhs.low_vertex_index(), rhs.beyond_vertex_index()); }
    self& operator=(self const& rhs) {
      if(this != &rhs) 
	init(rhs.low_vertex_index(), rhs.beyond_vertex_index()); 
      return *this;
    }

    void init(vertex_index_type low, vertex_index_type beyond) {
      base::init(low,beyond);
      init_this();
    }
    //! \brief Subrange with vertex bounds \c [low,beyond[
    subrange(base_grid_type const&         gg, vertex_index_type low, vertex_index_type beyond) : g(gg) 
    { init(low,beyond);  }
    //! \brief Subrange with vertex bounds \c [low,beyond[
    subrange(ref_ptr<base_grid_type const> gg, vertex_index_type low, vertex_index_type beyond) : g(gg) 
    { init(low,beyond);  }

    //! \brief Subrange with cell bounds \c [clow,chigh]
    subrange(base_grid_type         const& gg, BaseCell const& clow, BaseCell const& chigh)  : g(gg)
    { init(clow.index(), chigh.index() + index_type(2));  }
    //! \brief Subrange with cell bounds \c [clow,chigh]
    subrange(ref_ptr<base_grid_type const> gg, BaseCell const& clow, BaseCell const& chigh)  : g(gg)
    { init(clow.index(), chigh.index() + index_type(2));  }

    //! \brief Subrange with vertex bounds \c [vlow,vhigh]
    subrange(base_grid_type         const& gg, BaseVertex const& vlow, BaseVertex const& vhigh)  : g(gg)
    { init(vlow.index(), vhigh.index() + index_type(1));  }
    //! \brief Subrange with vertex bounds \c [vlow,vhigh]
    subrange(ref_ptr<base_grid_type const> gg, BaseVertex const& vlow, BaseVertex const& vhigh)  : g(gg)
    { init(vlow.index(), vhigh.index() + index_type(1));  }

    // Subranges from another subrange
    
    //! \brief Subrange with vertex bounds \c [low,beyond[
    subrange(self         const& gg, vertex_index_type  vlow, vertex_index_type vbeyond) : g(gg .BaseGrid()) 
    { init(vlow, vbeyond);  }
    //! \brief Subrange with vertex bounds \c [low,beyond[
    subrange(ref_ptr<self const> gg, vertex_index_type  vlow, vertex_index_type vbeyond) : g(gg->BaseGrid()) 
    { init(vlow, vbeyond);  }

    //! \brief Subrange with vertex bounds \c [vlow,vhigh]
    subrange(self         const& gg, Vertex const& vlow, Vertex const& vhigh) : g(gg .BaseGrid()) 
    { init(vlow.index(), vhigh.index() + index_type(1));  }
    //! \brief Subrange with vertex bounds \c [vlow,vhigh]
    subrange(ref_ptr<self const> gg, Vertex const& vlow, Vertex const& vhigh) : g(gg->BaseGrid()) 
    { init(vlow.index(), vhigh.index() + index_type(1));  }

    //! \brief Subrange with cell bounds \c [clow,chigh]
    subrange(self         const& gg, Cell const& clow, Cell const& chigh)  : g(gg .BaseGrid())
    { init(clow.index(), chigh.index() + index_type(2));  }
    //! \brief Subrange with cell bounds \c [clow,chigh]
    subrange(ref_ptr<self const> gg, Cell const& clow, Cell const& chigh)  : g(gg->BaseGrid())
    { init(clow.index(), chigh.index() + index_type(2));  }

    //@}

    void init_this() { this_c = ref_ptr<self const>(*this); }

    //! \brief Underlying grid
    ref_ptr<base_grid_type const> BaseGrid() const { return g;}
    ref_ptr<self const>           ThisGrid() const { return this_c;}

    //! \brief Conversion of subrange element handle to handle of underlying grid
    template<unsigned K>
    element_handle_t<base_grid_type, K>  base_handle(element_handle_t<self, K> h, unsigned dir) const 
    { return BaseGrid()->template index2handle<K>(handle2index(h,dir), dir);}


  }; // class subrange<unsigned DIM>


  /*! \brief An N-dimensional Cartesian grid

      \ingroup cartesianndmodule
     
     \see Test in  \ref test-cartesiannd-construct.C
     \todo Upward Incidence iterators
     \todo switch operators
  */
  template<unsigned DIM>
  class grid : public grid_base<grid<DIM>, DIM> {
    typedef grid_base<grid<DIM>, DIM> base;
    typedef grid<DIM>                 self;

    ref_ptr<self const> this_c;
  public:
    typedef GrAL::cartesian_grid_category_d<(int)DIM>     category;
    typedef self                             grid_type;
    typedef grid<DIM>                        base_grid_type;
    typedef grid<DIM-1>                      facet_grid_type;
    typedef typename base::vertex_index_type vertex_index_type;
    enum { dim = DIM};
    //! \name Constructors
    //@{
    //! Empty grid
    grid() { init_this(); }

    grid(self const& rhs) { init(rhs.low_vertex_index(), rhs.beyond_vertex_index()); }
    self& operator=(self const& rhs) {
      if(this != &rhs) 
	init(rhs.low_vertex_index(), rhs.beyond_vertex_index()); 
      return *this;
    }
    //! Grid with vertex indices \f$ x_i \in [0, b_i[ \f$ with \f$ b = \f$  \c beyond
    grid(vertex_index_type beyond) 
    { init(vertex_index_type(0), beyond); }
    //! Grid with vertex indices \f$ x_i \in [l_, b_i[ \f$ with \f$ l = \f$ \c  low and  \f$ b = \f$  \c beyond    
    grid(vertex_index_type low, vertex_index_type beyond)
    { init(low, beyond); }

    //! Grid with vertex indices \f$ (x_1,x_2) \in [0, b_1[\times[0,b_2[ \f$  (only 2D)
    grid(int b1, int b2);

    //! Grid with vertex indices \f$ (x_1,x_2,x_3) \in [0, b_1[\times[0,b_2[ \times[0,b_3[ \f$  (only 3D)
    grid(int b1, int b2, int b3);
    //@}

    void init(vertex_index_type low, vertex_index_type beyond) {
      base::init(low,beyond);
      init_this();
    }
    void init_this() { this_c = ref_ptr<self const>(*this); }

    ref_ptr<self const> BaseGrid() const { return this_c;}
    ref_ptr<self const> ThisGrid() const { return this_c;}

    template<unsigned K>
    element_handle_t<self, K> base_handle(element_handle_t<self, K> h, unsigned dir) const { return h;}

    
  }; // class grid<unsigned DIM>






  //----------------------------- sequence iterators ---------------------------------

  template<class CARTGRID, unsigned K>
  class sequence_iterator_t : public grid_types_base<CARTGRID, CARTGRID::dim> {
    typedef grid_types_base<CARTGRID, CARTGRID::dim> base;
    typedef sequence_iterator_t<CARTGRID, K>         self;
  public:
    typedef CARTGRID                                 grid_type;
    enum { dim = K, codim = CARTGRID::dim -K, griddim = CARTGRID::dim};
    struct category : grid_element_category_d<dim>, grid_sequence_iterator_category_d<dim> {};
    typedef typename int2element_tag<griddim,dim,codim>::type  element_type_tag;


    unsigned dimension()   const { return dim;}
    unsigned codimension() const { return codim;}

    typedef typename grid_type::base_grid_type  base_grid_type;
    //typedef element_handle_t<base_grid_type,K>  element_handle;
    typedef element_handle_t<grid_type,K>       element_handle;
    typedef element_handle_t<CARTGRID, K>       local_element_handle;

    typedef typename base::size_type         size_type;
    typedef typename base::index_type        index_type;
    typedef tuple<int, K>                    local_index_type;

    typedef self                             value_type;
    //    typedef typename element_type<grid_type, K, griddim - K>::type value_type;
  private:
    ref_ptr<grid_type const> g;
    local_element_handle     h;
    unsigned                 m; // index into the choice of basis vectors, \in [0, \choice{D, K}[

  public:
    sequence_iterator_t() {} 
    explicit sequence_iterator_t(grid_type         const& gg) : g(gg.ThisGrid()), h(0) { init_m();} 
    explicit sequence_iterator_t(ref_ptr<grid_type const> gg) : g(gg), h(0) { init_m();} 
    sequence_iterator_t(grid_type         const& gg, local_element_handle hh) : g(gg.ThisGrid()), h(hh) { init_m();}
    sequence_iterator_t(ref_ptr<grid_type const> gg, local_element_handle hh) : g(gg), h(hh) { init_m();}
    sequence_iterator_t(grid_type         const& gg, local_element_handle hh, unsigned mm) : g(gg.ThisGrid()), h(hh), m(mm) {}
    sequence_iterator_t(ref_ptr<grid_type const> gg, local_element_handle hh, unsigned mm) : g(gg), h(hh), m(mm) {}
    sequence_iterator_t(grid_type         const& gg, index_type idx) : g(gg.ThisGrid())  { init(idx,0); }
    sequence_iterator_t(ref_ptr<grid_type const> gg, index_type idx) : g(gg)  { init(idx,0); } 
    sequence_iterator_t(grid_type         const& gg, index_type idx, unsigned mm) : g(gg.ThisGrid())  { init(idx,mm); }
    sequence_iterator_t(ref_ptr<grid_type const> gg, index_type idx, unsigned mm) : g(gg)  { init(idx,mm); } 

    void init(index_type idx) {
      REQUIRE(g.NumOfDirections(K) == 1, "", 1);
      init(idx,0);
    }
    void init(index_type idx, unsigned dir) {
      m = dir;
      h = g-> template index2handle<K>(idx,dir);
    }


    template<class CARTSUBRANGE>
    sequence_iterator_t(sequence_iterator_t<CARTSUBRANGE, K> const& rhs) 
      : g(rhs.TheGrid().BaseGrid()), 
	h(g->template index2handle<K>(rhs.index(),rhs.direction())),  m(rhs.direction())
    {}
    /*
    // initialization from subrange 
    // this will work only if element_handle and local_element_handle are the same.
    template<class CARTSUBRANGE>
    sequence_iterator_t(ref_ptr<CARTSUBRANGE const> rge, element_handle hh) : g(rge->BaseGrid()), h(hh) { init_m();}

    template<class CARTSUBRANGE>
    sequence_iterator_t(        CARTSUBRANGE const& rge, element_handle hh) : g(rge. BaseGrid()), h(hh) { init_m();}
    */

    self&       operator++()       { cv(); ++h; advance_m(); return *this; }
    bool        IsDone()     const { cb(); return m >= g->maps[K].num_of_maps();}

    // this will use the implicit copy-constructor if value_type == self,
    // and the templated copy constructor otherwise.
    value_type  operator*()  const { cv(); return value_type(*this); } 
    // value_type  operator*()  const { cv(); return value_type(g->BaseGrid(), handle(), m);}

    local_element_handle local_handle() const { cb(); return h;}
    //! \c handle() return  unique element handle with respect to base grid
    element_handle     handle()    const { cb(); return h;} // TheGrid().base_handle(h,m);}
    unsigned           direction() const { cb(); return m;}

    //! for \f$ K \neq 0, D \f$, \c index() is not unique!
    index_type index()  const { cb(); return g->handle2index(h,m);} //  g->maps[K][m](h - g->offsets[K][m]);}
    index_type low_vertex_index() const { return index();}
    index_type high_vertex_index() const { 
      index_type res(index()); 
      res += delta_map<griddim>::template num2index<index_type>(K,m);
      return res;
    }

    typedef typename base::archetype_type archetype_type;
    typedef typename base::Vertex         Vertex;
    typedef typename base::vertex_handle  vertex_handle;
     
    // typedef grid_types<archetype_type>    archgt;
    typedef typename base::archgt           archgt;
    // typedef grid_types_arch_base<grid_type, griddim> archgt;
    // this is useful only for cells, i.e. K=DIM
    Vertex        V(typename archgt::Vertex const& av) const { return Vertex(TheGrid(), index() + av.Base().index());}
    vertex_handle v(typename archgt::Vertex const& av) const { return TheGrid().get_vertex_handle(index()+av.Base().index());}

    Vertex        V(typename archgt::vertex_handle av) const 
        { return V(typename archetype_type::Vertex(TheArchetype(), av));}
    vertex_handle v(typename archgt::vertex_handle av) const
     { return V(av).handle();}
    Vertex        V(int av) const { return V(typename archetype_type::vertex_handle(av));}
    vertex_handle v(int av) const { return v(typename archetype_type::vertex_handle(av));}


    // relative is the relative coordinates (offset) in the grid coordinates
    Vertex        V(index_type relative) const { return Vertex(TheGrid(), index() + relative);}
    vertex_handle v(index_type relative) const { return TheGrid().get_vertex_handle(index() + relative);}

    // local is offset on local coordinates of the element, i.e. has as many dimensions as  this->dimension()
    Vertex        Vl(local_index_type local) const { return V(full_index(local));}
    vertex_handle vl(local_index_type local) const { return v(full_index(local));}


    index_type full_index(local_index_type local) const {
      index_type full(0);
      typename delta_map<griddim>::vector_system dir = delta_map<griddim>::num2vec(K,direction());
      for(unsigned i = 0; i < local.size(); ++i)
	full[dir[i]] = local[i];
      return full;
    }

    ref_ptr<grid_type const> TheAnchor() const { return g;}
    // ref_ptr<grid_type const> TheGrid() const { return g;}
    grid_type      const& TheGrid()      const { return *g;}
    archetype_type const& TheArchetype() const { return TheGrid().ArchetypeOf(*this);}

    template<unsigned M>
    typename incidence_iterator_type<CARTGRID,K,M>::type FirstElement() const
    {  return typename incidence_iterator_type<CARTGRID,K,M>::type(*this);}
    template<unsigned M>
    typename incidence_iterator_type<CARTGRID,K,M>::type EndElement() const
    {  return typename incidence_iterator_type<CARTGRID,K,M>::type(*this,NumOfElements<M>());}


    typename incidence_iterator_type<CARTGRID,K,0>::type FirstVertex() const { return FirstElement<0>();}
    typename incidence_iterator_type<CARTGRID,K,1>::type FirstEdge  () const { return FirstElement<1>();}
    typename incidence_iterator_type<CARTGRID,K,2>::type FirstFace  () const { return FirstElement<2>();}
    typename incidence_iterator_type<CARTGRID,K,griddim-1>::type FirstFacet() const 
    { return FirstElement<griddim-1>();}
    typename incidence_iterator_type<CARTGRID,K,griddim>::type   FirstCell () const 
    { return FirstElement<griddim>();}
 
    typename incidence_iterator_type<CARTGRID,K,0>::type EndVertex() const { return EndElement<0>();}
    typename incidence_iterator_type<CARTGRID,K,1>::type EndEdge  () const { return EndElement<1>();}
    typename incidence_iterator_type<CARTGRID,K,2>::type EndFace  () const { return EndElement<2>();}
    typename incidence_iterator_type<CARTGRID,K,griddim-1>::type EndFacet() const 
    { return EndElement<griddim-1>();}
    typename incidence_iterator_type<CARTGRID,K,griddim>::type   EndCell () const 
    { return EndElement<griddim>();}



    template<unsigned M>
    size_type NumOfElements() const;

    size_type NumOfVertices() const { return NumOfElements<0>();}    
    size_type NumOfEdges   () const { return NumOfElements<1>();}    
    size_type NumOfFaces   () const { return NumOfElements<2>();}    
    size_type NumOfFacets  () const { return NumOfElements<griddim-1>();}    
    size_type NumOfCells   () const { return NumOfElements<griddim>  ();}    



    bool bound() const { return g != 0;}
    bool valid() const { return bound() && ! IsDone();}
    void cb()    const { REQUIRE(bound(), "", 1);}
    void cv()    const { REQUIRE(valid(), "", 1);}

    // find appropriate m by looking up the correct interval of h in offsets[K];
    void init_m() { m = 0; advance_m(); }
    void advance_m() { while(m < g->NumOfDirections(K) && !valid_map()) ++m;}
    // is h a handle for the current map m?
    bool valid_map() const { return (unsigned)h.h() < g->offsets[K][m+1];}

  }; // class sequence_iterator_t<class CARTGRID, unsigned K>  

 template<class CARTGRID, unsigned K>
 inline bool operator==(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs)
 { return lhs.local_handle() == rhs.local_handle();} 

 template<class CARTGRID, unsigned K>
 inline bool operator<(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs)
 { return lhs.local_handle() < rhs.local_handle();} 

  
 template<class CARTGRID, unsigned K>
 inline bool operator!=(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs)
 { return !(lhs==rhs);}
  
 template<class CARTGRID, unsigned K>
 inline bool operator>(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs)
 { return  (rhs<lhs);}

 template<class CARTGRID, unsigned K>
 inline bool operator<=(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs)
 { return  !(rhs<lhs);}

 template<class CARTGRID, unsigned K>
 inline bool operator>=(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs)
 { return  !(lhs<rhs);}



  //------------------------ incidence iterators -------------------------

  // <M>On<K>Iterator

  template<class CARTGRID, unsigned DA, unsigned DE>
  class downward_inc_iterator_t : public grid_types_base<CARTGRID, CARTGRID::dim> {
  private:
    typedef grid_types_base<CARTGRID, CARTGRID::dim>   base;
    typedef downward_inc_iterator_t<CARTGRID, DA, DE>  self;
  public:
    typedef grid_incidence_iterator_category_d<DE,DA>         category;
    typedef CARTGRID                                          grid_type;
    enum { dim = base::dim}; 
    typedef typename base::index_type                          index_type;
    typedef element_handle_t<grid_type,DE>                     element_handle;
    typedef typename element_type<grid_type, DA, dim-DA>::type anchor_type;
    typedef typename element_type<grid_type, DE, dim-DE>::type element_type;
    typedef element_type                                       value_type;
  private:
    anchor_type a;
    int         loc;

  public:
    downward_inc_iterator_t() {}
    explicit downward_inc_iterator_t(anchor_type const& aa, int ll = 0) : a(aa), loc(ll) 
    { } //  ::std::cout << "downward_inc_iterator_t<" << DA << "," << DE << ">" <<  ::std::endl; }

    self& operator++()  { cv(); ++loc; return *this;}
    element_type operator*() const { return element_type(TheGrid(), handle());}
    
    element_handle handle()   const { cv(); return TheGrid().template index2handle<DE>(index(), direction());}
    index_type index()        const { cv(); return a.index() + table()[loc].offset;}
    unsigned   direction()    const { cv(); return table()[loc].dir;}
    unsigned   local_handle() const { cb(); return loc;}

    bool IsDone() const { cb(); return loc >= (int)table().size(); }
    
    ref_ptr<anchor_type const> TheAnchor() const { return ref_ptr<anchor_type const>(a);}
    //  ref_ptr<grid_type   const> TheGrid()   const { return a.TheGrid();}
    grid_type const& TheGrid() const { return a.TheGrid();}

    bool bound() const { return a.valid();}
    bool valid() const { return bound() && ! IsDone();}
    void cb() const { REQUIRE(bound(), "", 1);}
    void cv() const { REQUIRE(valid(), "", 1);}
  private:
    typedef typename delta_map<dim>::inc_descriptor incidences;
    std::vector<incidences> const& table() const { return delta_map<dim>::sd->incs[DA][DE][a.direction()];}
    //std::vector<incidences>  table() const { return delta_map<dim>::sd->incs[DA][DE][a.direction()];}
  };


 template<class CARTGRID, unsigned DA, unsigned DE>
 inline bool operator==(downward_inc_iterator_t<CARTGRID,DA, DE> lhs, 
			downward_inc_iterator_t<CARTGRID,DA, DE> rhs)
 { return lhs.local_handle() == rhs.local_handle();} 

  //#ifndef GRAL_INCLUDES_RELOPS
 template<class CARTGRID, unsigned DA, unsigned DE>
 inline bool operator!=(downward_inc_iterator_t<CARTGRID,DA, DE> lhs, 
			downward_inc_iterator_t<CARTGRID,DA, DE> rhs)
 { return !(lhs==rhs);}
  //#endif




  // <M>On<K>Iterator

  template<class CARTGRID, unsigned DA, unsigned DE>
  class upward_inc_iterator_t : public grid_types_base<CARTGRID, CARTGRID::dim> {
  private:
    typedef CARTGRID                                 grid_type;
    typedef grid_types_base<CARTGRID, CARTGRID::dim> base;
    typedef upward_inc_iterator_t<CARTGRID, DA, DE>  self;
  public:
    typedef grid_incidence_iterator_category_d<DE,DA>      category;

    enum { dim = base::dim};
    typedef typename base::index_type                          index_type;
    typedef element_handle_t<grid_type,DE>                     element_handle;
    typedef typename element_type<grid_type, DA, dim-DA>::type anchor_type;
    typedef typename element_type<grid_type, DE, dim-DE>::type element_type;
    typedef element_type                                       value_type;
  private:
    anchor_type a;
    int         loc;

  public:
    upward_inc_iterator_t() {}
    explicit upward_inc_iterator_t(anchor_type const&         aa, int ll = 0) : a(aa), loc(ll) 
    {
      advance_till_valid();
    }

    self& operator++()  { cv(); ++loc; advance_till_valid(); return *this;}
    element_type operator*() const { return element_type(TheGrid(), handle());}
    
    element_handle handle()   const { cv(); return TheGrid().template index2handle<DE>(index(), direction());}
    index_type index()        const { cv(); return a.index() + table()[loc].offset;}
    unsigned   direction()    const { cv(); return table()[loc].dir;}
    unsigned   local_handle() const { cb(); return loc;}

    bool IsDone() const { cb(); return loc >= (int)table().size(); }
    
    ref_ptr<anchor_type const> TheAnchor() const { return ref_ptr<anchor_type const>(a);}
    //ref_ptr<grid_type   const> TheGrid()   const { return a.TheGrid();}
    grid_type const& TheGrid() const { return a.TheGrid();}

    bool bound() const { return a.valid();}
    bool valid() const { return bound() && ! IsDone();}
    void cb() const { REQUIRE(bound(), "", 1);}
    void cv() const { REQUIRE(valid(), "", 1);}
  private:
    void advance_till_valid() {
      while(! IsDone() && ! TheGrid().template valid_index<DE>(table()[loc].dir, a.index()+table()[loc].offset))
	++loc;
    }
    typedef typename delta_map<dim>::inc_descriptor incidences;
    std::vector<incidences> const& table() const { return delta_map<dim>::sd->incs[DA][DE][a.direction()];}
  };


 template<class CARTGRID, unsigned DA, unsigned DE>
 inline bool operator==(upward_inc_iterator_t<CARTGRID,DA, DE> lhs, 
			upward_inc_iterator_t<CARTGRID,DA, DE> rhs)
 { return lhs.local_handle() == rhs.local_handle();} 

 template<class CARTGRID, unsigned DA, unsigned DE>
 inline bool operator!=(upward_inc_iterator_t<CARTGRID,DA, DE> lhs, 
			upward_inc_iterator_t<CARTGRID,DA, DE> rhs)
 { return !(lhs==rhs);}



} // namespace cartesiannd



  // grid_types<>

template<unsigned DIM>
struct grid_types<cartesiannd::grid<DIM> > 
  : public grid_types_base<cartesiannd::grid_types_base<cartesiannd::grid<DIM>, DIM> > 
{
  typedef cartesiannd::grid<DIM>          grid_type;
  typedef cartesiannd::subrange<DIM>      cartesian_subrange_type;
};



template<unsigned DIM>
struct grid_types<cartesiannd::subrange<DIM> > 
  : public grid_types_base<cartesiannd::grid_types_base<cartesiannd::subrange<DIM>, DIM> > 
{
  typedef cartesiannd::subrange<DIM> grid_type;
  typedef cartesiannd::subrange<DIM> cartesian_subrange_type;
};


  // element_traits<>

template<unsigned DIM, unsigned DE>
struct element_traits<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>, DE> >
  : public element_traits_base<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>, DE> >
{
  typedef  element_traits_base<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>, DE> > base;
  typedef typename base::hasher_type_elem_base  hasher_type;
  typedef consecutive_integer_tag<0>            consecutive_tag;
  enum { dim = DE, codim = DIM - DE };
};


template<unsigned DIM, unsigned DE>
struct element_traits<cartesiannd::sequence_iterator_t<cartesiannd::subrange<DIM>, DE> >
  : public element_traits_base<cartesiannd::sequence_iterator_t<cartesiannd::subrange<DIM>, DE> >
{
  typedef  element_traits_base<cartesiannd::sequence_iterator_t<cartesiannd::subrange<DIM>, DE> > base;
  typedef typename base::hasher_type_elem_base  hasher_type;
  typedef consecutive_integer_tag<0>            consecutive_tag;
  enum { dim = DE, codim = DIM - DE };
};



// element_d etc for grid<DIM>

template<unsigned DIM, int DE>
struct element_handle_d<grid_types<cartesiannd::grid<DIM> >, DE> 
{ typedef cartesiannd::element_handle_t<cartesiannd::grid<DIM>, (unsigned)DE> type; };

template<unsigned DIM, int DE>
struct element_d<grid_types<cartesiannd::grid<DIM> >, DE> 
{ typedef cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>, (unsigned)DE> type; };

template<unsigned DIM, int DE>
struct sequence_iterator_d<grid_types<cartesiannd::grid<DIM> >, DE> 
{ typedef cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>, (unsigned)DE> type; };

template<unsigned DIM, int DE, int DA>
struct incidence_iterator_d<grid_types<cartesiannd::grid<DIM> >, DE, DA> 
{ typedef typename cartesiannd::incidence_iterator_type<cartesiannd::grid<DIM>, (unsigned)DA, (unsigned)DE>::type  type; };



// element_d etc for subrange<DIM>

template<unsigned DIM, int DE>
struct element_handle_d<grid_types<cartesiannd::subrange<DIM> >, DE> 
{ typedef cartesiannd::element_handle_t<cartesiannd::subrange<DIM>, (unsigned)DE> type; };

template<unsigned DIM, int DE>
struct element_d<grid_types<cartesiannd::subrange<DIM> >, DE> 
{ typedef cartesiannd::sequence_iterator_t<cartesiannd::subrange<DIM>, (unsigned)DE> type; };

template<unsigned DIM, int DE>
struct sequence_iterator_d<grid_types<cartesiannd::subrange<DIM> >, DE> 
{ typedef cartesiannd::sequence_iterator_t<cartesiannd::subrange<DIM>, (unsigned)DE> type; };

template<unsigned DIM, int DE, int DA>
struct incidence_iterator_d<grid_types<cartesiannd::subrange<DIM> >, DE, DA> 
{ typedef typename cartesiannd::incidence_iterator_type<cartesiannd::subrange<DIM>, (unsigned)DA, (unsigned)DE>::type  type; };



//-----------------------  inline functions  ----------------------------

namespace cartesiannd {

  template<class CARTGRID, unsigned DA>
  template<unsigned DE>
  inline
  typename sequence_iterator_t<CARTGRID,DA>::size_type
  sequence_iterator_t<CARTGRID,DA>::NumOfElements() const {
    if(DE < DA)
      return delta_map<griddim>::sd->incs[DA][DE][direction()].size();
    else {
      typename incidence_iterator_type<CARTGRID,DA,DE>::type it(FirstElement<DE>());
      size_type cnt = 0;
      while(!it.IsDone()) {
	++it;
	++cnt;
      }
      return cnt; 
    }
  }

  template<class CARTGRID, unsigned DIM>
  template<unsigned DA>
  sequence_iterator_t<CARTGRID, DA>
  grid_base<CARTGRID,DIM>::FirstElement() const { return sequence_iterator_t<CARTGRID, DA>(to_derived());}

  template<class CARTGRID, unsigned DIM>
  template<unsigned DA>
  sequence_iterator_t<CARTGRID, DA>
  grid_base<CARTGRID,DIM>::EndElement() const { 
    return sequence_iterator_t<CARTGRID, DA>(to_derived(), 
					    element_handle_t<CARTGRID,DA>(offsets[DA][maps[DA].num_of_maps()]), 
					    maps[DA].num_of_maps());
  }



  template<class CARTGRID, unsigned DIM>
  void grid_base<CARTGRID,DIM>::init(typename grid_base<CARTGRID,DIM>::vertex_index_type low,
				     typename grid_base<CARTGRID,DIM>::vertex_index_type beyond) 
  {
    //std::cout << "grid_base<CARTGRID,DIM>::init(" << "[" << low << "], [" << beyond << "[)\n";
    if(! delta_map<DIM>::initialized()) {
      //std::cout << "Initializing grid_base<" << DIM << ">" <<  std::endl;
      delta_map<DIM>::init();
    }
    // if(archetype_array == 0)
    if(archetype_initialized == 0) {
      archetype_initialized = new int(1);
      archetype_array = new archetype_type[1];
    }

    for(unsigned k = 0; k <= dim; ++k) {
      maps   [k].init(k, low, beyond);
      offsets[k].init(k);
      offsets[k][0] = 0;
      for(unsigned m = 1; m <= maps[k].num_of_maps(); ++m)
	offsets[k][m] = offsets[k][m-1] + maps[k][m-1].flat_size();
    }
  }

  template<class CARTGRID, unsigned DIM>
  void grid_base<CARTGRID,DIM>::print(std::ostream& out) const
  {
    //  out << "directions:\n";
    //  delta_map<DIM>::print(out);
    for(unsigned k = 0; k <= DIM; ++k) {
      out << "maps   [" << k << "]: ";
      maps[k].print(out); 
      out << "\n";
      out << "offsets[" << k << "]: "; 
      offsets[k].print(out); 
      out << "\n";
    }
  } 

  template<>
  inline 
  grid<2>::grid(int b1, int b2) {
    init(vertex_index_type(0), vertex_index_type(b1,b2)); 
  }

  template<>
  inline 
  grid<3>::grid(int b1, int b2, int b3) { 
    init(vertex_index_type(0), vertex_index_type(b1,b2,b3)); 
  }



  //--------------------- begin/end/size overloads --------------------

  template<class CART, unsigned K>
  sequence_iterator_t<CART,K> gral_begin(CART const& a, sequence_iterator_t<CART, K>) { return a.template FirstElement <K>();}
  template<class CART, unsigned K>
  sequence_iterator_t<CART,K> gral_end  (CART const& a, sequence_iterator_t<CART, K>) { return a.template EndElement   <K>();}
  template<class CART, unsigned K>
  int                            gral_size (CART const& a, sequence_iterator_t<CART, K>) { return a.template NumOfElements<K>();}

  template<class CART, unsigned DA, unsigned DE>
  downward_inc_iterator_t<CART ,DA,DE>
  gral_begin(sequence_iterator_t<CART,DA> const& a, downward_inc_iterator_t<CART ,DA,DE>)
  { return a.template FirstElement <DE>(); }

  template<class CART, unsigned DA, unsigned DE>
  downward_inc_iterator_t<CART ,DA,DE>
  gral_end  (sequence_iterator_t<CART,DA> const& a, downward_inc_iterator_t<CART ,DA,DE>)
  { return a.template EndElement   <DE>(); }

  template<class CART, unsigned DA, unsigned DE>
  int
  gral_size (sequence_iterator_t<CART,DA> const& a, downward_inc_iterator_t<CART ,DA,DE>) 
  { return a.template NumOfElements<DE>(); }
 


  template<class CART, unsigned DA, unsigned DE>
  upward_inc_iterator_t<CART ,DA,DE>
  gral_begin(sequence_iterator_t<CART,DA> const& a, upward_inc_iterator_t<CART ,DA,DE>)
  { return a.template FirstElement <DE>(); }

  template<class CART, unsigned DA, unsigned DE>
  upward_inc_iterator_t<CART ,DA,DE>
  gral_end  (sequence_iterator_t<CART,DA> const& a, upward_inc_iterator_t<CART ,DA,DE>)
  { return a.template EndElement   <DE>(); }

  template<class CART, unsigned DA, unsigned DE>
  int
  gral_size (sequence_iterator_t<CART,DA> const& a, upward_inc_iterator_t<CART ,DA,DE>) 
  { return a.template NumOfElements<DE>(); }


} // namespace cartesiannd

} // namespace GrAL 



#endif
