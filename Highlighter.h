/*
 *  Highlighter.h
 *  Notepac
 *
 *  Created by Lancelot on 12/04/09.
 *  Copyright 2009 BSC. All rights reserved.
 *
 */

#ifndef HIGHLIGHTER_H_INCLUDED
#define HIGHLIGHTER_H_INCLUDED
#include "QtGui"

class Highlighter : public QSyntaxHighlighter
	{
		Q_OBJECT
		
	public:
		Highlighter(QTextDocument *parent = 0);
		
	protected:
		void highlightBlock(const QString &text);
		
	private:
		struct HighlightingRule
		{
			QRegExp pattern;
			QTextCharFormat format;
		};
		QVector<HighlightingRule> highlightingRules;
		
		QRegExp commentStartExpression;
		QRegExp commentEndExpression;
		
		
		QTextCharFormat formatAttribut;
		QTextCharFormat formatBalise;
		QTextCharFormat formatBalisesPhp;
		QTextCharFormat formatTexte;
		QTextCharFormat formatValeur;
		
		QTextCharFormat keywordFormat;
		QTextCharFormat classFormat;
		QTextCharFormat singleLineCommentFormat;
		QTextCharFormat multiLineCommentFormat;
		QTextCharFormat quotationFormat;
		QTextCharFormat functionFormat;
	};


#endif // HIGHLIGHTER_H_INCLUDED

