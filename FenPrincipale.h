#ifndef HEADER_FENPRINCIPALE
#define HEADER_FENPRINCIPALE

#include <QtGui>
#include <QtWebKit>

#include <codeeditor.h>
#include <ftp.h>
#include "Highlighter.h"



class FenPrincipale : public QMainWindow
	{
		Q_OBJECT
		
	public:
		FenPrincipale();
	
	signals:
		void done();

		
	public slots:
		void aboutNotepac();
		void actionFtp();
		void actionMenuTriggered(QAction *);
		void affichage();
		void afficherSource();
		void ajouterDocument();
		void ajouterHistoriqueFichierFtp(QString, QString, QString, QString, QString);
		void ajouterHistoriqueFtp(QString);
		void annuler();
		void avancementTransfert(qint64, qint64);
		void brancheOrdinateur();
		void brancheSite();
		void champFtpReturnPressed();
		void champLocalReturnPressed();
		void changementBarreOutils(int);
		void changementDocument();
		void changementPageWeb(int);
		void changementPolice();
		void chargerPage();
		void clearCommands();
		void closeTab(int);
		void coller();
		void completerActivated(const QString &);
		void connexionFtp(QString, QString, QString, quint16 = 21);
		void connexionPreference1Ftp();
		void connexionPreference2Ftp();
		void connexionPreference3Ftp();
		void contextMenuOnPage(const QPoint &);
		void copier();
		void couper();
		void creerDossierLocal();
		void debutTransfert(int);
		void deconnexionFtp();
		void dossierPrecedentFtp();
		void dossierPrecedentLocal();
		void effacerHistoriqueEditeur();
		void effacerHistoriqueFtp();
		void fermerDocument();
		int Enregistrer();
		int EnregistrerSous();
		void EnregistrerTout();
		void envoyerFichier(const QModelIndex &);
		void erreur(QWebView *);
		QIcon favicon();
		void fermerOnglet();
		void finTransfert(int, bool);
		void google();
		void hasInternet(const QString &);
		void historique(const QUrl &);
		void imprimer();
		bool isFtpDir(const QModelIndex &);
		bool isLocalDir(const QModelIndex &);
		void itemOrdinateurChanged(QStandardItem *);
		void itemSiteChanged(QStandardItem *);
		void linkHovered(const QString &, const QString &, const QString &);
		void listageFichiersLocal(QString);
		void listageFichiersFtp(QString);
		void menuFichierLocal(const QPoint &);
		void menuFichierServeur(const QPoint &);
		void miseAJourCheminFtp(const QModelIndex &);
		void miseAJourCheminLocal(const QModelIndex &);
		void modificationDocument();
		void nouvelOnglet(QString = "");
		void nouveauFichier(QString);
		void obtenirFichier(const QModelIndex &);
		void ouvrirDossierLocal();
		void OuvrirFichier();
		void ouvrirFichierLocal();
		void ouvrirFichierRecent();
		void preferences();
		void quitter();
		void readSettingsEditeur();
		void readSettingsFtp();
		void ready();
		bool removeDir(QString);
		void renommerFichierLocal();
		void restoreSplitterState();
		void retablir();
		void saveSplitterState();
		void statutFtp(int statut);
		void supprimerFichierLocal();
		void toutSelectionner();
		void transferer();
		void w3c();
		void zoomIn();
		void zoomOut();
		void zoomNormal();
		
		
	private slots:
		void accueil();
		void actualiser();
		void changementTitre(const QString &);
		void changementUrl(const QUrl &);
        void chargementDebut();
        void chargementEnCours(int);
        void chargementTermine(bool);
		void ftpDone(bool);
		void ftpListInfo(const QUrlInfo &);
		void precedente();
        void stop();
		void suivante();
		void telecharger(QNetworkRequest);
		
		
		
	private:
		Highlighter *highlighter;
		bool ftpModelRowChanged, isConnected, haveInternet, boolDeconnexion, ok;
		int i, j, f, lignes, colonnes, row, index, ongletActif, compteur, currentChamp, ModificationOnglet1, taille, fichiers, onglet, arbo, arboLocal, valeurZoom, nombreTransfert, currentTransfert, secondes;
		quint16 portHost;
		
		QAction *actionAboutNotepac;
		QAction *actionAboutQt;
		QAction *actionAccueil;
		QAction *actionActualiser;
		QAction *actionAfficherBarreEtat;
		QAction *actionAfficherSource;
		QAction *actionAnnuler;
		QAction *actionColler;
		QAction *actionConnexionFtp;
		QAction *actionCopier;
		QAction *actionCopierAdresseLien;
		QAction *actionCopierTexteLien;
		QAction *actionCouper;
		QAction *actionClearCommandsFtp;
		QAction *actionCreerDossierLocal;
		QAction *actionEffacerHistoriqueEditeur;
		QAction *actionEffacerHistoriqueFtp;
		QAction *actionEnregistrer;
		QAction *actionEnregistrerLien;
		QAction *actionEnregistrerTout;
		QAction *actionEnregistrerSous;
		QAction *actionFermerDocument;
		QAction *actionFermerOnglet;
		QAction *actionFichierRecent;
		QAction *actionImprimer;
		QAction *actionNouveau;
		QAction *actionNouvelOnglet;
		QAction *actionOuvrir;
		QAction *actionOuvrirDansNouvelOnglet;
		QAction *actionOuvrirDossierLocal;
		QAction *actionOuvrirFichierLocal;
		QAction *actionOuvrirLien;
		QAction *actionPolice;
		QAction *actionPrecedent;
		QAction *actionPreferences;
		QAction *actionPreferenceFtp1;
		QAction *actionPreferenceFtp2;
		QAction *actionPreferenceFtp3;
		QAction *actionRenommerFichierLocal;
		QAction *actionRetablir;
		QAction *actionStop;
		QAction *actionSuivant;
		QAction *actionSupprimerFichierLocal;
		QAction *actionToolBar1;
		QAction *actionToolBar2;
		QAction *actionToolBar3;
		QAction *actionToutSelectionner;
		QAction *actionTransferer;
		QAction *actionQuitter;
		QAction *actionW3C;
		QAction *actionZoomAvant;
		QAction *actionZoomArriere;
		QAction *actionZoomNormal;
		QAction *m_action;
		QCompleter *completeurUrl;
		QCursor *curseurFenetre;
		QFile *fichier;
		QFile *fileToPut;
		QFile *fileToGet;
		QFtp *ftp;
		QLabel *texteHote;
		QLabel *texteIdentifiant;
		QLabel *texteMdp;
		QLabel *textePort;
		QLineEdit *champAdresse;
		QLineEdit *champCheminFtp;
		QLineEdit *champCheminLocal;
		QLineEdit *champGoogle;
		QLineEdit *champ_recherche;
		QLineEdit *choixTaille;
		QLineEdit *hote;
		QLineEdit *identifiant;
		QLineEdit *mdp;
		QList<QFile *> openedFiles;
		QList<QStandardItem *> itemsDossierPrecedentFtp;
		QList<QStandardItem *> itemsDossierPrecedentLocal;
		QList<QStandardItem *> itemsFichierLocal;
		QList<QStandardItem *> itemsFichier;
		QList<QStandardItem *> itemsOrdinateur;
		QList<QString > ancienneArborescenceFtp;
		QList<QString > ancienneArborescenceLocal;
		QList<int > DocumentsASauvegarder;
		QList<QString > listeDocuments;
		QList<QString > listeDossiersFtp;
		QList<QString > listeDossiersLocal;
		QList<QString > listeNomFichiersFtp;
		QList<QString > listeTailleFichiersFtp;
		QList<int > listeIdFtp;
		QList<QWebView *> listePageWeb;
		QListWidget *listeFichiers;
		QMenu *fichiersRecents;
		QMenu *menuAffichage;
		QMenu *menuAide;
		QMenu *menuBarreOutils;
		QMenu *menuConnexion;
		QMenu *menuEdition;
		QMenu *menuFichier;
		QMenu *menuNavigation;
		QMenu *menuTransfert;
		QModelIndex indexTransfert;
		QPushButton *connexion;
		QProgressBar *progression;
		qreal valeurDuZoom;
		QSettings *settings;
		QSpinBox *port;
		QSplitter *splitterHorizontalEditeur;
		QSplitter *pageChamp;
		QSplitter *splitterHorizontalFtp;
		QSplitter *splitterVerticalFtp;
		QStandardItem *item;
		QStandardItem *itemAvancement;
		QStandardItem *itemDestination;
		QStandardItem *itemDossier;
		QStandardItem *itemFichierLocal;
		QStandardItem *itemLastModified;
		QStandardItem *itemLastModifiedLocal;
		QStandardItem *itemName;
		QStandardItem *itemNewFile;
		QStandardItem *itemOrdinateur;
		QStandardItem *itemProvenance;
		QStandardItem *itemSansTitre;
		QStandardItem *itemSite;
		QStandardItem *itemSiteSize;
		QStandardItem *itemSiteFiles;
		QStandardItem *itemSize;
		QStandardItem *itemSizeLocal;
		QStandardItem *itemStatut;
		QStandardItem *itemType;
		QStandardItem *itemTypeLocal;
		QStandardItemModel *ftpModel;
		QStandardItemModel *localModel;
		QStandardItemModel *modelEditing;
		QStandardItemModel *telechargementsFtpModel;
		QStatusBar *barreEtat;
		QString accueilUrl;
		QString arborescenceFtp;
		QString arborescenceLocal;
		QString cheminFtp;
		QString currentCommand;
		QString currentDir;
		QString currentLocalDir;
		QString host;
		QString hoveredLink;
		QString hoveredLinkTitle;
		QString hoveredLinkContent;
		QString login;
		QString m_chaine;
		QString m_langue;
		QString nomDossierPrecedentFtp;
		QString nomDossierPrecedentLocal;
		QString nomOrdinateur;
		QString nomSite;
		QString password;
		QString statutRequeteFtp;
		QString tailleFichier;
		QStringList listeSousDossiers;
		QStringList pendingDirs;
		QTabWidget *onglets;
		QTabWidget *ongletsNavigateur;
		QTextEdit *champ1;
		QTextEdit *champ2;
		QTextEdit *champHistorique;
		QTextCursor curseur;
		QToolBar *toolBar1;
		QToolBar *toolBar2;
		QToolBar *toolBar3;
		QToolButton *closeTabButton;
		QTranslator translator;
		QTreeView *directoryViewer;
		QTreeView *ftpViewer;
		QTreeView *telechargementsFtpViewer;
		QUrl m_loadingUrl;
		QVBoxLayout *zone1;
		QWebView *currentPageWeb;
		QWebPage *webPage;
		QWebView *pageW3C;
		QWebView *webView;
		QWidget *m_widget;
		
		QStackedLayout listeChamps;
		CodeEditor* nouveauChamp;
		CodeEditor* sourceViewer;
		
		ProgressDelegate* m_barre;
		
		QAbstractItemModel *modelFromFile(const QString& fileName);
		
		QCompleter *completer;
		CodeEditor *completingTextEdit;

		
	protected:
		void closeEvent(QCloseEvent *event);
		
		bool eventFilter(QObject *, QEvent *);
		void dragEnterEvent(QDragEnterEvent *event);
		void dragMoveEvent(QDragMoveEvent *event);
		void dragLeaveEvent(QDragLeaveEvent *event);
		void dropEvent(QDropEvent *event);
		
		
	};


#endif
