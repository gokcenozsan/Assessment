#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TestApplication.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
class TestApplication : public QMainWindow
{
    Q_OBJECT

public:
    TestApplication(QWidget *parent = Q_NULLPTR);
	~TestApplication();
	private slots:
	void slot_OpenFileButtonClicked();
	void slot_FindButtonClicked();
	void slot_ReplaceButtonClicked();
	void slot_DeleteButtonClicked();


private:
    Ui::TestApplicationClass ui;
	QList<QString> readingList; // dosya okunur okunmaz elde edilen list
	QList<QString> totalWordList; // dosyada bulunan her bir kelimenin tutuldugu list
	char asteriskSign = '*';
	char minusSign = '-';

	int processedWordCount = 0;
	int finalWordCount = 0;

	bool checkInputTextIsValid(QString searchWord);
	bool checkSpecialCharacterIsExists(QString inputText);
	bool checkAsteriskSignIsExists(QString inputText);
	bool checkMinusSignIsExists(QString inputText);
	QString removeSpecialCharacter(QString inputText, char sign);
	QString getStringFromGivenSizedString(QString inputText);
	void searchAllWords(QString inputText);
	void saveNewFile();
};
