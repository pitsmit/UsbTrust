#pragma once

#include "entities/DeviceEvent/DeviceEvent.hpp"
#include "managers/DeviceEventNotifyManager/DeviceEventNotifyManager.hpp"
#include "managers/MountCoordinator/MountCoordinator.hpp"
#include "ports/IUsbDeviceContextProvider.hpp"

struct StrategyContext {
    DeviceEventNotifyManager &notifier;
    IUsbDeviceContextProvider &provider;
    MountCoordinator &coordinator;
};

class HandleEventStrategy {
  protected:
    StrategyContext context;

  public:
    HandleEventStrategy(StrategyContext &context_) : context(context_) {}
    virtual ~HandleEventStrategy() = default;
    virtual void handle(const core::path &node) = 0;
    virtual bool can_handle(EventType type) = 0;
};

class HandleInsertEventStrategy : public HandleEventStrategy {
  public:
    HandleInsertEventStrategy(StrategyContext &context_) : HandleEventStrategy(context_) {}

    void handle(const core::path &node) override;
    bool can_handle(EventType type) override;
};

class HandleRemoveEventStrategy : public HandleEventStrategy {
  public:
    HandleRemoveEventStrategy(StrategyContext &context_) : HandleEventStrategy(context_) {}

    void handle(const core::path &node) override;
    bool can_handle(EventType type) override;
};