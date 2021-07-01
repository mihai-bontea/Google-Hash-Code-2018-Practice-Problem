#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "Rectangle.h"

using namespace std;

class Data
{
public:
	int nr_rows, nr_columns, min_ingr, max_size;
	int **rect_sums;

	Data(string filename)
	{
		ifstream fin(filename);

		fin >> nr_rows >> nr_columns >> min_ingr >> max_size;

		// Allocate memory for rect_sums of siz (nr_rows + 1) x (nr_columns + 1)
		rect_sums = new int *[nr_rows + 1];
		for (int i = 0; i <= nr_rows; ++i)
			rect_sums[i] = new int[nr_columns + 1];

		// Initialize first line with 0
		for (int j = 0; j <= nr_columns; ++j)
			rect_sums[0][j] = 0;
		// Initialize first column with 0
		for (int i = 1; i <= nr_rows; ++i)
			rect_sums[i][0] = 0;

		// Read the elements and calculate partial sums for lines
		for (int i = 1; i <= nr_rows; ++i)
			for (int j = 1; j <= nr_columns; ++j)
			{
				char c;
				fin >> c;
				rect_sums[i][j] = rect_sums[i][j - 1] + ((c == 'M') ? 0 : 1);
			}
		fin.close();

		// Calculate the column sums of the line sums
		for (int j = 1; j <= nr_columns; ++j)
			for (int i = 1; i <= nr_rows; ++i)
				rect_sums[i][j] += rect_sums[i - 1][j];
	}

	~Data()
	{
		for (int i = 0; i <= nr_rows; ++i)
			delete[] rect_sums[i];
		delete[] rect_sums;
	}

	void output_to_file(string filename, vector<Rectang> &slices)
	{
		ofstream fout(filename);
		fout << slices.size() << '\n';
		for (auto slice_it = slices.begin(); slice_it != slices.end(); ++slice_it)
		{
			fout << (*slice_it).upper_left.first << " " << (*slice_it).upper_left.second << " ";
			fout << (*slice_it).lower_right.first << " " << (*slice_it).lower_right.second << '\n';
		}
		fout.close();
	}
};