#include <catch/catch.hpp>

#include "cppast/cppast.h"
#include "cppparser/cppparser.h"
#include "embedded-snippet-test-base.h"

class UniformInitTest : public EmbeddedSnippetTestBase
{
protected:
  UniformInitTest()
    : EmbeddedSnippetTestBase(__FILE__)
  {
  }
};

TEST_CASE_METHOD(UniformInitTest, "uniform init test")
{
#if TEST_CASE_SNIPPET_STARTS_FROM_NEXT_LINE
  auto x = std::vector<int> {5, 2, 3};
#endif
  auto testSnippet = getTestSnippetParseStream(__LINE__ - 2);

  cppparser::CppParser parser;
  const auto           ast = parser.parseStream(testSnippet.data(), testSnippet.size());
  REQUIRE(ast != nullptr);

  const auto members = GetAllOwnedEntities(*ast);
  REQUIRE(members.size() == 1);

  cppast::CppConstVarEPtr var = members[0];
  REQUIRE(var);

  cppast::CppConstUniformInitializerExprEPtr assignExpr = var->assignValue();
  REQUIRE(assignExpr);
}
