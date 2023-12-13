#include <iostream>

#include "FenPrincipale.h"
#include "Highlighter.h"
#include "codeeditor.h"




void FenPrincipale::ajouterDocument()
{
        QListWidgetItem *itemNewFile = new QListWidgetItem(QString(" Sans titre ")+QString::number(compteur++), listeFichiers);
	itemNewFile->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/document.png"));
	
	listeFichiers->addItem(itemNewFile);
	
	nouveauChamp = new CodeEditor;
	nouveauChamp->setLineWrapMode(QPlainTextEdit::NoWrap);
	nouveauChamp->setTabStopWidth(20);

        // Changement de la police
        #if defined(Q_WS_WIN)
                nouveauChamp->setFont(QFont("Courier New", 12));
        #elif defined(Q_WS_X11)
                nouveauChamp->setFont(QFont("Monospace", 10));
        #elif defined(Q_WS_MAC)
                nouveauChamp->setFont(QFont("Monaco", 12));
        #endif
	
	curseur = nouveauChamp->textCursor();
	
	highlighter = new Highlighter(nouveauChamp->document());
	
	pageChamp->addWidget(nouveauChamp);
	
	
	listeChamps.addWidget(nouveauChamp);
	listeDocuments.append("0");
	
	
	index = pageChamp->indexOf(nouveauChamp);
	pageChamp->widget(index)->setVisible(true);
	
	for (i = 0; i < pageChamp->count(); i++)
	{
		if (i != index)
		{
			pageChamp->widget(i)->setVisible(false);
		}
	}
	
	listeFichiers->setCurrentItem(itemNewFile, QItemSelectionModel::ClearAndSelect);
	
	// Auto - complétion
    completer->setModel(modelFromFile(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/ressources/wordlist.txt"));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    nouveauChamp->setCompleter(completer);
	
	QObject::connect(nouveauChamp, SIGNAL(textChanged()), this, SLOT(modificationDocument()));
}


QAbstractItemModel *FenPrincipale::modelFromFile(const QString& fileName)
{	
    QFile file(fileName);
	
    if (!file.open(QFile::ReadOnly))
	{
        return new QStringListModel(completer);
	}

	
    QStringList words;
    
    while (!file.atEnd())
	{
        QByteArray line = file.readLine();
        
		if (!line.isEmpty())
		{
            words << line.trimmed();
		}
    }
	
	
    return new QStringListModel(words, completer);
}


void FenPrincipale::annuler()
{
	reinterpret_cast<CodeEditor*>(listeChamps.widget(listeFichiers->currentRow()))->undo();
}


void FenPrincipale::changementDocument()
{	
	listeChamps.setCurrentIndex(listeFichiers->currentRow());
	
	if (listeDocuments[listeFichiers->currentRow()] != "0")
	{
		barreEtat->showMessage(listeDocuments[listeFichiers->currentRow()]);
	}
	else
	{
		barreEtat->showMessage("Prêt");
	}
}


void FenPrincipale::changementPolice()
{
    bool ok = false;
	
    QFont police = QFontDialog::getFont(&ok, QFont("Monaco", 8), this, "Choisissez une police");
	
	if (ok)
	{
		listeChamps.widget(listeFichiers->currentRow())->setFont(police);
	}
}


void FenPrincipale::coller()
{
	reinterpret_cast<CodeEditor*>(listeChamps.widget(listeFichiers->currentRow()))->paste();
}


void FenPrincipale::copier()
{
	reinterpret_cast<CodeEditor*>(listeChamps.widget(listeFichiers->currentRow()))->copy();
}


void FenPrincipale::couper()
{
	reinterpret_cast<CodeEditor*>(listeChamps.widget(listeFichiers->currentRow()))->cut();
}


void FenPrincipale::effacerHistoriqueEditeur()
{
	settings->remove("editeur/fichiers_recents/");
	readSettingsEditeur();
}


int FenPrincipale::Enregistrer()
{
	if (listeDocuments[listeFichiers->currentRow()] == "0")
	{
		EnregistrerSous();
	}
	else
	{
		QString CheminFichierAEnregistrer = listeDocuments[listeFichiers->currentRow()];
		QFile fichier(CheminFichierAEnregistrer);
		
		if(fichier.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			fichier.write(reinterpret_cast<CodeEditor*>(listeChamps.widget(listeFichiers->currentRow()))->toPlainText().toAscii());
			fichier.close();
			
			return 1;
		}
		else
		{
			return 0;
		}
	}
	
	barreEtat->showMessage("Fichier enregistré avec succès");
}

int FenPrincipale::EnregistrerSous()
{
	QString CheminFichierAEnregistrer = QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QDir::homePath(), "Tous types (*.*)");
	QFile fichier(CheminFichierAEnregistrer);
	
	if(fichier.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		fichier.write(reinterpret_cast<CodeEditor*>(listeChamps.widget(listeFichiers->currentRow()))->toPlainText().toAscii());
		fichier.close();
		
		
		listeDocuments[listeFichiers->currentRow()] = CheminFichierAEnregistrer;
		listeFichiers->currentItem()->setText(CheminFichierAEnregistrer.section('/', -1)); // Changement du texte de l'item
		listeFichiers->currentItem()->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/document.png"));		
		
		barreEtat->showMessage("Fichier enregistré avec succès");
		return 1;
	}
	else
	{
		return 0;
	}
}

void FenPrincipale::EnregistrerTout()				// Cette fonction bug
{													// Cette fonction bug
	for (i = 0; i < listeFichiers->count(); i++)	// Cette fonction bug
	{												// Cette fonction bug
		listeFichiers->setCurrentRow(i);			// Cette fonction bug
		Enregistrer();								// Cette fonction bug
	}												// Cette fonction bug
}	


void FenPrincipale::fermerDocument()
{
	QList<QListWidgetItem *> selectedFiles = listeFichiers->selectedItems();
	
	if(selectedFiles.size() >= 1)
	{
		if (listeFichiers->count() != 1)
		{
			foreach(QListWidgetItem *i, selectedFiles)
			{
				listeFichiers->setCurrentRow(listeFichiers->row(i));
				
				if (DocumentsASauvegarder.contains(listeFichiers->currentRow()))
				{
					int reponse = QMessageBox::information(this, "", "<b>\"" + listeFichiers->currentItem()->text() + "\" n'a pas été enregistré.</b><br />Voullez-vous enregistrer les modifications ?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			
					if (reponse == QMessageBox::Yes)
					{
						Enregistrer();
						// Les opérations de fermeture se feront �  la fin de la fonction
					}
					else if (reponse == QMessageBox::No)
					{
						// Les opérations de fermeture se feront �  la fin de la fonction
					}
					else if (reponse == QMessageBox::Cancel)
					{
						return;
					}
		
					DocumentsASauvegarder.removeOne(listeFichiers->currentRow());				// Suppression du champ dans la liste de sauvegarde
				}
	
				listeChamps.removeItem(listeChamps.itemAt(listeFichiers->currentRow()));		// Mise �  jour de la liste des champs
				listeFichiers->removeItemWidget(listeFichiers->currentItem());					// Suppression de l'item
				delete i;	// Supression de l'item dans le QListWidget
			}
		}
	}
}


void FenPrincipale::modificationDocument()
{
	QPlainTextEdit *champModifier = qobject_cast<QPlainTextEdit *>(sender());
	
	if (champModifier->toPlainText() != "")
	{
		// listeFichiers->currentItem()->setBackground(QBrush(QColor(255, 222, 222)));
		
		listeFichiers->currentItem()->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/document-modifier.png"));
		DocumentsASauvegarder.append(listeFichiers->currentRow());
	}
}


void FenPrincipale::OuvrirFichier()
{	
	QString FichierAOuvrir = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QDir::homePath(), "Fichiers (*.html *.css *.php *.js *.htm *.asp *.txt *.rtf *.cpp *.c *.hpp *.h *.pro *.exe *.app)");
    QFile file(FichierAOuvrir);
	
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return;
	}
	
	if (!listeDocuments.contains(FichierAOuvrir))
	{
		QTextStream in(&file); // Un petit QTextStream pour pouvoir récupérer le contenu du fichier ouvert
		
		ajouterDocument();
		
		while(!in.atEnd()) // Une boucle : tant qu'on n'est pas �  la fin du fichier ...
		{
			reinterpret_cast<CodeEditor*>(listeChamps.widget(listeFichiers->currentRow()))->insertPlainText(in.readLine() + "\n"); // ... on insert dans la zone de texte une ligne plus \n (qui fait un retour �  la ligne)
		}
		
		listeFichiers->currentItem()->setText(file.fileName().section('/', -1)); // Changement du texte de l'item
		listeFichiers->currentItem()->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/document.png"));
		
		
		listeDocuments[listeFichiers->currentRow()] = FichierAOuvrir;
		
		barreEtat->showMessage(FichierAOuvrir);
		
		onglets->setCurrentIndex(0); // On se place ensuite dans l'onglet de l'éditeur
		
		
		
		// Enregistrement du fichier dans la liste des fichiers recemment ouverts
		settings->beginGroup("editeur/fichiers_recents");
		QStringList cles = settings->childGroups();
		settings->endGroup();
		
		m_chaine = FichierAOuvrir;
		
		settings->setValue("editeur/fichiers_recents/" + m_chaine.remove(QChar('/'), Qt::CaseInsensitive) + "/nom_abrege", file.fileName().section('/', -1));
		settings->setValue("editeur/fichiers_recents/" + m_chaine.remove(QChar('/'), Qt::CaseInsensitive) + "/nom_complet", FichierAOuvrir);
		
		readSettingsEditeur(); // On met �  jour la liste des fichiers recemment ouverts
	}
	else
	{
		QMessageBox::information(this, "Fichier ouvert", "Le fichier : \"" + FichierAOuvrir.section('/', -1) + "\" est déj�  ouvert !");
	}
}


void FenPrincipale::ouvrirFichierRecent()
{
	m_action = qobject_cast<QAction *>(sender());
	
	nouveauFichier(m_action->whatsThis());
}


void FenPrincipale::readSettingsEditeur()
{
	fichiersRecents->clear(); // D'abord, on supprime toutes les actions du menu pour le mettre �  jour
	
	settings->beginGroup("editeur/fichiers_recents");
	QStringList cles = settings->childGroups();
	settings->endGroup();
	
	
	for (i = 0; i <= cles.size()-1; i++) // Ensuite, on met les actions une par une
	{
		if (i <= 9) // On pourra mettre au maximum 10(9+1) fichiers dans le menu "Fichiers récents"
		{
			actionFichierRecent = new QAction(settings->value("editeur/fichiers_recents/" + cles[i] + "/nom_abrege").toString(), this); // On crée l'action
			actionFichierRecent->setStatusTip(settings->value("editeur/fichiers_recents/" + cles[i] + "/nom_complet").toString());		// On lui donne le chemin entier du fichier comme infobulle de barre d'état
			actionFichierRecent->setWhatsThis(settings->value("editeur/fichiers_recents/" + cles[i] + "/nom_complet").toString());		// La fonction setWhatThis() va nous servir ici d'une BDD miniature pour enregistrer des infos sur le fichier (ici, le chemin complet du fichier)
			actionFichierRecent->setIcon(QIcon("images/fichiers/" + settings->value("editeur/fichiers_recents/" + cles[i] + "/nom_abrege").toString().section('.', 1) + ".png")); // On met l'image correspondant �  l'extension du fichier
			
			fichiersRecents->addAction(actionFichierRecent);
			
			QObject::connect(actionFichierRecent, SIGNAL(triggered()), this, SLOT(ouvrirFichierRecent()));
		}
	}
	
	
	if (cles.size() == 0) // Si il n'y pas de fichier enregistré... 
	{
		actionFichierRecent = new QAction("Aucun fichier n'a été ouvert récemment", this);
		actionFichierRecent->setDisabled(true);
		fichiersRecents->addAction(actionFichierRecent);
		
		actionEffacerHistoriqueEditeur->setDisabled(true);
	}
	else
	{
		actionFichierRecent->setDisabled(false);
		actionEffacerHistoriqueEditeur->setDisabled(false);
	}
	
	fichiersRecents->addSeparator();
	fichiersRecents->addAction(actionEffacerHistoriqueEditeur);
}


void FenPrincipale::retablir()
{
	reinterpret_cast<CodeEditor*>(listeChamps.widget(listeFichiers->currentRow()))->redo();
}


void FenPrincipale::toutSelectionner()
{
	reinterpret_cast<CodeEditor*>(listeChamps.widget(listeFichiers->currentRow()))->selectAll();
}


