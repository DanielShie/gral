#ifndef NMWR_GB_GRAL_TEST_VERTEX_ON_VERTEX_ITERATOR_C
#define NMWR_GB_GRAL_TEST_VERTEX_ON_VERTEX_ITERATOR_C

// $LICENSE

#include "Gral/Test/test-vertex-on-vertex-iterator.h"
#include "Utility/pre-post-conditions.h"

template<class G>
bool test_vertex_on_vertex_iterator(G const& g, std::ostream & out)
{
  typedef grid_types<G> gt;
  typedef typename gt::VertexIterator         VertexIterator;
  typedef typename gt::VertexOnVertexIterator VertexOnVertexIterator;

  for(VertexIterator f(g); ! f.IsDone(); ++f) {
    int v_cnt = 0;
    for(VertexOnVertexIterator vf(*f); ! vf.IsDone(); ++vf, ++v_cnt) {
      ;
    }
    REQUIRE_ALWAYS(v_cnt == (int)(*f).NumOfVertices(), 
                   "v_cnt = " << v_cnt << " != f.NumOfVertices() = " 
                   << (*f).NumOfVertices() << '\n',1);

    VertexOnVertexIterator v;
    v = (*f).FirstVertex();
    VertexOnVertexIterator w = v;
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

#endif