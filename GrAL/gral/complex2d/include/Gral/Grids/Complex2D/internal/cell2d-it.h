#ifndef NMWR_GB_CELL2D_ITERATOR_H
#define NMWR_GB_CELL2D_ITERATOR_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#include "Gral/Grids/Complex2D/complex2d.h"

class Cell2D_Iterator : public complex2d_types {
private:

  typedef Cell2D_Iterator self;
  typedef Complex2D Complex;
  friend class Complex2D;

public:
  //-------- DATA --------------
  // public for access by grid_function<Cell2D,T>
  cell_handle _pos;    // current cell position
private:
  ComplexPtr  _cc;     // reference to grid
  cell_handle _endpos; // past-the-end cell position

public:
  //--------------- constructors -----------------------------

  Cell2D_Iterator() : _pos(-1),  _cc(0), _endpos(-1) {}

  Cell2D_Iterator(Complex2D const& CC) 
    : _pos(0), _cc(&CC), _endpos(CC.NumOfCells())
  {}

  Cell2D_Iterator(const cell_handle& c, Complex2D const& CC)
    : _pos(c),  _cc(&CC),  _endpos(CC.NumOfCells())
  {}
  
  Cell2D_Iterator(const Cell2D& C) 
    : _pos(C._pos), _cc(& C.TheGrid()), _endpos(C.TheGrid().NumOfCells())
  {}
 
  // copying
  Cell2D_Iterator(const self& s)
    { _pos=s._pos; _endpos = s._endpos; _cc = s._cc;}
  self& operator=(const self& s)
    { if (this != &s) { _pos=s._pos; _endpos = s._endpos; _cc = s._cc;}  return (*this);}
    

  //------------ iterator operations ----------------
  self& operator++() { ++_pos; return (*this);}
  Cell  operator*() const {return (_cc->cell(_pos));}
  bool IsDone() const { return (_pos == _endpos);}


  //-------------- anchor ---------------------------

  const Complex2D& TheGrid() const { return (*_cc);}

  cell_handle handle() const { return _pos;}

  friend bool operator==(const self& ls, const self& rs)
    { 
      REQUIRE((ls._cc == rs._cc), "comparsion between iterators from different grids!\n",1);
      return ( ls._pos == rs._pos);
    }
  friend bool operator<(const self& ls, const self& rs)
    { 
      REQUIRE((ls._cc == rs._cc), "comparsion between iterators from different grids!\n",1);
      return ( ls._pos < rs._pos);
    }

};



/*
inline CellOnCell2D_Iterator::operator Cell2D_Iterator() const
{
  return Cell2D_Iterator(*_nb,_cc);
}
*/

#endif
