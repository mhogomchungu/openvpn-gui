/*
 *
 *  Copyright (c) 2013
 *  name : mhogo mchungu
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QDialog>
#include <QFile>
#include <QDir>
#include <QCloseEvent>
#include <QDebug>

namespace Ui {
class logwindow;
}

class logwindow : public QDialog
{
	Q_OBJECT
public:
	explicit logwindow( QWidget * parent = 0 ) ;
	~logwindow() ;
	void ShowUI( void ) ;
private slots:
	void pbClose( void ) ;
private:
	void closeEvent( QCloseEvent * ) ;
	Ui::logwindow * m_ui ;
};

#endif // LOGWINDOW_H
