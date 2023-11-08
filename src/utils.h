#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

void write_vector_to_file(const std::vector<std::string>& strings, const std::string& file);
bool is_sorted(const std::string& s1, const std::string& s2);
void sort_file(const std::string& input_file, const std::string& output_file);
std::vector<std::string>read_vector_from_file(const std::string& file);

#endif
