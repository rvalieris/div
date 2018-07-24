
#include <QDebug>
#include <archive.h>
#include <archive_entry.h>
#include "qarchive.h"

struct QArchive_internal {
	int response;
	struct archive_entry * entry;
	struct archive * archive;

	QArchive_internal();
	~QArchive_internal();
};

QArchive_internal::QArchive_internal() {
	archive = archive_read_new();
	archive_read_support_filter_all(archive);
	archive_read_support_format_all(archive);
}

QArchive_internal::~QArchive_internal() {
	response = archive_read_free(archive);
}

bool QArchive::openArchive(QString archive_file, QArchive_internal & data) {
	QByteArray qba = archive_file.toUtf8();
	data.response = archive_read_open_filename(data.archive, qba.data(), BUFFER_SIZE);
	if(data.response != ARCHIVE_OK) return false;
	return true;
}

bool QArchive::listFiles(QString archive_file, QStringList & out) {
	QArchive_internal data;
	if(!QArchive::openArchive(archive_file, data)) return false;

	while(archive_read_next_header(data.archive, &data.entry) == ARCHIVE_OK) {
		out.append(QString(archive_entry_pathname(data.entry)));
	}
	return true;
}

bool QArchive::readFile(QString archive_file, QString target, QByteArray & out) {

	QArchive_internal data;
	if(!QArchive::openArchive(archive_file, data)) return false;

	bool read_ok = false;
	bool found_file = false;
	while(!found_file && archive_read_next_header(data.archive, &data.entry) == ARCHIVE_OK) {
		QString f(archive_entry_pathname(data.entry));
		if(f == target) {
			found_file = true;
			read_ok = QArchive::readData(data, out);
		}
	}
	return read_ok;
}

bool QArchive::readFirstFile(QString archive_file, QByteArray & out) {
	QArchive_internal data;
	if(!QArchive::openArchive(archive_file, data)) return false;

	bool read_ok = false;
	while(archive_read_next_header(data.archive, &data.entry) == ARCHIVE_OK) {
		if(archive_entry_filetype(data.entry) == AE_IFREG) {
			read_ok = QArchive::readData(data, out);
			break;
		}
	}
	return read_ok;
}

bool QArchive::readData(QArchive_internal & data, QByteArray & out) {
	const void * buff;
	size_t size;
	off_t offset;
	bool read_ok = false;
	bool still_reading = true;
	while(still_reading) {
		data.response = archive_read_data_block(data.archive, &buff, &size, &offset);
		if(data.response == ARCHIVE_EOF) {
			read_ok = true;
			still_reading = false;
		}
		else if(data.response != ARCHIVE_OK) {
			// some error
			still_reading = false;
		}
		else { // keep reading
			const char * p = static_cast<const char*>(buff);
			out.append(p, size);
		}
	}
	return read_ok;
}


