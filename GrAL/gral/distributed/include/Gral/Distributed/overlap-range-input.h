#ifndef NMWR_GB_OVERLAP_RANGE_INPUT_H
#define NMWR_GB_OVERLAP_RANGE_INPUT_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Utility/pre-post-conditions.h"
#include <iostream>
#include <fstream>

namespace GrAL {

/*! \brief Input adapter for an overlap range
   \ingroup overlapds

   This class plays the same role as overlap_range.
   It does not allow access to individual ranges (exposed, shared etc),
   only to copy them as a whole.
 */
template<class E>
class overlap_range_input {
  typedef overlap_range_input<E> self;

  mutable std::istream* in;
  bool is_in_owned;  // is in referenced or owned?

  unsigned npriv, nexposed, nshared, ncopied;

  
public:
  typedef element_traits<E>        et;
  typedef typename et::handle_type element_handle;
  typedef unsigned                 size_type;
  typedef element_handle           value_type;

  overlap_range_input() 
    : in(0),  is_in_owned(true), 
      npriv(0), nexposed(0), nshared(0), ncopied(0) 
  {}
  overlap_range_input(std::istream& in_) 
    : in(&in_), is_in_owned(false)
  {
    init();
  }

  void init(std::ostream& in_) {
    in(&in_); 
    is_in_owned = false;
    init();
  }

  void init(std::string const& in_nm)
  {
    std::ifstream* fin = new std::ifstream(in_nm.c_str());
    REQUIRE( (fin->is_open()), "Could not open file " << in_nm << "!\n", 1);
    in = fin;
    is_in_owned = true;
    init();
  }


private:
  void init() {
    (*in) >> npriv >> nexposed >> nshared >> ncopied;
  }

public:
  size_type size()  const { return (npriv + nexposed + nshared + ncopied);}
  bool      empty() const { return (size() == 0);}

  template<class OVLPRANGE, class FILTER>
  void copy_to(OVLPRANGE & ovlprange, FILTER const& f) const;
};


/*! \brief Copy an overlap range from an input adapter
    \ingroup overlapds
     This is a specialization of CopyOverlapRanges for copying
     from an  overlap_range_input adapter.
*/
template
< class E, 
  class DestRange, 
  class Filter
>
void CopyOverlapRanges(DestRange                   & dest, 
                       overlap_range_input<E> const& src, 
                       Filter                 const& f)
{ src.copy_to(dest,f);}

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/overlap-range-input.C"
#endif


#endif
