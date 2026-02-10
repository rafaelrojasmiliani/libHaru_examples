#include "libharu_examples/invoice_example.h"

#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  const std::string output_pdf = (argc > 1) ? argv[1] : "invoice_formal_example.pdf";

  libharu_examples::InvoiceExample::Provider provider{
      "John Smith",
      "4490 Oak Drive, Albany, NY 12210",
      "john.smith@provider.example",
  };

  libharu_examples::InvoiceExample::Client client{
      "Jessie M Horne",
      "4312 Wood Road, New York, NY 10031",
      "jessie.horne@client.example",
  };

  std::vector<libharu_examples::InvoiceExample::Item> items{
      {"Front and rear brake cables", 1, 100.00},
      {"New set of pedal arms", 2, 25.00},
      {"Labor 3hrs", 3, 15.00},
  };

  libharu_examples::InvoiceExample example;
  if (!example.createInvoidcw(provider, client, items, output_pdf)) {
    std::cerr << "Failed to create formal invoice PDF: " << output_pdf << '\n';
    return 1;
  }

  std::cout << "Created formal invoice PDF: " << output_pdf << '\n';
  return 0;
}
