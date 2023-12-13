#include <iostream>

#include "FenPrincipale.h"
#include "ftp.h"
#include "Highlighter.h"





void FenPrincipale::avancementTransfert(qint64 done, qint64 total)
{
	row = 0;
	
	for (i = 0; i < listeIdFtp.size(); i++)
	{
		if (ftp->currentId() == listeIdFtp[i])
		{
			row = i;
		}
	}

	
	if (currentCommand == "put")
	{
		m_barre = new ProgressDelegate;
		telechargementsFtpViewer->setItemDelegateForColumn(4, m_barre);
		
		telechargementsFtpModel->setData(telechargementsFtpModel->item(row, 4)->index(), (done*100) / total);	
		
		telechargementsFtpModel->item(row, 5)->setText("Upload en cours...");
	}
	else if (currentCommand == "get")
	{
		m_barre = new ProgressDelegate;
		telechargementsFtpViewer->setItemDelegateForColumn(4, m_barre);
		
		telechargementsFtpModel->setData(telechargementsFtpModel->item(row, 4)->index(), (done*100) / total);	
		
		telechargementsFtpModel->item(row, 5)->setText("Téléchargement en cours...");
	}
}


void FenPrincipale::actionFtp()
{		
	if (isConnected)
	{
		deconnexionFtp();
	}
	else
	{
		connexionFtp(hote->text(), identifiant->text(), mdp->text(), port->value());
	}
}


void FenPrincipale::ajouterHistoriqueFichierFtp(QString nom, QString provenance, QString destination, QString taille, QString statut)
{
	itemName = new QStandardItem();
	itemName->setText(nom);
	itemName->setTextAlignment(Qt::AlignLeft);
	itemName->setEditable(false);
	
	itemProvenance = new QStandardItem();
	itemProvenance->setText(provenance);
	itemProvenance->setEditable(false);
	
	itemDestination = new QStandardItem();
	itemDestination->setText(destination);
	itemDestination->setEditable(false);
	
	itemSize = new QStandardItem();
	itemSize->setText(taille);
	itemSize->setTextAlignment(Qt::AlignRight);
	itemSize->setEditable(false);
	
	itemAvancement = new QStandardItem();
	itemAvancement->setText(statut);
	itemAvancement->setTextAlignment(Qt::AlignLeft);
	itemAvancement->setEditable(false);
	
	itemStatut = new QStandardItem();
	itemStatut->setText("En attente...");
	itemStatut->setTextAlignment(Qt::AlignLeft);
	itemStatut->setEditable(false);
	
	
	QList<QStandardItem *> itemsFichier;
	itemsFichier << itemName <<  itemProvenance << itemDestination << itemSize << itemAvancement << itemStatut;
	telechargementsFtpModel->appendRow(itemsFichier);
	
    telechargementsFtpViewer->resizeColumnToContents(0);
	telechargementsFtpViewer->setAllColumnsShowFocus(true);	
}


void FenPrincipale::ajouterHistoriqueFtp(QString texte)
{
	champHistorique->setHtml(champHistorique->toHtml() + "<span style=\"font-size:small;\">" + texte + "</span>");
}


void FenPrincipale::brancheOrdinateur()
{
	QStringList entetesLocal;
	entetesLocal << "Nom du fichier" << "Taille" << "Type" << "Dernière modification";
	localModel->setHorizontalHeaderLabels(entetesLocal);
	
	
	itemOrdinateur = new QStandardItem();
	itemOrdinateur->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/ordinateur.png"));
	itemOrdinateur->setText(nomOrdinateur);
	itemOrdinateur->setEditable(true);
	
	QList<QStandardItem *> itemsOrdinateur;
	itemsOrdinateur << itemOrdinateur;
	localModel->appendRow(itemsOrdinateur);
}


void FenPrincipale::brancheSite()
{
	QStringList entetesFtp;
	entetesFtp << "Nom du fichier" << "Taille" << "Type" << "Dernière modification";
	ftpModel->setHorizontalHeaderLabels(entetesFtp);
	
	itemSite = new QStandardItem();
	itemSite->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/site.png"));
	itemSite->setText(nomSite);
	itemSite->setEditable(true);
	
	itemSiteSize = new QStandardItem();
	itemSiteSize->setText(QString::number(taille));
	itemSiteSize->setTextAlignment(Qt::AlignRight);
	itemSiteSize->setEditable(false);
	
	itemSiteFiles = new QStandardItem();
	itemSiteFiles->setText(QString::number(fichiers) + " fichiers");
	itemSiteFiles->setTextAlignment(Qt::AlignLeft);
	itemSiteFiles->setEditable(false);
	
	
	QList<QStandardItem *> itemsSite;
	itemsSite << itemSite <<  itemSiteSize << itemSiteFiles;
	ftpModel->appendRow(itemsSite);
}


void FenPrincipale::champFtpReturnPressed()
{
	if (isConnected)
	{
		if (champCheminFtp->text().isEmpty()) // Si il est vide, on rajoute un slash
		{
			champCheminFtp->setText("/");
			champFtpReturnPressed();
			return;
		}
		
		
		QDir *dossier = new QDir(champCheminFtp->text());
		
		if (dossier->exists())
		{
			arborescenceFtp = champCheminFtp->text();
			listageFichiersFtp(champCheminFtp->text());
		}
		else
		{
			QMessageBox::critical(this, "Erreur", "Le dossier n'existe pas.");
			
			champCheminFtp->setText(arborescenceFtp);
		}
	}
	else
	{
		champCheminFtp->setText("");
		QMessageBox::critical(this, "Erreur", "Vous devez être connecté pour changer l'arborescence.");
	}
}


void FenPrincipale::champLocalReturnPressed()
{
	if (champCheminLocal->text().isEmpty()) // Si il est vide, on rajoute un slash
	{
		champCheminLocal->setText("/");
		champLocalReturnPressed();
		return;
	}
	
	
	QDir *dossier = new QDir(champCheminLocal->text());
	
	if (dossier->exists())
	{
		arborescenceLocal = champCheminLocal->text();
		listageFichiersLocal(champCheminLocal->text());
	}
	else
	{
		QMessageBox::critical(this, "Erreur", "Le dossier n'existe pas.");
		
		champCheminLocal->setText(arborescenceLocal);
	}
}


void FenPrincipale::connexionFtp(QString host, QString login, QString password, quint16 portHost)
{	
	// Mise à jour des champs
	hote->setText(host);
	identifiant->setText(login);
	mdp->setText(password);
	port->setValue(portHost);
	
	if (!isConnected)
	{		
		ftp = new QFtp(this);

		// On se connecte au site
		ftp->connectToHost(host, portHost);
		ftp->login(login, password);
	}
	else
	{
		int reponse = QMessageBox::question(this, "Déjà connecté", "Vous êtes déjà connecté.<br />Souhaitez vous couper la connexion courante ?", QMessageBox::No | QMessageBox::Yes);
		
		if (reponse == QMessageBox::Yes)
		{
			deconnexionFtp();
			
			ftp = new QFtp(this);

			ftp->connectToHost(host, portHost);
			ftp->login(login, password);
		}
		else
		{
			return;
		}
	}
	

	QObject::connect(ftp, SIGNAL(stateChanged(int)), this, SLOT(statutFtp(int)));
    QObject::connect(ftp, SIGNAL(done(bool)), this, SLOT(ftpDone(bool)));
    QObject::connect(ftp, SIGNAL(listInfo(const QUrlInfo &)), this, SLOT(ftpListInfo(const QUrlInfo &)));
	
	QObject::connect(ftp, SIGNAL(commandStarted(int)), this, SLOT(debutTransfert(int)));
	QObject::connect(ftp, SIGNAL(dataTransferProgress(qint64, qint64)), this, SLOT(avancementTransfert(qint64, qint64)));
	QObject::connect(ftp, SIGNAL(commandFinished(int, bool)), this, SLOT(finTransfert(int, bool)));
}

void FenPrincipale::connexionPreference1Ftp()
{	
	QString cleHote = actionPreferenceFtp1->text();
	connexionFtp(settings->value("FTP/identifiants/" + cleHote + "/hote").toString(), settings->value("FTP/identifiants/" + cleHote + "/login").toString(), settings->value("FTP/identifiants/" + cleHote + "/motdepasse").toString(), settings->value("FTP/identifiants/" + cleHote + "/port").value<quint16>());
}

void FenPrincipale::connexionPreference2Ftp()
{	
	QString cleHote = actionPreferenceFtp2->text();
	connexionFtp(settings->value("FTP/identifiants/" + cleHote + "/hote").toString(), settings->value("FTP/identifiants/" + cleHote + "/login").toString(), settings->value("FTP/identifiants/" + cleHote + "/motdepasse").toString(), settings->value("FTP/identifiants/" + cleHote + "/port").value<quint16>());
}

void FenPrincipale::connexionPreference3Ftp()
{	
	QString cleHote = actionPreferenceFtp3->text();
	connexionFtp(settings->value("FTP/identifiants/" + cleHote + "/hote").toString(), settings->value("FTP/identifiants/" + cleHote + "/login").toString(), settings->value("FTP/identifiants/" + cleHote + "/motdepasse").toString(), settings->value("FTP/identifiants/" + cleHote + "/port").value<quint16>());
}


void FenPrincipale::clearCommands()
{	
	ftp->abort();
	telechargementsFtpModel->clear();
}


void FenPrincipale::creerDossierLocal()
{
	QModelIndex index = directoryViewer->currentIndex().sibling(directoryViewer->currentIndex().row(), 0);
	QStandardItem *item = localModel->itemFromIndex(index);
	
	if (item)
	{
		bool ok = false;
		QString nouveauDossier = QInputDialog::getText(this, "Créer un dossier", "Nouveau dossier : ", QLineEdit::Normal, QString(), &ok, Qt::Sheet);
		
		if (ok && !nouveauDossier.isEmpty())
		{
			arborescenceLocal = champCheminLocal->text();
			
			if (QDir(arborescenceLocal + "/" + nouveauDossier).exists())
			{
				QMessageBox::information(this, "Créer un dossier", "Le dossier existe déjà !");
			}
			
			QDir(arborescenceLocal).mkdir(nouveauDossier);
			listageFichiersLocal(arborescenceLocal);
		}
	}
}

// *************************** D *************************** //

void FenPrincipale::debutTransfert(int id)
{	
	if (ftp->currentCommand() == QFtp::Put)
	{
		nombreTransfert++;
		currentTransfert = id;
		currentCommand = "put";
		
		if (nombreTransfert >= 1)
		{
			actionClearCommandsFtp->setDisabled(false);
		}
		else
		{
			actionClearCommandsFtp->setDisabled(true);
		}
		
		listeIdFtp.append(id);
	}
	else if (ftp->currentCommand() == QFtp::Get)
	{
		nombreTransfert++;
		currentTransfert = id;
		currentCommand = "get";
		
		if (nombreTransfert >= 1)
		{
			actionClearCommandsFtp->setDisabled(false);
		}
		else
		{
			actionClearCommandsFtp->setDisabled(true);
		}
		
		listeIdFtp.append(id);
	}
}


void FenPrincipale::deconnexionFtp()
{		
	champCheminFtp->setText("");
	ftpModel->clear();
	ftp->abort();
	ftp->close();
}

void FenPrincipale::dossierPrecedentFtp()
{
	itemName = new QStandardItem();
	itemName->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/fichier_precedent_ftp.png"));
	itemName->setText(nomDossierPrecedentFtp);
	itemName->setEditable(true);
	
	itemSize = new QStandardItem();
	itemSize->setText("0");
	itemSize->setTextAlignment(Qt::AlignRight);
	itemSize->setEditable(false);
	
	itemType = new QStandardItem();
	itemType->setText("Dossier");
	itemType->setEditable(false);
	
	QList<QStandardItem *> itemsDossierPrecedentFtp;
	itemsDossierPrecedentFtp << itemName << itemSize << itemType;
	itemSite->appendRow(itemsDossierPrecedentFtp);
}


void FenPrincipale::dossierPrecedentLocal()
{
	itemName = new QStandardItem();
	itemName->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/fichier_precedent_ftp.png"));
	itemName->setText(nomDossierPrecedentLocal);
	itemName->setEditable(true);
	
	itemSize = new QStandardItem();
	itemSize->setText("0");
	itemSize->setTextAlignment(Qt::AlignRight);
	itemSize->setEditable(false);
	
	itemType = new QStandardItem();
	itemType->setText("Dossier");
	itemType->setEditable(false);
	
	QList<QStandardItem *> itemsDossierPrecedentLocal;
	itemsDossierPrecedentLocal << itemName << itemSize << itemType;
	itemOrdinateur->appendRow(itemsDossierPrecedentLocal);
}


void FenPrincipale::effacerHistoriqueFtp()
{
	settings->remove("FTP/identifiants");
	readSettingsFtp();
}


void FenPrincipale::envoyerFichier(const QModelIndex & index)
{
	if (isConnected)
	{
		arborescenceLocal = champCheminLocal->text();
		
		QString chemin = arborescenceLocal + "/" + localModel->itemFromIndex(index)->text();
		fileToPut = new QFile(chemin); // Je récupère le chemin du fichier sur lequel j'ai cliqué
	
		if (fileToPut->open(QIODevice::ReadOnly)) // Je vérifie si je peux l'ouvrir
		{
			ajouterHistoriqueFichierFtp(localModel->itemFromIndex(index)->text(), arborescenceLocal.section('/', -1), arborescenceFtp.section('/', -1), QString::number(fileToPut->size()), "0/" + QString::number(fileToPut->size())); // On ajoute le fichier à la liste des fichiers envoyés
			
			ftp->put(fileToPut, arborescenceFtp + "/" + localModel->itemFromIndex(index)->text()); // J'envoie le fichier
		}
		else // Si je ne peux pas l'ouvrir, c'est qu'il s'est produit une erreur
		{
			QMessageBox::warning(this, "Erreur", "Ouverture du fichier impossible :<br />" + chemin);
			delete fileToPut;
			
			// L'erreur peut peut-être venir d'un fichier qui n'existe plus, on rafraichit donc le local et le serveur pour mettre à jour les fichiers
			listageFichiersFtp(arborescenceFtp);		// On rafraichit les fichiers du site
			listageFichiersLocal(arborescenceLocal);	// On rafraichit les fichiers de l'ordinateur
			
			return;
		}
	}
}


void FenPrincipale::finTransfert(int id, bool erreur)
{	
	if (ftp->currentCommand() == QFtp::Login && erreur)
	{
		ajouterHistoriqueFtp("<span style=\"color:red;\">Identifiants incorrects. Déconnexion</span>");
		deconnexionFtp();
		return;
	}

	
	if (ftp->currentCommand() == QFtp::Get)
	{
		fileToGet->close();
		delete fileToGet;
	}
	
	if (erreur)
	{
		// On affiche pas l'erreur, c'est la fonction "finTransfert()" qui s'en charge
		
		for (i = 0; i < listeIdFtp.size(); i++)
		{
			if (id == listeIdFtp[i])
			{
				if (ftp->currentCommand() == QFtp::Put)
				{
					telechargementsFtpModel->item(row, 5)->setText("Upload échoué");
				}
				else if (ftp->currentCommand() == QFtp::Get)
				{
					telechargementsFtpModel->item(row, 5)->setText("Téléchargement échoué");
				}
			}
		}
	}
	else
	{
		for (i = 0; i < listeIdFtp.size(); i++)
		{
			if (id == listeIdFtp[i])
			{
				listageFichiersFtp(arborescenceFtp);		// On rafraichit les fichiers du site
				listageFichiersLocal(arborescenceLocal);	// On rafraichit les fichiers de l'ordinateur
				
				
				if (ftp->currentCommand() == QFtp::Put)
				{
					telechargementsFtpModel->item(row, 5)->setText("Upload réussi");
					barreEtat->showMessage("Fichier envoyé avec succés", 2000);
				}
				else if (ftp->currentCommand() == QFtp::Get)
				{
					telechargementsFtpModel->item(row, 5)->setText("Téléchargement réussi");
					barreEtat->showMessage("Fichier reçu avec succés", 2000);
				}
			}
		}
	}

	nombreTransfert--;
	
	if (nombreTransfert >= 1)
	{
		actionClearCommandsFtp->setDisabled(false);
	}
	else
	{
		actionClearCommandsFtp->setDisabled(true);
	}
	
}


void FenPrincipale::ftpDone(bool erreur)
{
	if (erreur)
	{
		if (ftp->error() == QFtp::NoError)
		{
			ajouterHistoriqueFtp("<span style=\"color:red;\">Aucune erreur ne s'est produite !</span>");
		}
		else if (ftp->error() == QFtp::HostNotFound)
		{
			ajouterHistoriqueFtp("<span style=\"color:red;\">Le serveur est introuvable.</span>");
		}
		else if (ftp->error() == QFtp::ConnectionRefused)
		{
			ajouterHistoriqueFtp("<span style=\"color:red;\">La connexion a été refusée par le serveur.</span>");
		}
		else if (ftp->error() == QFtp::NotConnected)
		{
			ajouterHistoriqueFtp("<span style=\"color:red;\">Vous ne pouvez pas lancer de commande car vous n'êtes pas connecté.</span>");
		}
		else if (ftp->error() == QFtp::UnknownError)
		{
			statutRequeteFtp = "erreur inconnue";
			
			ajouterHistoriqueFtp("<span style=\"color:red;\">Une erreur s'est produite : " + ftp->errorString() + "</span>");
		}
	}
	else
	{
		qDeleteAll(openedFiles);
		openedFiles.clear();
		
		ftpViewer->resizeColumnToContents(0);
		ftpViewer->setAllColumnsShowFocus(true);
		ftpViewer->sortByColumn(2, Qt::AscendingOrder);
	}
}



void FenPrincipale::ftpListInfo(const QUrlInfo &urlInfo)
{	
	if (urlInfo.isDir())
	{
		itemDossier = new QStandardItem();
		itemDossier->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/folder.png"));
		itemDossier->setText(urlInfo.name());
		itemDossier->setEditable(true);
		
		listeNomFichiersFtp.append(urlInfo.name());
		
		
		itemSize = new QStandardItem;
		itemSize->setText(QString::number(urlInfo.size()));
		itemSize->setTextAlignment(Qt::AlignRight);
		itemSize->setEditable(false);
		
		listeTailleFichiersFtp.append(QString::number(urlInfo.size()));
		
		
		itemType = new QStandardItem;
		itemType->setText("Dossier");
		itemType->setEditable(false);
		
		itemLastModified = new QStandardItem;
		itemLastModified->setText(urlInfo.lastModified().toString("Le dd.MM.yyyy à hh:mm"));
		itemLastModified->setEditable(false);
		
		
		QList<QStandardItem *> itemsDossier;
		itemsDossier << itemDossier << itemSize << itemType << itemLastModified;
		
		listeDossiersFtp.append(itemDossier->text());
		
		itemSite->appendRow(itemsDossier);
		
		pendingDirs.append("/" + currentDir + "/" + urlInfo.name());
		taille += urlInfo.size();
	}
	
	if (urlInfo.isFile() && urlInfo.isReadable())
	{
		itemName = new QStandardItem;
		itemName->setText(urlInfo.name());
		itemName->setEditable(true);
		
		listeNomFichiersFtp.append(urlInfo.name());
		
		
		itemSize = new QStandardItem;
		itemSize->setText(QString::number(urlInfo.size()));
		itemSize->setTextAlignment(Qt::AlignRight);
		itemSize->setEditable(false);
		
		listeTailleFichiersFtp.append(QString::number(urlInfo.size()));
		
		
		itemType = new QStandardItem;
		itemType->setEditable(false);
		
		if (urlInfo.name().endsWith(".png") || urlInfo.name().endsWith(".gif") || urlInfo.name().endsWith(".bmp") || urlInfo.name().endsWith(".svg") || urlInfo.name().endsWith(".tiff") || urlInfo.name().endsWith(".jpeg") || urlInfo.name().endsWith(".jpg") || urlInfo.name().endsWith(".JPG") || urlInfo.name().endsWith(".PNG"))
		{
			itemName->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/image.png"));
			itemType->setText("Image");
		}
		else if (urlInfo.name().endsWith(".mp3") || urlInfo.name().endsWith(".wav") || urlInfo.name().endsWith(".wma") || urlInfo.name().endsWith(".ogg") || urlInfo.name().endsWith(".mid"))
		{
			itemName->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/musique.png"));
			itemType->setText(tr("Musique"));
		}
		else if (urlInfo.name().endsWith(".avi") || urlInfo.name().endsWith(".MP4"))
		{
			itemName->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/video.png"));
			itemType->setText("Vidéo");
		}
		else if (urlInfo.name().endsWith(".html") || urlInfo.name().endsWith(".htm") || urlInfo.name().endsWith(".css") || urlInfo.name().endsWith(".php") || urlInfo.name().endsWith(".js")  || urlInfo.name().endsWith(".asp") || urlInfo.name().endsWith(".do"))
		{
			if (urlInfo.name().endsWith(".css"))
			{
				itemName->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/fichier-css.png"));
			}
			else if (urlInfo.name().endsWith(".js"))
			{
				itemName->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/fichier-js.png"));
			}
			else
			{
				itemName->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/document-web.png"));
			}
			
			itemType->setText(tr("Fichier web"));
		}
		else if (urlInfo.name().endsWith(".txt") || urlInfo.name().endsWith(".rtf") || urlInfo.name().endsWith(".doc") || urlInfo.name().endsWith(".docx") || urlInfo.name().endsWith(".pro"))
		{
			itemName->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/fichier-texte.png"));
			itemType->setText(tr("Fichier texte"));
		}
		else if (urlInfo.name().endsWith(".c") || urlInfo.name().endsWith(".cpp") || urlInfo.name().endsWith(".h") || urlInfo.name().endsWith(".hpp"))
		{
			if (urlInfo.name().endsWith(".c"))
			{
				itemName->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/fichier-c.png"));
			}
			else if (urlInfo.name().endsWith(".cpp"))
			{
				itemName->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/fichier-cpp.png"));
			}
			else
			{
				itemName->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/fichier-h.png"));
			}
			
			itemType->setText(tr("Fichier texte"));
		}
		else if (urlInfo.name().endsWith(".app"))
		{
			itemName->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/application.png"));
			itemType->setText(tr("Application"));
		}
		else if (urlInfo.name().endsWith(".exe"))
		{
			itemName->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/executable.png"));
			itemType->setText("Programme");
		}
		else
		{
			itemName->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/fichier-texte.png"));
			itemType->setText("Fichier texte");
		}
		
		itemLastModified = new QStandardItem;
		itemLastModified->setText(urlInfo.lastModified().toString("Le dd.MM.yyyy à hh:mm"));
		itemLastModified->setEditable(false);
		
		QList<QStandardItem *> itemsFile;
		itemsFile << itemName << itemSize << itemType << itemLastModified;
		itemSite->appendRow(itemsFile);
		
		taille += urlInfo.size();
		fichiers++;
	}
	
	i++;
	itemSiteSize->setText(QString::number(taille) + " octets");
	itemSiteFiles->setText(QString::number(fichiers) + " fichiers");
}


bool FenPrincipale::isFtpDir(const QModelIndex &index)
{
	if (listeDossiersFtp.contains(ftpModel->itemFromIndex(index)->text())) // Si la liste des dossiers du site contient cet item, alors c'est un dossier
	{	
		return true;
	}
	else
	{
		return false;
	}
}


bool FenPrincipale::isLocalDir(const QModelIndex &index)
{
	if (listeDossiersLocal.contains(localModel->itemFromIndex(index)->text())) // Si la liste contient cet item, alors c'est un dossier
	{	
		return true;
	}
	else
	{
		return false;
	}
}


void FenPrincipale::itemOrdinateurChanged(QStandardItem *item)
{
	nomOrdinateur = itemOrdinateur->text();
	settings->setValue("FTP/local/parametres/nomOrdinateur", nomOrdinateur);
}


void FenPrincipale::itemSiteChanged(QStandardItem *item)
{
	nomSite = itemSite->text();
	settings->setValue("FTP/local/parametres/nomSite", nomSite);
}


void FenPrincipale::listageFichiersLocal(QString chemin)
{
	localModel->clear();
	listeDossiersLocal.clear();
	
	
	brancheOrdinateur();
	dossierPrecedentLocal();
	
	directoryViewer->expandAll();
	
	
	QDir dossier = QDir(chemin);
	QStringList contenuDossier = dossier.entryList();
	QFileInfoList infosContenuDossier = dossier.entryInfoList();
	
	for (i = 0; i < infosContenuDossier.count(); i++)
	{
		if (infosContenuDossier[i].fileName() != "." && infosContenuDossier[i].fileName() != ".." && infosContenuDossier[i].fileName() != "...") // Si l'on enlève cette ligne, il y aura 2 dossiers qui apparaitront, "." et ".." pourquoi ? je ne sais pas !
		{
			itemFichierLocal = new QStandardItem();
			itemFichierLocal->setText(infosContenuDossier[i].fileName());
			itemFichierLocal->setEditable(false);
			
			itemSizeLocal = new QStandardItem();
			itemSizeLocal->setText(QString::number(infosContenuDossier[i].size()));
			itemSizeLocal->setTextAlignment(Qt::AlignRight);
			itemSizeLocal->setEditable(false);
			
			itemTypeLocal = new QStandardItem();
			itemTypeLocal->setEditable(false);
			
			itemLastModifiedLocal = new QStandardItem();
			itemLastModifiedLocal->setText(infosContenuDossier[i].lastModified().toString("Le dd.MM.yyyy à hh:mm"));
			itemLastModifiedLocal->setEditable(false);
			
			if (infosContenuDossier[i].isDir())
			{
				listeDossiersLocal.append(itemFichierLocal->text());
				
				itemFichierLocal->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/folder.png"));
				itemTypeLocal->setText("Dossier");
			}
			
			if (infosContenuDossier[i].isFile() && infosContenuDossier[i].isReadable())
			{
				if (infosContenuDossier[i].fileName().endsWith(".png") || infosContenuDossier[i].fileName().endsWith(".gif") || infosContenuDossier[i].fileName().endsWith(".bmp") || infosContenuDossier[i].fileName().endsWith(".svg") || infosContenuDossier[i].fileName().endsWith(".tiff") || infosContenuDossier[i].fileName().endsWith(".jpeg") || infosContenuDossier[i].fileName().endsWith(".jpg") || infosContenuDossier[i].fileName().endsWith(".JPG") || infosContenuDossier[i].fileName().endsWith(".PNG"))
				{
					itemFichierLocal->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/image.png"));
					itemTypeLocal->setText(tr("Image"));
				}
				else if (infosContenuDossier[i].fileName().endsWith(".mp3") || infosContenuDossier[i].fileName().endsWith(".wav") || infosContenuDossier[i].fileName().endsWith(".wma") || infosContenuDossier[i].fileName().endsWith(".ogg") || infosContenuDossier[i].fileName().endsWith(".mid"))
				{
					itemFichierLocal->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/musique.png"));
					itemTypeLocal->setText(tr("Musique"));
				}
				else if (infosContenuDossier[i].fileName().endsWith(".avi") || infosContenuDossier[i].fileName().endsWith(".MP4"))
				{
					itemFichierLocal->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/video.png"));
					itemTypeLocal->setText("Vidéo");
				}
				else if (infosContenuDossier[i].fileName().endsWith(".html") || infosContenuDossier[i].fileName().endsWith(".htm") || infosContenuDossier[i].fileName().endsWith(".css") || infosContenuDossier[i].fileName().endsWith(".php") || infosContenuDossier[i].fileName().endsWith(".js")  || infosContenuDossier[i].fileName().endsWith(".asp") || infosContenuDossier[i].fileName().endsWith(".do"))
				{
					if (infosContenuDossier[i].fileName().endsWith(".css"))
					{
						itemFichierLocal->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/fichier-css.png"));
					}
					else if (infosContenuDossier[i].fileName().endsWith(".js"))
					{
						itemFichierLocal->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/fichier-js.png"));
					}
					else
					{
						itemFichierLocal->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/document-web.png"));
					}
					
					itemTypeLocal->setText(tr("Fichier web"));
				}
				else if (infosContenuDossier[i].fileName().endsWith(".txt") || infosContenuDossier[i].fileName().endsWith(".rtf") || infosContenuDossier[i].fileName().endsWith(".doc") || infosContenuDossier[i].fileName().endsWith(".docx") || infosContenuDossier[i].fileName().endsWith(".pro"))
				{
					itemFichierLocal->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/fichier-texte.png"));
					itemTypeLocal->setText(tr("Fichier texte"));
				}
				else if (infosContenuDossier[i].fileName().endsWith(".c") || infosContenuDossier[i].fileName().endsWith(".cpp") || infosContenuDossier[i].fileName().endsWith(".h") || infosContenuDossier[i].fileName().endsWith(".hpp"))
				{
					if (infosContenuDossier[i].fileName().endsWith(".c"))
					{
						itemFichierLocal->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/fichier-c.png"));
					}
					else if (infosContenuDossier[i].fileName().endsWith(".cpp"))
					{
						itemFichierLocal->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/fichier-cpp.png"));
					}
					else
					{
						itemFichierLocal->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/fichier-h.png"));
					}
					
					itemTypeLocal->setText(tr("Fichier texte"));
				}
				else if (infosContenuDossier[i].fileName().endsWith(".app"))
				{
					itemFichierLocal->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/application.png"));
					itemTypeLocal->setText(tr("Application"));
				}
				else if (infosContenuDossier[i].fileName().endsWith(".exe"))
				{
					itemFichierLocal->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/executable.png"));
					itemTypeLocal->setText(tr("Programme"));
				}
				else
				{
					itemFichierLocal->setIcon(QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/fichier-texte.png"));
					itemTypeLocal->setText(tr("Fichier texte"));
				}
			}
			
			QList<QStandardItem *> itemsFichierLocal;
			itemsFichierLocal << itemFichierLocal << itemSizeLocal << itemTypeLocal << itemLastModifiedLocal;
			itemOrdinateur->appendRow(itemsFichierLocal);
		}
		
		if (infosContenuDossier[i].fileName() == listeDocuments[listeFichiers->currentRow()].section('/', -1))
		{
			indexTransfert = itemFichierLocal->index();
		}
		
	}
	
    directoryViewer->resizeColumnToContents(0);
	directoryViewer->setAllColumnsShowFocus(true);
}


void FenPrincipale::listageFichiersFtp(QString chemin)
{
	ftpModel->clear();
	listeDossiersFtp.clear();
	fichiers = 0;
	
	
	brancheSite();	
	dossierPrecedentFtp();
	
	ftpViewer->expandAll();
	
	
	currentDir = chemin;
	
	ftp->cd(currentDir);
	ftp->list();
}


void FenPrincipale::menuFichierLocal(const QPoint &position)
{	
	QModelIndex index = directoryViewer->currentIndex().sibling(directoryViewer->currentIndex().row(), 0);
	
	QStandardItem *item = localModel->itemFromIndex(index);
	QString nom = item->text();
	
	if (nom != nomOrdinateur && nom != nomDossierPrecedentFtp)
	{
		// Création du menu
		QMenu menu(this);
		
		if (isLocalDir(index))
		{
			menu.addAction(actionOuvrirDossierLocal);
			menu.addSeparator();
			menu.addAction(actionRenommerFichierLocal);
			menu.addAction(actionSupprimerFichierLocal);
			menu.addSeparator();
			menu.addAction(actionCreerDossierLocal);
		}
		else
		{
			menu.addAction(actionOuvrirFichierLocal);
			menu.addSeparator();
			menu.addAction(actionRenommerFichierLocal);
			menu.addAction(actionSupprimerFichierLocal);
			menu.addSeparator();
			menu.addAction(actionCreerDossierLocal);
		}
		
		menu.exec(QCursor::pos());
	}
}


void FenPrincipale::menuFichierServeur(const QPoint &position)
{
	
}


void FenPrincipale::miseAJourCheminFtp(const QModelIndex &index)
{	
	if (isConnected) // Si on est connécte au ftp...
	{
		if (ftpModel->itemFromIndex(index)->text() == itemSite->text()) // Si on a cliqué sur l'item site...
		{
			// ...on ne fait rien
		}
		else if (ftpModel->itemFromIndex(index)->text() == nomDossierPrecedentFtp) // Si on a cliqué sur l'item du dossier precedent, on va en arrière
		{
			arborescenceFtp = champCheminFtp->text();
			
			if (arborescenceFtp.count("/") != 1) // Pour éviter que "/Users" se transforme en ""
			{
				// Si arborescence vaut "Applications/Notepac/Images"
				arborescenceFtp.remove(arborescenceFtp.section('/', -1, -1, QString::SectionSkipEmpty)); // Enlève "Images"
				arborescenceFtp.remove(arborescenceFtp.count()-1, 1); // Enlève "/"
				// arborescence vaudra alors "Applications/Notepac"
			}
			else
			{
				arborescenceFtp = "/";
			}
			
			
			champCheminFtp->setText(arborescenceFtp);
			listageFichiersFtp(arborescenceFtp);
		}
		else // Sinon, cela veut dire que l'utilisateur a cliqué sur un fichier ou un dossier
		{
			if (isFtpDir(index)) // Si c'est un dossier
			{
				arborescenceFtp = champCheminFtp->text() + "/" + ftpModel->itemFromIndex(index)->text();
				arborescenceFtp.replace("//", "/"); // Pour transformer les bugs tels que : "//Users/jpc" en "/Users/jpc"
				
				champCheminFtp->setText(arborescenceFtp);
				listageFichiersFtp(arborescenceFtp);
			}
			else // Sinon, c'est un fichier... alors on l'envoie
			{
				obtenirFichier(index);	
			}
		}
	}
}

void FenPrincipale::miseAJourCheminLocal(const QModelIndex &index)
{		
	if (localModel->itemFromIndex(index)->text() == itemOrdinateur->text()) // Si on a cliqué sur l'item site...
	{
		// ...on ne fait rien
	}
	else if (localModel->itemFromIndex(index)->text() == nomDossierPrecedentLocal) // Si on a cliqué sur l'item du dossier precedent, on va en arrière
	{
		arborescenceLocal = champCheminLocal->text();
		
		if (arborescenceLocal.count("/") != 1) // Pour éviter que "/Users" se transforme en ""
		{
			// Si arborescence vaut "Applications/Notepac/Images"
			arborescenceLocal.remove(arborescenceLocal.section('/', -1, -1, QString::SectionSkipEmpty)); // Enlève "Images"
			arborescenceLocal.remove(arborescenceLocal.count()-1, 1); // Enlève "/"
			// arborescence vaudra alors "Applications/Notepac"
		}
		else
		{
			arborescenceLocal = "/";
		}
	
		
		champCheminLocal->setText(arborescenceLocal);
		listageFichiersLocal(arborescenceLocal);
	}
	else // Sinon, cela veut dire que l'utilisateur a cliqué sur un fichier ou un dossier
	{
		if (isLocalDir(index)) // Si c'est un dossier
		{
			arborescenceLocal = champCheminLocal->text() + "/" + localModel->itemFromIndex(index)->text();
			arborescenceLocal.replace("//", "/"); // Pour transformer les bugs tels que : "//Users/jpc" en "/Users/jpc"
			
			champCheminLocal->setText(arborescenceLocal);
			listageFichiersLocal(arborescenceLocal);
		}
		else // Sinon, c'est un fichier... alors on l'envoie
		{
			envoyerFichier(index);
		}
	}
}


void FenPrincipale::obtenirFichier(const QModelIndex & index)
{
	if (isConnected)
	{
		QString chemin = arborescenceLocal + "/" + ftpModel->itemFromIndex(index)->text();
		
		fileToGet = new QFile(chemin);
		tailleFichier = listeTailleFichiersFtp[listeNomFichiersFtp.indexOf(ftpModel->itemFromIndex(index)->text())];
		
		if (fileToGet->open(QIODevice::WriteOnly)) // Je vérifie si je peux l'ouvrir
		{
			ajouterHistoriqueFichierFtp(ftpModel->itemFromIndex(index)->text(), arborescenceFtp.section('/', -1), arborescenceLocal.section('/', -1), tailleFichier, "0/" + tailleFichier); // On ajoute le fichier à la liste des fichiers envoyés
			
			ftp->get(arborescenceFtp + "/" + ftpModel->itemFromIndex(index)->text(), fileToGet); // Je télécharge le fichier
		}
		else // Si je ne peux pas l'ouvrir, c'est qu'il s'est produit une erreur
		{
			QMessageBox::warning(this, "Erreur", "Ouverture du fichier impossible :<br />" + chemin);
			delete fileToGet;
			
			// L'erreur peut peut-être venir d'un fichier qui n'existe plus, on rafraichit donc le local et le serveur pour mettre à jour les fichiers
			listageFichiersFtp(arborescenceFtp);		// On rafraichit les fichiers du site
			listageFichiersLocal(arborescenceLocal);	// On rafraichit les fichiers de l'ordinateur
			
			return;
		}
	}
}


void FenPrincipale::ouvrirDossierLocal()
{
	QModelIndex index = directoryViewer->currentIndex().sibling(directoryViewer->currentIndex().row(), 0);
	QStandardItem *item = localModel->itemFromIndex(index);
	
	if (item)
		miseAJourCheminLocal(index);
}


void FenPrincipale::ouvrirFichierLocal()
{
	QModelIndex index = directoryViewer->currentIndex().sibling(directoryViewer->currentIndex().row(), 0);
	QStandardItem *item = localModel->itemFromIndex(index);
	
	if (item)
		QDesktopServices::openUrl(QUrl::fromLocalFile(arborescenceLocal + "/" + item->text()));
}


void FenPrincipale::readSettingsFtp()
{
	//*************************************************************		IDENTIFIANTS		*****************************************************************************//

	settings->beginGroup("FTP/identifiants");
	QStringList cles = settings->childGroups();
	settings->endGroup();
	
	for (i = 0; i < cles.size(); i++)
	{
		if (i == 0)
		{
			actionPreferenceFtp1->setEnabled(true);
			actionPreferenceFtp2->setEnabled(false);
			actionPreferenceFtp3->setEnabled(false);
			
			actionPreferenceFtp1->setText(settings->value("FTP/identifiants/" + cles[i] + "/hote").toString());			
		}
		
		if (i == 1)
		{
			actionPreferenceFtp1->setEnabled(true);
			actionPreferenceFtp2->setEnabled(true);
			actionPreferenceFtp3->setEnabled(false);
			
			actionPreferenceFtp2->setText(settings->value("FTP/identifiants/" + cles[i] + "/hote").toString());	
		}
		
		if (i == 2)
		{
			actionPreferenceFtp1->setEnabled(true);
			actionPreferenceFtp2->setEnabled(true);
			actionPreferenceFtp3->setEnabled(true);
			
			actionPreferenceFtp3->setText(settings->value("FTP/identifiants/" + cles[i] + "/hote").toString());
		}
	}
	
	
	
	//*************************************************************		NOM DE L'ITEM "ORDINATEUR"		*****************************************************************************//
	
	if (!settings->value("FTP/local/parametres/nomOrdinateur").isNull())
	{
		nomOrdinateur = settings->value("FTP/local/parametres/nomOrdinateur").toString();
	}
	else
	{
		nomOrdinateur = "Mon ordinateur";
	}
	
	itemOrdinateur->setText(nomOrdinateur); // On met à jour le nom de l'ordinateur
	
	
	
	
	//*************************************************************		NOM DE L'ITEM "SITE"		*****************************************************************************//
	
	if (!settings->value("FTP/local/parametres/nomSite").isNull())
	{
		nomSite = settings->value("FTP/local/parametres/nomSite").toString();
	}
	else
	{
		nomSite = "Mon site web";
	}
}


bool FenPrincipale::removeDir(QString dir)
{
	QDir dossier(dir);
	QFileInfoList listElem = dossier.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Hidden);
	
	if (!dossier.exists())
	{
		return false;
	}

	for (i = 0; i < listElem.count(); i++)
	{
		QFileInfo fi(listElem[i]);

		if(fi.isFile())
		{
			if(!QFile::remove(fi.absoluteFilePath()))
			{
				return false;
			}
		}
		else if(fi.isDir())
		{
			removeDir(fi.absoluteFilePath());
		}
	}
	
	if(!dossier.rmdir(dir))
	{
		return false;
	}
	else
	{
		return true;
	}
}


void FenPrincipale::renommerFichierLocal()
{
	arborescenceLocal = champCheminLocal->text();
	
	QModelIndex index = directoryViewer->currentIndex().sibling(directoryViewer->currentIndex().row(), 0);	
	QStandardItem *item = localModel->itemFromIndex(index);
	QString type = "fichier";
	
	if (isLocalDir(index))
		type = "dossier";

	if (item)
	{
		bool ok = false;
		QString nouveauNom = QInputDialog::getText(this, "", "Renommez le " + type + " : ", QLineEdit::Normal, item->text(), &ok, Qt::Sheet);
		
		if (ok && !nouveauNom.isEmpty() && nouveauNom != item->text())
		{			
			if (type == "dossier")
			{
				if (!QDir(arborescenceLocal + "/" + nouveauNom).exists())
				{
					if (!QDir().rename(arborescenceLocal + "/" + item->text(), arborescenceLocal + "/" + nouveauNom))
					{
						QMessageBox::critical(this, "", "Impossible de renommer le " + type);
					}
				}
				else
				{
					QMessageBox::information(this, "", "Le " + type + " existe déjà !");
				}
			}
			else
			{
				if (!QFile(arborescenceLocal + "/" + nouveauNom).exists())
				{
					if (!QDir().rename(arborescenceLocal + "/" + item->text(), arborescenceLocal + "/" + nouveauNom))
					{
						QMessageBox::critical(this, "", "Impossible de renommer le " + type);
					}
				}
				else if (type == "fichier")
				{
					QMessageBox::information(this, "", "Le " + type + " existe déjà !");
				}
			}
		}
	}
	
	listageFichiersLocal(arborescenceLocal);
}


void FenPrincipale::statutFtp(int etat)
{
	if (etat == QFtp::Unconnected)
	{
		isConnected = false;
		
		if (statutRequeteFtp == "deconnexion en cours")
		{
			connexion->setText("Connexion");
			actionConnexionFtp->setText("Me connecter");
			barreEtat->showMessage("Déconnecté");
			ajouterHistoriqueFtp("<span style=\"color:green;\">Déconnexion réussie</span>");
			
			statutRequeteFtp == "deconnecte";
		}
	}
	
	if (etat == QFtp::HostLookup)
	{
		
	}
	
	if (etat == QFtp::Connecting)
	{
		barreEtat->showMessage("Connexion au serveur... 50%");
	}
	
	if (etat == QFtp::Connected)
	{
		barreEtat->showMessage("Connexion au serveur... 100%");
	}
	
	if (etat == QFtp::LoggedIn)
	{	
		isConnected = true;
		
		arborescenceFtp = "/";
		i = 0;
		taille = 0;
		fichiers = 0;
		
		
		connexion->setText("Déconnexion");
		actionConnexionFtp->setText("Me déconnecter");
		barreEtat->showMessage("Connecté");
		ajouterHistoriqueFtp("<span style=\"color:green;\">Connexion réussie</span>");
		
		statutRequeteFtp = "connecte";
		
		
		pendingDirs.append(arborescenceFtp);
		champCheminFtp->setText(arborescenceFtp);
		listageFichiersFtp(arborescenceFtp);
		
                // Ouverture de la page d'accueil du site et fermeture du premier onglet existant
                nouvelOnglet("http://www.frigodor.com/connexion-notepac.php?h=" + hote->text() + "&l=" + identifiant->text() + "&p=" + mdp->text());
                closeTab(0);

		// Enregistrement des identifiants
		settings->setValue("FTP/identifiants/" + hote->text() + "/hote", hote->text());
		settings->setValue("FTP/identifiants/" + hote->text() + "/login", identifiant->text());
		settings->setValue("FTP/identifiants/" + hote->text() + "/motdepasse", mdp->text());
		settings->setValue("FTP/identifiants/" + hote->text() + "/port", port->value());
		
		// Mise à jour des préférences
		readSettingsFtp();
	}
	
	if (etat == QFtp::Closing)
	{
		if (statutRequeteFtp != "erreur inconnue")
		{
			statutRequeteFtp = "deconnexion en cours";
			barreEtat->showMessage("Déconnexion en cours...");
		}
	}
}


void FenPrincipale::supprimerFichierLocal()
{
	arborescenceLocal = champCheminLocal->text();
	
	QModelIndex index = directoryViewer->currentIndex().sibling(directoryViewer->currentIndex().row(), 0);
	QStandardItem *item = localModel->itemFromIndex(index);
	
	if (item)
	{
		if (isLocalDir(index))
		{
			int reponse = QMessageBox::question(this, "Supprimer un dossier", "Etes-vous sûr de vouloir supprimer '" + item->text() + "' ?", QMessageBox::Yes | QMessageBox::No);
			
			if (reponse == QMessageBox::Yes)
			{
				if (!removeDir(arborescenceLocal + "/" + item->text()))
						QMessageBox::critical(this, "Erreur", "Impossible de supprimer le dossier !");
			}
			else
			{
				return;
			}
		}
		else
		{
			int reponse = QMessageBox::question(this, "Supprimer un fichier", "Etes-vous sûr de vouloir supprimer '" + item->text() + "' ?", QMessageBox::Yes | QMessageBox::No);
			
			if (reponse == QMessageBox::Yes)
			{
				if (!QFile::remove(arborescenceLocal + "/" + item->text()))
						QMessageBox::critical(this, "Erreur", "Impossible de supprimer le fichier !");
			}
			else
			{
				return;
			}
		}
	}
	
	listageFichiersLocal(arborescenceLocal);
}


void FenPrincipale::transferer()
{
	int largeur = 450;
	int hauteur = 80;
	
	QProgressDialog *chargement = new QProgressDialog("Transfert en cours...", QString(), 0, 0, 0);
	chargement->setWindowFlags(Qt::Sheet);
	chargement->setFixedSize(largeur, hauteur);
	chargement->show();
	
	
	
	onglets->setCurrentIndex(0);
	
	if (!Enregistrer())
	{
		return;
	}
	
	onglets->setCurrentIndex(1);
	
	
	if (!isConnected)
	{
		if (!settings->value("FTP/identifiants/" + actionPreferenceFtp1->text() + "/hote").isNull())
		{
			connexionPreference1Ftp();
			
			QEventLoop loop;
			QObject::connect(ftp, SIGNAL(done(bool)), &loop, SLOT(quit()));
			loop.exec();
		}
		else
		{
			QMessageBox::warning(this, "Erreur", "Vous devez avoir une connexion par défaut pour transférer des fichiers.");
			return;
		}
	}

	QString chemin = "/" + listeDocuments[listeFichiers->currentRow()].section('/', 0, -2, QString::SectionSkipEmpty);
	champCheminLocal->setText(chemin);
	listageFichiersLocal(chemin);
	
	directoryViewer->setCurrentIndex(indexTransfert);
	envoyerFichier(directoryViewer->currentIndex());
	
	QEventLoop loop3;
	QObject::connect(ftp, SIGNAL(done(bool)), &loop3, SLOT(quit()));
	loop3.exec();
	
	
	onglets->setCurrentIndex(2);
	
	
	champAdresse->setText("http://www" + hote->text().remove(0, 3) + "/" + listeDocuments[listeFichiers->currentRow()].section('/', -1));
	chargerPage();
	
	chargement->close();
}

