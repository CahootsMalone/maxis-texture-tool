#include "Helpers.h"

namespace Helpers
{
	// TODO confirm this handles -ve values correctly.
	int BytesToInt32(const unsigned char* bytes) {
		// Little endian
		return (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];
	}
}