#pragma once
#include <iostream>

using namespace std;

class Rectang
{
public:
	pair<int, int> upper_left;
	pair<int, int> lower_right;

	Rectang()
	{
		upper_left = lower_right = make_pair(-1, -1);
	}

	Rectang(int x1, int y1, int x2, int y2)
	{
		upper_left = make_pair(x1, y1);
		lower_right = make_pair(x2, y2);
	}

	bool overlaps(Rectang &other)
	{
		return (this->upper_left.first <= other.lower_right.first && this->lower_right.first >= other.upper_left.first &&
			this->upper_left.second <= other.lower_right.second && this->lower_right.second >= other.upper_left.second);
	}

	bool in_rectangle(pair<int, int> point)
	{
		return (point.first >= this->upper_left.first && point.second >= this->upper_left.second &&
			point.first <= this->lower_right.first && point.second <= this->lower_right.second);
	}

	bool is_valid()
	{
		return (upper_left.first != -1);
	}

	int size()
	{
		if (upper_left.first == -1)
			return 0;
		return (lower_right.first - upper_left.first) * (lower_right.second - upper_left.second);
	}
};