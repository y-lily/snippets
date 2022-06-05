void use(const std::string& file) {
  std::ifstream in { file };
  if (!in) { throw std::runtime_error("Can't open " + file); }

  int lineNum = 0;
  for (std::string line; std::getline(in, line); ) {
    ++lineNum;
    // Make use of the line
  }
}
