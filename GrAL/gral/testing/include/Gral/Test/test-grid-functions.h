#ifndef NMWR_GB_GRAL_TEST_GRID_FUNCTIONS_H
#define NMWR_GB_GRAL_TEST_GRID_FUNCTIONS_H

// $LICENSE

#include <iostream>

template<class GRID, class E>
bool test_grid_functions(GRID const& G, std::ostream& out);

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-grid-functions.C"
#endif

#endif