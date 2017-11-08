//! Stress vs Time
//!
//! Get input which are the results from mpm3d

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

//! \brief    Get stress vs time
//! \details  Get input, get stress values for each time-step, write output

int main() {
  try {
    //! Reading input file
    //! Error would be reported

    //! 1 for Krishna's code, and 0 for Shyamini's code
    bool mpm_code = 1;

    //! Initialize a vector stress to contain the stress of a point
    std::vector<std::array<double, 3>> vec_data;
    std::array<double, 3> coordinates;
    std::vector<double> time_step;
    unsigned total_num_points;
    const unsigned ntime = 100;
    std::string data; 

    std::vector<std::string> data_vec{"stress", "strain", "velocity"};

    //! User input inputFilename and outputFilename, and point_id
    std::string foldername;
    std::string inputfilename;
    std::string outputfilename;
    std::string point_id_str;
    unsigned point_id;

    std::cout << "Type the working folder, default: [bin/mc_test/]: ";
    std::getline(std::cin, foldername);
    if (foldername == "") foldername = "bin/mc_test/";

    std::cout << "Type the point id, default: [0]: ";
    std::getline(std::cin, point_id_str);
    if (point_id_str == "") point_id_str = "0";
    std::istringstream point_id_ss(point_id_str); 
    point_id_ss >> point_id;


    for (const auto& component : data_vec) {
      data = component;

      //! Get output file name
      outputfilename = foldername + data + "_" + std::to_string(point_id) + ".txt";

      //! Loop through different input file name
      for (unsigned t = 0; t <= ntime; ++t) {

        //! Get vector time_step that contains the time index
        time_step.push_back(t * 1000);

        //! Get inputfilename
        if (mpm_code) {

          //! Use below for Krishna's code
          if (t < 10) {
            inputfilename = foldername + data + "00" + std::to_string(t) + "000.vtk";
          } else if (t < 100) {
            inputfilename = foldername + data + "0" + std::to_string(t) + "000.vtk";
          } else if (t < 1000) {
            inputfilename = foldername + data + "" + std::to_string(t) + "000.vtk";
          } else {
            inputfilename = foldername + data + std::to_string(t) + "000.vtk";
          }
        } else {
          //! Use below for Shyamini's code
          if (t == 0) {
            inputfilename = foldername + data + std::to_string(t) + ".vtk";
          } else {
            inputfilename = foldername + data + std::to_string(t) + "000.vtk";
          }
        }


        //! Open input file and store x-dir
        std::ifstream inputFile;
        inputFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        inputFile.open(inputfilename);

        //! Declare temporary variable
        char unused_char;
        std::string unused_lines;
        std::array<double, 3> current_data;
        unsigned skip_char;

        if (mpm_code) {
          //! Use below for Krishna's code
          skip_char = 66;
        } else {

          //! Use below for Shyamini's code
          if (data == "stress") {
            skip_char = 78;
          } else if (data == "strain") {
            skip_char = 77;
          } else if (data == "velocity")  {
            skip_char = 80;
          } else {
            std::cout << "Not specified\n";
            skip_char = 78;
          }
        }

        //! Loop through the unused characters     
        for (unsigned i = 0; i < skip_char; ++i) {
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
        inputFile >> current_data.at(0);
        inputFile >> current_data.at(1);
        inputFile >> current_data.at(2);

        //! Store in stress vector
        vec_data.emplace_back(current_data);

        inputFile.close();
        std::cout << "The input file for time step: " << std::to_string(t)
                  << "000 has been read."
                  << "\n";
      }

      //! Open output file and store all data
      std::ofstream outputFile(outputfilename);

      for (unsigned i = 0; i <= ntime; ++i) {
        outputFile << time_step.at(i) << "\t" << vec_data.at(i).at(0) << "\t"
                   << vec_data.at(i).at(1) << "\t" << vec_data.at(i).at(2) << "\n";
      }

      outputFile.close();
      std::cout << "The output file has been generated."
                << "\n";

      //! initialize vec_data
      vec_data.clear();

    }
  }

  catch (std::exception& except) {
    std::cout << "Caught exception: '" << except.what() << "'\n";
    return EXIT_FAILURE;
  }
}
