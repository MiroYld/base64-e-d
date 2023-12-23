#include <iostream>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <string>
#include <vector>

static const auto APP_HELPER  = "-h";
static const auto APP_VERSION = "-v";
static const auto APP_ENCODE  = "-e";
static const auto APP_DECODE  = "-d";

std::string base64_encode(const std::string &input) {
  BIO* bio,*b64;
  BUF_MEM *buffer_ptr;

  b64 = BIO_new(BIO_f_base64());
  bio = BIO_new(BIO_s_mem());
  bio = BIO_push(b64, bio);

  BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // Ignore newline characters

  BIO_write(bio, input.c_str(), input.length());
  BIO_flush(bio);

  BIO_get_mem_ptr(bio, &buffer_ptr);
  BIO_set_close(bio, BIO_NOCLOSE);
  BIO_free_all(bio);

  return std::string(buffer_ptr->data, buffer_ptr->length);
}

std::string base64_decode(const std::string &input) {
  BIO *bio, *b64;
  auto *buffer = new char[input.length()];
  std::copy(input.begin(), input.end(), buffer);

  b64 = BIO_new(BIO_f_base64());
  bio = BIO_new_mem_buf(buffer, input.length());
  bio = BIO_push(b64, bio);

  BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // Ignore newline characters

  auto d_length = BIO_read(bio, buffer, input.length());
  BIO_free_all(bio);

  return std::string(buffer, d_length);
}

void display_version() {
  std::cout << "Version 0.1\n";
}

void display_help() {
  std::cout << "Usage: ./base_64 [options]\n"
            << "Options:\n"
            << "  -h Display this help message\n"
            << "  -v Display the version number\n"
            << "  -e Base64 encoding\n"
            << "  -d Base64 decoding\n";
}

int main(int argc, char **argv) {
  if (argc == 1) {
    display_help();
    return 0;
  }

  for (auto it = argv + 1; it < argv + argc; ++it) {
    const auto &arg = std::string(*it);

    if (arg == APP_VERSION) {
      display_version();
      return 0;
    } else if (arg == APP_HELPER) {
      display_help();
      return 0;
    } else if (arg == APP_ENCODE && (it + 1) < (argv + argc)) {
      const auto input = std::string(*(it + 1));
      std::cout << "Encoded: " << base64_encode(input) << "\n";
      return 0;
    } else if (arg == APP_DECODE && (it + 1) < (argv + argc)) {
      const auto input = std::string(*(it + 1));
      std::cout << "Decoded: " << base64_decode(input) << "\n";
      return 0;
    } else if ((arg == APP_ENCODE || arg == APP_DECODE) &&(it + 1) >= (argv + argc)) {
      std::cerr << "Missing argument for option " << arg << "\n";
      display_help();
      return 1;
    } else {
      std::cerr << "Unknown option or missing argument: " << arg << "\n";
      display_help();
      return 1;
    }
  }

  return 0;
}
