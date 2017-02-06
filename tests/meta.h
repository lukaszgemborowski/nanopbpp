#pragma once
#include <tuple>
#include <nanopbpp/metadata.h>
#include "test.pb.h"

static nanopbpp::messages_metadata_set<
	nanopbpp::message_metadata<IntegerContainer>,
	nanopbpp::message_metadata<Extendable>
> messages_metadata(
	IntegerContainer_fields,
	Extendable_fields

);
