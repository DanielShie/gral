#ifndef NMWR_GB_GEOM_GROUP_H
#define NMWR_GB_GEOM_GROUP_H

/*----------------------------------------------------------------------------
    
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/

#include "list.h"

#include "Graphics/geom.h"

#include "Graphics/translators.h"
#include "Graphics/renderable-object.h"

class geom_group : public geom {
  typedef geom_group self;
public:
  geom_group() {}
  self& add_geom(const RenderableGeom& g) { geoms.push_back(g); return *this;}
  virtual void write_geom_to(rendering_language& L) const {
    L.begin_block();
    for(list<RenderableGeom>::const_iterator g = geoms.begin(); g != geoms.end(); ++g)
      L.filter(*g);
    L.end_block();
  }
  friend geom_group& operator<<(geom_group& grp, const RenderableGeom& g) 
    {  return grp.add_geom(g);}
private:
  list<RenderableGeom> geoms;
};

#endif
