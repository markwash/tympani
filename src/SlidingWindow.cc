// Copyright 2010 <Mark Washenberger>

#include "SlidingWindow.h"

#include <stdio.h>
#include <stdlib.h>

namespace tympani {

SlidingWindow::SlidingWindow(int width)
    : width_(width),
      count_(0),
      left_(NULL),
      right_(NULL) {
  left_ = new int[width_];
  right_ = new int[width_];
  for (int i = 0; i < width_; i++) {
    left_[i] = 0;
    right_[i] = 0;
  }
}

SlidingWindow::~SlidingWindow() {
  delete[] left_;
  delete[] right_;
}

void SlidingWindow::add(int left, int right) {
  left_[count_] = left;
  right_[count_] = right;
  count_ = (count_ + 1) % width_;
}

void SlidingWindow::correlations(int *corr) {
  int left_idx, right_idx;
  int left, right;
  for (int i = 0; i < width_; i++) {
    left_idx = (count_ + i) % width_;
    right_idx = (count_ + width_ - i - 1) % width_;
    left = left_[left_idx] / 0x100;
    right = right_[right_idx] / 0x100;
    corr[i] = left * right;
  }
}

}  // namespace tympani
