#ifndef GRAL_BASE_GB_FACET_ITERATOR_OF_CELL_SET_H
#define GRAL_BASE_GB_FACET_ITERATOR_OF_CELL_SET_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Utility/pre-post-conditions.h"

namespace GrAL {

//----------------------------------------------------------------
// 
/*! \brief  Iterator over the facets of a set of cells.
    \ingroup iterators
    
    \templateparams
     - \c CellIt: Model of $GrAL CellIterator
     - \c CellIt::grid_type: Has \c Facet and \c Cell types.
     - \c CellIt::Cell: Model of $GrAL FacetRange

    <b> Implementation note: </b> <br>
    This uses a partial grid function to mark visited facets,
    hence this approach works also with other element types.

    \deprecated This class is obsolete, use closure_iterator instead.

    \see Test in \ref  test-facet-iterator-of-cell-set.C  
    \see \ref iterators module  
    \see  vertex_iterator_of_cell_set

    
*/
//----------------------------------------------------------------

template<class CELLIT, class GT = grid_types<typename CELLIT::grid_type> >
class facet_iterator_of_cell_set {
  typedef facet_iterator_of_cell_set<CELLIT, GT> self;
public:
  typedef GT                                gt;
  typedef typename gt::grid_type            grid_type;
  typedef typename gt::Facet                Facet;
  typedef typename gt::Cell                 Cell;
  typedef typename gt::FacetOnCellIterator  FacetOnCellIterator;
  typedef typename gt::facet_handle         facet_handle;
  typedef CELLIT                            CellIterator;

private:
  //---------- DATA ---------------
  partial_grid_function<Facet,bool> visited;
  CellIterator       major;
  FacetOnCellIterator minor;
  //--------- END DATA -------------


public:
  facet_iterator_of_cell_set() {} 
  facet_iterator_of_cell_set(grid_type const& g) 
    : visited(g)
  {
    major = g.FirstCell();
    if(! major.IsDone())
      minor = FacetOnCellIterator(*major);
  }
  facet_iterator_of_cell_set(const CellIterator& c)
    : visited(c.TheGrid())
    { 
      major = c;
      if (! major.IsDone()) 
	minor =  FacetOnCellIterator(*major); 
    }

  self&   operator++() {
    cv();
    advance();
    while(! IsDone() && visited(value()))
      advance();
    return *this;
  }
  Facet operator*()     const { cv(); return value();}
  bool  IsDone()        const { cb(); return major.IsDone();}
  facet_handle handle() const { cv(); return minor.handle();}

  grid_type const& TheGrid() const { cb(); return major.TheGrid();}
  bool operator==(self const& rhs)
  { rhs.cb(); cb(); return (rhs.major == major) && (rhs.minor == minor);}
  bool operator!=(self const& rhs) { return !(*this==rhs);}

  bool bound() const { return major.bound();}
  bool valid() const { return bound() && minor.valid();}
  void cb()    const { REQUIRE(bound(), "", 1);}
  void cv()    const { REQUIRE(valid(), "", 1);}
private:
  Facet  value() const { cv(); return (*minor);}
    void advance() { 
    visited[value()] = true;
    ++minor; 
    if(minor.IsDone()) {
      ++major;
      if(! major.IsDone())
	minor =  FacetOnCellIterator(*major); 
    } 
  }
};

} // namespace GrAL 

#endif
