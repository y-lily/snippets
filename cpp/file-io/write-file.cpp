template <typename T>
void write_file(const std::string& file, const std::vector<T>& src, const std::string& delim = "\n") {
  std::ofstream out { file };
  if (!out) { throw std::runtime_error("Can't open " + file); }

  for (T t : src) { out << t << delim; }
}
