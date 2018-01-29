#ifndef RANGES_H
#define RANGES_H
#include "script/script.h"
#include "serialize.h"
#include "graph.h"

// An range has start and end index
class CRange {
public:
	unsigned int start;
	unsigned int end;
	CRange() {
		SetNull();
	}
	CRange(const unsigned int s, const unsigned int e) {
		start = s;
		end = e;
	}
	ADD_SERIALIZE_METHODS;

	template <typename Stream, typename Operation>
	inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
		READWRITE(VARINT(start));
		READWRITE(VARINT(end));
	}

	friend bool operator==(const CRange &a, const CRange &b) {
		return (
			a.start == b.start
			&& a.end == b.end
			);
	}

	CRange operator=(const CRange &b) {
		start = b.start;
		end = b.end;
		return *this;
	}

	friend bool operator!=(const CRange &a, const CRange &b) {
		return !(a == b);
	}
	void SetNull() { start = 0; end = 0;}
	bool IsNull() const { return (start == 0 && end == 0); }
};
unsigned int validateRangesAndGetCount(const std::vector<CRange> &arr);
bool compareRange(const CRange &i1, const CRange &i2);
void mergeRanges(std::vector<CRange> &arr, std::vector<CRange> &output);
void subtractRanges(std::vector<CRange> &arr, const std::vector<CRange> &del, std::vector<CRange> &output);
#endif // RANGES_H