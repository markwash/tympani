#include "AveragingSlidingWindow.h"

#include <stdint.h>
#include <stdlib.h>

namespace tympani {

AveragingSlidingWindow::AveragingSlidingWindow(SlidingWindow *window,
                                               int memory)
    : window_(window), memory_(memory), avg_corr_(NULL), tmp_corr_(NULL) {
  avg_corr_ = new int[width()];
  tmp_corr_ = new int[width()];
  for (int i = 0; i < width(); i++) {
    avg_corr_[i] = 0;
    tmp_corr_[i] = 0;
  }
}

AveragingSlidingWindow::~AveragingSlidingWindow() {
  delete[] avg_corr_;
  delete[] tmp_corr_;
}

void AveragingSlidingWindow::add(int left, int right) {
  window_->add(left, right);
  window_->correlations(tmp_corr_);
  average();
}

void AveragingSlidingWindow::average() {
  int64_t tmp;
  for (int i = 0; i < width(); i++) {
    tmp = avg_corr_[i];
    tmp *= memory_ - 1;
    tmp += tmp_corr_[i];
    tmp /= memory_;
    avg_corr_[i] = (int) tmp;
  }
}

void AveragingSlidingWindow::correlations(int *corr) {
  for (int i = 0; i < width(); i++) {
    corr[i] = avg_corr_[i];
  }
}

}  // namepace tympani
