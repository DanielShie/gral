#ifndef GRAL_GB_BASE_ARCHETYPE_GEOMETRY_H
#define GRAL_GB_BASE_ARCHETYPE_GEOMETRY_H

// $LICENSE_NEC_2005

#include "Gral/Base/common-grid-basics.h"
#include "Geometry/point-traits.h"


namespace GrAL {

  namespace coord_type_for_archetype_geometry { 

    template<class REAL, unsigned DIM, class GRID>
    class coord_type : public array_operators<coord_type<REAL, DIM, GRID>, REAL, DIM>
    {
      typedef REAL real_type;
      enum { dim = DIM };
      real_type X[dim];
    public:
      coord_type() {}
      coord_type(real_type x) { for(int i = 0; i < dim; ++i) X[i] = x;}
      coord_type(real_type x, real_type y) { X[0] = x; X[1] = y;}
      coord_type(real_type x, real_type y, real_type z) { X[0] = x; X[1] = y; X[2] = z;}
    
      real_type  operator()(int i) const { ch(i); return X[i];}
      real_type  operator[](int i) const { ch(i); return X[i];}
      real_type& operator[](int i)       { ch(i); return X[i];}
      
    private:
      void ch(int i) const { REQUIRE(0 <= i && i < dim, "i= " << i << " out of range [0, " << dim-1 << "]\n",1); }
    };

  } // namespace coord_type_for_archetype_geometry

    /*! \brief Geometry to provide local coordinates for an archetype 

     \templateparams
       - GRID  The grid type this class is the archetype geometry of.
       - ARCH  the archetype corresponding to this archetype geometry    

     \note The GRID template parameter is mainly used to avoid mixing up local coord types 
     from different grids having the same archetype - the local archetype geometries
     may differ nevertheless. 
     For instance, when using polygons as archetype for a 2D grid, it may be
     useful to use zero-centered regular polygons as archetype geometry.
     If the same polygon type is used for a 2D triangulation, it may be more natural
     to use the standard triangle {0, e_1, e_2}.
    */

  template<class GRID, class ARCH> 
  class archetype_geometry {
  public:
    typedef GRID                        master_grid_type;
    typedef ARCH                        archetype_type;
    typedef ARCH                        grid_type;
    typedef grid_types<archetype_type>  gt;
    typedef typename gt::Vertex         Vertex;
    typedef typename gt::vertex_handle  vertex_handle;
    
    enum { spacedim = gt::dimension_tag::dim + 1};
    
    typedef double real_type;

    typedef coord_type_for_archetype_geometry::coord_type<real_type, spacedim, GRID> coord_type;

    /*    
    class coord_type : public array_operators<coord_type, real_type, spacedim> 
    {
      typedef typename  archetype_geometry<GRID, ARCH>::real_type real_type;
      enum { dim = archetype_geometry<GRID, ARCH>::spacedim };
      real_type X[dim];
    public:
      coord_type() {}
      coord_type(real_type x) { for(int i = 0; i < dim; ++i) X[i] = x;}
      coord_type(real_type x, real_type y) { X[0] = x; X[1] = y;}
      coord_type(real_type x, real_type y, real_type z) { X[0] = x; X[1] = y; X[2] = z;}

      real_type  operator()(int i) const { ch(i); return X[i];}
      real_type  operator[](int i) const { ch(i); return X[i];}
      real_type& operator[](int i)       { ch(i); return X[i];}

    private:
      void ch(int i) const { REQUIRE(0 <= i && i < dim, "i= " << i << " out of range [0, " << dim-1 << "]\n",1); }
      
    };
    */

  private:
    grid_function<Vertex, coord_type> coords;
  public:    
    archetype_geometry() {}
    archetype_geometry(grid_type const& g) : coords(g) {}
    
    coord_type const& coord(Vertex        const& v) const { return coords(v);}
    coord_type const& coord(vertex_handle const& v) const { return coords(v);}
    coord_type      & coord(Vertex        const& v)       { return coords[v];}
    coord_type      & coord(vertex_handle const& v)       { return coords[v];}


    unsigned space_dimension() const { return spacedim;}

    ref_ptr<grid_type const> TheGrid() const { return  ref_ptr<grid_type const>(coords.TheGrid());}
  };


  template<class REAL, unsigned DIM, class GRID>
  class point_traits<coord_type_for_archetype_geometry::coord_type<REAL, DIM, GRID> >
    : public point_traits_fixed_size_array<coord_type_for_archetype_geometry::coord_type<REAL, DIM, GRID>,
					   REAL, DIM>
  {};

} // namespace GrAL

#endif