#ifndef SOMEIP_CRYPTO_HPP
#define SOMEIP_CRYPTO_HPP

#include "types.hpp"
#include <memory>
#include <string>

namespace someip {

// Encryption/Decryption using AES-256-GCM
class Crypto {
public:
    // Key sizes
    static constexpr size_t AES_KEY_SIZE = 32;   // 256 bits
    static constexpr size_t AES_IV_SIZE = 12;    // 96 bits (recommended for GCM)
    static constexpr size_t AES_TAG_SIZE = 16;   // 128 bits
    
    Crypto();
    ~Crypto();
    
    // Set encryption key (must be AES_KEY_SIZE bytes)
    bool set_key(const Payload& key);
    
    // Generate a random key
    static Payload generate_key();
    
    // Generate a random IV/nonce
    static Payload generate_iv();
    
    // Encrypt data with AES-256-GCM
    // Returns:  IV (12 bytes) + Ciphertext + Tag (16 bytes)
    std::optional<Payload> encrypt(const Payload& plaintext,
                                    const Payload& additional_data = {});
    
    // Decrypt data encrypted with AES-256-GCM
    // Input: IV (12 bytes) + Ciphertext + Tag (16 bytes)
    std::optional<Payload> decrypt(const Payload& ciphertext,
                                    const Payload& additional_data = {});
    
    // HMAC-SHA256 for message authentication
    static Payload hmac_sha256(const Payload& key, const Payload& data);
    
    // SHA-256 hash
    static Payload sha256(const Payload& data);
    
    // Secure random bytes
    static Payload random_bytes(size_t length);
    
    // Derive key from password using PBKDF2
    static Payload derive_key(const std::string& password,
                               const Payload& salt,
                               int iterations = 10000);
    
    // Check if crypto is available (OpenSSL initialized)
    static bool is_available();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

// Secure SOME/IP message wrapper
struct SecureSomeIpMessage {
    Uint32 sequence_number;
    Payload encrypted_payload;  // IV + Ciphertext + Tag
    Payload additional_auth_data;
    
    Payload serialize() const;
    static SecureSomeIpMessage deserialize(const Payload& data);
};

// Secure communication wrapper
class SecureCommunication {
public:
    SecureCommunication();
    ~SecureCommunication();
    
    // Set pre-shared key
    bool set_psk(const Payload& key);
    
    // Encrypt a SOME/IP message
    std:: optional<Payload> encrypt_message(const SomeIpMessage& msg);
    
    // Decrypt a SOME/IP message
    std::optional<SomeIpMessage> decrypt_message(const Payload& encrypted_data);
    
    // Get current sequence number
    Uint32 current_sequence() const { return sequence_number_; }
    
    // Reset sequence number (e.g., after key rotation)
    void reset_sequence() { sequence_number_ = 0; }

private:
    Crypto crypto_;
    std::atomic<Uint32> sequence_number_{0};
    std::mutex mutex_;
};

}  // namespace someip

#endif  // SOMEIP_CRYPTO_HPP