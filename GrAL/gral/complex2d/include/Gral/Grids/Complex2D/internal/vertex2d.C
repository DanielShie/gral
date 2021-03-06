#ifndef GRAL_GB_COMPLEX2D_VERTEX2D_C
#define GRAL_GB_COMPLEX2D_VERTEX2D_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

inline Vertex2D::Vertex2D(Complex2D const& cc)
  { *this = *(cc.FirstVertex());} 

inline bool Vertex2D::valid() const { return (0 <= _pos && _pos < TheGrid().NumOfVertices());}

inline
vertex_base const&
Vertex2D::base() const  { return (_cc->_vertices[_pos]);}
 
inline 
CellOnVertex2D_Iterator Vertex2D::FirstCell() const
{ return CellOnVertexIterator(*this);}

inline 
CellOnVertex2D_Iterator Vertex2D::EndCell() const
{ return CellOnVertexIterator(NumOfCells(), *this);}

inline Vertex2D::cell_handle 
Vertex2D::cell(int c) const
{ return (base()._cells[c]);}

} // namespace GrAL {

#endif
