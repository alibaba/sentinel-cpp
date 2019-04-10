#pragma once

#include <memory>

#include "sentinel-core/statistic/base/sliding_window_metric.h"

namespace Sentinel {
namespace Stat {

long SlidingWindowMetric::Complete() {

}

long SlidingWindowMetric::MaxComplete() {

}

long SlidingWindowMetric::Exception() {

}

long SlidingWindowMetric::Block() {

}

long SlidingWindowMetric::Pass() {

}

long SlidingWindowMetric::Rt() {

}

long SlidingWindowMetric::MinRt() {

}

void SlidingWindowMetric::AddException(int n) {

}

void SlidingWindowMetric::AddBlock(int n) {

}

void SlidingWindowMetric::AddComplete(int n) {

}

void SlidingWindowMetric::AddPass(int n) {

}

void SlidingWindowMetric::AddRt(long rt) {

}

double SlidingWindowMetric::WindowIntervalInSec() const {

}
int SlidingWindowMetric::SampleCount() const {

}

}  // namespace Stat
}  // namespace Sentinel
