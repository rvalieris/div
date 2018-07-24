
#ifndef THUMBNAILMODEL_H
#define THUMBNAILMODEL_H

#include <QAbstractListModel>
#include <QIcon>
#include <QVector>
#include <QFileInfo>

struct ThumbnailModelItem;
struct ThumbnailModelItem {
	QIcon icon;
	QString label;
	QString absPath;
	bool isDir;
};


class ThumbnailModel : public QAbstractListModel {
	Q_OBJECT

public:

	enum {
		AbsPathRole = Qt::UserRole + 1,
		IsDirRole,
	};

	ThumbnailModel(QObject * parent = nullptr);
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;

	void clear();
	QModelIndex previousThumbnail(QModelIndex s) const;
	QModelIndex nextThumbnail(QModelIndex s) const;
	QModelIndex previousIndex(QModelIndex i) const;
	QModelIndex nextIndex(QModelIndex i) const;
	void addThumbnail(QFileInfo fi);
	QModelIndex findIndex(QFileInfo fi);
private:
	QVector<ThumbnailModelItem> data_vector;
};

#endif
