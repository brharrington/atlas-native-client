#pragma once

#include "../util/config.h"
#include "registry.h"
#include "stepnumber.h"
#include "subscription.h"
#include <array>

namespace atlas {

namespace meter {

class AtlasRegistry : public Registry {
  class impl;
  std::unique_ptr<impl> impl_;

 public:
  AtlasRegistry(int64_t freq_millis, const Clock* clock) noexcept;

  AtlasRegistry(const AtlasRegistry& registry) = delete;
  ~AtlasRegistry() override;

  const Clock& clock() const noexcept override;

  IdPtr CreateId(std::string name, Tags tags) const noexcept override;

  std::shared_ptr<Counter> counter(IdPtr id) noexcept override;

  std::shared_ptr<DCounter> dcounter(IdPtr id) noexcept override;

  std::shared_ptr<Timer> timer(IdPtr id) noexcept override;

  std::shared_ptr<Gauge<double>> gauge(IdPtr id) noexcept override;

  std::shared_ptr<LongTaskTimer> long_task_timer(IdPtr id) noexcept override;

  std::shared_ptr<DistributionSummary> distribution_summary(
      IdPtr id) noexcept override;

  std::shared_ptr<DDistributionSummary> ddistribution_summary(
      IdPtr id) noexcept override;

  void RegisterMonitor(std::shared_ptr<Meter> meter) noexcept override;

  Meters meters() const noexcept override;

  Measurements measurements() noexcept override;

  std::shared_ptr<Gauge<double>> max_gauge(IdPtr id) noexcept override;

 protected:
  void expire_meters() noexcept;

 private:
  const Clock* clock_;
  int64_t freq_millis_;
  Tags freq_tags;
  mutable std::shared_ptr<Gauge<double>> meters_size;
  mutable std::shared_ptr<Counter> expired_meters;

  std::shared_ptr<Meter> InsertIfNeeded(std::shared_ptr<Meter> meter) noexcept;
  std::shared_ptr<Meter> GetMeter(const IdPtr& id) noexcept;

  template <typename M, typename... Args>
  std::shared_ptr<M> CreateAndRegisterAsNeeded(IdPtr id,
                                               Args&&... args) noexcept {
    auto existing = GetMeter(id);
    if (existing) {
      return std::static_pointer_cast<M>(existing);
    }
    std::shared_ptr<M> new_meter_ptr{
        std::make_shared<M>(std::move(id), std::forward<Args>(args)...)};
    auto meter_ptr = InsertIfNeeded(new_meter_ptr);
    return std::static_pointer_cast<M>(meter_ptr);
  }
};
}  // namespace meter
}  // namespace atlas
