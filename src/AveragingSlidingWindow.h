// Copyright 2010 <Mark Washenberger>

#ifndef SRC_AVERAGINGSLIDINGWINDOW_H_
#define SRC_AVERAGINGSLIDINGWINDOW_H_

#include "SlidingWindow.h"

namespace tympani {

class AveragingSlidingWindow {
 public:
  explicit AveragingSlidingWindow(SlidingWindow *window, int memory);
  ~AveragingSlidingWindow();

  void add(int left, int right);
  void correlations(int *corr);
  int width() { return window_->width(); }

 private:
  void average();

  SlidingWindow *window_;
  int *avg_corr_;
  int *tmp_corr_;
  int memory_;
};

}  // namespace tympani

#endif  // SRC_AVERAGINGSLIDINGWINDOW_H_

