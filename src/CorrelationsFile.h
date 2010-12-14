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

}  // namespace tympani

#endif  // SRC_CORRELATIONSFILE_H_
