/*
High-level overview
-------------------
This executable drives the more formal invoice variant while reusing the exact same
`InvoiceExample` API surface (`Provider`, `Client`, `Item`, `createInvoidcw`).

1) Create invoice entities with data resembling a formal business invoice.
2) Call the shared invoice renderer, which uses libHaru to produce the polished layout.
3) Surface generation status to the command line.

libHaru logic addressed by this executable
------------------------------------------
- The libHaru API usage remains in the implementation layer (`src/invoice_example.cpp`).
- This file demonstrates that multiple visual scenarios can be produced by passing different
  data into the same rendering pipeline.
*/
#include "libharu_examples/invoice_example.h"

#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  // Step 1: Resolve destination output filename.
  const std::string output_pdf = (argc > 1) ? argv[1] : "invoice_formal_example.pdf";

  // Step 2: Build formal invoice sample data.
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

  // Step 3: Render formal invoice and translate result into program exit status.
  libharu_examples::InvoiceExample example;
  if (!example.createInvoidcw(provider, client, items, output_pdf)) {
    std::cerr << "Failed to create formal invoice PDF: " << output_pdf << '\n';
    return 1;
  }

  std::cout << "Created formal invoice PDF: " << output_pdf << '\n';
  return 0;
}
