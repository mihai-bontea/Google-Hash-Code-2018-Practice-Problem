#include "Data.h"
#include "Rectangle.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <Windows.h>

using namespace std;

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
	vector<Rectang> *slices  = new vector<Rectang>[data.nr_rows + 1];
	vector<Rectang> result;

	int max_size_root = sqrt(data.max_size);
	int max_length = max_size_root;
	int current_surface = max_length * max_length;
	while (current_surface <= data.max_size)
	{
		++max_length;
		current_surface += max_size_root;
	}

	for (int i = 1; i <= data.nr_rows; ++i)
	{
		for (int j = 1; j <= data.nr_columns; ++j)
			if (!in_a_slice(slices, make_pair(i, j), max_length))
			{
				int add_i = 0;
				int add_j = 0;
				Rectang best_slice;
				Rectang new_slice;
				// Check if size is smaller than max size
				while ((add_i + 1) * (add_j + 1) <= data.max_size && (add_i + i) <= data.nr_rows && (add_j + j) <= data.nr_columns)
				{
					// Check if the number of ingredients is greater than min_ingr
					int nr_tomato = data.rect_sums[add_i + i][add_j + j] - data.rect_sums[i - 1][add_j + j] -
						data.rect_sums[add_i + i][j - 1] + data.rect_sums[i - 1][j - 1];
					int nr_mushroom = ((add_i + 1) * (add_j + 1)) - nr_tomato;

					if (nr_tomato >= data.min_ingr && nr_mushroom >= data.min_ingr)
						new_slice = Rectang(i, j, add_i + i, add_j + j);

					// Check if the new slice doesn't overlap with previous
					if (!slice_overlaps(slices, new_slice, max_length))
						best_slice = new_slice;

					if ((add_i + 2) * (add_j + 2) <= data.max_size)
					{
						++add_i;
						++add_j;
					}
					else if ((add_i + 2) * (add_j + 1) <= data.max_size)
						++add_i;
					else
						++add_j;
				}
				// Add best slice to the vector
				if (best_slice.is_valid())
				{
					slices[i].push_back(best_slice);
					result.push_back(best_slice);
				}
			}
	}

	delete[] slices;

	return result;
}

int main()
{
	const string in_prefix = "../../input_files/";
	const string out_prefix = "../../output_files/sol1/";
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