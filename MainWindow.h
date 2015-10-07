
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include <QMainWindow>

#include "Conversion.h"

class QIntValidator;

/**
 * Main class for the application
 */
class MainWindow : public QMainWindow, private Ui_MainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);
	~MainWindow();

private:
	/** Populates the drop-down menus in the GUI */
	void populateMenus();

	/** Returns the index of the currently selected item in the GUI */
	ItemIndex getCurrentSelection() const;

	/** 
	 * Returns all items which convert into the currently handled item. In other words,
	 * this creates the next level of the directed graph.
	 */
	void getDirectConversions(std::vector<ConversionState> &conv_states, std::size_t state_idx) const;

	/** Returns all conversions that convert from a to b */
	std::vector<Conversion> const getPossibleConversions(std::vector<ConversionState> const& conv_states, std::size_t state_idx) const;

	/** Updates the number of required items and actions to yield the number of currently handled items */
	void getItemsNeeded(std::vector<ConversionState> &conv_states, std::size_t state_idx) const;
	
	/** Displays the results in a table */
	void displayResults(std::vector<ConversionState> const& conv_states);

	/** Puts together a string of all intermediate items used in the conversion chain for text output of the results */
	QString const getIntermediateItemsString(std::vector<ConversionState> const& conv_states, std::size_t target_idx) const;

	/** Contains all categories and items */
	std::vector<ItemCategory> _categories;

	/** Contains all possible conversion including ratios */
	std::vector<Conversion> _conversions;

	QIntValidator* _validator;

private slots:
	void on_catBox_currentIndexChanged(int idx);
	void on_startButton_pressed();
	void showInfo() const;
};

#endif //MAINWINDOW_H
