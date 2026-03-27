/*++
 * unified_atomspace.cpp — Instantiation point for UnifiedAtomSpace
 *--*/
#include "cogpy_bridge.hpp"

/* Force template instantiation for the unified atomspace */
namespace winkognn {
    /* Explicit instantiation ensures symbols are available for linking */
    template class std::vector<cog::Handle>;
}
