#ifndef GRAL_GB_BASE_ARCHETYPES_H
#define GRAL_GB_BASE_ARCHETYPES_H

// $LICENSE_NEC 

#include "Gral/Base/common-grid-basics.h"
#include "Utility/ref-ptr.h"

/*! Helper class to reuse the archtype functionality of a base grid


*/
template<class DERIVED, class BASEGRID, class GT = grid_types<BASEGRID> >
class archetypes {
private:
  ref_ptr<BASEGRID const> base_grid() const 
  { return ref_ptr<BASEGRID const>(static_cast<DERIVED const*>(this)->BaseGrid());}
  //  typedef typename DERIVED::Cell        d_Cell;
  //  typedef typename DERIVED::cell_handle d_cell_handle;
  typedef typename GT::Cell b_Cell;
  typedef typename GT::cell_handle b_cell_handle;
public:
  typedef typename GT::archetype_type     archetype_type;
  typedef typename GT::archetype_handle   archetype_handle;
  typedef typename GT::archetype_iterator archetype_iterator;

  archetype_iterator BeginArchetype() const { return base_grid()->BeginArchetype();}
  archetype_iterator EndArchetype()   const { return base_grid()->EndArchetype();}
  unsigned NumOfArchetypes() const { return base_grid()->NumOfArchetypes();}
  archetype_handle handle(archetype_iterator  it) const { return base_grid()->handle(it);}
  archetype_type const& Archetype(archetype_handle a) const { return base_grid()->Archetype(a);}

  // this works if  DERIVED::Cell can be implicitely converted to GT::Cell
  archetype_type const& ArchetypeOf(b_Cell const& c) const { return base_grid()->ArchetypeOf(c);}
  archetype_type const& ArchetypeOf(b_cell_handle c) const { return base_grid()->ArchetypeOf(c);}
  archetype_handle archetype_of(b_Cell const& c) const { return base_grid()->archetype_of(c);}
  archetype_handle archetype_of(b_cell_handle c) const { return base_grid()->archetype_of(c);}
};


template<class GT>
struct archetype_grid_types {
  typedef typename GT::archetype_type     archetype_type;
  typedef typename GT::archetype_handle   archetype_handle;
  typedef typename GT::archetype_iterator archetype_iterator;
};

#endif