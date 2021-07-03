#include "Data.h"
#include "Rectangle.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <Windows.h>

using namespace std;

vector<pair<int, int> > gen_all_rect_sizes(Data &data)
{
	// Generate all pairs of <length, width> of rectangles with valid area
	vector <pair<int, int>> results;
	for (int i = 1; i <= data.max_size; ++i)
		for (int j = 1; j <= data.max_size; ++j)
			if (i * j >= 2 * data.min_ingr && i * j <= data.max_size)
				results.push_back(make_pair(i, j));
	
	// Sort rectangles by area
	sort(results.begin(), results.end(), 
		[](const pair<int, int> &a, const pair<int, int> &b) -> bool
	{
		return (a.first * a.second) < (b.first * b.second);
	});

	return results;
}

inline bool in_a_slice(vector<Rectang> *slices, pair<int, int> point, int max_size)
{
	for (int line_index = point.first; line_index > max(point.first - max_size, 0); --line_index)
		for (auto slice_it = slices[line_index].begin(); slice_it != slices[line_index].end(); ++slice_it)
			if ((*slice_it).in_rectangle(point))
				return true;
	return false;
}

inline bool slice_overlaps(vector<Rectang> *slices, Rectang &slice, int max_size)
{
	for (int line_index = slice.upper_left.first; line_index > max(slice.upper_left.first - max_size, 0); --line_index)
		for (auto slice_it = slices[line_index].begin(); slice_it != slices[line_index].end(); ++slice_it)
			if ((*slice_it).overlaps(slice))
				return true;
	return false;
}

vector<Rectang> simulate(Data &data)
{
	// slices[i] = vector containing all rectangles with upper_left corner starting on line i
	vector<Rectang> *slices = new vector<Rectang>[data.nr_rows + 1];
	vector<Rectang> result;

	// Getting the vector containing all <length, width> pairs of valid rectangles
	vector<pair<int, int>> all_rect_config = gen_all_rect_sizes(data);
	for (int z = 0; z < all_rect_config.size(); ++z)
		cout << all_rect_config[z].first << " " << all_rect_config[z].second << '\n';

	for (int i = 1; i <= data.nr_rows; ++i)
		for (int j = 1; j <= data.nr_columns; ++j)
			if (!in_a_slice(slices, make_pair(i, j), data.max_size))
			{
				Rectang best_slice;
				Rectang new_slice;
				// Going over the possible <length, width> pairs
				for (auto it = all_rect_config.begin(); it != all_rect_config.end(); ++it)
				{
					// Determining lower_right corner
					int i2 = i + (*it).first - 1;
					int j2 = j + (*it).second - 1;

					// Skip if outside the matrix
					if (i2 > data.nr_rows || j2 > data.nr_columns)
						continue;

					// Check if the number of ingredients is greater than min_ingr
					int nr_tomato = data.rect_sums[i2][j2] - data.rect_sums[i - 1][j2] -
						data.rect_sums[i2][j - 1] + data.rect_sums[i - 1][j - 1];

					int nr_mushroom = ((*it).first * (*it).second) - nr_tomato;

					// Skip if not enough ingredients
					if (nr_tomato < data.min_ingr || nr_mushroom < data.min_ingr)
						continue;

					new_slice = Rectang(i, j, i2, j2);

					// Make sure the new slice doesn't overlap with previous
					if (!slice_overlaps(slices, new_slice, data.max_size))
						if (new_slice.size() > best_slice.size())
							best_slice = new_slice;
				}

				// Add the best slice to the vector
				if (best_slice.is_valid())
				{
					slices[i].push_back(best_slice);
					result.push_back(best_slice);
				}
			}

	delete[] slices;

	return result;
}

int main()
{
	const string in_prefix = "../../input_files/";
	const string out_prefix = "../../output_files/sol2/";
	vector<string> input_files = { "example.in", "small.in", "medium.in", "big.in" };
	for (auto in_file_it = input_files.begin(); in_file_it != input_files.end(); ++in_file_it)
	{
		cout << "Now working on " << (*in_file_it) << '\n';
		Data data(in_prefix + (*in_file_it));

		vector<Rectang> result = simulate(data);

		string out_filename = out_prefix + (*in_file_it).substr(0, ((*in_file_it).find('.'))) + ".out";
		data.output_to_file(out_filename, result);
	}

	system("pause");
	return 0;
}