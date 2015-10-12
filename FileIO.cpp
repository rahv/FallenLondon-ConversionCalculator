#include "FileIO.h"

#include <iostream>
#include <fstream>
#include <QString>

#include "Conversion.h"
#include "ItemCategory.h"
#include "Utils.h"


namespace FileIO
{

/** hard-coded file names for categories, items and conversions */
static std::string const cat_file_name = "categories.txt";
static std::string const item_file_name = "items.txt";
static std::string const conversion_file_name = "conversions.txt";

/** reads the next line from the stream into a string */
std::string getNextLine(std::ifstream &in)
{
	std::string line("");
	getline(in, line);
	while (line.compare("") == 0 || line[0] == '#')
		getline(in,line);
	return line;
}

/** reads the categories file */
int readCategories(std::vector<ItemCategory> &categories)
{
	std::ifstream in( cat_file_name.c_str() );
	// file could not be opened for some reason 
	if (!in.is_open())
	{
		QString error = "Error reading file \"" + QString::fromStdString(cat_file_name) + "\".";
		Utils::message(error);
		return 1;
	}

	std::string line ("");
	std::list<std::string> line_content;
	while ( getline(in, line) )
	{
		// skip current line because it's empty or it's a comment
		if (line.compare("") == 0 || line[0] == '#')
			continue;

		line_content = Utils::splitString(line, '\t');
		auto it = line_content.cbegin(); 
		// more than two columns found in current line
		if (line_content.size() != 2)
		{
			Utils::message("File format error in categories file.");
			return 2;
		}

		if (atoi((it++)->c_str()) != categories.size())
		{
			Utils::message("Index error in categories file. Category-IDs must be numbered consecutively.");
			return 3;
		}

		categories.push_back(ItemCategory(QString::fromStdString(*it)));
	}
	
	return 0;
}

/** reads the items file */
int readItems(std::vector<ItemCategory> &categories)
{
	std::ifstream in( item_file_name.c_str() );
	// file could not be opened for some reason 
	if (!in.is_open())
	{
		QString error = "Error reading file \"" + QString::fromStdString(item_file_name) + "\".";
		Utils::message(error);
		return 1;
	}

	std::string line ("");
	std::list<std::string> line_content;
	while ( getline(in, line) )
	{
		// skip current line because it's empty or it's a comment
		if (line.compare("") == 0 || line[0] == '#')
			continue;

		line_content = Utils::splitString(line, '\t');
		auto it = line_content.cbegin();
		// more than three columns found in current line
		if (line_content.size() != 4)
		{
			Utils::message("File format error in items file.");
			return 2;
		}
		
		std::size_t const cat_idx (atoi((it++)->c_str()));
		if ( cat_idx >= categories.size())
		{
			Utils::message("Undefined category in items file.");
			return 3;
		}

		if (atoi((it++)->c_str()) != categories[cat_idx].size())
		{
			Utils::message("Index error in items file. Item-IDs must be numbered consecutively within categories.");
			return 4;
		}

		categories[cat_idx].addName(QString::fromStdString(*it++));
		categories[cat_idx].addValue(QString::fromStdString(*it++).toFloat());
	}
	
	return 0;
}

/** reads the conversions file */
int readConversions(std::vector<ItemCategory> const& categories,
                    std::vector<Conversion> &conversions)
{
	std::ifstream in( conversion_file_name.c_str() );
	// file could not be opened for some reason 
	if (!in.is_open())
	{
		QString error = "Error reading file \"" + QString::fromStdString(conversion_file_name) + "\".";
		Utils::message(error);
		return 1;
	}

	std::string line ("");
	std::list<std::string> line_content;
	while ( getline(in, line) )
	{
		// skip current line because it's empty or it's a comment
		if (line.compare("") == 0 || line[0] == '#')
			continue;

		line_content = Utils::splitString(line, '\t');
		auto it = line_content.cbegin();
		// more than six columns found in current line
		if (line_content.size() != 6)
		{
			Utils::message("File format error in conversions file.");
			return 2;
		}
		
		std::size_t const i_cat_idx (atoi((it++)->c_str()));
		if ( i_cat_idx >= categories.size())
		{
			Utils::message("Undefined category in items file.");
			return 3;
		}

		std::size_t const i_item_idx (atoi((it++)->c_str()));
		if ( i_item_idx >= categories[i_cat_idx].size())
		{
			Utils::message("Undefined item for category \"" + categories[i_cat_idx].name() + "\".");
			return 4;
		}

		std::size_t const i_amount (atoi((it++)->c_str()));

		std::size_t const o_cat_idx (atoi((it++)->c_str()));
		if ( o_cat_idx >= categories.size())
		{
			Utils::message("Undefined category in items file.");
			return 5;
		}

		std::size_t const o_item_idx (atoi((it++)->c_str()));
		if ( o_item_idx >= categories[o_cat_idx].size())
		{
			Utils::message("Undefined item for category \"" + categories[i_cat_idx].name() + "\".");
			return 6;
		}

		if (i_cat_idx == o_cat_idx && i_item_idx == o_item_idx)
		{
			Utils::message("Item-types can not be converted to themselves (" + categories[i_cat_idx].item(i_item_idx) + ").");
			continue;
		}

		conversions.push_back(Conversion(i_cat_idx, i_item_idx, i_amount, o_cat_idx, o_item_idx, atoi((it++)->c_str())));
	}
	
	return 0;
}

int readInputFiles(std::vector<ItemCategory> &categories,
                   std::vector<Conversion> &conversions)
{
	if (readCategories(categories) > 0)
		return 1;
	if (readItems(categories) > 0)
		return 2;
	if (readConversions(categories, conversions) > 0)
		return 3;

	return 0;
}

}

