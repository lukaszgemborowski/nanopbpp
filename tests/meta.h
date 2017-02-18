#pragma once
#include <tuple>
#include <nanopbpp/message_metadata_set.h>
#include <nanopbpp/extension_metadata_set.h>
#include "test.pb.h"


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

