#ifndef CONVERSION_H
#define CONVERSION_H

#include "ItemCategory.h"

typedef std::pair<std::size_t,std::size_t> ItemIndex;

/**
 * This stores the state of current calculations for conversion chains
 */
struct ConversionState
{
	/// Input item type
	ItemIndex const input_idx;
	/// Index of next conversion in chain
	std::size_t const output_idx;
	/// Number of required input items
	std::size_t input;
	/// Number of requested/necessary output items
	std::size_t output;
	/// Number of actions required for the complete chain up to the originally requested item(s)
	std::size_t actions;
	/// Actual number of resulting (final) output items
	std::size_t target_output;
	/// Value of items during conversion chain
	float value;
};

/**
* Stores a conversion consisting of the ItemIndex for the input item (i.e. Category + ItemID), 
* amount of input items, ItemIndex of the output item and their amount.
*/
class Conversion
{
public:
	Conversion(std::size_t input_cat, std::size_t input_id, std::size_t input_amount,
	           std::size_t output_cat, std::size_t output_id, std::size_t output_amount)
	: _input_index(std::make_pair(input_cat, input_id)), _input_amount(input_amount),
	  _output_index(std::make_pair(output_cat, output_id)), _output_amount(output_amount)
	{};

	ItemIndex const getInputIdx () const { return _input_index; }
	ItemIndex const getOutputIdx() const { return _output_index; }
	std::size_t getInputAmount() const { return _input_amount; }
	std::size_t getOutputAmount() const { return _output_amount; }

private:
	std::size_t _input_amount;
	std::size_t _output_amount;
	ItemIndex _input_index;
	ItemIndex _output_index;
};

#endif //CONVERSION_H
