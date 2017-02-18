# nanopb++
Lightweight C++ wrapper(s) library for nanopb implementation.
The goal is to simplify some common tasks using C++ and metaprogramming
wherever possible.

## Idea
Library is simplifying some common tasks using modern C++ features and it is also introducing more type safety to your project. To achieve it nanopb++ is introducing something which I personally like to call "metadata". This is description of your messages in C++ with some helper templates. It's most painfull part of using nanopb++ but it's done only once for a set of messages. In some future this task may be automated but for now it must be done manually. Example of such description for this proto file:
```
syntax = "proto2";
import "nanopb.proto";

message IntegerContainer {
	required int32 a = 1;
	optional int32 b = 2;
}

message FloatContainer {
	required float c = 1;
}

message Extendable {
	extensions 10 to 100;
}

extend Extendable {
	optional IntegerContainer field_a = 11;
	optional FloatContainer field_b = 12;
}

```
would be something like this:
```cpp
static nanopbpp::message_metadata_set<
	nanopbpp::message_metadata<IntegerContainer>,
	nanopbpp::message_metadata<Extendable,
		nanopbpp::extension_metadata_set<
			nanopbpp::extension_metadata<field_a_tag, IntegerContainer>,
			nanopbpp::extension_metadata<field_b_tag, FloatContainer>
		>
	>
> messages_metadata(
	{ IntegerContainer_fields },
	{ Extendable_fields, {field_a, field_b} }
);
```
it's creating a relationship between message type (like `IntegerContainer`) and it's pb_field_t description. Additionally it can describe extensions of the message. Now if you have such meta-description you can use it with nanopb++ helper functions such as encode and decode (extract from test cases):
```cpp
using namespace nanopbpp;

IntegerContainer source = {0}, destination = {0};

// prepare data to be encoded in protobuf, basically
// you need to fill standard nanopb generated structure
source.a = 10;
source.has_b = true;
source.b = 20;

std::array<uint8_t, 64> buffer = {0};

// encode message to buffer
ASSERT_TRUE(encode(messages_metadata, buffer.begin(), buffer.end(), source));

// decode message from buffer
ASSERT_TRUE(decode(messages_metadata, buffer.begin(), buffer.end(), destination));

ASSERT_EQ(source.a, destination.a);
ASSERT_EQ(source.b, destination.b);
ASSERT_EQ(source.has_b, destination.has_b);
```
simple, huh? I'm trying to keep all the basic scenarios as test cases in [scenarios.cc](https://github.com/lukaszgemborowski/nanopbpp/blob/master/tests/scenarios.cc) file.

## Current status
"work in progress" with very limited functionality and a small set of
unit tests. To see use-cases please see tests directory.

## Building
as for now the library contains only C++ headers and it does not require anything special, only compiler with C++11 support. If you want to run tests than you need some additional things:

* cmake >= 3.2
* protoc
* gtest library development files

additionally you need to checkout nanopb submodule in the project (`git submodule update --init nanopb`) and run `make` in nanopb/generator/proto directory. After that you can create some temporary build directory, run `cmake` and then `make`. Tests will be build for you, test executable name is simply "tests". You can run it now with `./tests` command.
