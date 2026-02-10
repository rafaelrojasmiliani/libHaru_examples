#include "libharu_examples/invoice_example.h"

#include <gtest/gtest.h>

TEST(InvoiceExampleTest, ReturnsFalseForInvalidArguments) {
  libharu_examples::InvoiceExample example;

  libharu_examples::InvoiceExample::Provider provider{"Provider", "", ""};
  libharu_examples::InvoiceExample::Client client{"Client", "", ""};
  std::vector<libharu_examples::InvoiceExample::Item> items{{"Item", 1, 10.0}};

  EXPECT_FALSE(example.createInvoidcw(provider, client, items, ""));
  EXPECT_FALSE(example.createInvoidcw({}, client, items, "invoice.pdf"));
  EXPECT_FALSE(example.createInvoidcw(provider, {}, items, "invoice.pdf"));
  EXPECT_FALSE(example.createInvoidcw(provider, client, {}, "invoice.pdf"));
}

TEST(InvoiceExampleTest, ReturnsFalseForInvalidItemValues) {
  libharu_examples::InvoiceExample example;

  libharu_examples::InvoiceExample::Provider provider{"Provider", "", ""};
  libharu_examples::InvoiceExample::Client client{"Client", "", ""};

  std::vector<libharu_examples::InvoiceExample::Item> bad_quantity{{"Item", 0, 10.0}};
  std::vector<libharu_examples::InvoiceExample::Item> bad_price{{"Item", 1, -2.0}};

  EXPECT_FALSE(example.createInvoidcw(provider, client, bad_quantity, "invoice.pdf"));
  EXPECT_FALSE(example.createInvoidcw(provider, client, bad_price, "invoice.pdf"));
}
