#pragma once
#include <tuple>
#include <nanopbpp/metadata.h>
#include "test.pb.h"


static nanopbpp::messages_metadata_set<
	nanopbpp::message_metadata<IntegerContainer>,
	nanopbpp::message_metadata<Extendable,
		nanopbpp::extensions_metadata_set<
			nanopbpp::extension_metadata<field_a_tag, IntegerContainer>,
			nanopbpp::extension_metadata<field_b_tag, FloatContainer>
		>
	>
> messages_metadata(
	{ IntegerContainer_fields },
	{ Extendable_fields, {field_a, field_b} }
);

