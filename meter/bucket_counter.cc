#include "bucket_counter.h"

#include <utility>
namespace atlas {
namespace meter {

BucketCounter::BucketCounter(Registry* registry, IdPtr id,
                             BucketFunction bucket_function)
    : Meter{std::move(id), registry->clock()},
      registry_{registry},
      bucket_function_{std::move(bucket_function)} {}

std::ostream& BucketCounter::Dump(std::ostream& os) const {
  os << "BucketCounter{" << *id_ << "}";
  return os;
}

static const Measurements kEmptyMeasurements;
Measurements BucketCounter::Measure() const { return kEmptyMeasurements; }

void BucketCounter::Record(int64_t amount) noexcept {
  static const std::string kBucket{"bucket"};
  const auto& bucket = bucket_function_(amount);
  registry_->counter(id_->WithTag(Tag::of(kBucket, bucket)))->Increment();
  Updated();
}

}  // namespace meter
}  // namespace atlas
