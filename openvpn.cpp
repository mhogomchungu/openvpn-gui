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
	qDebug() << "intance already running" ;
	return 1 ;
}

int openvpn::autoStartDisabled()
{
	qDebug() << "instance already running" ;
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
	ac->setText( "start VPN session" ) ;
	m_menu->addAction( ac ) ;
	connect( ac,SIGNAL( triggered() ),this,SLOT( startProcess() ) ) ;

	ac = new QAction( m_menu ) ;
	ac->setText( "start VPN session" ) ;
	m_menu->addAction( ac ) ;
	connect( ac,SIGNAL( triggered() ),this,SLOT( showLogOutPut() ) ) ;

	ac = new QAction( m_menu ) ;
	ac->setText( "show log output" ) ;
	m_menu->addAction( ac ) ;
	connect( ac,SIGNAL( triggered() ),this,SLOT( showLogOutPut() ) ) ;

	ac = new QAction( m_menu ) ;
	ac->setText( "exit" ) ;
	m_menu->addAction( ac ) ;
	connect( ac,SIGNAL( triggered() ),this,SLOT( exitApplication() ) ) ;

	KStatusNotifierItem::setContextMenu( m_menu ) ;

	m_processIsRunning = false ;\

	this->setVPNstate() ;

	KStatusNotifierItem::setStandardActionsEnabled( false ) ;
}

void openvpn::trayIconClicked( bool b,QPoint p )
{
	Q_UNUSED( b ) ;
	Q_UNUSED( p ) ;

	if( m_processIsRunning ){
		m_processIsRunning = false ;
		m_process->terminate() ;
	}else{
		this->startProcess() ;
	}

	this->setVPNstate() ;
}

void openvpn::readyReadStandardOutput()
{
	;
}

void openvpn::showLogOutPut()
{
	logwindow * w = new logwindow() ;
	w->ShowUI() ;
}

void openvpn::startProcess()
{
	m_process = new QProcess( this ) ;

	connect( m_process,SIGNAL( finished( int,QProcess::ExitStatus ) ),this,SLOT( finished( int,QProcess::ExitStatus ) ) ) ;

	m_process->setWorkingDirectory( "/home/ink/qqq" ) ;

	m_processIsRunning = true ;

	QString arg = "/usr/bin/openvpn-cli " + this->getConfigFile() ;

	m_process->start( arg,QIODevice::ReadOnly ) ;

	this->setVPNstate() ;
}

void openvpn::stopProcess()
{
	if( m_processIsRunning ){
		m_processIsRunning = false ;
		m_process->terminate() ;
	}
}

void openvpn::processDestroyed( QObject * obj )
{
	qDebug() << "process not running" ;
	Q_UNUSED( obj ) ;
}

void openvpn::finished( int i,QProcess::ExitStatus s )
{
	Q_UNUSED( i ) ;
	Q_UNUSED( s ) ;
	m_process->deleteLater() ;
	m_process = 0 ;
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

	qDebug() << "quitting there are no config file to use" ;

	this->exitApplication() ;
	return QString( "" ) ;
}

void openvpn::setVPNstate()
{
	if( m_processIsRunning ){
		QString icon = "object-locked.png" ;
		KStatusNotifierItem::setIconByName( icon );
		KStatusNotifierItem::setAttentionIconByName( icon ) ;
		KStatusNotifierItem::setToolTip( icon,tr( "status" ),tr( "VPN tunnel is running" ) ) ;
	}else{
		QString icon = "object-unlocked.png" ;
		KStatusNotifierItem::setIconByName( icon );
		KStatusNotifierItem::setAttentionIconByName( icon ) ;
		KStatusNotifierItem::setToolTip( icon,tr( "status" ),tr( "VPN tunnel is not running" ) ) ;
	}
}

