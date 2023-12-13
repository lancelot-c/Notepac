#include "FenPrincipale.h"
#include "Highlighter.h"





Highlighter::Highlighter(QTextDocument *parent)
: QSyntaxHighlighter(parent)
{
	HighlightingRule rule;
	
	keywordFormat.setForeground(Qt::darkBlue);
	keywordFormat.setFontWeight(QFont::Bold);
	QStringList keywordPatterns;
	//les mots clÈs
	keywordPatterns << "and" << "or" << "AND" << "OR" << "&&" << "xor" << "__FILE__" << "exception" << "__LINE__" << "array" << "as" << "break" << "case" << "class" << "const" << "continue"
	<< "declare" << "default" << "die" << "do" << "echo" << "else" << "elseif" << "empty" << "enddeclare" << "endfor" << "endforeach" << "endif" << "endswitch"
	<< "endwhile" << "eval" << "exit" << "extends" << "for" << "foreach" << "function" << "global" << "if" << "include" << "include_once" << "isset" << "list" << "new"
	<< "print" << "require" << "require_once" << "return" << "static" << "switch" << "unset" << "use" << "var" << "while" << "__FUNCTION__" << "__CLASS__" << "__METHOD__"
	<< "final" << "php_user_filter" << "interface" << "implements" << "extends" << "public" << "private" << "protected" << "abstract" << "clone" << "try" << "catch" << "throw"
	<< "cfunction" << "old_function";
	foreach (const QString &pattern, keywordPatterns) {
		rule.pattern = QRegExp("\\b"+pattern+"\\b");
		rule.format = keywordFormat;
		highlightingRules.append(rule);
	}
	classFormat.setFontWeight(QFont::Bold);
	classFormat.setForeground(Qt::darkMagenta);
	rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
	rule.format = classFormat;
	highlightingRules.append(rule);
	
	
	
	
	
	// Les numéros
	classFormat.setFontWeight(QFont::Bold);
	classFormat.setForeground(QColor(255,140,0));
	rule.pattern = QRegExp("[^h][0-9]");
	rule.format = classFormat;
	highlightingRules.append(rule);
	
	
	// Texte
	formatTexte.setForeground(Qt::black);
	rule.pattern = QRegExp(">.{1,}</");
	rule.format = formatTexte;
	highlightingRules.append(rule);
	
	
	// Balises HTML
	formatBalise.setForeground(QBrush(QColor(26, 0, 247)));
	rule.pattern = QRegExp("<[A-Za-z\\!]{1,}[a-zA-Z0-9=\\s-]*([^>]*)>");
	rule.format = formatBalise;
	highlightingRules.append(rule);
	
	rule.pattern = QRegExp("</[A-Za-z0-9]{1,}>");
	rule.format = formatBalise;
	highlightingRules.append(rule);
	
	
	// Valeurs
	formatValeur.setForeground(Qt::red);
	rule.pattern = QRegExp("\"[^\"]+\"");
	rule.format = formatValeur;
	highlightingRules.append(rule);
	
	rule.pattern = QRegExp("'([^\']*)'");
	rule.format = formatValeur;
	highlightingRules.append(rule);
	
	
	// Attributs
	formatAttribut.setForeground(QBrush(QColor(127, 128, 55)));
	rule.pattern = QRegExp(" [A-Za-z-]{1,}=");
	rule.format = formatAttribut;
	highlightingRules.append(rule);
	
	
	// Balises PHP
	formatBalisesPhp.setForeground(QBrush(QColor(115, 115, 129)));
	rule.pattern = QRegExp("<[^php]php");
	rule.format = formatBalisesPhp;
	highlightingRules.append(rule);
	
	rule.pattern = QRegExp("[^a-zA-Z0-9/\"'_]>");
	rule.format = formatBalisesPhp;
	highlightingRules.append(rule);
	
	
	// Fonctions
	functionFormat.setForeground(Qt::darkBlue);
	rule.pattern = QRegExp("[a-zA-Z0-9_]{1,}([a-zA-Z0-9/\"'_]{1,});");
	rule.format = functionFormat;
	highlightingRules.append(rule);
	
	
	// Commentaires C++
	singleLineCommentFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegExp("[^:]//[^\n]*");
	rule.format = singleLineCommentFormat;
	highlightingRules.append(rule);
	
	
	multiLineCommentFormat.setForeground(Qt::red);
	
	
	// Commentaires PHP
	commentStartExpression = QRegExp("/\\*");
	commentEndExpression = QRegExp("\\*/");
}
void Highlighter::highlightBlock(const QString &text)
{
	foreach (const HighlightingRule &rule, highlightingRules) {
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}
	setCurrentBlockState(0);
	int startIndex = 0;
	if (previousBlockState() != 1)
		startIndex = commentStartExpression.indexIn(text);
	while (startIndex >= 0) {
		int endIndex = commentEndExpression.indexIn(text, startIndex);
		int commentLength;
		if (endIndex == -1) {
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		} else {
			commentLength = endIndex - startIndex
			+ commentEndExpression.matchedLength();
		}
		setFormat(startIndex, commentLength, multiLineCommentFormat);
		startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
	}
}
