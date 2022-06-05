template <typename T>
void read_file(const std::string& file, std::vector<T>& dest) {
  std::ifstream in { file };
  if (!in) { throw std::runtime_error("Can't open " + file); }

  for (T t; in >> t; ) { dest.emplace_back(t); }
}
