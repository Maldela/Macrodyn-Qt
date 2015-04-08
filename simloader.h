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
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:

    explicit SimLoader(QObject *parent = 0);
    virtual ~SimLoader();
    inline QString model() const { return m_modelName; }
    void setModel(const QString&);
    inline int graphTyp() const { return m_graphTyp; }
    void setGraphTyp(int);
    void setText(const QString &arg);
    Q_INVOKABLE void setGraphItem(MacrodynGraphicsItem *);

    QString text() const;


signals:

    void modelChanged();
    void graphTypChanged();
    void simulate();
    void textChanged();


public slots:

    Q_INVOKABLE void runSimulation();
    Q_INVOKABLE void loadSimulationfromFile(const QString&);
    Q_INVOKABLE void saveSimulationToFile();
    Q_INVOKABLE void loadSimulationfromUrl(const QUrl& url) { loadSimulationfromFile(url.toLocalFile()); }
    Q_INVOKABLE void printModelParameters() const { if (m_modelPointer) m_modelPointer->printParamset(); }
    Q_INVOKABLE void savePdf();


protected slots:

    void jobFinished();
    void loadSimulation();


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
    QString modelName;
    QString m_text;
    QString m_lastText;

    int m_graphTyp;
    int m_paramCount;
};

#endif // SIMLOADER_H
