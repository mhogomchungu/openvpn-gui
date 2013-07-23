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

#include "logwindow.h"
#include "ui_logwindow.h"

logwindow::logwindow( QWidget * parent ) :QDialog( parent ),m_ui( new Ui::logwindow )
{
	m_ui->setupUi( this ) ;
	connect( m_ui->pbClose,SIGNAL( clicked() ),this,SLOT( pbClose() ) ) ;
}

logwindow::~logwindow()
{
	delete m_ui ;
}

void logwindow::ShowUI()
{
	QFile f( QDir::homePath() + "/.openvpn/output.log" ) ;

	f.open( QIODevice::ReadOnly ) ;

	m_ui->textEditLogField->setText( QString( f.readAll() ) ) ;

	this->show() ;
}

void logwindow::pbClose()
{
	this->hide() ;
	this->deleteLater() ;
}

void logwindow::closeEvent( QCloseEvent * e )
{
	e->ignore() ;
	this->pbClose() ;
}
