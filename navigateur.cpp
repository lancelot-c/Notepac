#include <iostream>

#include "FenPrincipale.h"
#include "Highlighter.h"







void FenPrincipale::actionMenuTriggered(QAction *action)
{
	if (action->text() == actionOuvrirLien->text())
	{
		champAdresse->setText(hoveredLink);
		chargerPage();
	}
	else if (action->text() == actionOuvrirDansNouvelOnglet->text())
	{
		nouvelOnglet(hoveredLink);
	}
	else if (action->text() == actionCopierAdresseLien->text())
	{
		QApplication::clipboard()->setText(hoveredLink);
	}
	else if (action->text() == actionCopierTexteLien->text())
	{
		QApplication::clipboard()->setText(hoveredLinkContent);
	}
	else if (action->text() == actionEnregistrerLien->text())
	{
		telecharger(QNetworkRequest(QUrl(hoveredLink)));
	}
}


void FenPrincipale::actualiser()
{
    listePageWeb[ongletsNavigateur->currentIndex()]->reload();
}


void FenPrincipale::afficherSource()
{
	QString codeSource = listePageWeb[ongletsNavigateur->currentIndex()]->page()->mainFrame()->toHtml();
    
	sourceViewer = new CodeEditor;
	sourceViewer->setPlainText(codeSource);
    sourceViewer->setWindowTitle(tr("Code source de la page : ") + listePageWeb[ongletsNavigateur->currentIndex()]->url().toString());
	sourceViewer->setWindowFlags(Qt::Tool);
	sourceViewer->setAttribute(Qt::WA_DeleteOnClose);
	sourceViewer->setLineWrapMode(QPlainTextEdit::NoWrap);
    sourceViewer->setMinimumWidth(640);
    sourceViewer->show();
	
	highlighter = new Highlighter(sourceViewer->document());
}


void FenPrincipale::changementPageWeb(int index)
{
	favicon().pixmap(16, 16).save(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/favicon.png");
	champAdresse->setStyleSheet("padding: 0 20px;" "background: url(\"" + QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/favicon.png\") no-repeat center left;");
	
	champAdresse->setText(listePageWeb[index]->url().toString());
	ongletsNavigateur->setTabIcon(index, favicon());
}


void FenPrincipale::changementTitre(const QString & titreComplet)
{
	webView = qobject_cast<QWebView *>(sender());
	int index = ongletsNavigateur->indexOf(webView);
	
	if (index != -1)
	{
		QString titreCourt = titreComplet;
	
		// On tronque le titre pour éviter des onglets trop larges
		int longueurTitre = 34;
    
		if (titreComplet.size() > longueurTitre)
		{
			titreCourt = titreComplet.left(longueurTitre-3) + tr("...");
		}
	
		ongletsNavigateur->setTabText(index, titreCourt);
	}
}


void FenPrincipale::changementUrl(const QUrl & url)
{
	champAdresse->setText(url.toString());
}


void FenPrincipale::chargementDebut()
{
	actionActualiser->setVisible(false);
	actionStop->setVisible(true);
	
	webView = qobject_cast<QWebView *>(sender());
	m_loadingUrl = champAdresse->text();
	
	
	if (onglets->currentIndex() == 2)
	{
		barreEtat->showMessage("");
		progression->setVisible(true);
		actionStop->setDisabled(false);
		
		int index = ongletsNavigateur->indexOf(webView);
		
		if (index != -1)
		{
			QIcon favicon = QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/chargement.gif");
			
			if (ongletsNavigateur->indexOf(webView) == ongletsNavigateur->currentIndex())
			{
				champAdresse->setStyleSheet("padding: 0 20px;" "background: url(\"" + QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/chargement.gif\") no-repeat center left;");
			}
			
			ongletsNavigateur->setTabIcon(index, favicon);
			ongletsNavigateur->setTabText(index, "Chargement...");
		}
	}
	
	historique(QUrl(m_loadingUrl)); // Ajout de l'adresse à l'historique
}



void FenPrincipale::chargementEnCours(int pourcentage)
{
    progression->setValue(pourcentage);
}



void FenPrincipale::chargementTermine(bool ok)
{
	webView = qobject_cast<QWebView *>(sender());
	
	actionActualiser->setVisible(true);
	actionStop->setVisible(false);
	
	if (ok) // Si le chargement de la page c'est bien déroulé...
	{
		actionW3C->setDisabled(false);
		
		if (ongletsNavigateur->indexOf(webView) == ongletsNavigateur->currentIndex())
		{
			favicon().pixmap(16, 16).save(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/favicon.png");
			champAdresse->setFixedHeight(22);
			champAdresse->setStyleSheet("padding: 0 20px;" "background: url(\"" + QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/favicon.png\") no-repeat center left;");
		}
	
		ongletsNavigateur->setTabIcon(listePageWeb.indexOf(webView), favicon());
		
		
		progression->setVisible(false);
		actionStop->setDisabled(true);
		ready();
		
		// Surligne en jaune tous les liens
			// QString code = "$('a').each( function () { $(this).css('background-color', 'yellow') } )";		
			// webView->page()->mainFrame()->evaluateJavaScript(code);

	}
	else // Si une erreur est survenu pendant le chargement de la page...
	{
		actionW3C->setDisabled(true);
		erreur(webView);
	}
}



void FenPrincipale::chargerPage()
{
    QString url = champAdresse->text();
	
	if (url.endsWith(".exe")) // A modifier
	{
		telecharger(QNetworkRequest(QUrl(url)));
		return;
	}
	
	
	
	if (url == "")
	{
		return;	
	}
	
	if (!url.startsWith("http://"))
	{
		url = "http://" + url;
	}
	
	if (!url.endsWith("/") && !url.contains(".")) // Pour éviter que : http://www.google.com/ se transforme en http://www.google.com/.com
	{
		if (!url.endsWith(".com") && !url.endsWith(".fr") && !url.endsWith(".ca") && !url.endsWith(".uk") && !url.endsWith(".be") && !url.endsWith(".edu") && !url.endsWith(".info") && !url.endsWith(".net") && !url.endsWith(".org") && !url.endsWith(".tv"))
		{
			url += ".com";
		}
	}
	
	
	listePageWeb[ongletsNavigateur->currentIndex()]->load(QUrl(url));
}


void FenPrincipale::closeTab(int index) // Cette fonction est appelée lorsque l'utilisateur clique sur la croix d'un onglet
{
	if (ongletsNavigateur->count() > 1) // S'il y a plusieurs onglets
	{
		ongletsNavigateur->removeTab(index);
	}
}


void FenPrincipale::completerActivated(const QString &adresse)
{
	champAdresse->setText(adresse);
	chargerPage();
}


void FenPrincipale::contextMenuOnPage(const QPoint &position)
{
	QMenu *menu = new QMenu(this);
	
	if (barreEtat->currentMessage().startsWith("http")) // Si le texte de la barre d'état commence par "http", c'est qu'on survole un lien
	{	
		hoveredLink = settings->value("Navigateur/hoveredLink").toString();
		hoveredLinkTitle = settings->value("Navigateur/hoveredLinkTitle").toString();
		hoveredLinkContent = settings->value("Navigateur/hoveredLinkContent").toString();
		
		actionOuvrirLien = new QAction("Ouvrir", this);
		actionOuvrirDansNouvelOnglet = new QAction("Ouvrir dans un nouvel onglet", this);
		actionCopierAdresseLien = new QAction("Copier l'adresse du lien", this);
		actionCopierTexteLien = new QAction("Copier le texte du lien", this);
		actionEnregistrerLien = new QAction("Enregistrer la page du lien", this);
	
		menu->addAction(actionOuvrirLien);
		menu->addAction(actionOuvrirDansNouvelOnglet);
		menu->addSeparator();
		menu->addAction(actionCopierAdresseLien);
		menu->addAction(actionCopierTexteLien);
		menu->addSeparator();
		menu->addAction(actionEnregistrerLien);
	}
	else
	{
		menu->addAction(actionAfficherSource);
		menu->addAction(actionW3C);
	}

	QObject::connect(menu, SIGNAL(triggered(QAction *)), this, SLOT(actionMenuTriggered(QAction *)));
	
	menu->exec(QCursor::pos());
}


void FenPrincipale::erreur(QWebView *pageWeb)
{
    QFile file(QLatin1String("htmls/introuvable.html"));
    Q_ASSERT(file.open(QIODevice::ReadOnly));
    
	QString title = tr("Erreur au chargement de la page").arg(m_loadingUrl.toString());
    QString html = QString(file.readAll())
	.arg(title)
	.arg("Cette page n'existe pas")
	.arg(m_loadingUrl.toString());
	
    QBuffer imageBuffer;
    imageBuffer.open(QBuffer::ReadWrite);
    QIcon icon = QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/warning.png");
    QPixmap pixmap = icon.pixmap(QSize(60, 60)); //Redimenssion de l'image
    
	if (pixmap.save(&imageBuffer, "PNG"))
	{
        html.replace(QLatin1String("IMAGE_BINARY_DATA_HERE"), QString(QLatin1String(imageBuffer.buffer().toBase64())));
    }
	
    QList<QWebFrame*> frames;
    frames.append(pageWeb->page()->mainFrame());
    
	while (!frames.isEmpty())
	{
        QWebFrame *frame = frames.takeFirst();
        
		if (frame->url() == m_loadingUrl)
		{
            frame->setHtml(html, m_loadingUrl);
            return;
		}
		
        QList<QWebFrame *> children = frame->childFrames();
        foreach(QWebFrame *frame, children)
		frames.append(frame);
    }
    
	
        pageWeb->page()->mainFrame()->setHtml(html, m_loadingUrl);
		currentPageWeb = pageWeb;
	
	QIcon favicon = QIcon(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/warning.png");
	ongletsNavigateur->setTabIcon(ongletsNavigateur->currentIndex(), favicon);
	
	actionActualiser->setVisible(true);
	actionStop->setVisible(false);
	ready();
}


QIcon FenPrincipale::favicon()
{
    return QWebSettings::iconForUrl(QUrl(listePageWeb[ongletsNavigateur->currentIndex()]->url()));
}


void FenPrincipale::fermerOnglet() // Cette fonction est appelée lorsque l'utilisateur enclenche l'action "actionFermerOnglet"
{
	closeTab(ongletsNavigateur->currentIndex());
}


void FenPrincipale::google()
{
    QString url = champGoogle->text();
	
    listePageWeb[ongletsNavigateur->currentIndex()]->load(QUrl("http://www.google.fr/search?q=" + url));	
}


void FenPrincipale::hasInternet(const QString &texte)
{
	if (texte == "about:blank")
	{
		// haveInternet == false;
		champAdresse->setText("");
		
		QMessageBox::warning(this, "Attention", "Vous n'êtes pas connecté à Internet.<br />Vous ne pourrez donc pas profiter de toutes les fonctionnalitées du logiciel.<br />");
	}
	else
	{
		// haveInternet == true;
	}
}


void FenPrincipale::historique(const QUrl &url)
{
	QString adresse = url.toString();
	
	// Ajout de l'adresse au compléteur
	QFile file(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/ressources/historique.txt");
	
	if (!file.open(QFile::ReadWrite))
	{
		return;
	}
	
	
	QList<QString> listeAdresses;
	
	
	while (!file.atEnd())
	{
		QByteArray line = file.readLine();
		
		listeAdresses.append(file.readLine());
	}
	
	if (!listeAdresses.contains(adresse))
	{
		QTextStream out(&file);
		out << "\r\n" << adresse;
	}
	
	listeAdresses.clear();
	file.close();
	
	
	// Mise à jour du compléteur
		completeurUrl->setModel(modelFromFile(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/ressources/historique.txt"));
}


void FenPrincipale::imprimer()
{
	if (onglets->currentIndex() == 0)
	{
		QPrinter * imprimante = new QPrinter;
		QPrintDialog * dialogue = new QPrintDialog(imprimante, this);
		
		if(dialogue->exec() == QDialog::Accepted)
		{
			reinterpret_cast<CodeEditor*>(listeChamps.widget(listeFichiers->currentRow()))->print(imprimante);
		}
	}
	
	if (onglets->currentIndex() == 2)
	{
		QPrinter * imprimante = new QPrinter;
		QPrintDialog * dialogue = new QPrintDialog(imprimante, this);
		
		if(dialogue->exec() == QDialog::Accepted)
		{
			listePageWeb[ongletsNavigateur->currentIndex()]->print(imprimante);
		}
	}
}


void FenPrincipale::linkHovered(const QString &lien, const QString &titre, const QString &textContent)
{
	barreEtat->showMessage(lien);
	
	settings->setValue("Navigateur/hoveredLink", lien);
	settings->setValue("Navigateur/hoveredLinkTitle", titre);
	settings->setValue("Navigateur/hoveredLinkContent", textContent);
}


void FenPrincipale::nouvelOnglet(QString url)
{
    QWebView *nouvellePageWeb = new QWebView;
    listePageWeb.append(nouvellePageWeb);
	
    webPage = nouvellePageWeb->page();
	
    QNetworkDiskCache *disqueCache = new QNetworkDiskCache(this);
    QString location = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
    disqueCache->setCacheDirectory(location);
    nouvellePageWeb->page()->networkAccessManager()->setCache(disqueCache);
    nouvellePageWeb->setContextMenuPolicy(Qt::CustomContextMenu);
	
	
        ongletsNavigateur->addTab(nouvellePageWeb, "Nouvel onglet");
	ongletsNavigateur->setCurrentIndex(ongletsNavigateur->count()-1);
	
	listePageWeb[ongletsNavigateur->currentIndex()]->setContentsMargins(0, 0, 0, 0);
	
	champAdresse->setText("");
	
	if (url != "")
	{
		listePageWeb[ongletsNavigateur->currentIndex()]->load(QUrl(url));
	}
	
	
    // Gestion des signaux envoyés par la page web
	
        // QObject::connect(webPage, SIGNAL(printRequested(webPage->currentFrame())), this, SLOT(imprimer()));    // ça marche pas :(
	
	QObject::connect(nouvellePageWeb, SIGNAL(loadStarted()), this, SLOT(chargementDebut()));
        QObject::connect(nouvellePageWeb, SIGNAL(loadProgress(int)), this, SLOT(chargementEnCours(int)));
        QObject::connect(nouvellePageWeb, SIGNAL(loadFinished(bool)), this, SLOT(chargementTermine(bool)));
	
	QObject::connect(nouvellePageWeb->page(), SIGNAL(linkHovered(const QString &, const QString &, const QString &)), this, SLOT(linkHovered(const QString &, const QString &, const QString &)));
	QObject::connect(nouvellePageWeb, SIGNAL(urlChanged(const QUrl &)), this, SLOT(changementUrl(const QUrl &)));
	QObject::connect(nouvellePageWeb, SIGNAL(titleChanged(QString)), this, SLOT(changementTitre(QString)));
	
	QObject::connect(nouvellePageWeb, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(contextMenuOnPage(const QPoint &)));
	QObject::connect(nouvellePageWeb->page(), SIGNAL(downloadRequested(QNetworkRequest)), this, SLOT(telecharger(QNetworkRequest)));
}


void FenPrincipale::precedente()
{
    listePageWeb[ongletsNavigateur->currentIndex()]->back();
}


void FenPrincipale::stop()
{
    listePageWeb[ongletsNavigateur->currentIndex()]->stop();
}

void FenPrincipale::suivante()
{
    listePageWeb[ongletsNavigateur->currentIndex()]->forward();
}

// *************************** T *************************** //

void FenPrincipale::telecharger(QNetworkRequest request)
{
	QMessageBox::information(this, "Téléchargement", "Vous avez téléchargé un logiciel !");
}


// *************************** W *************************** //

void FenPrincipale::w3c()
{	
    pageW3C->setWindowTitle("Validation de la page : " + listePageWeb[ongletsNavigateur->currentIndex()]->url().toString());
	pageW3C->setWindowFlags(Qt::Tool);
    pageW3C->setMinimumWidth(640);
	pageW3C->load("http://validator.w3.org/check?uri=" + listePageWeb[ongletsNavigateur->currentIndex()]->url().toString());
    pageW3C->show();
}

// *************************** Z *************************** //

void FenPrincipale::zoomIn()
{	
	valeurZoom++;
	listePageWeb[ongletsNavigateur->currentIndex()]->setZoomFactor(valeurZoom);
}

void FenPrincipale::zoomOut()
{	
	valeurZoom--;
	listePageWeb[ongletsNavigateur->currentIndex()]->setZoomFactor(valeurZoom);
}

void FenPrincipale::zoomNormal()
{	
	valeurZoom = 1;
	listePageWeb[ongletsNavigateur->currentIndex()]->setZoomFactor(valeurZoom);
}


