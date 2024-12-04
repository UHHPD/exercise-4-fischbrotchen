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

Data average_datasets(Data data1, Data data2) {
    Data averaged_data = data1;
    
    for (int i = 0; i < data1.size(); i++) {

    }


    return averaged_data;
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
    char A;
    char B;
    char C;
    char D;
    vector<string> exp_names;
    data_vector.push_back(datA);
    exp_names.push_back("A");
    data_vector.push_back(datB);
    exp_names.push_back("B");
    data_vector.push_back(datC);
    exp_names.push_back("C");
    data_vector.push_back(datD);
    exp_names.push_back("D");

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



    cout << "\n1e)" << endl;
    Data data_A_plus_B = data_vector[0] + data_vector[1];
    cout << data_A_plus_B.measurement(1) << endl;
    cout << data_vector[0].measurement(1) << endl;
    // testing the sum method
    // for (int i = 0; i < data_vector[0].size(); ++i) {
    //     cout << ((1/pow(data_vector[0].error(i),2))*data_vector[0].measurement(i)+(1/pow(data_vector[1].error(i),2))*data_vector[1].measurement(i))/((1/pow(data_vector[0].error(i),2)) + (1/pow(data_vector[1].error(i),2))) << endl;
    //     cout << data_A_plus_B.measurement(i) << endl;
    // } 

    cout << "\n\n2a)" << endl;
    int num_deviations = 0;
    for (int std_devs = 2; std_devs <= 3; std_devs += 1){
        for (int i = 0; i < data_vector.size(); i++){
            for (int j = 0; j < data_vector.size(); j++) {
                if (i == j) {
                    continue;
                }
                else {
                    num_deviations = data_vector[i].checkCompatibility(data_vector[j],2);
                    cout << "Number of deviations for exp "<< exp_names[i] << " and " << exp_names[j] << " for n = "<< std_devs << ": " << num_deviations << endl;
                }
            }
        }
    }
    

    cout << "\n2b)" << endl;
    cout << "For exp A, Chi^2/n_dof = " << data_vector[0].chi_sq_ndof() << endl;
    cout << "For exp B, Chi^2/n_dof = " << data_vector[1].chi_sq_ndof() << endl;
    cout << "For exp C, Chi^2/n_dof = " << data_vector[2].chi_sq_ndof() << endl;
    cout << "For exp D, Chi^2/n_dof = " << data_vector[3].chi_sq_ndof() << endl;
    cout << "For n = 2, we would expect about " << 0.05 * data_vector[0].size() << " deviations, for n = 3, we would expect about " << 0.003 * data_vector[0].size() << " deviations." << endl;


    cout << "\n2c)" << endl;
    Data all_data_summed = data_vector[0] + data_vector[1]+ data_vector[2]+ data_vector[3];
    cout << "The value of chi^2/n_dof for all datasets combined is: " << all_data_summed.chi_sq_ndof() << endl;





  return 0;
}
