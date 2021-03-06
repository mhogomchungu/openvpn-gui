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


#include "openvpn.h"

openvpn::openvpn()
{
	;
}

openvpn::~openvpn()
{
	m_menu->deleteLater() ;
}

int openvpn::instanceAlreadyRunning()
{
	qDebug() << tr( "instance already running" ) ;
	return 1 ;
}

int openvpn::autoStartDisabled()
{
	qDebug() << tr( "instance already running" ) ;
	return 1 ;
}

bool openvpn::autoStartEnabled()
{
	return true ;
}

void openvpn::start()
{
	QMetaObject::invokeMethod( this,"run",Qt::QueuedConnection ) ;
}

void openvpn::run()
{
	KStatusNotifierItem::setStatus( KStatusNotifierItem::NeedsAttention ) ;
	KStatusNotifierItem::setCategory( KStatusNotifierItem::ApplicationStatus ) ;

	connect( this,SIGNAL( activateRequested( bool,QPoint ) ),this,SLOT( trayIconClicked( bool,QPoint ) ) ) ;

	m_menu = new KMenu() ;

	QAction * ac = new QAction( m_menu ) ;
	ac->setText( tr( "Start VPN session" ) ) ;
	m_menu->addAction( ac ) ;
	connect( ac,SIGNAL( triggered() ),this,SLOT( startProcess() ) ) ;

	ac = new QAction( m_menu ) ;
	ac->setText( tr( "Stop VPN session" ) ) ;
	m_menu->addAction( ac ) ;
	connect( ac,SIGNAL( triggered() ),this,SLOT( stopProcess() ) ) ;

	ac = new QAction( m_menu ) ;
	ac->setText( tr( "Show log output" ) ) ;
	m_menu->addAction( ac ) ;
	connect( ac,SIGNAL( triggered() ),this,SLOT( showLogOutPut() ) ) ;

	ac = new QAction( m_menu ) ;
	ac->setText( tr( "Exit" ) ) ;
	m_menu->addAction( ac ) ;
	connect( ac,SIGNAL( triggered() ),this,SLOT( exitApplication() ) ) ;

	KStatusNotifierItem::setContextMenu( m_menu ) ;

	m_processIsRunning = false ;

	this->setVPNstate() ;

	KStatusNotifierItem::setStandardActionsEnabled( false ) ;

	m_configFile = this->getConfigFile() ;
}

void openvpn::trayIconClicked( bool b,QPoint p )
{
	Q_UNUSED( b ) ;
	Q_UNUSED( p ) ;

	if( m_processIsRunning ){
		if( m_process ){
			m_process->terminate() ;
		}else{
			qDebug() << tr( "BUGG!!,trayIconClicked():tried to access a void pointer" ) ;
		}
	}else{
		this->startProcess() ;
	}
}

void openvpn::readyReadStandardOutput()
{
	;
}

void openvpn::showLogOutPut()
{
	logwindow::Show() ;
}

void openvpn::startProcess()
{
	if( m_configFile.isEmpty() ){
		QString icon = "security-low.png" ;
		KStatusNotifierItem::setIconByName( icon );
		KStatusNotifierItem::setAttentionIconByName( icon ) ;
		KStatusNotifierItem::setToolTip( icon,tr( "Status" ),tr( "Could not find any openvpn configuration file to use" ) ) ;
		m_configFile = this->getConfigFile() ;
	}else{
		m_process = new QProcess( this ) ;

		connect( m_process,SIGNAL( finished( int,QProcess::ExitStatus ) ),this,SLOT( finished( int,QProcess::ExitStatus ) ) ) ;

		m_process->setWorkingDirectory( QDir::homePath() + "/.openvpn" ) ;

		m_processIsRunning = true ;

		QString arg = QString( OPEN_VPN_CLI_PATH ) + QString( " " ) + m_configFile ;

		m_process->start( arg,QIODevice::ReadOnly ) ;

		this->setVPNstate() ;
	}
}

void openvpn::stopProcess()
{
	if( m_processIsRunning ){
		if( m_process ){
			m_process->terminate() ;
		}else{
			qDebug() << tr( "BUGG!!,stopProcess():tried to access a void pointer" ) ;
		}
	}
}

void openvpn::processDestroyed( QObject * obj )
{
	qDebug() << tr( "process is not running" ) ;
	Q_UNUSED( obj ) ;
}

void openvpn::finished( int i,QProcess::ExitStatus s )
{
	Q_UNUSED( i ) ;
	Q_UNUSED( s ) ;
	if( m_process ){
		m_process->deleteLater() ;
		m_process = 0 ;
	}else{
		qDebug() << tr( "BUGG!!,stopProcess():tried to access a void pointer" ) ;
	}
	m_processIsRunning = false ;
	this->setVPNstate() ;
}

void openvpn::exitApplication()
{
	if( m_processIsRunning ){
		;
	}else{
		QCoreApplication::exit( 1 ) ;
	}
}

QString openvpn::getConfigFile()
{
	QDir d ;
	d.setPath( QDir::homePath() + "/.openvpn" ) ;

	QStringList l = d.entryList() ;

	int j = l.size() ;
	for( int i = 0 ; i < j ; i++ ){
		const QString& e = l.at( i ) ;
		if( e.endsWith( ".conf" ) || e.endsWith( ".opvn" ) ){
			return e ;
		}
	}

	return QString( "" ) ;
}

void openvpn::setVPNstate()
{
	if( m_processIsRunning ){
		QString icon = "security-high.png" ;
		KStatusNotifierItem::setIconByName( icon );
		KStatusNotifierItem::setAttentionIconByName( icon ) ;
		KStatusNotifierItem::setToolTip( icon,tr( "Status" ),tr( "VPN tunnel is running" ) ) ;
	}else{
		QString icon = "security-low.png" ;
		KStatusNotifierItem::setIconByName( icon );
		KStatusNotifierItem::setAttentionIconByName( icon ) ;
		KStatusNotifierItem::setToolTip( icon,tr( "Status" ),tr( "VPN tunnel is NOT running" ) ) ;
	}
}

