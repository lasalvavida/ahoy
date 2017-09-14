#include "ParserTest.h"

using namespace ahoy;
using namespace std;

TEST(ParserTest, Parse_Simple) {
	bool flag = false;
	string result;

	Parser* parser = new Parser();
	parser->define("flag", &flag);
	parser->define("result", &result)->defaults("default");

	EXPECT_EQ(flag, false);
	EXPECT_EQ(result, "default");

	bool value = parser->parse(4, new const char*[4] {
		"test", "--flag", "--result", "not-default"
	});

	ASSERT_EQ(value, true);
	EXPECT_EQ(flag, true);
	EXPECT_EQ(result, "not-default");

	delete parser;
}

TEST(ParserTest, Parse_Aliases) {
	bool flag = false;
	bool anotherFlag = false;

	Parser* parser = new Parser();
	parser->define("flag", &flag)->alias("alias")->description("This is a flag.");
	parser->define("anotherFlag", &anotherFlag);

	EXPECT_EQ(flag, false);

	bool value = parser->parse(2, new const char*[2]{
		"test", "--alias"
	});

	ASSERT_EQ(value, true);
	EXPECT_EQ(flag, true);
	EXPECT_EQ(anotherFlag, false);

	flag = false;
	parser->registerAlias("anotherFlag", "alias");
	parser->registerAlias("alias", "superAlias");

	value = parser->parse(2, new const char*[2]{
		"test", "--superAlias"
	});

	ASSERT_EQ(value, true);
	EXPECT_EQ(flag, false);
	EXPECT_EQ(anotherFlag, true);

	delete parser;
}

TEST(ParserTest, Parse_Required) {
	bool flag = false;

	Parser* parser = new Parser();
	parser->quiet(true)->define("flag", &flag)->require();

	bool value = parser->parse(2, new const char*[2]{
		"test", "--wrongFlag"
	});

	ASSERT_EQ(value, false);

	delete parser;
}

TEST(ParserTest, Parse_NotEnoughArguments) {
	int value;

	Parser* parser = new Parser();
	parser->quiet(true)->define("value", &value);

	bool result = parser->parse(2, new const char*[2]{
		"test", "--value"
	});

	ASSERT_EQ(result, false);

	result = parser->parse(3, new const char*[3]{
		"test", "--value", "10"
	});

	ASSERT_EQ(result, true);
	EXPECT_EQ(value, 10);

	delete parser;
}

TEST(ParserTest, Parse_Indices) {
	int zero = -1;
	int one = -1;
	int two = -1;

	Parser* parser = new Parser();
	parser->quiet(true);
	parser->define("zero", &zero)->index(0);
	parser->define("one", &one)->index(1);
	parser->define("two", &two)->index(2);

	bool result = parser->parse(4, new const char*[4]{
		"test", "0", "1", "2"
	});

	ASSERT_EQ(result, true);
	EXPECT_EQ(zero, 0);
	EXPECT_EQ(one, 1);
	EXPECT_EQ(two, 2);

	result = parser->parse(5, new const char*[5]{
		"test", "4", "--one", "5", "6"
	});

	ASSERT_EQ(result, true);
	EXPECT_EQ(zero, 4);
	EXPECT_EQ(one, 5);
	EXPECT_EQ(two, 6);

	delete parser;
}

TEST(ParserTest, Parse_Indices_Required) {
	string str1, str2;

	Parser* parser = new Parser();
	parser->quiet(true);
	parser->define("str1", &str1)->require()->index(0);
	parser->define("str2", &str2)->require()->index(1);

	bool result = parser->parse(3, new const char*[3]{
		"test", "one", "two"
	});

	ASSERT_EQ(result, true);
	EXPECT_EQ(str1, "one");
	EXPECT_EQ(str2, "two");
}

TEST(ParserTest, Parse_Array) {
	vector<int> intValues;
	vector<string> stringValues;

	Parser* parser = new Parser();
	parser->define("intValues", &intValues)->defaults({ 4, 5 });
	parser->define("stringValues", &stringValues)->count(1);

	ASSERT_EQ(intValues.size(), 2);
	EXPECT_EQ(intValues[0], 4);
	EXPECT_EQ(intValues[1], 5);

	bool result = parser->parse(8, new const char*[8]{
		"test", "--intValues", "2", "1", "0",
		"--stringValues", "one", "two"
	});

	ASSERT_EQ(result, true);
	ASSERT_EQ(intValues.size(), 3);
	EXPECT_EQ(intValues[0], 2);
	EXPECT_EQ(intValues[1], 1);
	EXPECT_EQ(intValues[2], 0);

	ASSERT_EQ(stringValues.size(), 1);
	EXPECT_EQ(stringValues[0], "one");

	delete parser;
}
