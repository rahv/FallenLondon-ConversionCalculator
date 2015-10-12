#ifndef ITEMCATEGORY_H
#define ITEMCATEGORY_H

#include <vector>
#include <QString>

/**
 * Stores an item category including all item in that category
 */
class ItemCategory
{
public:
	ItemCategory(QString name)
	: _name(name)
	{}

	void addName(QString const& item_name) { _items.push_back(item_name); }
	void addValue(float value) { _value.push_back(value); }
	QString const& item(std::size_t idx) const { return _items[idx]; }
	QString const& name() const { return _name; }
	std::size_t size() const { return _items.size(); }

private:
	QString _name;
	std::vector<QString> _items;
	std::vector<float> _value;
};


#endif //ITEMCATEGORY_H

