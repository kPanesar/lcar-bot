/* 
 * File:   debug.h
 * Author: n8
 *
 * Created on August 26, 2016, 11:29 PM
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <QLoggingCategory>

namespace gcs
{

     Q_DECLARE_LOGGING_CATEGORY(lcar_bot); 
    
namespace dbg
{
    
    void InitDbg();
    
    void MsgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    QString FileFromPath(const QString& path);

}

}
#endif /* DEBUG_H */