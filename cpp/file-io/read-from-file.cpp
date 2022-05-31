template <typename T>
void read_from_file(const std::string& file, std::vector<T>& destination) {
  std::ifstream input { file };
  if (!input) { throw std::runtime_error("Can't open" + file); }

  for (T t; input >> t;) {
    destination.emplace_back(t);
  }
}
