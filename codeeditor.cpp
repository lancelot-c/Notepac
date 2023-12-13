#include <QtGui>
#include "codeeditor.h"

#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QtDebug>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>


CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
	lineNumberArea = new LineNumberArea(this);
	
	actionAnnuler = new QAction(tr("Annuler"), this);
	actionCopier= new QAction(tr("Copier"), this);
	actionColler = new QAction(tr("Coller"), this);
	actionCouper = new QAction(tr("Couper"), this);
	actionRetablir = new QAction("Rétablir", this);
	actionToutSelectionner = new QAction("Tout sélectionner", this);
	
	actionAnnuler->setDisabled(true);
	actionCopier->setDisabled(true);
	actionColler->setDisabled(true);
	actionCouper->setDisabled(true);
	actionRetablir->setDisabled(true);
	actionToutSelectionner->setDisabled(true);
	
	
	
	
	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(const QRect &, int)), this, SLOT(updateLineNumberArea(const QRect &, int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
	
	connect(this, SIGNAL(undoAvailable(bool)), this, SLOT(canUndo(bool)));
	connect(this, SIGNAL(redoAvailable(bool)), this, SLOT(canRedo(bool)));
	connect(this, SIGNAL(copyAvailable(bool)), this, SLOT(canCopy(bool)));
	
	updateLineNumberAreaWidth(0);
	highlightCurrentLine();
	
	// QKeySequence("Ctrl+E").toString(QKeySequence::NativeText));
}




void CodeEditor::setCompleter(QCompleter *completer)
{
    if (c)
        // QObject::disconnect(c, 0, this, 0);
	
    c = completer;
	
    if (!c)
        return;
	
    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(c, SIGNAL(activated(const QString&)),
                     this, SLOT(insertCompletion(const QString&)));
}



QCompleter *CodeEditor::completer() const
{
    return c;
}



void CodeEditor::insertCompletion(const QString& completion)
{
    if (c->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}



QString CodeEditor::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}



void CodeEditor::focusInEvent(QFocusEvent *e)
{
    if (c)
        c->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}



void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    if (c && c->popup()->isVisible())
	{
        // The following keys are forwarded by the completer to the widget
		
		switch (e->key())
		{
			case Qt::Key_Enter:
			case Qt::Key_Return:
			case Qt::Key_Escape:
			case Qt::Key_Tab:
			case Qt::Key_Backtab:
				e->ignore(); 
				return; // let the completer do default behavior
			default:
				break;
		}
    }
	
    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    
	if (!c || !isShortcut) // dont process the shortcut when we have a completer
        QPlainTextEdit::keyPressEvent(e);
	
	
	
    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!c || (ctrlOrShift && e->text().isEmpty()))
        return;
	
    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();
	
    if (!isShortcut && (hasModifier || e->text().isEmpty() || completionPrefix.length() < 3))
	{
        c->popup()->hide();
        return;
    }
	
    if (completionPrefix != c->completionPrefix())
	{
        c->setCompletionPrefix(completionPrefix);
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }
    
	QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0) + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr); // popup it up!
}

















int CodeEditor::lineNumberAreaWidth()
{
	int digits = 1;
	int max = qMax(1, blockCount());
	while (max >= 10) {
		max /= 10;
		++digits;
	}
	
	int space = 20 + fontMetrics().width(QLatin1Char('9')) * digits; // Sur cette ligne, on peut modifier la taille du bloc qui affiche les numéros de ligne -> "13"
	
	return space;
}



void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}



void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
	if (dy)
		lineNumberArea->scroll(0, dy);
	else
		lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
	
	if (rect.contains(viewport()->rect()))
		updateLineNumberAreaWidth(0);
}



void CodeEditor::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);
	
	QRect cr = contentsRect();
	lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}



void CodeEditor::highlightCurrentLine()
{
	QList<QTextEdit::ExtraSelection> extraSelections;
	
	if (!isReadOnly()) {
		QTextEdit::ExtraSelection selection;
		
		QColor lineColor = QColor(Qt::yellow).lighter(180);
		
		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}
	
	setExtraSelections(extraSelections);
}



void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
	QPainter painter(lineNumberArea);
	painter.fillRect(event->rect(), Qt::lightGray);
	
	
	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int) blockBoundingRect(block).height();
	
	while (block.isValid() && top <= event->rect().bottom())
	{
		if (block.isVisible() && bottom >= event->rect().top())
		{
			QString number = QString::number(blockNumber + 1);
			painter.setPen(Qt::darkGray);
			painter.drawText(-2, top, lineNumberArea->width()-6, fontMetrics().height(), Qt::AlignRight, number); // Position du texte -> "-2"
		}
		
		block = block.next();
		top = bottom;
		bottom = top + (int) blockBoundingRect(block).height();
		++blockNumber;
	}
}


void CodeEditor::canCopy(bool texteSelectionner)
{
	if (texteSelectionner)
	{
		actionCopier->setDisabled(false);
		actionCouper->setDisabled(false);
	}
	else
	{
		actionCopier->setDisabled(true);
		actionCouper->setDisabled(true);
	}
}


void CodeEditor::canRedo(bool redo)
{
	if (redo)
	{
		actionRetablir->setDisabled(false);
	}
	else
	{
		actionRetablir->setDisabled(true);
	}
}


void CodeEditor::canUndo(bool undo)
{
	if (undo)
	{
		actionAnnuler->setDisabled(false);
	}
	else
	{
		actionAnnuler->setDisabled(true);
	}
}


void CodeEditor::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu(this);
	
	
	if (canPaste())
	{
		actionColler->setDisabled(false);
	}
	else
	{
		actionColler->setDisabled(true);
	}
	
	
	if (toPlainText() != "")
	{
		actionToutSelectionner->setDisabled(false);
	}
	else
	{
		actionToutSelectionner->setDisabled(true);
	}
	
	
	QObject::connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(undo()));
	QObject::connect(actionRetablir, SIGNAL(triggered()), this, SLOT(redo()));
	QObject::connect(actionCouper, SIGNAL(triggered()), this, SLOT(cut()));
	QObject::connect(actionCopier, SIGNAL(triggered()), this, SLOT(copy()));
	QObject::connect(actionColler, SIGNAL(triggered()), this, SLOT(paste()));
	QObject::connect(actionToutSelectionner, SIGNAL(triggered()), this, SLOT(selectAll()));
	
	
	menu.addAction(actionAnnuler);
	menu.addAction(actionRetablir);
	menu.addSeparator();
	menu.addAction(actionCouper);
	menu.addAction(actionCopier);
	menu.addAction(actionColler);
	menu.addSeparator();
	menu.addAction(actionToutSelectionner);
	
	
	menu.exec(event->globalPos());
}



