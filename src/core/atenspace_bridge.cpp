/*++
 * atenspace_bridge.cpp — ATenSpace Bridge instantiation
 *--*/
#include "atenspace_bridge.hpp"

namespace winkognn {
    static const char* atenspace_version = "ATenSpace-1.0";
    const char* get_atenspace_version() { return atenspace_version; }
}
