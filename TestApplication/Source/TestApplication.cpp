#include "TestApplication.h"

TestApplication::TestApplication(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	connect(ui.pbOpenFile, SIGNAL(clicked()), this, SLOT(slot_OpenFileButtonClicked()));
	connect(ui.pbFind, SIGNAL(clicked()), this, SLOT(slot_FindButtonClicked()));
	connect(ui.pbReplace, SIGNAL(clicked()), this, SLOT(slot_ReplaceButtonClicked()));
	connect(ui.pbDelete, SIGNAL(clicked()), this, SLOT(slot_DeleteButtonClicked()));
}

TestApplication::~TestApplication()
{}


//SLOTS
void TestApplication::slot_OpenFileButtonClicked() {

	QString fileName = QFileDialog::getOpenFileName(this, tr("Select Input File"), "C:/", tr("TXT File (*.txt)"));
	if (fileName.isEmpty())
		return;
	QFile file(fileName);
	if (file.open(QIODevice::ReadWrite | QIODevice::Text))
	{
		ui.leInputFile->setText(fileName);
		QTextStream inputStream(&file);
		while (!inputStream.atEnd())
		{
			QString line = inputStream.readLine();
			
			readingList.append(line); // to save all lines

			auto wordList = line.split(" ",QString::KeepEmptyParts);
			for(auto word : wordList)
			{
				totalWordList.append(word); // to keep all words
			}
			ui.sbTotalWordCount->setValue(totalWordList.size());
		}
		file.close();
	}
	else
	{
		QMessageBox::critical(this, tr("File Error"), tr("This file does not exists"));
		return;
	}
}

void TestApplication::slot_FindButtonClicked()
{
	auto searchWord = ui.leInputText->text();
	processedWordCount = 0 ;
	if (checkInputTextIsValid(searchWord)) // check input key is empty 
	{
		QString newWord;

		if (checkSpecialCharacterIsExists(searchWord))
		{
			if (checkAsteriskSignIsExists(searchWord)) // kelime *xxx* seklinde mi kontrolu
			{
				newWord = removeSpecialCharacter(searchWord, '*');
				searchAllWords(newWord);
			}
			else if (checkMinusSignIsExists(searchWord)) // kelime -xx- seklinde mi
			{
				auto searchWordSize = searchWord.size();
				newWord = removeSpecialCharacter(searchWord,'-');

				for (auto word : totalWordList)
				{
					if (searchWordSize == word.size())
					{
						auto currentWord = getStringFromGivenSizedString(word);
						if (currentWord.contains(newWord))
						{
							processedWordCount++; // islenmis tum kelimelerin sayisini tutan flag
						}
						else
							;
					}
				}
			}
		}
		else
		{
			newWord = searchWord;
			searchAllWords(newWord);

		}
		//iki durumda da searchword güncellenecek ve bilgiler doldurulacak

		ui.sbProcessedWordCount->setValue(processedWordCount);
		ui.sbFinalWordCount->setValue(totalWordList.size());
		ui.sbTotalWordCount->setValue(totalWordList.size());
	}
}
void TestApplication::slot_ReplaceButtonClicked()
{ 
	auto searchWord = ui.leReplaceInputText->text();
	auto replaceWord = ui.leReplacedText->text();
	processedWordCount = 0;
	if (checkInputTextIsValid(searchWord))
	{

		QString newWord;
		if (checkAsteriskSignIsExists(searchWord)) // kelime *xxx* seklinde mi kontrolu
		{
			newWord = removeSpecialCharacter(searchWord, asteriskSign);
			searchAllWords(newWord);
		}
		else if (checkMinusSignIsExists(searchWord)) // kelime -xx- seklinde mi
		{
			newWord = removeSpecialCharacter(searchWord, minusSign);

		}
		else
		{
			newWord = searchWord;
		}
		for (auto& line : readingList) // tüm okunan listeden silme islemi gerceklestirilecek
		{
			if (line.contains(newWord))
			{
				line.replace(newWord, replaceWord);
				processedWordCount++;
			}
		}
		saveNewFile();
		//searchword güncellenecek ve bilgiler doldurulacak
		ui.sbProcessedWordCount->setValue(processedWordCount);
		ui.sbFinalWordCount->setValue(totalWordList.size());
		ui.sbTotalWordCount->setValue(totalWordList.size());
	}
}
void TestApplication::slot_DeleteButtonClicked()
{
	auto searchWord = ui.leInputText->text();
	processedWordCount = 0;
	if (checkInputTextIsValid(searchWord))
	{
		QString newWord;
		if (checkAsteriskSignIsExists(searchWord)) // kelime *xxx* seklinde mi kontrolu
		{
			newWord = removeSpecialCharacter(searchWord, asteriskSign);
			searchAllWords(newWord);
		}
		else if (checkMinusSignIsExists(searchWord)) // kelime -xx- seklinde mi
		{
			newWord = removeSpecialCharacter(searchWord, minusSign);

		}
		else
		{
			newWord = searchWord;
		}

		for (auto& line : readingList) // tüm okunan listeden silme islemi gerceklestirilecek
		{
			if (line.contains(newWord))
			{
				line.replace(newWord, "");
				processedWordCount++;
			}
		}
		ui.sbProcessedWordCount->setValue(processedWordCount);
		auto finalVal = totalWordList.size() - processedWordCount;
		ui.sbFinalWordCount->setValue(finalVal);
		ui.sbTotalWordCount->setValue(totalWordList.size());
		saveNewFile();
	}
}

//FUNCTIONS
bool TestApplication::checkSpecialCharacterIsExists(QString inputText)
{
	bool returnVal = false;
	for (int i = 0; i < inputText.size(); i++)
	{
		auto currentChar = inputText.at(i);
		if (currentChar == minusSign)
		{
			returnVal  = returnVal| true;
		}
		else if (currentChar == asteriskSign)
		{
			returnVal = returnVal | true;
		}
		else
			returnVal = returnVal | false;
	}
	return returnVal;
}

bool TestApplication::checkAsteriskSignIsExists(QString inputText)
{
	// kelimenin ilk harfi ile son harfinin * olma durumunun kontrolu
	if (inputText.at(0) == asteriskSign)
	{
		if (inputText.at(inputText.size() - 1) == asteriskSign)
		{
			return true;
		}
	}
	return false;
}
bool TestApplication::checkMinusSignIsExists(QString inputText)
{
	// kelimenin ilk harfi ile son harfinin - olma durumunun kontrolu
	if (inputText.at(0) == minusSign)
	{
		if (inputText.at(inputText.size() - 1) == minusSign)
		{
			return true;
		}
	}
	return false;
}


bool TestApplication::checkInputTextIsValid(QString searchWord)
{
	if (searchWord.isEmpty())
	{
		QMessageBox::warning(this, tr("Empty Key Error"), tr("Searching Key can not be empty."));
		return false;
	}
	return true;
}



QString TestApplication::removeSpecialCharacter(QString inputText, char sign)
{
	// * veya - karakterleri geldiginde onlar silinip aranacak kelime doner
	for (int i = 0; i < inputText.size(); i++)
	{
		if (inputText.at(i) == sign)
		{
			inputText.replace(sign, ""); // gercek kelimeyi elde etmek icin
		}
	}
	return inputText;
}

QString TestApplication::getStringFromGivenSizedString(QString inputText)
{
	// size'i uyumlu olan kelimelerde arama yapilip -aaa- icerisinde bulunmasi icin ilk ve son char cikarilmali
	
	for (int i = 0; i < inputText.size(); i++)
	{
		
	}
	if (inputText.size() > 0)
	{
		inputText = inputText.remove(inputText.at(0));
		auto lastIndex = inputText.length();
		auto a = inputText.at(lastIndex - 1);
		inputText.replace(lastIndex - 1, lastIndex, "");;
	}
	return inputText;
}

void TestApplication::searchAllWords(QString inputText)
{
	for (auto word : totalWordList)
	{
		if (word == inputText || word.contains(inputText)) // direkt ayni kelime var mi ya da kelimeyi iceriyor mu?
		{
			processedWordCount++; // islenmis tum kelimelerin sayisini tutan flag
		}
		else
			;
	}
}

void TestApplication::saveNewFile()
{
	QFile outputFile(ui.leInputFile->text()); // acilmis dosyanin yolunu almak icin kullanildi
	if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&outputFile);
		out.setCodec("UTF-16");
		for (auto line : readingList)
		{
			out << line;
		}
		outputFile.close();
	}
	else;
}
