// Copyright 2010 <Mark Washenberger>

#ifndef SRC_SLIDINGWINDOW_H_
#define SRC_SLIDINGWINDOW_H_

#include <list>

namespace tympani {

class SlidingWindow {
 public:
  explicit SlidingWindow(int width);
  ~SlidingWindow();

  void add(int left, int right);
  void correlations(int *corr);

 private:
  int width_;
  int count_;
  int *left_;
  int *right_;
};

}  // namespace tympani

#endif  // SRC_SLIDINGWINDOW_H_

