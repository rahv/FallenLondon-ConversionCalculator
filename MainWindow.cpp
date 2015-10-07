#include "MainWindow.h"

#include <algorithm>
#include <iostream>

#include "FileIO.h"
#include "InfoDialog.h"
#include "ItemCategory.h"
#include "Utils.h"

#include <QComboBox>
#include <QIntValidator>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QStringList>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent)
: _validator(new QIntValidator(1, 1e+6, this))
{
	int const ret (FileIO::readInputFiles(_categories, _conversions));
	if (ret != 0)
		exit(ret);
	setupUi(this);
	this->itemAmount->setValidator(_validator);
	populateMenus();
}

MainWindow::~MainWindow()
{
}

void MainWindow::showInfo() const
{
	InfoDialog dlg;
	dlg.exec();
}

void MainWindow::populateMenus()
{
	QStringList cat_list;
	std::size_t const n_cats (_categories.size());
	for (std::size_t i=0; i<n_cats; ++i)
		cat_list << _categories[i].name();
	this->catBox->addItems(cat_list);
}

void MainWindow::on_catBox_currentIndexChanged(int idx)
{
	this->itemBox->clear();
	std::vector<ItemIndex> items;
	QStringList item_list;
	std::size_t const n_conv (_conversions.size());
	for (std::size_t i=0; i<n_conv; ++i)
	{
		if (_conversions[i].getOutputIdx().first != idx)
			continue;

		ItemIndex current_item (_conversions[i].getOutputIdx());
		if (std::find(items.cbegin(), items.cend(), current_item) == items.end())
		{
			items.push_back(current_item);
			item_list << _categories[idx].item(current_item.second);
		}
	}
	this->itemBox->addItems(item_list);
}

ItemIndex MainWindow::getCurrentSelection() const
{
	std::size_t const cat_idx (this->catBox->currentIndex());
	std::size_t const n_items (_categories[cat_idx].size());
	for (std::size_t i=0; i<n_items; ++i)
		if (_categories[cat_idx].item(i) == this->itemBox->currentText())
			return std::make_pair(cat_idx, i);
	return std::make_pair(cat_idx, std::numeric_limits<std::size_t>::max());
}

void MainWindow::getDirectConversions(std::vector<ConversionState> &conv_states, std::size_t state_idx) const
{
	std::size_t const n_conv (_conversions.size());
	for (std::size_t i=0; i<n_conv; ++i)
	{
		ItemIndex const input_idx (_conversions[i].getInputIdx());
		// add item if it's convertable into target type and *not* in the list already (to avoid conversion circles)
		if ( _conversions[i].getOutputIdx() == conv_states[state_idx].input_idx && 
		     std::none_of(conv_states.cbegin(), conv_states.cend(), [&input_idx](ConversionState s){ return s.input_idx == input_idx; }))
			conv_states.push_back( {input_idx, state_idx, 0, conv_states[state_idx].input, conv_states[state_idx].actions} );
	}
}

std::vector<Conversion> const MainWindow::getPossibleConversions(std::vector<ConversionState> const& conv_states, std::size_t state_idx) const
{
	std::vector<Conversion> conv;
	std::size_t const n_conv (_conversions.size());
	for (std::size_t i=0; i<n_conv; ++i)
	{
		if (_conversions[i].getInputIdx()  == conv_states[state_idx].input_idx &&
		    _conversions[i].getOutputIdx() == conv_states[conv_states[state_idx].output_idx].input_idx)
			conv.push_back(_conversions[i]);
	}
	
	// if multiple conversions exist, order them by output amount
	if (conv.size()>1)
		std::sort( conv.begin(), conv.end(), [](Conversion a, Conversion b) { return a.getOutputAmount() < b.getOutputAmount(); } );
	return conv;
}

void MainWindow::getItemsNeeded(std::vector<ConversionState> &conv_states, std::size_t state_idx) const
{
	std::vector<Conversion> const conv (getPossibleConversions(conv_states, state_idx));
	if (conv.empty())
		return;

	// calculate number of items needed
	// slightly different methods are applied for minimising the number of input items or input actions
	std::size_t rest_amount = conv_states[state_idx].output;
	for (std::size_t i=conv.size()-1; i>0; --i)
	{
		std::size_t const ratio (static_cast<std::size_t>(std::floor( conv[i].getOutputAmount()/ static_cast<double>(conv[i-1].getOutputAmount()))));
		std::size_t batch (static_cast<std::size_t>(std::floor( rest_amount/ static_cast<double>(conv[i].getOutputAmount()))));
		
		if (this->itemsButton->isChecked() && (conv[i-1].getOutputAmount() * (ratio-1)) < (rest_amount - conv[i].getOutputAmount() * batch))
			batch++;
		if (this->actionsButton->isChecked() && conv[i-1].getOutputAmount() < (rest_amount - conv[i].getOutputAmount() * batch))
			batch++;
		conv_states[state_idx].input   += (conv[i].getInputAmount() * batch);
		conv_states[state_idx].actions += batch;
		rest_amount = (rest_amount > (conv[i].getOutputAmount() * batch)) ? rest_amount-(conv[i].getOutputAmount() * batch) : 0;
	}
	std::size_t const n_actions (static_cast<std::size_t>(std::ceil(rest_amount / static_cast<double>(conv[0].getOutputAmount()))));
	conv_states[state_idx].input   += (n_actions * conv[0].getInputAmount());
	conv_states[state_idx].actions += n_actions;
}

void MainWindow::displayResults(std::vector<ConversionState> const& conv_states)
{
	int const n_states (static_cast<int>(conv_states.size()));
	QStandardItemModel* model (new QStandardItemModel(n_states-1, 4, this));
	model->setHorizontalHeaderItem(0, new QStandardItem("# Items"));
	model->setHorizontalHeaderItem(1, new QStandardItem("Item type"));
	model->setHorizontalHeaderItem(2, new QStandardItem("# Actions"));
	model->setHorizontalHeaderItem(3, new QStandardItem("Intermediate items"));
	this->tableView->setModel(model);
	this->tableView->setColumnWidth(0, 70);
	this->tableView->setColumnWidth(1, 270);
	this->tableView->setColumnWidth(2, 100);
	this->tableView->setColumnWidth(3, 600);

	for (int i=1; i<n_states; ++i)
	{
		QStandardItem *const n_input_items (new  QStandardItem(QString::number(conv_states[i].input) + " x"));
		QStandardItem *const item_name (new  QStandardItem(_categories[conv_states[i].input_idx.first].item(conv_states[i].input_idx.second)));
		QStandardItem *const n_actions (new  QStandardItem("(" + QString::number(conv_states[i].actions) + " actions)"));
		QStandardItem *const intermediates(new  QStandardItem(getIntermediateItemsString(conv_states, i)));
		model->setItem(i-1, 0, n_input_items);
		model->setItem(i-1, 1, item_name);
		model->setItem(i-1, 2, n_actions);
		model->setItem(i-1, 3, intermediates);
	}
}

QString const MainWindow::getIntermediateItemsString(std::vector<ConversionState> const& conv_states, std::size_t target_idx) const
{
	QString str ("");
	if (conv_states[target_idx].output_idx == 0)
		str = "(direct conversion)";
	else
	{
		str += "via ";
		target_idx = conv_states[target_idx].output_idx;
		while (target_idx != 0)
		{
			ItemIndex const current_idx (conv_states[target_idx].input_idx);
			str += (_categories[current_idx.first].item(current_idx.second) + ", ");
			target_idx = conv_states[target_idx].output_idx;
		}
	}
	return str;
}

void MainWindow::on_startButton_pressed()
{
	// check if input is okay
	int pos (0);
	if (_validator->validate(this->itemAmount->text(), pos) != QValidator::Acceptable)
	{
		Utils::message("Input amount not valid.");
		return;
	}

	// start building the directed graph and calculate item numbers for each conversion step
	std::vector<ConversionState> conv_states;
	conv_states.push_back( { getCurrentSelection(), 0, this->itemAmount->text().toInt(), this->itemAmount->text().toInt(), 0 } );
	getDirectConversions(conv_states, 0);
	for (std::size_t i=1; i<conv_states.size(); ++i)
	{
		getItemsNeeded(conv_states, i);
		getDirectConversions(conv_states, i);
	}

	displayResults(conv_states);
}

