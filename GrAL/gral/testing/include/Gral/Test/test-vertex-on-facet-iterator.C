#ifndef NMWR_GB_GRAL_TEST_VERTEX_ON_FACET_ITERATOR_C
#define NMWR_GB_GRAL_TEST_VERTEX_ON_FACET_ITERATOR_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Test/test-vertex-on-facet-iterator.h"
#include "Utility/pre-post-conditions.h"

namespace GrAL {

template<class G, class GT>
bool test_vertex_on_facet_iterator(G const& g, ::std::ostream & out, GT);

template<class G>
bool test_vertex_on_facet_iterator(G const& g, ::std::ostream & out)
{ return test_vertex_on_facet_iterator(g, out, grid_types<G>()); }

template<class G, class GT>
bool test_vertex_on_facet_iterator(G const& g, ::std::ostream & out, GT)
{
  typedef grid_types<G> gt;
  typedef typename gt::FacetIterator         FacetIterator;
  typedef typename gt::VertexOnFacetIterator VertexOnFacetIterator;

  for(FacetIterator f(g); ! f.IsDone(); ++f) {
    int v_cnt = 0;
    for(VertexOnFacetIterator vf(*f); ! vf.IsDone(); ++vf, ++v_cnt) {
      ;
    }
    REQUIRE_ALWAYS(v_cnt == (int)(*f).NumOfVertices(), 
                   "v_cnt = " << v_cnt << " != f.NumOfVertices() = " 
                   << (*f).NumOfVertices() << '\n',1);

    VertexOnFacetIterator v;
    v = (*f).FirstVertex();
    VertexOnFacetIterator w = v;
    for( ; !v.IsDone(); ++v, ++w) {
      //REQUIRE_ALWAYS( &(v.TheGrid()) == &(f.TheGrid()), "",1);
      REQUIRE_ALWAYS( ( v ==  w), "Iterators differ!\n",1);
      REQUIRE_ALWAYS( (*v == *w), "Iterator values differ!\n",1);
    }
    REQUIRE_ALWAYS( (w.IsDone()), "", 1);
    REQUIRE_ALWAYS( (v == w), "Past-the-end iterators differ!\n", 1);
  
    if((*f).NumOfVertices() > 0) {
      v = (*f).FirstVertex();
      w = v;
      ++v;
      for( ; !v.IsDone(); ++v, ++w) 
        REQUIRE_ALWAYS( (*v != *w), "Iterators point to same vertex!\n",1);
    }
  }
  return true;
}

} // namespace GrAL

#endif
