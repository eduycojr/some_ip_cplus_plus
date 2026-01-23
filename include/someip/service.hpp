#ifndef SOMEIP_SERVICE_HPP
#define SOMEIP_SERVICE_HPP

#include "types.hpp"
#include <vector>
#include <map>
#include <mutex>

namespace someip {

struct Method {
    MethodId id;
    MethodHandler handler;
    Method() = default;
    Method(MethodId i, MethodHandler h) : id(i), handler(std::move(h)) {}
};

class ServiceRegistry {
public:
    // Register a method
    void register_method(ServiceId svc, MethodId mth, MethodHandler handler) {
        std::lock_guard<std::mutex> lk(mutex_);
        registry_[std::make_pair(svc, mth)] = std::move(handler);
    }

    // Unregister
    void unregister_method(ServiceId svc, MethodId mth) {
        std::lock_guard<std::mutex> lk(mutex_);
        registry_.erase(std::make_pair(svc, mth));
    }

    // Find handler; returns nullopt if not found
    std::optional<MethodHandler> find_handler(ServiceId svc, MethodId mth) {
        std::lock_guard<std::mutex> lk(mutex_);
        auto it = registry_.find(std::make_pair(svc, mth));
        if (it == registry_.end()) return std::nullopt;
        return it->second;
    }
private:
    std::map<std::pair<ServiceId, MethodId>, MethodHandler> registry_;
    std::mutex mutex_;
};

} // namespace someip

#endif // SOMEIP_SERVICE_HPP