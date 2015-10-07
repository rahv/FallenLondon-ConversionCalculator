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

	void add(QString const& item_name) { _items.push_back(item_name); }
	QString const& item(std::size_t idx) const { return _items[idx]; }
	QString const& name() const { return _name; }
	std::size_t size() const { return _items.size(); }

private:
	QString _name;
	std::vector<QString> _items;
};


#endif //ITEMCATEGORY_H

