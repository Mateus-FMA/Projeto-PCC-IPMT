#include <iostream>
#include <string>

#include <getopt.h>

#include "utils.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    std::cout << "Incorrect number of arguments." << std::endl;
    return EXIT_FAILURE; 
  }

  std::string mode = argv[1];
  ++optind;

  if (!mode.compare("index")) {
    ipmt::Option long_options[] = {
      {"compression", required_argument, nullptr, 'c'},
      {"indextype", required_argument, nullptr, 'i'}
    };
  } else if (!mode.compare("search")){
    ipmt::Option long_options[] = {
      {"count", no_argument, nullptr, 'c'},
      {"pattern", required_argument, nullptr, 'p'}
    };
  } else if (!mode.compare("-h") || !mode.compare("--help")) {
    ipmt::PrintHelp();
  } else {
    std::cout << "Unimplemented mode." << std::endl;
    return EXIT_FAILURE;
  }

  
  std::cout << "Hello, world!" << std::endl;
  return 0;
}
