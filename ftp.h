/*
 *  ftp.h
 *  Notepac
 *
 *  Created by Lancelot on 14/06/09.
 *  Copyright 2009 BSC. All rights reserved.
 *
 */

#ifndef HEADER_FTP
#define HEADER_FTP

#include <QtGui>

class ProgressDelegate : public QItemDelegate
	{
	public:
		inline void paint(QPainter *painter, const QStyleOptionViewItem &option,
						  const QModelIndex &index ) const
		{
			
			if (index.column() != 4) 
			{
				QItemDelegate::paint(painter, option, index);
				return;
			}

			
			// On crée l'environnement "QStyleOptionProgressBarV2" 
			QStyleOptionProgressBar progressBarOption;
			
			progressBarOption.type = QStyleOption::SO_ProgressBar;
			progressBarOption.state = QStyle::State_Selected | QStyle::State_Mini;
			progressBarOption.direction = QApplication::layoutDirection();
			progressBarOption.rect = option.rect;
			progressBarOption.fontMetrics = QApplication::fontMetrics();
			progressBarOption.minimum = 0;
			progressBarOption.maximum = 100;
			progressBarOption.textAlignment = Qt::AlignCenter;
			progressBarOption.textVisible = true;

			
			
			// Progression de la barre
			int progress = index.data(Qt::DisplayRole).toInt();
			progressBarOption.progress = progress < 0 ? 0 : progress;
			progressBarOption.text = QString::number(progress) + "%";
			
			// On dessine la barre
			QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
		}
	};

#endif