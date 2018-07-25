
#include <QtDebug>
#include "thumbnailmodel.h"
#include "thumbnailengine.h"


ThumbnailModel::ThumbnailModel(QObject * parent)
	: QAbstractListModel(parent) {
}

QVariant ThumbnailModel::data(const QModelIndex & index, int role) const {
	if(data_vector.size() > index.row()) {
		switch(role) {
			case Qt::DisplayRole: return QVariant(data_vector[index.row()].label);
			case Qt::DecorationRole: return QVariant(data_vector[index.row()].icon);
			case Qt::ForegroundRole: return QVariant::fromValue(QColor(Qt::white));
			case ThumbnailModel::AbsPathRole: return QVariant(data_vector[index.row()].absPath);
			case ThumbnailModel::IsDirRole: return QVariant(data_vector[index.row()].isDir);
		}
	}
	return QVariant();
}

int ThumbnailModel::rowCount(const QModelIndex & /*parent*/) const {
	return data_vector.size();
}

void ThumbnailModel::clear() {
	beginResetModel();
	data_vector.clear();
	endResetModel();
}

QModelIndex ThumbnailModel::previousThumbnail(QModelIndex s) const {
	QModelIndex i = s;
	while(true) {
		i = previousIndex(i);
		if(!i.isValid()) break;
		if(!i.data(IsDirRole).toBool()) return i;
	}
	return s;
}

QModelIndex ThumbnailModel::nextThumbnail(QModelIndex s) const {
	QModelIndex i = s;
	while(true) {
		i = nextIndex(i);
		if(!i.isValid()) break;
		if(!i.data(IsDirRole).toBool()) return i;
	}
	return s;
}

QModelIndex ThumbnailModel::previousIndex(QModelIndex i) const {
	if(i.isValid() && i.row() > 0) {
		return index(i.row()-1);
	}
	return QModelIndex();
}

QModelIndex ThumbnailModel::nextIndex(QModelIndex i) const {
	if(i.isValid() && data_vector.size() > i.row() + 1) {
		return index(i.row()+1);
	}
	return QModelIndex();
}

void ThumbnailModel::addThumbnail(QFileInfo fi) {
	ThumbnailModelItem i;
	i.icon = QIcon(new ThumbnailEngine(fi));
	i.label = fi.fileName();
	i.absPath = fi.absoluteFilePath();
	i.isDir = fi.isDir();

	int row = data_vector.size();
	beginInsertRows(QModelIndex(), row, row);
	data_vector.append(i);
	endInsertRows();
}

QModelIndex ThumbnailModel::findIndex(QFileInfo fi) {
	for(int i = 0; i < data_vector.size(); i++) {
		if(data_vector[i].label == fi.fileName()) return createIndex(i, 0, this);
	}
	return QModelIndex();
}

