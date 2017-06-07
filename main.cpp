//! Stress vs Time
//!
//! Get input which are the results from mpm3d

#include <array>
#include <fstream>
#include <iostream>
#include <vector>

//! \brief    Get stress vs time
//! \details  Get input, get stress values for each time-step, write output

int main() {
  try {
    //! Reading input file
    //! Error would be reported

    //! Initialize a vector stress to contain the stress of a point
    std::vector<std::array<double, 3>> stress;
    std::array<double, 3> coordinates;
    std::vector<double> time_step;
    unsigned total_num_points;
    const unsigned point_id = 101;
    const unsigned ntime = 50;

    //! User input inputFilename and outputFilename
    std::string foldername;
    std::string inputfilename;
    std::string outputfilename;

    std::cout << "Type the working folder, default: [bin/column1_elastic/]: ";
    std::getline(std::cin, foldername);
    if (foldername == "") foldername = "bin/column1_elastic/";

    //! Get output file name
    outputfilename = foldername + "stress_" + std::to_string(point_id) + ".txt";

    //! Loop through different input file name
    for (unsigned t = 0; t <= ntime; ++t) {

      //! Get vector time_step that contains the time index
      time_step.push_back(t * 1000);

      //! Get inputfilename
      if (t < 10) {
        inputfilename = foldername + "Stress0" + std::to_string(t) + "000.vtk";
      } else {
        inputfilename = foldername + "Stress" + std::to_string(t) + "000.vtk";
      }

      //! Open input file and store x-dir
      std::ifstream inputFile;
      inputFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      inputFile.open(inputfilename);

      //! Declare temporary variable
      char unused_char;
      std::string unused_lines;
      double value;
      std::array<double, 3> current_stress;

      //! Loop through the unused characters
      for (unsigned i = 0; i < 66; ++i) {
        inputFile >> unused_char;
      }

      //! Get total number of points
      inputFile >> total_num_points;

      if (t == 0) {
        //! Again loop through the unused string to find the coords
        for (unsigned i = 0; i <= point_id; ++i) {
          std::getline(inputFile, unused_lines);
        }

        //! Get the coordinates
        inputFile >> coordinates.at(0);
        inputFile >> coordinates.at(1);
        inputFile >> coordinates.at(2);

        //! Again loop through the unused string to the interested line
        for (unsigned i = 0; i < total_num_points * 3 + 4; ++i) {
          std::getline(inputFile, unused_lines);
        }
      } else {
        //! Again loop through the unused string to the interested line
        for (unsigned i = 0; i < total_num_points * 3 + 5 + point_id; ++i) {
          std::getline(inputFile, unused_lines);
        }
      }

      //! Loop through points to get all stress
      inputFile >> current_stress.at(0);
      inputFile >> current_stress.at(1);
      inputFile >> current_stress.at(2);

      //! Store in stress vector
      stress.emplace_back(current_stress);

      inputFile.close();
      std::cout << "The input file for time step: " << std::to_string(t)
                << "000 has been read."
                << "\n";
    }

    //! Open output file and store all data
    std::ofstream outputFile(outputfilename);

    //! Write the coordinates
    for (double coordinate : coordinates) {
      outputFile << coordinate << "\t";
    }
    outputFile << "\n";

    for (unsigned i = 0; i <= ntime; ++i) {
      outputFile << time_step.at(i) << "\t" << stress.at(i).at(0) << "\t"
                 << stress.at(i).at(1) << "\t" << stress.at(i).at(2) << "\n";
    }

    outputFile.close();
    std::cout << "The output file has been generated."
              << "\n";

  }

  catch (std::exception& except) {
    std::cout << "Caught exception: '" << except.what() << "'\n";
    return EXIT_FAILURE;
  }
}
