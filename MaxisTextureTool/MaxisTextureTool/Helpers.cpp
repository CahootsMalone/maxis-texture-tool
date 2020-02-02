#include "Helpers.h"

namespace Helpers
{
	// TODO confirm this handles -ve values correctly.
	int BytesToInt32(const unsigned char* bytes) {
		// Little endian
		return (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];
	}

	void Int32ToBytes(int value, unsigned char* destination) {
		// Little endian
		destination[0] = 0xFF & value;
		destination[1] = 0xFF & (value >> 8);
		destination[2] = 0xFF & (value >> 16);
		destination[3] = 0xFF & (value >> 24);
	}
}