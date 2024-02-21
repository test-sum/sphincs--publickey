#include <iostream>
#include <fstream>
#include <vector>
#include "oqs/oqs.h"

std::vector<uint8_t> read_binary_file(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    std::vector<uint8_t> contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return contents;
}

int main() {
    // Initialize SPHINCS+ with the same variant used for signing
    OQS_SIG *sig = OQS_SIG_new(OQS_SIG_alg_sphincs_shake_128f_simple);
    if (!sig) {
        std::cerr << "SPHINCS+ initialization failed." << std::endl;
        return 1;
    }

    // Load the public key from file
    auto public_key = read_binary_file("public.key");

    // Prompt for the name of the message file to verify
    std::string message_file;
    std::cout << "Enter the name of the message file to verify: ";
    std::getline(std::cin, message_file);

    // Read message from file
    auto message = read_binary_file(message_file);

    // Load the signature from file
    auto signature = read_binary_file("signature.sig");

    // Verify the signature
    if (OQS_SIG_verify(sig, message.data(), message.size(), signature.data(), signature.size(), public_key.data()) == OQS_SUCCESS) {
        std::cout << "Signature is valid." << std::endl;
    } else {
        std::cerr << "Signature is invalid." << std::endl;
        OQS_SIG_free(sig);
        return 1;
    }

    // Clean up
    OQS_SIG_free(sig);
    return 0;
}
