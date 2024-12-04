#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <cmath>

using namespace std;

double background_prediction(double x, double alpha, double beta, double gamma, double delta) {
    double f = alpha + beta * x + gamma * exp(-delta*x);
    return f;
}

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
        delta_y = abs(measurement(i) - data_to_compare_to.measurement(i));
        // cout << delta_y << endl;
        sigma_delta_y = sqrt(pow(error(i),2)+pow(data_to_compare_to.error(i),2));
        // cout << sigma_delta_y << endl;
        if (delta_y > n * sigma_delta_y) {
            number_of_deviations += 1;
            // cout << "i: " << i << ", delta_y: " << delta_y << ", n*sigma_delta_y: " << n*sigma_delta_y << endl;
        }
    }
    return number_of_deviations;
}

Data Data::operator+(const Data& other) const {

    Data data_sum("exp_A");

    // measurements
    std::vector<double> average_y;
    std::vector<double> uncert_aver_y;
    for (int i = 0; i < size(); ++i) {
        double w_1 = 1/pow(error(i),2);
        double w_2 = 1/pow(other.error(i),2);
        average_y.push_back(
            (w_1*measurement(i)+w_2*other.measurement(i))/(w_1 + w_2)
        );
        uncert_aver_y.push_back(
            sqrt(1/(w_1 + w_2))
        );
    }
    data_sum.setData(average_y);
    data_sum.setUncertainties(uncert_aver_y);

    // uncertainties
    // std::vector<double> uncertainty_on_y;
    // for (size_t i = 0; i < m_uncertainties.size(); ++i) {
    //     uncertainty_on_y.push_back(sqrt(1/(measurement(i)+other.measurement(i))));
    // }
    // data_sum.setUncertainties(uncertainty_on_y); // Set summed uncertainties

    return data_sum;
}

double Data::chi_sq_ndof() {
    double alpha = 0.005;
    double beta = -0.00001;
    double gamma = 0.08;
    double delta = 0.015;
    double ndof = 56;
    double chi_sq = 0;
    for (int i = 0; i < size(); i++) {
        chi_sq += pow(measurement(i) - background_prediction(binCenter(i), alpha, beta, gamma, delta),2)/pow(error(i),2);
    }
    double chi_sq_over_ndof = chi_sq/ndof;
    return chi_sq_over_ndof;
}

