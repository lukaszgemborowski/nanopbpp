#pragma once
#include <tuple>
#include <nanopbpp/metadata.h>
#include "test.pb.h"

static std::tuple<
	nanopbpp::message_metadata<IntegerContainer>
> messages_metadata = std::make_tuple(IntegerContainer_fields);
