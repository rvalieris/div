
#ifndef QARCHIVE_H
#define QARCHIVE_H

#include <QString>
#include <QStringList>

struct QArchive_internal;
class QArchive {

public:
	static bool listFiles(QString archive_file, QStringList & out);
	static bool readFile(QString archive_file, QString target, QByteArray & out);
	static bool readFirstFile(QString archive_file, QByteArray & out);

private:
	static bool openArchive(QString archive_file, QArchive_internal & data);
	static bool readData(QArchive_internal & data, QByteArray & out);
	static const int BUFFER_SIZE = 16384;
};


#endif
