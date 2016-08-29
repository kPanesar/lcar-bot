
#ifndef _UNANSWEREDQUERIES_H
#define _UNANSWEREDQUERIES_H


#include <rqt_gcs/simple_gcs.h>
#include "ui_UnansweredQueries.h"
#include <QSignalMapper>

namespace rqt_gcs
{
    
class SimpleGCS;
    
class UnansweredQueries : public QWidget
{
Q_OBJECT
public:

    struct QueryStat
    {
        int uav_id;
        QImage original_img;
        QImage framed_img;
        QString og_img_file_path;
        QString fr_img_file_path;
        
        ~QueryStat()
        { }
    };
    
    UnansweredQueries(SimpleGCS*);
    virtual ~UnansweredQueries();
    void  addQueryWidget(QueryStat*, QString&);
    void addUnansweredQueriesFromDisk();
    int uavIdFromDir(QString&);

private:
    Ui::UnansweredQueries widget_;
    QMap<QString, QVBoxLayout*> layout_by_ap_type;
    QMap <QString, QVector<QueryStat*> > queries_map;
    
    SimpleGCS * gcs;

    QSignalMapper * accept_mapper;
    QSignalMapper * reject_mapper;

    void answerQuery(QWidget*, QString, bool);
    
public slots:
    void acceptQuery(QWidget*);
    void rejectQuery(QWidget*);
};

}// namespace rqt_gcs
//Q_DECLARE_METATYPE(rqt_gcs::UnansweredQueries::QueryStat)
#endif /* _UNANSWEREDQUERIES_H */
