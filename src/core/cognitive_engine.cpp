/*++
 * cognitive_engine.cpp — Instantiation for UnifiedCognitiveEngine
 *--*/
#include "cogpy_bridge.hpp"

/* Translation unit anchor */
namespace winkognn {
    static const char* engine_version = "1.0.0";
    const char* get_engine_version() { return engine_version; }
}
