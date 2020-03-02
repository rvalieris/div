
#include <QApplication>
#include <QCommandLineParser>
#include "div.h"
#include "qarchive.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	app.setApplicationVersion(APP_VERSION);

	QCommandLineParser parser;
	parser.setApplicationDescription("a dry image viewer");
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("path", QObject::tr("file or directory to display"));
	parser.process(app);

	Div div;
	div.show();

	if(parser.positionalArguments().size() > 0) {
		QString filename(parser.positionalArguments().at(0));
		div.setCurrentFile(filename);
	}
	else {
		div.setCurrentFile(QDir::currentPath());
	}

	return app.exec();
}

/**
int main(int argc, char * argv[]) {
	if(argc > 1) {
		QString f(argv[1]);

		QStringList list;
		if(QArchive::listFiles(f, list)) {
			qDebug() << list;
		}
		else {
			qDebug() << "archive read error";
		}
	}
}
*/
