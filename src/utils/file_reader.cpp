#include "file_reader.h"

#include <fstream>
#include <sstream>

std::string LoadFileToString(const std::string& path) {
  std::ifstream file(path, std::ios::in | std::ios::binary);
  if (!file) {
    throw std::runtime_error("Failed to open file: " + path);
  }

  std::string contents;
  file.seekg(0, std::ios::end);
  contents.resize(file.tellg());
  file.seekg(0, std::ios::beg);
  file.read(&contents[0], contents.size());
  file.close();
  return contents;
}
