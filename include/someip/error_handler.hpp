#ifndef SOMEIP_ERROR_HANDLER_HPP
#define SOMEIP_ERROR_HANDLER_HPP

#include "types.hpp"
#include <chrono>
#include <functional>
#include <map>
#include <mutex>
#include <future>
#include <queue>

namespace someip {

// Error codes specific to the library
enum class ErrorCode {
    SUCCESS = 0,
    TIMEOUT,
    CONNECTION_FAILED,
    SERVICE_NOT_FOUND,
    METHOD_NOT_FOUND,
    INVALID_MESSAGE,
    SERIALIZATION_ERROR,
    DESERIALIZATION_ERROR,
    NETWORK_ERROR,
    TLS_ERROR,
    CERTIFICATE_ERROR,
    ENCRYPTION_ERROR,
    DECRYPTION_ERROR,
    INTERNAL_ERROR
};

// Error information
struct Error {
    ErrorCode code;
    std::string message;
    std::chrono::steady_clock::time_point timestamp;
    
    Error(ErrorCode c = ErrorCode::SUCCESS, const std:: string& msg = "")
        : code(c), message(msg), timestamp(std::chrono::steady_clock::now()) {}
};

// Error callback type
using ErrorCallback = std::function<void(const Error&)>;

// Pending request with timeout
struct PendingRequest {
    SessionId session_id;
    std::chrono::steady_clock::time_point deadline;
    std::promise<MethodResult> promise;
    std::function<void(const Error&)> error_callback;
};

// Error handler and timeout manager
class ErrorHandler {
public: 
    ErrorHandler();
    ~ErrorHandler();
    
    void start();
    void stop();
    
    // Set global error callback
    void set_error_callback(ErrorCallback callback);
    
    // Report an error
    void report_error(ErrorCode code, const std::string& message = "");
    
    // Register a pending request with timeout
    std::future<MethodResult> register_pending_request(
        SessionId session_id,
        std::chrono::milliseconds timeout,
        std::function<void(const Error&)> error_callback = nullptr);
    
    // Complete a pending request
    void complete_request(SessionId session_id, const MethodResult& result);
    
    // Cancel a pending request
    void cancel_request(SessionId session_id, ErrorCode reason);
    
    // Get error statistics
    struct Stats {
        uint64_t total_errors;
        uint64_t timeout_errors;
        uint64_t network_errors;
        uint64_t protocol_errors;
    };
    Stats get_stats() const;
    
    // Convert error code to string
    static std::string error_code_to_string(ErrorCode code);
    static std::string return_code_to_string(ReturnCode code);

private:
    void timeout_check_loop();
    
    ErrorCallback error_callback_;
    std::map<SessionId, std:: unique_ptr<PendingRequest>> pending_requests_;
    
    Stats stats_{0, 0, 0, 0};
    
    std::atomic<bool> running_{false};
    std::thread timeout_thread_;
    std::mutex mutex_;
    std::condition_variable cv_;
};

}  // namespace someip

#endif  // SOMEIP_ERROR_HANDLER_HPP