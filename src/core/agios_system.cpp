/*++
 * agios_system.cpp — AGI-OS System instantiation
 *--*/
#include "cogpy_bridge.hpp"

namespace winkognn {
    static const char* agios_version = "AGI-OS-1.0.0";
    const char* get_agios_version() { return agios_version; }
}
