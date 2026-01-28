#ifndef SOMEIP_CERTIFICATES_HPP
#define SOMEIP_CERTIFICATES_HPP

#include "types.hpp"
#include <memory>
#include <string>
#include <vector>

namespace someip {

// X.509 Certificate information
struct CertificateInfo {
    std::string subject;
    std::string issuer;
    std::string serial_number;
    std::chrono::system_clock::time_point not_before;
    std::chrono::system_clock::time_point not_after;
    std:: vector<std::string> subject_alt_names;
    std::string public_key_algorithm;
    int key_size_bits;
    bool is_ca;
};

// Certificate verification result
struct VerificationResult {
    bool valid;
    std::string error_message;
    int error_code;
};

// Certificate manager for X.509 certificates
class CertificateManager {
public: 
    CertificateManager();
    ~CertificateManager();
    
    // Load certificate from file (PEM or DER format)
    bool load_certificate(const std::string& cert_path);
    
    // Load certificate from memory (PEM format)
    bool load_certificate_pem(const std::string& pem_data);
    
    // Load private key from file
    bool load_private_key(const std::string& key_path,
                          const std::string& password = "");
    
    // Load private key from memory (PEM format)
    bool load_private_key_pem(const std::string& pem_data,
                               const std::string& password = "");
    
    // Load CA certificates for verification
    bool load_ca_certificates(const std::string& ca_path);
    
    // Add a CA certificate from memory
    bool add_ca_certificate_pem(const std::string& pem_data);
    
    // Get certificate information
    std::optional<CertificateInfo> get_certificate_info() const;
    
    // Verify a certificate chain
    VerificationResult verify_certificate(