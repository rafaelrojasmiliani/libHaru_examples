#include "libharu_examples/invoice_example.h"

#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  const std::string output_pdf = (argc > 1) ? argv[1] : "invoice_example.pdf";

  libharu_examples::InvoiceExample::Provider provider{
      "Example Provider Ltd.",
      "42 Provider Street, Example City",
      "accounts@provider.example",
  };

  libharu_examples::InvoiceExample::Client client{
      "Client Co.",
      "100 Client Avenue, Demo Town",
      "billing@client.example",
  };

  std::vector<libharu_examples::InvoiceExample::Item> items{
      {"Design and planning", 6, 75.00},
      {"Implementation", 12, 95.00},
      {"Validation and handover", 4, 85.00},
  };

  libharu_examples::InvoiceExample example;
  if (!example.createInvoidcw(provider, client, items, output_pdf)) {
    std::cerr << "Failed to create invoice PDF: " << output_pdf << '\n';
    return 1;
  }

  std::cout << "Created invoice PDF: " << output_pdf << '\n';
  return 0;
}
