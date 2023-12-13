#include <iostream>

#include "FenPrincipale.h"
#include "Highlighter.h"


FenPrincipale::FenPrincipale() : isConnected(false), ftp(new QFtp(this)), completer(new QCompleter(this))
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	
	
    // Propriétés de la fenêtre
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8")); // Encodage de la fenĆŖtre pour éviter des problĆØmes avec les caractĆØres péciaux
	
	
        if (!QDir(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/cache/").exists()) // Si le dossier de cache n'existe pas, je le crée
	{
		QDir(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS")).mkdir("cache");
	}
	
	QWebSettings::setIconDatabasePath(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/cache/");
	
	
        setWindowTitle("Notepac 1.1");
	setUnifiedTitleAndToolBarOnMac(true);
	setMinimumSize(50, 50);
        setAcceptDrops(true);
	
	
	// QDialogButtonBox::ButtonLayout(3); // Style de Gnome, dans le mĆŖme genre que ceux d'ITunes :) , mais Ć§a marche pas !
	
	
        // Récupération des arguments sous Windows et Linux [pour Mac, se référer Ć  la fonction eventFilter(QObject *, QEvent *)]
	QStringList arguments = QCoreApplication::arguments();
	
        if(arguments.count() > 1) // S'il y a plus qu'un argument, l'utilisateur veut ouvrir un fichier (le 1er argument étant le chemin de l'éxecutable)
	{
		for (i = 1; i < arguments.count(); i++)
		{
			nouveauFichier(arguments.at(i));
		}
	}
	
	
	
	
        // Création des préférences
    settings = new QSettings("Lancelot", "Notepac");
	
        // Création des actions
        actionAboutNotepac = new QAction("A propos de Notepac", this);
        actionAboutQt = new QAction("A propos de Qt", this);
        actionAccueil = new QAction("Accueil", this);
        actionActualiser = new QAction("Actualiser", this);
        actionAfficherBarreEtat = new QAction("Barre d'état", this);
        actionAfficherSource = new QAction("Code source de la page", this);
        actionAnnuler = new QAction("Annuler", this);
	actionConnexionFtp = new QAction("Me connecter", this);
        actionCopier= new QAction("Copier", this);
        actionColler = new QAction("Coller", this);
        actionCouper = new QAction("Couper", this);
        actionCreerDossierLocal = new QAction("Créer un dossier", this);
        actionClearCommandsFtp = new QAction("Traiter la file d'attente", this);
        actionEffacerHistoriqueEditeur = new QAction("Effacer l'historique", this);
        actionEffacerHistoriqueFtp = new QAction("Effacer l'historique", this);
        actionEnregistrer = new QAction("Enregistrer", this);
        actionEnregistrerSous = new QAction("Enregistrer sous...", this);
        actionEnregistrerTout = new QAction("Tout enregistrer", this);
        actionFermerDocument = new QAction("Fermer", this);
        actionFermerOnglet = new QAction("Fermer l'onglet", this);
        actionImprimer = new QAction("Imprimer...", this);
        actionNouveau = new QAction("Nouveau", this);
        actionNouvelOnglet = new QAction("Nouvel onglet", this);
        actionOuvrir = new QAction("Ouvrir...", this);
	actionOuvrirDossierLocal = new QAction("Ouvrir le dossier", this);
	actionOuvrirFichierLocal = new QAction("Ouvrir le fichier", this);
	actionPolice = new QAction("Afficher les polices", this);
        actionPrecedent = new QAction("Précédent", this);
        actionPreferences = new QAction("Préférences...", this);
        actionPreferenceFtp1 = new QAction("Connexion 1", this);
        actionPreferenceFtp2 = new QAction("Connexion 2", this);
        actionPreferenceFtp3 = new QAction("Connexion 3", this);
	actionRenommerFichierLocal = new QAction("Renommer", this);
        actionRetablir = new QAction("Rétablir", this);
        actionToolBar1 = new QAction("", this);
        actionToolBar2 = new QAction("", this);
        actionToolBar3 = new QAction("", this);
        actionToutSelectionner = new QAction("Tout sélectionner", this);
        actionTransferer = new QAction("Transférer", this);
        actionStop = new QAction("Arreter", this);
        actionSuivant = new QAction("Suivant", this);
	actionSupprimerFichierLocal = new QAction("Supprimer", this);
        actionQuitter = new QAction("Quitter", this);
	actionW3C = new QAction("Validation de la page", this);
        actionZoomAvant = new QAction("Agrandir", this);
        actionZoomArriere = new QAction("Réduire", this);
        actionZoomNormal = new QAction("Normal", this);
	
	
        actionAccueil->setIcon(QIcon(":/icones/images/home.png"));
        actionActualiser->setIcon(QIcon(":/icones/images/actualiser.png"));
        actionAfficherSource->setIcon(QIcon(":/icones/images/code-source.png"));
        actionEnregistrer->setIcon(QIcon(":/icones/images/save.png"));
        actionEnregistrerSous->setIcon(QIcon(":/icones/images/save.png"));
        actionEnregistrerTout->setIcon(QIcon(":/icones/images/save.png"));
        actionFermerDocument->setIcon(QIcon(":/icones/images/corbeille.png"));
        actionFermerOnglet->setIcon(QIcon(":/icones/images/fermer.png"));
        actionImprimer->setIcon(QIcon(":/icones/images/print.png"));
        actionNouveau->setIcon(QIcon(":/icones/images/ouvrir.png"));
        actionNouvelOnglet->setIcon(QIcon(":/icones/images/onglet.png"));
        actionOuvrir->setIcon(QIcon(":/icones/images/ouvrir-fichier.png"));
        actionPrecedent->setIcon(QIcon(":/icones/images/precedente.png"));
        actionPreferenceFtp1->setIcon(QIcon(":/icones/images/network.png"));
        actionPreferenceFtp2->setIcon(QIcon(":/icones/images/network.png"));
        actionPreferenceFtp3->setIcon(QIcon(":/icones/images/network.png"));
        actionStop->setIcon(QIcon(":/icones/images/stop.png"));
        actionSuivant->setIcon(QIcon(":/icones/images/suivante.png"));
        actionTransferer->setIcon(QIcon(":/images/transferer.png"));
        actionW3C->setIcon(QIcon(":/icones/images/w3c.png"));
	
	
	
	actionActualiser->setShortcut(QKeySequence("F5"));
	actionAnnuler->setShortcut(QKeySequence(QKeySequence::Undo));
	actionAfficherSource->setShortcut(QKeySequence("Ctrl+U"));
	actionColler->setShortcut(QKeySequence(QKeySequence::Paste));
	actionCopier->setShortcut(QKeySequence(QKeySequence::Copy));
	actionCouper->setShortcut(QKeySequence(QKeySequence::Cut));
	actionEnregistrer->setShortcut(QKeySequence(QKeySequence::Save));
	actionEnregistrerSous->setShortcut(QKeySequence(QKeySequence::SaveAs));
	actionFermerDocument->setShortcut(QKeySequence(QKeySequence::Close));
	actionFermerOnglet->setShortcut(QKeySequence(QKeySequence::Close));
	actionImprimer->setShortcut(QKeySequence(QKeySequence::Print));
	actionNouveau->setShortcut(QKeySequence(QKeySequence::New));
	actionNouvelOnglet->setShortcut(QKeySequence(QKeySequence::AddTab));
	actionOuvrir->setShortcut(QKeySequence(QKeySequence::Open));
	actionPolice->setShortcut(QKeySequence("Ctrl+Shift+P"));
	actionPrecedent->setShortcut(QKeySequence::Back);
	actionRetablir->setShortcut(QKeySequence(QKeySequence::Redo));
	actionStop->setShortcut(QKeySequence("Ctrl+."));
	actionSuivant->setShortcut(QKeySequence::Forward);
	actionToutSelectionner->setShortcut(QKeySequence(QKeySequence::SelectAll));
	actionTransferer->setShortcut(QKeySequence("Ctrl+Shift+F"));
	actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
	actionW3C->setShortcut(QKeySequence("Ctrl+3"));
	actionZoomAvant->setShortcut(QKeySequence(QKeySequence::ZoomIn));
	actionZoomArriere->setShortcut(QKeySequence(QKeySequence::ZoomOut));
	actionZoomNormal->setShortcut(QKeySequence("Ctrl+0"));
	
	
	
	actionAfficherBarreEtat->setCheckable(true);
	actionAfficherBarreEtat->setChecked(true);
	actionClearCommandsFtp->setDisabled(true);
	actionPreferenceFtp1->setEnabled(false);
	actionPreferenceFtp2->setEnabled(false);
	actionPreferenceFtp3->setEnabled(false);
	actionStop->setDisabled(true);
	actionToolBar1->setCheckable(true);
	actionToolBar2->setCheckable(true);
	actionToolBar3->setCheckable(true);
	
	actionActualiser->setVisible(true);
	actionStop->setVisible(false);
	
	actionQuitter->setMenuRole(QAction::QuitRole);
	actionAboutQt->setMenuRole(QAction::AboutQtRole);
	actionAboutNotepac->setMenuRole(QAction::AboutRole);
	actionPreferences->setMenuRole(QAction::PreferencesRole);
	
	
    // Création des menus
	// Menu Fichier
	menuFichier = menuBar()->addMenu(tr("&Fichier"));
	menuFichier->addAction(actionNouveau);
	menuFichier->addAction(actionOuvrir);
	menuFichier->addAction(actionEnregistrer);
	menuFichier->addAction(actionEnregistrerSous);
	menuFichier->addAction(actionEnregistrerTout);
	menuFichier->addAction(actionFermerDocument);
	menuFichier->addAction(actionTransferer);
	menuFichier->addSeparator();
	menuFichier->addAction(actionImprimer);
	menuFichier->addSeparator();
        fichiersRecents = menuFichier->addMenu("Fichiers &récents");
	menuFichier->addAction(actionQuitter);
	
	
	// Menu Edition
	menuEdition = menuBar()->addMenu(tr("&Edition"));
	menuEdition->addAction(actionAnnuler);
	menuEdition->addAction(actionRetablir);
	menuEdition->addSeparator();
	menuEdition->addAction(actionCouper);
	menuEdition->addAction(actionCopier);
	menuEdition->addAction(actionColler);
	menuEdition->addSeparator();
	menuEdition->addAction(actionToutSelectionner);
	
	
	// Menu Affichage
	menuAffichage = menuBar()->addMenu(tr("&Affichage"));
	menuAffichage->addAction(actionPreferences);
	menuAffichage->addSeparator();
	menuBarreOutils = menuAffichage->addMenu(tr("Barre d'outils"));
	menuBarreOutils->addAction(actionToolBar1);
	menuBarreOutils->addAction(actionToolBar2);
	menuBarreOutils->addAction(actionToolBar3);
	menuAffichage->addAction(actionAfficherBarreEtat);
	menuAffichage->addAction(actionPolice);
	menuAffichage->addSeparator();
	menuAffichage->addAction(actionAfficherSource);
	menuAffichage->addAction(actionW3C);
	
	
	// Menu Serveur
		menuConnexion = menuBar()->addMenu("Serveur");
		menuConnexion->addAction(actionConnexionFtp);
		menuConnexion->addSeparator();
		menuConnexion->addAction(actionPreferenceFtp1);
		menuConnexion->addAction(actionPreferenceFtp2);
		menuConnexion->addAction(actionPreferenceFtp3);
		menuConnexion->addSeparator();
		menuConnexion->addAction(actionEffacerHistoriqueFtp);
	
	
	// Menu Transfert
	menuTransfert = menuBar()->addMenu(tr("Transfert"));
	menuTransfert->addAction(actionClearCommandsFtp);
	
	
	// Menu Navigation
	menuNavigation = menuBar()->addMenu(tr("&Navigation"));
	menuNavigation->addAction(actionPrecedent);
	menuNavigation->addAction(actionSuivant);
	menuNavigation->addAction(actionActualiser);
	menuNavigation->addAction(actionStop);
	menuNavigation->addAction(actionNouvelOnglet);
	menuNavigation->addAction(actionFermerOnglet);
	menuNavigation->addAction(actionAccueil);
	menuNavigation->addSeparator();
	menuNavigation->addAction(actionImprimer);
	QMenu *menuZoom = menuNavigation->addMenu(tr("Zoom"));
	menuZoom->addAction(actionZoomAvant);
	menuZoom->addAction(actionZoomArriere);
	menuZoom->addSeparator();
	menuZoom->addAction(actionZoomNormal);
	
	
	// Menu "A Propos"
	menuAide = menuBar()->addMenu(tr("A propos"));
	menuAide->addAction(actionAboutQt);
	menuAide->addAction(actionAboutNotepac);
	
	
	
    // Création des barres d'outils (une par onglet)
        // Barre d'outil numéro 1
	champ_recherche = new QLineEdit("");
	champ_recherche->setStyleSheet("border: 1px solid gray;"
								   "border-radius: 10px;"
								   "max-width: 165px;"
								   "padding: 0 18px;"
								   "selection-background-color: silver;"
								   "background : transparent url(images/search.png) no-repeat left top;"
								   "background-color: white;"
								   "border-style: hidden;"
								   );
	
	toolBar1 = addToolBar(tr("Editeur de texte"));
	toolBar1->addAction(actionNouveau);
	toolBar1->addAction(actionOuvrir);
	toolBar1->addAction(actionEnregistrer);
	toolBar1->addAction(actionFermerDocument);
	toolBar1->addAction(actionTransferer);
	
	
        // Barre d'outil numéro 2
	texteHote = new QLabel;
        texteHote->setText("Hôte : ");
	hote = new QLineEdit("");
	hote->setMaximumWidth(140);
	
	texteIdentifiant = new QLabel;
	texteIdentifiant->setText(tr("Identifiant : "));
	texteIdentifiant->setStyleSheet("margin-left:20px;");
	identifiant = new QLineEdit("");
	identifiant->setMaximumWidth(140);
	
	texteMdp = new QLabel;
	texteMdp->setText(tr("Mot de passe : "));
	texteMdp->setStyleSheet("margin-left:20px;");
	mdp = new QLineEdit("");
	mdp->setEchoMode(QLineEdit::Password);
	mdp->setMaximumWidth(140);
	
	textePort = new QLabel;
	textePort->setText(tr("Port : "));
	textePort->setStyleSheet("margin-left:20px;");
	port = new QSpinBox();
	port->setRange(0, 65536);
	port->setValue(21);
	
	connexion = new QPushButton("Connexion");
	connexion->setMenu(menuConnexion);
	
	
	toolBar2 = addToolBar(tr("Transfert des fichiers"));
	toolBar2->addWidget(texteHote);
	toolBar2->addWidget(hote);
	toolBar2->addWidget(texteIdentifiant);
	toolBar2->addWidget(identifiant);
	toolBar2->addWidget(texteMdp);
	toolBar2->addWidget(mdp);
	toolBar2->addWidget(textePort);
	toolBar2->addWidget(port);
	toolBar2->addWidget(connexion);
	
	QObject::connect(hote, SIGNAL(returnPressed()), this, SLOT(actionFtp()));
	QObject::connect(identifiant, SIGNAL(returnPressed()), this, SLOT(actionFtp()));
	QObject::connect(mdp, SIGNAL(returnPressed()), this, SLOT(actionFtp()));
	
	
        // Barre d'outil numéro 3
	champAdresse = new QLineEdit("http://www.google.fr");
								
	completeurUrl = new QCompleter(modelFromFile(QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/ressources/historique.txt"), this);
	completeurUrl->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
	completeurUrl->setCaseSensitivity(Qt::CaseInsensitive);
	completeurUrl->setWrapAround(false);
	champAdresse->setCompleter(completeurUrl);
	QObject::connect(completeurUrl, SIGNAL(activated(const QString &)), this, SLOT(completerActivated(const QString &)));
	
	
	champGoogle = new QLineEdit("Google");
	champGoogle->setMaximumWidth(200);
        champGoogle->setStyleSheet("padding: 1 20px;" "background: url(\"" + QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/logo-google.png\") no-repeat center left;");
	
	QObject::connect(champAdresse, SIGNAL(returnPressed()), this, SLOT(chargerPage()));
	QObject::connect(champGoogle, SIGNAL(returnPressed()), this, SLOT(google()));
	
	
	toolBar3 = addToolBar(tr("Navigation"));
	toolBar3->addAction(actionPrecedent);
	toolBar3->addAction(actionSuivant);
	toolBar3->addAction(actionActualiser);
	toolBar3->addAction(actionStop);
	toolBar3->addAction(actionW3C);
	toolBar3->addAction(actionNouvelOnglet);
	toolBar3->addAction(actionAccueil);
	toolBar3->addWidget(champAdresse);
	toolBar3->addWidget(champGoogle);
    
	// Mise Ć  jour des actions d'affichage des barres d'outils
	actionToolBar1->setText(toolBar1->windowTitle());
	actionToolBar2->setText(toolBar2->windowTitle());
	actionToolBar3->setText(toolBar3->windowTitle());
	
	
        // Création des onglets
	onglets = new QTabWidget;
	onglets->setContentsMargins(0, 0, 0, 0);
	setCentralWidget(onglets);
	
	pageChamp = new QSplitter(Qt::Horizontal);
	splitterHorizontalEditeur = new QSplitter(Qt::Horizontal);
	splitterHorizontalFtp = new QSplitter(Qt::Horizontal);
	splitterVerticalFtp = new QSplitter(Qt::Vertical);
	
        champHistorique = new QTextEdit("<span style=\"font-size:small;\">Pour commencer, veuillez vous connecter avec vos identifiants qui vont ont été communiqués par votre hébergeur.</span><br />");
	champHistorique->setReadOnly(true);
	champHistorique->setMaximumHeight(120);
	
	QStringList entetes;
	entetes << "Nom du fichier" << "Provenance" << "Destination" << "Taille" << "Avancement" << "Statut";
	telechargementsFtpModel = new QStandardItemModel(0, entetes.size(), this);
	telechargementsFtpModel->setHorizontalHeaderLabels(entetes);
	telechargementsFtpViewer = new QTreeView;
	telechargementsFtpViewer->setAlternatingRowColors(false);
	telechargementsFtpViewer->setModel(telechargementsFtpModel);
	telechargementsFtpViewer->setMaximumHeight(200);
	
	
	// Onglet 1
	
	compteur = 1;
	listeFichiers = new QListWidget;
	listeFichiers->setMaximumWidth(200);
	listeFichiers->setEditTriggers(QAbstractItemView::AllEditTriggers);
	listeFichiers->setSelectionMode(QAbstractItemView::ExtendedSelection);
	
	ajouterDocument();
	
	splitterHorizontalEditeur->addWidget(listeFichiers);
	splitterHorizontalEditeur->addWidget(pageChamp);
	
	
	
	//Onglet 2
        // CĆ“té local
	QString arborescenceLocal = QDir::homePath();

	nomDossierPrecedentLocal =  "...";
	
	QStringList entetesLocal;
	entetesLocal << tr("Nom du fichier") << tr("Taille") << tr("Type") << "DerniĆØre modification";
	
	localModel = new QStandardItemModel(0, entetesLocal.size(), this);
	localModel->setHorizontalHeaderLabels(entetesLocal);
	
    directoryViewer = new QTreeView;
    directoryViewer->setModel(localModel);
	directoryViewer->setSelectionMode(QAbstractItemView::ExtendedSelection);
	directoryViewer->setContextMenuPolicy(Qt::CustomContextMenu);
    directoryViewer->header()->setStretchLastSection(true);
    directoryViewer->header()->setSortIndicator(0, Qt::AscendingOrder);
    directoryViewer->header()->setSortIndicatorShown(true);
	directoryViewer->header()->setClickable(true);
	directoryViewer->setAnimated(true);
	
	QObject::connect(directoryViewer, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(menuFichierLocal(const QPoint &)));
	
	listageFichiersLocal(arborescenceLocal);
	
	
        // CĆ“té serveur
	nombreTransfert = 0;
	arborescenceFtp = "";
	
	nomDossierPrecedentFtp = "...";
	
	
	
	QList<QStandardItem *> itemsEnfant;
	
	QStringList entetesFtp;
	entetesFtp << "Nom du fichier" << "Taille" << "Type" << "DerniĆØre modification";
	
	ftpModel = new QStandardItemModel(0, entetesFtp.size(), this);
	ftpModel->setHorizontalHeaderLabels(entetesFtp);
	
	ftpViewer = new QTreeView;
	ftpViewer->setModel(ftpModel);
	ftpViewer->setColumnWidth(0, 200);
	ftpViewer->setContextMenuPolicy(Qt::CustomContextMenu);
    ftpViewer->header()->setStretchLastSection(true);
    ftpViewer->header()->setSortIndicator(0, Qt::AscendingOrder);
    ftpViewer->header()->setSortIndicatorShown(true);
	ftpViewer->header()->setClickable(true);
	ftpViewer->setAnimated(true);
	ftpViewer->setColumnWidth(0, 250);
	ftpViewer->setColumnWidth(1, 120);
	ftpViewer->setColumnWidth(2, 130);
	ftpViewer->setIndentation(20);
	ftpViewer->resizeColumnToContents(0);
	ftpViewer->setAllColumnsShowFocus(true);
	ftpViewer->setSortingEnabled(true);
	ftpViewer->sortByColumn(2, Qt::AscendingOrder);
	ftpViewer->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ftpViewer->setExpandsOnDoubleClick(true);
	
	ftpViewer->setDragEnabled(true);
	ftpViewer->setAcceptDrops(true);
	ftpViewer->setDropIndicatorShown(true);
	ftpViewer->setDragDropMode(QAbstractItemView::InternalMove);
	
	QObject::connect(ftpViewer, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(menuFichierServeur(const QPoint &)));
	

	
	QWidget* m_widgetLayout1 = new QWidget(splitterHorizontalFtp);
	QWidget* m_widgetLayout2 = new QWidget(splitterHorizontalFtp);
	
	QVBoxLayout *layout1 = new QVBoxLayout(m_widgetLayout1);
	layout1->setContentsMargins(0, 0, 0, 0);
	QVBoxLayout *layout2 = new QVBoxLayout(m_widgetLayout2);
	layout2->setContentsMargins(0, 0, 0, 0);
	
	champCheminLocal = new QLineEdit(arborescenceLocal);
        champCheminLocal->setStyleSheet("padding: 2 20px;" "background: url(\"" + QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/dossier.png\") repeat-y center left;");
	champCheminFtp = new QLineEdit(arborescenceFtp);
        champCheminFtp->setStyleSheet("padding: 2 20px;" "background: url(\"" + QCoreApplication::applicationDirPath().remove("/Notepac.app/Contents/MacOS") + "/images/dossier.png\") repeat-y center left;");
	
    layout1->addWidget(champCheminLocal);
    layout1->addWidget(directoryViewer);
	
	layout2->addWidget(champCheminFtp);
    layout2->addWidget(ftpViewer);
	
	
	splitterHorizontalFtp->addWidget(m_widgetLayout1);
	splitterHorizontalFtp->addWidget(m_widgetLayout2);
	
	splitterVerticalFtp->addWidget(champHistorique);
	splitterVerticalFtp->addWidget(splitterHorizontalFtp);
	splitterVerticalFtp->addWidget(telechargementsFtpViewer);
	
	
	// Onglet 3	
	valeurZoom = 1;
	pageW3C = new QWebView;
	
	ongletsNavigateur = new QTabWidget;
	ongletsNavigateur->setStyleSheet("QTabBar::tab { width:255 px; color:black; font-weight:lighter; }");
	ongletsNavigateur->setDocumentMode(true);
	ongletsNavigateur->setAcceptDrops(true);
	ongletsNavigateur->setTabsClosable(true);
    ongletsNavigateur->setMovable(true);
	ongletsNavigateur->setUsesScrollButtons(true);
	
	QWebSettings * const webSettings = QWebSettings::globalSettings();
    webSettings->setAttribute(QWebSettings::PluginsEnabled, true);  // Installation du plugin pour utiliser Flash
	
	QList<QWebView > listePageWeb;
        accueilUrl = "http://www.google.fr"; // On définit la page d'accueil du navigateur
	nouvelOnglet("http://www.frigodor.com/recensement-notepac.php?accueil=" + accueilUrl);
	
	// Assemblage des onglets
    onglets->addTab(splitterHorizontalEditeur, "Edition du site");
    onglets->addTab(splitterVerticalFtp, "Transfert des fichiers");
    onglets->addTab(ongletsNavigateur, "Apercu en ligne");
	
	
	
        //Création de la barre d'état
	barreEtat = statusBar();
	ready();
	
	progression = new QProgressBar(this);
    progression->setVisible(false);
    progression->setMaximumHeight(3);	
    barreEtat->addWidget(progression, 2);
	
	
	// Signaux et Slots
		// Actions
	QObject::connect(actionAboutNotepac, SIGNAL(triggered()), this, SLOT(aboutNotepac()));
	QObject::connect(actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	QObject::connect(actionAccueil, SIGNAL(triggered()), this, SLOT(accueil()));
	QObject::connect(actionActualiser, SIGNAL(triggered()), this, SLOT(actualiser()));
	QObject::connect(actionAfficherBarreEtat, SIGNAL(triggered()), this, SLOT(affichage()));
	QObject::connect(actionAfficherSource, SIGNAL(triggered()), this, SLOT(afficherSource()));
	QObject::connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(annuler()));
	QObject::connect(actionClearCommandsFtp, SIGNAL(triggered()), this, SLOT(clearCommands()));
	QObject::connect(actionColler, SIGNAL(triggered()), this, SLOT(coller()));
	QObject::connect(actionConnexionFtp, SIGNAL(triggered()), this, SLOT(actionFtp()));
	QObject::connect(actionCopier, SIGNAL(triggered()), this, SLOT(copier()));
	QObject::connect(actionCouper, SIGNAL(triggered()), this, SLOT(couper()));
	QObject::connect(actionEffacerHistoriqueEditeur, SIGNAL(triggered()), this, SLOT(effacerHistoriqueEditeur()));
	QObject::connect(actionEffacerHistoriqueFtp, SIGNAL(triggered()), this, SLOT(effacerHistoriqueFtp()));
	QObject::connect(actionEnregistrer, SIGNAL(triggered()), this, SLOT(Enregistrer()));
	QObject::connect(actionEnregistrerSous, SIGNAL(triggered()), this, SLOT(EnregistrerSous()));
	QObject::connect(actionEnregistrerTout, SIGNAL(triggered()), this, SLOT(EnregistrerTout()));
	QObject::connect(actionFermerDocument, SIGNAL(triggered()), this, SLOT(fermerDocument()));
	QObject::connect(actionFermerOnglet, SIGNAL(triggered()), this, SLOT(fermerOnglet()));
	QObject::connect(actionImprimer, SIGNAL(triggered()), this, SLOT(imprimer()));
	QObject::connect(actionNouveau, SIGNAL(triggered()), this, SLOT(ajouterDocument()));
	QObject::connect(actionNouvelOnglet, SIGNAL(triggered()), this, SLOT(nouvelOnglet()));
	QObject::connect(actionOuvrir, SIGNAL(triggered()), this, SLOT(OuvrirFichier()));
	QObject::connect(actionPolice, SIGNAL(triggered()), this, SLOT(changementPolice()));
	QObject::connect(actionPrecedent, SIGNAL(triggered()), this, SLOT(precedente()));
	QObject::connect(actionPreferences, SIGNAL(triggered()), this, SLOT(preferences()));
	QObject::connect(actionPreferenceFtp1, SIGNAL(triggered()), this, SLOT(connexionPreference1Ftp()));
	QObject::connect(actionPreferenceFtp2, SIGNAL(triggered()), this, SLOT(connexionPreference2Ftp()));
	QObject::connect(actionPreferenceFtp3, SIGNAL(triggered()), this, SLOT(connexionPreference3Ftp()));
	QObject::connect(actionRetablir, SIGNAL(triggered()), this, SLOT(retablir()));
	QObject::connect(actionStop, SIGNAL(triggered()), this, SLOT(stop()));
	QObject::connect(actionSuivant, SIGNAL(triggered()), this, SLOT(suivante()));
	QObject::connect(actionToolBar1, SIGNAL(triggered()), this, SLOT(affichage()));
	QObject::connect(actionToolBar2, SIGNAL(triggered()), this, SLOT(affichage()));
	QObject::connect(actionToolBar3, SIGNAL(triggered()), this, SLOT(affichage()));
	QObject::connect(actionToutSelectionner, SIGNAL(triggered()), this, SLOT(toutSelectionner()));
	QObject::connect(actionTransferer, SIGNAL(triggered()), this, SLOT(transferer()));
	QObject::connect(actionQuitter, SIGNAL(triggered()), this, SLOT(quitter()));
	QObject::connect(actionW3C, SIGNAL(triggered()), this, SLOT(w3c()));
	QObject::connect(actionZoomAvant, SIGNAL(triggered()), this, SLOT(zoomIn()));
	QObject::connect(actionZoomArriere, SIGNAL(triggered()), this, SLOT(zoomOut()));
	QObject::connect(actionZoomNormal, SIGNAL(triggered()), this, SLOT(zoomNormal()));
	
	QObject::connect(actionOuvrirDossierLocal, SIGNAL(triggered()), this, SLOT(ouvrirDossierLocal()));
	QObject::connect(actionOuvrirFichierLocal, SIGNAL(triggered()), this, SLOT(ouvrirFichierLocal()));
	QObject::connect(actionRenommerFichierLocal, SIGNAL(triggered()), this, SLOT(renommerFichierLocal()));
	QObject::connect(actionSupprimerFichierLocal, SIGNAL(triggered()), this, SLOT(supprimerFichierLocal()));
	QObject::connect(actionCreerDossierLocal, SIGNAL(triggered()), this, SLOT(creerDossierLocal()));
	
	
	// Autres
	QObject::connect(onglets, SIGNAL(currentChanged(int)), this, SLOT(changementBarreOutils(int)));
	
	
	QObject::connect(listeFichiers, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(changementDocument()));
	
	
	QObject::connect(directoryViewer, SIGNAL(clicked(const QModelIndex &)), directoryViewer, SLOT(setCurrentIndex(const QModelIndex &)));
	QObject::connect(directoryViewer, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(miseAJourCheminLocal(const QModelIndex &)));
	QObject::connect(directoryViewer, SIGNAL(entered(const QModelIndex &)), this, SLOT(miseAJourCheminLocal(const QModelIndex &)));
	QObject::connect(ftpViewer, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(miseAJourCheminFtp(const QModelIndex &)));
	QObject::connect(ftpViewer, SIGNAL(entered(const QModelIndex &)), this, SLOT(miseAJourCheminFtp(const QModelIndex &)));
	QObject::connect(localModel, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(itemOrdinateurChanged(QStandardItem *)));
	QObject::connect(ftpModel, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(itemSiteChanged(QStandardItem *)));
	QObject::connect(champCheminFtp, SIGNAL(returnPressed()), this, SLOT(champFtpReturnPressed()));
	QObject::connect(champCheminLocal, SIGNAL(returnPressed()), this, SLOT(champLocalReturnPressed()));
	
	
	QObject::connect(champAdresse, SIGNAL(textChanged(const QString &)), this, SLOT(hasInternet(const QString &)));
	QObject::connect(ongletsNavigateur, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
	QObject::connect(ongletsNavigateur, SIGNAL(currentChanged(int)), this, SLOT(changementPageWeb(int)));
	
	
	
	
	// On place l'utilisateur dans le premier onglet...
        int onglet = 0;
	
	changementBarreOutils(onglet); // On charge les barres d'outils
	onglets->setCurrentIndex(onglet);
	
	
	readSettingsEditeur();
	readSettingsFtp();
	
	restoreSplitterState();
	
	
	QApplication::restoreOverrideCursor();
}






// *************************** FONCTIONS *************************** //

bool FenPrincipale::eventFilter(QObject *obj, QEvent *event)
{
        if(event->type() == QEvent::FileOpen) // Si l'utilisateur a envoyé des arguments au programme
	{
		nouveauFichier(((QFileOpenEvent*)event)->file());
		return true;
	}
        else // Sinon, le problĆØme a été déclenché normalement
	{
		return QObject::eventFilter(obj, event);
	}
}






void FenPrincipale::aboutNotepac()
{
        QMessageBox::about(this, "A propos de Notepac...", "Notepac 1.1<br />Site web : <a href=\"http://www.frigodor.com\">http://www.frigodor.com</a><br />Développeur : <a href=\"http://www.siteduzero.com/membres-294-57094.html\">Lancelot Chardonnet (Frigodor)</a>");
}


void FenPrincipale::preferences()
{
        QMessageBox::information(this, "Préférences", "Notepac ne contient aucune préférence pour le moment... elles seront disponibles dans la prochaine version.");
}


void FenPrincipale::affichage()
{
        if (actionAfficherBarreEtat->isChecked()) // Si l'action est cochée, on affiche la barre d'état
	{
		barreEtat->show();
	}
        else // Sinon, on cache la barre d'état
	{
		barreEtat->hide();
	}
	
	
        if (actionToolBar1->isChecked()) // Si l'action est cochée, on affiche la barre d'outil
	{
		toolBar1->show();
	}
        else // Sinon, on cache la barre d'état
	{
		toolBar1->hide();
	}
	
        if (actionToolBar2->isChecked()) // Si l'action est cochée, on affiche la barre d'outil
	{
		toolBar2->show();
	}
        else // Sinon, on cache la barre d'état
	{
		toolBar2->hide();
	}
	
        if (actionToolBar3->isChecked()) // Si l'action est cochée, on affiche la barre d'outil
	{
		toolBar3->show();
	}
        else // Sinon, on cache la barre d'état
	{
		toolBar3->hide();
	}
}


void FenPrincipale::changementBarreOutils(int index)
{
	if (index == 0)
	{
		toolBar1->show();
		toolBar2->hide();
		toolBar3->hide();
		
		actionToolBar1->setChecked(true);
		actionToolBar2->setChecked(false);
		actionToolBar3->setChecked(false);
		
		actionFermerDocument->setEnabled(true);
		actionFermerOnglet->setEnabled(false);
	}
	
	if (index == 1)
	{
		toolBar1->hide();
		toolBar2->show();
		toolBar3->hide();
		
		actionToolBar1->setChecked(false);
		actionToolBar2->setChecked(true);
		actionToolBar3->setChecked(false);
	}
	
	if (index == 2)
	{
		toolBar1->hide();
		toolBar2->hide();
		toolBar3->show();
		
		actionToolBar1->setChecked(false);
		actionToolBar2->setChecked(false);
		actionToolBar3->setChecked(true);
		
		actionFermerDocument->setEnabled(false);
		actionFermerOnglet->setEnabled(true);
	}
}


void FenPrincipale::nouveauFichier(QString chemin)
{
	QString FichierAOuvrir = chemin;
	
    QFile file(FichierAOuvrir);
	
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return;
	}
	
	
        QTextStream in(&file); // Un petit QTextStream pour pouvoir récupérer le contenu du fichier ouvert
	
	reinterpret_cast<CodeEditor*>(listeChamps.widget(listeFichiers->currentRow()))->setPlainText("");
	
	while(!in.atEnd()) // Une boucle : tant qu'on n'est pas Ć  la fin du fichier ...
	{
		reinterpret_cast<CodeEditor*>(listeChamps.widget(listeFichiers->currentRow()))->insertPlainText(in.readLine() + "\n"); // ... on insert dans la zone de texte une ligne plus \n (qui fait un retour Ć  la ligne)
	}
	
	listeFichiers->currentItem()->setText(file.fileName().section('/', -1)); // Changement du texte de l'item
	
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
	
	readSettingsEditeur(); // On met Ć  jour la liste des fichiers recemment ouverts
}


void FenPrincipale::saveSplitterState()
{
	settings->setValue("EDITEUR/parametres/widgets/splitter/horizontal", splitterHorizontalEditeur->saveState());
	settings->setValue("FTP/parametres/widgets/splitter/vertical", splitterVerticalFtp->saveState());
	settings->setValue("FTP/parametres/widgets/splitter/horizontal", splitterHorizontalFtp->saveState());
}


void FenPrincipale::restoreSplitterState()
{
        if (!settings->value("EDITEUR/parametres/widgets/splitter/horizontal").isNull()) // Si on a enregistré quelquechose
	{
		splitterHorizontalEditeur->restoreState(settings->value("EDITEUR/parametres/widgets/splitter/horizontal").toByteArray());
		splitterVerticalFtp->restoreState(settings->value("FTP/parametres/widgets/splitter/vertical").toByteArray());
		splitterHorizontalFtp->restoreState(settings->value("FTP/parametres/widgets/splitter/horizontal").toByteArray());
	}
}


void FenPrincipale::ready()
{
        //barreEtat->showMessage("Prêt");
}


// *************************** FONCTION ACCUEIL *************************** //
void FenPrincipale::accueil() // Cette fonction n'est pas dans navigateur.cpp car sinon, elle ne marche pas... pourquoi ? c'est un mystĆØre...
{
    listePageWeb[ongletsNavigateur->currentIndex()]->load(QUrl(accueilUrl));
}









// *************************** DRAG AND DROP *************************** //

void FenPrincipale::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}


void FenPrincipale::dragMoveEvent(QDragMoveEvent *event)
{
	event->acceptProposedAction();
}


void FenPrincipale::dropEvent(QDropEvent *event)
{
	const QMimeData *mimeData = event->mimeData();
	
	if(mimeData->hasUrls())
	{
		QList<QUrl> urlList = mimeData->urls();
		for(int i = 0; i < urlList.size() && i < 32; ++i)
		{
                        // on récupĆØre le chemin du fichier déposé
			QString url = urlList.at(i).path();
			
			#ifdef COMPILATION_WINDOWS
				url = url.right(url.length() - 1);
			#endif
			
			if (onglets->currentIndex() == 1)
			{
				champCheminLocal->setText(url);
				listageFichiersLocal(url);
			}
			else if (onglets->currentIndex() == 0)
			{
                                nouveauFichier(url); // On ouvre le fichier dans l'éditeur
			}
			
			barreEtat->showMessage("Fichier entrant : " + url, 5000);
		}
	}
	
	event->acceptProposedAction();
}


void FenPrincipale::dragLeaveEvent(QDragLeaveEvent *event)
{
	event->accept();
}





void FenPrincipale::quitter()
{
	
	listeFichiers->setCurrentRow(listeFichiers->count()-1);
	
	while (listeFichiers->count() == 0)
	{
		fermerDocument();
	}
	
	
	saveSplitterState();
	QCoreApplication::quit();
}


void FenPrincipale::closeEvent(QCloseEvent * event)
{
	quitter();
}
