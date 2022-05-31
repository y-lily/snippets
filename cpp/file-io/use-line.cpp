void use(const string& file) {
  std::ifstream input { file };
  if (!input) { throw std::runtime_error("Can't open " + file + "\n"); }

  int lineNum = 0;
  for (std::string line; getline(input, line);) {
    ++lineNum;
    // make use of the line
    // ...
  }
}
