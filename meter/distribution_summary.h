#pragma once

#include <cstdint>

namespace atlas {
namespace meter {

template <typename T>
class DistributionSummaryNumber {
 public:
  virtual ~DistributionSummaryNumber() = default;
  virtual void Record(T amount) noexcept = 0;
  virtual int64_t Count() const noexcept = 0;
  virtual T TotalAmount() const noexcept = 0;
};

using DistributionSummary = DistributionSummaryNumber<int64_t>;
using DDistributionSummary = DistributionSummaryNumber<double>;

}  // namespace meter
}  // namespace atlas
