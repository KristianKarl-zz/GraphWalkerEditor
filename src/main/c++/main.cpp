#include <QtGui>
#include <boost/program_options.hpp>
#include <fstream>

#include "gui/MainWindow.h"

namespace po = boost::program_options;
int logLevel = 0;
std::ostream logStream ( std::cout.rdbuf() );

std::string currentSystemTime() {
  time_t ltime;
  struct tm* today;
  char time_str[64];

  ::time ( &ltime );

  today = ::localtime ( &ltime );
  ::strftime ( time_str, sizeof ( time_str ), "%Y-%m-%d %H:%M:%S", today );
  return std::string ( time_str );
}

void debugMessageOutput ( QtMsgType type, const char* msg ) {

  logStream << currentSystemTime();

  switch ( type ) {
  case QtDebugMsg:

    if ( logLevel >= 3 ) {
      logStream << " DEBUG - " << msg << std::endl;
    }

    break;

  case QtWarningMsg:

    if ( logLevel >= 2 ) {
      logStream << " WARNING - " << msg << std::endl;
    }

    break;

  case QtCriticalMsg:

    if ( logLevel >= 1 ) {
      logStream << " CRITICAL - " << msg << std::endl;
    }

    break;

  case QtFatalMsg:

    if ( logLevel >= 0 ) {
      logStream << " FATAL - " << msg << std::endl;
    }

    abort();
  }
}

int main ( int argc, char* argv[] ) {
  int return_code = 0;
  std::ofstream logFile;

  try {
    // Declare the supported options.
    po::options_description desc ( "Allowed options" );
    desc.add_options() ( "version", "print version information and exit" ) ( "help", "produce help message" ) ( "graph_file", po::value<std::string>(), "open GWE using graph file" ) ( "log_file",
        po::value<std::string>(), "set the log file" ) ( "log_level", po::value<std::string>(),
            "set the debug level, where:\n\
debug    - High log level\n\
warning  - Log warnings\n\
critical - Log errors\n\
fatal    - Log fatals" );

    po::variables_map vm;

    try {
      po::store ( po::parse_command_line ( argc, argv, desc ), vm );

    }
    catch
      ( std::exception& e ) {
      std::cerr << "Problem when parsing the arguments: " << e.what() << std::endl;;
      return 1;
    }

    po::notify ( vm );

    if ( vm.count ( "help" ) ) {
      std::cout << desc << "\n";
      return 1;
    }

    if ( vm.count ( "version" ) ) {
      std::cout << "GraphWalker Editor 1.0.0" << "\n";
      return 1;
    }

    if ( vm.count ( "log_level" ) ) {
      std::cout << "Log level was set to " << vm["log_level"].as<std::string> () << ".\n";

      if ( vm["log_level"].as<std::string> () == "debug" ) {
        logLevel = 3;
      } else if ( vm["log_level"].as<std::string> () == "warning" ) {
        logLevel = 2;
      } else if ( vm["log_level"].as<std::string> () == "critical" ) {
        logLevel = 1;
      } else if ( vm["log_level"].as<std::string> () == "fatal" ) {
        logLevel = 0;
      }
    }
    else {
      std::cout << "Log level was not set. Default is: debug" << std::endl;
      logLevel = 3;
    }

    if ( vm.count ( "log_file" ) ) {
      std::cout << "Log file level was set to " << vm["log_file"].as<std::string> () << ".\n";
      logFile.open ( QFile::encodeName ( vm["log_file"].as<std::string>().c_str() ), std::ios_base::out | std::ios_base::app );

      // redirect output to our log file
      logStream.rdbuf ( logFile.rdbuf() );
    }

    logStream << currentSystemTime() << " STARTING - Version: 1.0.0" << std::endl;
    qInstallMsgHandler ( debugMessageOutput );

    Q_INIT_RESOURCE ( gwe );

    QApplication app ( argc, argv );
    MainWindow* mainWindow = 0;

    if ( vm.count ( "graph_file" ) ) {
      mainWindow = new MainWindow ( vm["graph_file"].as<std::string> ().c_str() );
    }
    else {
      mainWindow = new MainWindow;
    }

    mainWindow->setGeometry ( 100, 100, 800, 500 );
    mainWindow->show();

    return_code = app.exec();

  }
  catch
    ( std::exception& e ) {
    qCritical ( "%s", e.what() );
    logStream << currentSystemTime() << " FATAL - Abnormal termination of program" << std::endl;
    return 1;
  }

  logStream << currentSystemTime() << "CLOSING - Shutdown code: " << return_code << std::endl;
  return return_code;
}
