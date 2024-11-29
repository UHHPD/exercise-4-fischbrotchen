#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <cmath>

#include "Data.hh"

// generic function comparing two values of some type T used later for int and
// double
template <class T>
bool testEqual(const std::string& name, T expected, T real) {
  if (expected != real) {
    std::cout << "(" << name << ": <" << expected << ">"
              << ", " << real << ") ";
    return false;
  }
  return true;
}

bool testReadingSize() {
  std::cout << "testReadingSize...";
  Data datA("testA");
  return testEqual<int>("size", 1, datA.size());
}

bool testReadingMeasurement() {
  std::cout << "testReadingMeasurement...";
  Data datA("testA");
  return testEqual("measurement", 10., datA.measurement(0));
}

bool testReadingBinEdges() {
  std::cout << "testReadingBinEdges...";
  Data datA("testA");
  return testEqual("bin low", 0., datA.binLow(0)) &&
         testEqual("bin high", 1., datA.binHigh(0));
}

bool testReadingErrors() {
  std::cout << "testReadingErrors...";
  Data datA("testA");
  return testEqual("error", 2., datA.error(0));
}

bool testCopyConstructor() {
  std::cout << "testCopyConstructor...";
  Data a("testA");
  Data b(a);
  Data c = a;  // equivalent to Data c(a)
  return testEqual("measurement", 10., b.measurement(0)) &&
         testEqual("measurement", 10., c.measurement(0));
}

void runTests() {
  std::cout << "running tests...\n";
  std::vector<std::function<bool()> > tests(
      {testReadingSize, testReadingMeasurement, testReadingBinEdges,
       testReadingErrors, testCopyConstructor});
  for (auto test : tests)
    std::cout << (test() ? " ok" : " FAILED!") << std::endl;
}

int main() {
  using namespace std;

  cout << "******************************************************" << endl;
  runTests();
  cout << "******************************************************" << endl;
  // create an object which holds data of experiment A
  Data datA("exp_A");
  Data datB("exp_B");
  Data datC("exp_C");
  Data datD("exp_D");
  
  // here is the data from experiment A
  cout << "bin 27: from " << datA.binLow(27) << " to " << datA.binHigh(27)
       << endl;
  cout << "measurement of experiment A in bin 27: " << datA.measurement(27)
       << endl;

    // create an object which holds the data of all four experiments
    vector<Data> data_vector;
    data_vector.push_back(datA);
    data_vector.push_back(datB);
    data_vector.push_back(datC);
    data_vector.push_back(datD);

  cout << "Measurement in bin 27 for experiment A, B, C, D:" << endl;
  for (int i = 0; i < data_vector.size(); i++) {
    cout << data_vector[i].measurement(27) << endl;
  }

    // calculate delta_y for bin 27 for exp A and B + uncertainty
    cout << "\n1d)" << endl; 
    double meas_a_27 = data_vector[0].measurement(27);
    double meas_b_27 = data_vector[1].measurement(27);
    double delta_y_27 = sqrt(pow(meas_b_27 - meas_a_27,2));
    cout << "The difference of the measurements in bin 27 for experiments A and B is " << delta_y_27 << endl;

    double uncert_a_27 = data_vector[0].error(27);
    double uncert_b_27 = data_vector[1].error(27);
    double uncertainty_delta_y_27 = sqrt(pow(-1 * uncert_b_27,2) + pow(1 * uncert_a_27,2));
    cout << "The uncertainty on that is " << uncertainty_delta_y_27 << ", so the measurements agree with each other within 1 sigma" << endl;

    int num_deviations_AB = data_vector[0].checkCompatibility(data_vector[1],1);
    int num_deviations_BC = data_vector[1].checkCompatibility(data_vector[2],1);
    int num_deviations_CD = data_vector[2].checkCompatibility(data_vector[3],1);
    int num_deviations_DA = data_vector[3].checkCompatibility(data_vector[1],1);
    cout << "Number of deviations for exp A and exp B for n = 1: " << num_deviations_AB << endl;
    cout << "Number of deviations for exp B and exp C for n = 1: " << num_deviations_BC << endl;
    cout << "Number of deviations for exp C and exp D for n = 1: " << num_deviations_CD << endl;
    cout << "Number of deviations for exp D and exp A for n = 1: " << num_deviations_DA << endl;
  return 0;
}
