
#include "InfoDialog.h"
#include <QTextStream>

InfoDialog::InfoDialog(QDialog* parent) 
: QDialog(parent)
{
	setupUi(this);
	this->infoTextBrowser->setOpenExternalLinks(true);
	this->setText();
}

void InfoDialog::setText()
{
	QString text;
	QTextStream stream(&text);
	stream << "<p>This a small programme based on mechanics in the browser game "
		<< "<a href=\"http://fallenlondon.storynexus.com\">Fallen London</a>. "
		<< "It calculates the amount of input items needed for conversion "
		<< "into a selected item type. All possible chains of conversion are "
		<< "found and displayed, although a lot of these might not actually be practical "
		<< "choices in the game.</p>"
		<< "<p>Two modes of calculation are available: The first mode minimises the "
		<< "number of input items needed to get the selected number of output items. "
		<< "The second mode minimises the actions needed for the conversion. This is probably "
		<< "only interesting for players with advanced characters who can easily obtain "
		<< "large quantaties of basic items. A simple example: If you need 130 <em>Romantic "
		<< "Notions</em>, the first mode will calculate that you need 650 <em>Drops of Prisoner's "
		<< "Honey</em> and will use up 4 actions for conversion. The second mode will calculate that "
		<< "you need 1000 <em>Drops</em> and convert these in 2 actions.</p>"
		<< "<p><a href=\"http://fallenlondon.storynexus.com\">Fallen London</a> is "
		<< "a game by <a href=\"http://www.failbettergames.com/\">FailBetterGames</a>. "
		<< "All the wonderfully bizarre names for items appearing here are "
		<< "their intellectual property.</p>"
		<< "<p>For discussions on the game check out the "
		<< "<a href=\"http://community.failbettergames.com/\">official forums</a> or "
		<< "the <a href=\"https://www.reddit.com/r/fallenlondon/\">Fallen London subreddit</a>.</p>";

	this->infoTextBrowser->setHtml(*(stream.string()));
}

void InfoDialog::on_okPushButton_pressed()
{
	this->done(QDialog::Accepted);
}

