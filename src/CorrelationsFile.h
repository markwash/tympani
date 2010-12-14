// Copyright 2010 <Mark Washenberger>

#ifndef SRC_CORRELATIONSFILE_H_
#define SRC_CORRELATIONSFILE_H_

#include <stdio.h>

namespace tympani {

template <typename T>
class CorrelationsFile {
 public:
  CorrelationsFile(int window_width)
      : file_(NULL),
        window_width_(window_width) {}
  ~CorrelationsFile();

  int open(const char *path);
  int write(const T *correlations);
  int close();

  int window_width() { return window_width_; }

 private:

  int write_header();

  FILE *file_;
  int window_width_;
};

struct Header {
  int version;
  int width;
};

template <typename T>
CorrelationsFile<T>::~CorrelationsFile() {
  close();
}

template <typename T>
int CorrelationsFile<T>::open(const char *path) {
  if (file_ != NULL)
    return -1;
  file_ = fopen(path, "w");
  if (file_ == NULL)
    return -1;
  return write_header();
}

template <typename T>
int CorrelationsFile<T>::write(const T *correlations) {
  int written;
  written = fwrite(correlations, sizeof(T), window_width_, file_);
  if (written < window_width_)
    return -1;
  return 0;
}

template <typename T>
int CorrelationsFile<T>::close() {
  if (file_ == NULL)
    return 0;
  int ret;
  ret = fclose(file_);
  if (ret != 0)
    return -1;
  file_ = NULL;
  return 0;
}

template <typename T>
int CorrelationsFile<T>::write_header() {
  struct Header header;
  header.version = 1;
  header.width = window_width_;
  int written = fwrite(&header, sizeof(header), 1, file_);
  if (written < 1)
    return -1;
  return 0;
}

}  // namespace tympani

#endif  // SRC_CORRELATIONSFILE_H_
