#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class LineNumberArea;
class QCompleter;
class QAbstractItemModel;

class CodeEditor : public QPlainTextEdit
	{
		Q_OBJECT
		
	public:
		CodeEditor(QWidget *parent = 0);
		
		void lineNumberAreaPaintEvent(QPaintEvent *event);
		int lineNumberAreaWidth();
		

		
	protected:
		void resizeEvent(QResizeEvent *event);
		void contextMenuEvent(QContextMenuEvent *event);

		
	private slots:
		void canCopy(bool);
		void canRedo(bool);
		void canUndo(bool);
		void updateLineNumberAreaWidth(int newBlockCount);
		void highlightCurrentLine();
		void updateLineNumberArea(const QRect &, int);
	
		
	private:
		QAction *actionAnnuler;
		QAction *actionRetablir;
		QAction *actionCouper;
		QAction *actionCopier;
		QAction *actionColler;
		QAction *actionToutSelectionner;

		
		QWidget *lineNumberArea;
		
		
		
		
	// ***************   AUTO - COMPLETION   ******************// 
	public:	
		void setCompleter(QCompleter *c);
		QCompleter *completer() const;
		
	protected:
		void keyPressEvent(QKeyEvent *e);
		void focusInEvent(QFocusEvent *e);
		
		private slots:
		void insertCompletion(const QString &completion);
		
	private:
		QString textUnderCursor() const;
		
	private:
		QCompleter *c;
	};








class LineNumberArea : public QWidget
	{
	public:
		LineNumberArea(CodeEditor *editor) : QWidget(editor) {
			codeEditor = editor;
		}
		
		QSize sizeHint() const {
			return QSize(codeEditor->lineNumberAreaWidth(), 0);
		}
		
	protected:
		void paintEvent(QPaintEvent *event) {
			codeEditor->lineNumberAreaPaintEvent(event);
		}
		
	private:
		CodeEditor *codeEditor;
	};

#endif // CODEEDITOR_H
