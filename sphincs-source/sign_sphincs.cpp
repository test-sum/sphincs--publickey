#include <iostream>
#include <fstream>
#include <vector>
#include "oqs/oqs.h"

std::vector<uint8_t> read_binary_file(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for reading: " + filename);
    }
    std::vector<uint8_t> contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return contents;
}

bool file_exists(const std::string &filename) {
    std::ifstream file(filename);
    bool exists = file.good();
    file.close();
    return exists;
}

int main() {
    OQS_SIG *sig = nullptr; // Declare sig here to ensure it's available throughout the function.
    try {
        //sig = OQS_SIG_new(OQS_SIG_alg_sphincs_shake_256f_simple);
          OQS_SIG *sig = OQS_SIG_new(OQS_SIG_alg_sphincs_shake_256f_simple);

        if (!sig) {
            throw std::runtime_error("SPHINCS+ initialization failed. This could be due to an unsupported algorithm variant or an issue with the liboqs library.");
        }

        if (file_exists("public.key") && file_exists("private.key")) {
            std::cout << "Keys already exist." << std::endl;
        } else {
            size_t public_key_len = sig->length_public_key;
            size_t private_key_len = sig->length_secret_key;
            std::vector<uint8_t> public_key(public_key_len);
            std::vector<uint8_t> private_key(private_key_len);

            if (OQS_SIG_keypair(sig, public_key.data(), private_key.data()) != OQS_SUCCESS) {
                throw std::runtime_error("Key generation failed. Check if the liboqs library supports the chosen algorithm variant.");
            }
            std::cout << "Keys generated successfully." << std::endl;

            std::ofstream pub_file("public.key", std::ios::binary);
            if (!pub_file.is_open()) {
                throw std::runtime_error("Failed to open public.key for writing.");
            }
            pub_file.write(reinterpret_cast<const char*>(public_key.data()), public_key.size());
            pub_file.close();

            std::ofstream priv_file("private.key", std::ios::binary);
            if (!priv_file.is_open()) {
                throw std::runtime_error("Failed to open private.key for writing.");
            }
            priv_file.write(reinterpret_cast<const char*>(private_key.data()), private_key.size());
            priv_file.close();

            std::cout << "Keys saved to files." << std::endl;
        }

        auto private_key = read_binary_file("private.key");
        std::string message_file;
        std::cout << "Enter the name of the message file to sign: ";
        std::getline(std::cin, message_file);
        auto message = read_binary_file(message_file);

        size_t signature_len = sig->length_signature;
        std::vector<uint8_t> signature(signature_len, 0);

        if (OQS_SIG_sign(sig, signature.data(), &signature_len, message.data(), message.size(), private_key.data()) != OQS_SUCCESS) {
            throw std::runtime_error("Signing failed. Ensure the message and keys are correctly formatted and valid.");
        }
        signature.resize(signature_len); // Resize signature vector to actual signature length

        std::ofstream sig_file("signature.sig", std::ios::binary);
        if (!sig_file.is_open()) {
            throw std::runtime_error("Failed to open signature.sig for writing.");
        }
        sig_file.write(reinterpret_cast<const char*>(signature.data()), signature.size());
        sig_file.close();

        std::cout << "Message signed and signature saved to file." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        if (sig) { // Check if sig was successfully allocated before attempting to free it.
            OQS_SIG_free(sig);
        }
        return 1;
    }

    if (sig) { // Ensure sig is not nullptr before calling OQS_SIG_free.
        OQS_SIG_free(sig);
    }
    return 0;
}
