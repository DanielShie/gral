#ifndef GRAL_GB_SEQUENCE_INTEGER_ITERATOR_H
#define GRAL_GB_SEQUENCE_INTEGER_ITERATOR_H

// LICENSE_NEC_2003


template<class INT, class C=INT>
class integer_iterator {
  typedef integer_iterator<INT, C> self;
  INT i;
public:
  typedef INT value_type;

  integer_iterator()   {}
  integer_iterator(value_type const& ii) : i(ii) {}
  
  value_type operator*() const { return i;}
  self& operator++() { ++i; return *this;}
  self& operator--() { --i; return *this;}
};


template<class INT, class C>
inline bool operator==(integer_iterator<INT, C> i1, integer_iterator<INT, C> i2) { return *i1 == *i2;}


#ifndef GRAL_INCLUDES_RELOPS
template<class INT, class C>
inline bool operator!=(integer_iterator<INT, C> i1, integer_iterator<INT, C> i2) { return *i1 != *i2;}
#endif


#endif