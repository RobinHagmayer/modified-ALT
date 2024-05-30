#include <iostream>
#include <string>

int main() {
  std::cout << "Hello World!" << std::endl;
  std::string file_path = std::string(RESOURCE_PATH) + "/toy.fmi";
  std::cout << file_path << std::endl;
}
