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

#ifndef OPENVPN_H
#define OPENVPN_H

#include <QString>
#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QStringList>
#include <QMetaObject>
#include <QCoreApplication>
#include <kstatusnotifieritem.h>
#include <kmenu.h>

#include "logwindow.h"

class openvpn : public KStatusNotifierItem
{
	Q_OBJECT
public:
	explicit openvpn() ;
	~openvpn();
	static int instanceAlreadyRunning( void ) ;
	static int autoStartDisabled( void ) ;
	static bool autoStartEnabled( void ) ;
	void start( void ) ;
private slots:
	void run( void ) ;
	void trayIconClicked( bool,QPoint ) ;
	void readyReadStandardOutput() ;
	void showLogOutPut( void ) ;
	void exitApplication( void ) ;
	void startProcess( void ) ;
	void stopProcess( void ) ;
	void processDestroyed( QObject * ) ;
	void finished( int i,QProcess::ExitStatus s ) ;
private:
	QString getConfigFile( void ) ;
	void setVPNstate( void ) ;
	QProcess * m_process ;
	bool m_processIsRunning ;
	KMenu * m_menu ;
};

#endif // OPENVPN_H
