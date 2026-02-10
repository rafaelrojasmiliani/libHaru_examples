#pragma once

#include <string>
#include <vector>

namespace libharu_examples {

class InvoiceExample {
 public:
  struct Client {
    std::string name;
    std::string address;
    std::string email;
  };

  struct Provider {
    std::string name;
    std::string address;
    std::string email;
  };

  struct Item {
    std::string description;
    int quantity;
    double unit_price;
  };

  bool createInvoidcw(const Provider& provider,
                      const Client& client,
                      const std::vector<Item>& items,
                      const std::string& output_pdf_path) const;
};

}  // namespace libharu_examples
