#pragma once

namespace Helpers
{
	int BytesToInt32(const unsigned char*);

	void Int32ToBytes(int value, unsigned char* destination);
}