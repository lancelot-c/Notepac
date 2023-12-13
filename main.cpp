#include <QApplication>
#include <QtGui>
#include <iostream>

#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

#include "FenPrincipale.h"
#include "Highlighter.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	
    FenPrincipale fenetre;
    QApplication::instance()->installEventFilter(&fenetre); // C'est cette ligne qui va servir à récupérer les arguments de la fonctin main()

    fenetre.resize(1100, 600);
    fenetre.show(); // showMaximized() Affiche la fenêtre avec la taille maximum
	
    return app.exec();
}
