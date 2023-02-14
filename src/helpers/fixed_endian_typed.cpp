#include <ptpe/helpers/fixed_endian_typed.hpp>
using namespace ptpe;
const bool FindEndian::isBig = FindEndian::ef.c[0] == 0;

