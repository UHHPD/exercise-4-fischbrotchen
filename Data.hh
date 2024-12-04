#ifndef DATA_HH
#define DATA_HH

#include <vector>
#include <string>
#include <iostream>

class Data {
 public:
  Data(const std::string& filename);

  unsigned int size() const { return m_data.size(); }
  double measurement(int i) const { return m_data[i]; }
  double binCenter(int i) const { return (m_bins[i+1] + m_bins[i])/2; }
  double binLow(int i) const { return m_bins[i]; }
  double binHigh(int i) const { return m_bins[i+1]; }
  double error(int i) const { return m_uncertainties[i]; }
  void setData(const std::vector<double>& data) { m_data = data; }
  void setBins(const std::vector<double>& bins) { m_bins = bins; }
  void setUncertainties(const std::vector<double>& uncertainties) { m_uncertainties = uncertainties; }
  int checkCompatibility(const Data& in, int n);
  Data operator+(const Data& other) const;
  double chi_sq_ndof();
 private:
  Data() {}  // disallow empty data set
  void assertSizes();
  std::vector<double> m_data;
  std::vector<double> m_bins;
  std::vector<double> m_uncertainties;
};

#endif
