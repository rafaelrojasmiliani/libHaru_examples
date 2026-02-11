/*
High-level overview
-------------------
This executable demonstrates the basic invoice generation scenario using the
`InvoiceExample` API exposed by the library.

1) Build provider, client, and line-item data structures.
2) Call `InvoiceExample::createInvoidcw(...)`, which applies libHaru rendering logic.
3) Print user-facing success/failure output.

libHaru logic addressed by this executable
------------------------------------------
- Similar to the text example, this file focuses on data assembly and orchestration.
- Actual low-level libHaru operations (fonts, lines, geometry, save lifecycle) are encapsulated
  in `src/invoice_example.cpp`.
*/
#include "libharu_examples/invoice_example.h"

#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  // Step 1: Resolve destination output filename.
  const std::string output_pdf = (argc > 1) ? argv[1] : "invoice_example.pdf";

  // Step 2: Build sample business entities used by the invoice renderer.
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

  // Step 3: Invoke the invoice generation API and return a process status code.
  libharu_examples::InvoiceExample example;
  if (!example.createInvoidcw(provider, client, items, output_pdf)) {
    std::cerr << "Failed to create invoice PDF: " << output_pdf << '\n';
    return 1;
  }

  std::cout << "Created invoice PDF: " << output_pdf << '\n';
  return 0;
}
