#pragma once

#include "bucket_functions.h"
#include "counter.h"
#include "meter.h"
#include "registry.h"

namespace atlas {
namespace meter {
class BucketCounter : public Meter, public DistributionSummary {
 public:
  BucketCounter(Registry* registry, IdPtr id, BucketFunction bucket_function);
  std::ostream& Dump(std::ostream& os) const override;
  Measurements Measure() const override;
  void Record(int64_t amount) noexcept override;
  int64_t Count() const noexcept override { return 0; };
  int64_t TotalAmount() const noexcept override { return 0; };

 private:
  Registry* registry_;
  BucketFunction bucket_function_;
};
}  // namespace meter
}  // namespace atlas
