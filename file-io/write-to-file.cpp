template <typename T>
void write_to_file(const std::string& file, const std::vector<T>& source, const std::string& delim = " ") {
  std::ofstream output { file };
  if (!output) { throw std::runtime_error("Can't open " + file + "\n"); }

  for (T t : source) {
    output << t << delim;
  }
}
