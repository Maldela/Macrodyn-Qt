#ifndef SIMLOADER_H
#define SIMLOADER_H

#include <QObject>
#include <QUrl>

#include "block.h"
#include "Models/baseModel.h"
#include "Graphics/macrodyngraphicsitem.h"
#include "Jobs/job.h"
#include "simthread.h"

#include <QQuickTextDocument>
#include <QtGui/QTextCharFormat>
#include <QtCore/QTextCodec>
#include <qqmlfile.h>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class SimLoader : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int graphTyp READ graphTyp WRITE setGraphTyp NOTIFY graphTypChanged)
    Q_PROPERTY(QObject *graphItem READ graphItem WRITE setGraphItem NOTIFY graphItemChanged)
    Q_PROPERTY(QQuickItem *target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(QUrl fileUrl READ fileUrl WRITE setFileUrl NOTIFY fileUrlChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:

    explicit SimLoader(QObject *parent = 0);
    virtual ~SimLoader();
    inline QString model() const { return m_modelName; }
    void setModel(const QString&);
    inline int graphTyp() const { return m_graphTyp; }
    void setGraphTyp(int);
    inline QObject *graphItem() const { return qobject_cast<QObject *>(m_graph); }
    void setGraphItem(QObject *);

    QQuickItem *target() { return m_target; }
    void setTarget(QQuickItem *target);
    QUrl fileUrl() const;
    QString text() const;

    QQuickItem *m_target;
    QTextDocument *m_doc;
    QUrl m_fileUrl;
    QString m_text;

signals:

    void modelChanged();
    void graphTypChanged();
    void graphItemChanged();
    void simulate();

Q_SIGNALS:
    void targetChanged();
    void textChanged();
    void fileUrlChanged();


public slots:

    Q_INVOKABLE void runSimulation();
    Q_INVOKABLE void loadSimulationfromFile(const QString&);
    Q_INVOKABLE void saveSimulationfromFile(/*const QString&*/);
    Q_INVOKABLE void loadSimulationfromUrl(const QUrl& url) { loadSimulationfromFile(url.toLocalFile()); }
    Q_INVOKABLE void saveSimulationfromUrl(const QUrl& url) { saveSimulationfromFile(/*url.toLocalFile()*/); }
    Q_INVOKABLE void printModelParameters() const { if (m_modelPointer) m_modelPointer->printParamset(); }
    Q_INVOKABLE void savePdf();

public Q_SLOTS:
    void setFileUrl(const QUrl &arg);
    void setText(const QString &arg);


protected slots:

    void jobFinished();


protected:

    SimThread simThread;
    QString lastFileName;
    QString m_modelName;
    baseModel *m_modelPointer;
    conParam m_conBlock;
    MacrodynGraphicsItem *m_graph;
    xyRange *m_axes;
    Job *m_runJob;
    xyRange *stateSpace;
    xyRange *xDef;
    xyRange *yDef;
    qreal *plotLines;
    QString figureName;

    int m_graphTyp;
    int m_paramCount;
};

#endif // SIMLOADER_H
