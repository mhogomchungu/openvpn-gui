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
#include <QApplication>

#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocalizedstring.h>
#include <kuniqueapplication.h>
#include <QDebug>

int startApp( void )
{
	if( KUniqueApplication::start() ){
		KUniqueApplication a ;
		openvpn w;
		w.start();
		return a.exec();
	}else{
		return openvpn::instanceAlreadyRunning() ;
	}
}

int main(int argc, char *argv[])
{
	KAboutData aboutData( 	"openvpn",
				0,
				ki18n( "openvpn" ),
				"1.2.1",
				ki18n( "openvpn" ),
				KAboutData::License_GPL_V2,
				ki18n( "(c)2013,ink Francis\nemail:mhogomchungu@gmail.com" ),
				ki18n( "mhogomchungu@gmail.com" ),
				"(c)2013,ink Francis\nemail:mhogomchungu@gmail.com",
				"https://github.com/mhogomchungu/openvpn/issues" );

	KCmdLineArgs::init( argc,argv,&aboutData );

	KCmdLineOptions options;

	KCmdLineArgs::addCmdLineOptions( options ) ;
	KUniqueApplication::addCmdLineOptions();

	if( KCmdLineArgs::allArguments().contains( "-a" ) ){
		if( openvpn::autoStartEnabled() ){
			return startApp() ;
		}else{
			return openvpn::autoStartDisabled() ;
		}
	}else{
		return startApp() ;
	}
}
