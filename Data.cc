#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>

using namespace std;

Data::Data(const std::string& filename) {
  ifstream file(filename);

  // check if file opened correctly
  if (!file.is_open()) {
    throw std::runtime_error("Error could not open file with name: " +
                             filename);
  }

  int size = -1;
  file >> size;

  // read in data from file: m_siz+1 bin edges
  for (int i = 0; i < size + 1; ++i) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }
  // read in data from file: m_siz bin contents
  for (int i = 0; i < size; ++i) {
    double entries;
    file >> entries;
    m_data.push_back(entries);
  }
    // read in data from file: m_siz uncertainties
  for (int i = 0; i < size; ++i) {
    double uncertainties;
    file >> uncertainties;
    m_uncertainties.push_back(uncertainties);
  }
  // done! close the file
  file.close();

  assertSizes();
};

void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }

int Data::checkCompatibility(const Data& in, int n) {
    Data data_to_compare_to = in;
    // n = number of standard deviations

    double delta_y;
    double sigma_delta_y;
    int number_of_deviations = 0;

    for (int i = 0; i < data_to_compare_to.size(); i++){
        // cout << i << endl;
        delta_y = abs(m_data[i] - data_to_compare_to.m_data[i]);
        // cout << delta_y << endl;
        sigma_delta_y = sqrt(pow(m_uncertainties[i],2)+pow(data_to_compare_to.m_uncertainties[i],2));
        // cout << sigma_delta_y << endl;
        if (delta_y > n * sigma_delta_y) {
            number_of_deviations += 1;
            // cout << "i: " << i << ", delta_y: " << delta_y << ", n*sigma_delta_y: " << n*sigma_delta_y << endl;
        }
    }
    return number_of_deviations;
}
