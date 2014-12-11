#ifndef SIMLOADER_H
#define SIMLOADER_H

#include <QObject>
#include <QUrl>

#include "block.h"
#include "Models/baseModel.h"
#include "Graphics/macrodyngraphicsitem.h"

class SimLoader : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int graphTyp READ graphTyp WRITE setGraphTyp NOTIFY graphTypChanged)
    Q_PROPERTY(QObject *graphItem READ graphItem WRITE setGraphItem NOTIFY graphItemChanged)

public:

    explicit SimLoader(QObject *parent = 0);
    virtual ~SimLoader();
    inline QString model() const { return m_modelName; }
    void setModel(const QString&);
    inline int graphTyp() const { return m_graphTyp; }
    void setGraphTyp(int);
    inline QObject * graphItem() const { return qobject_cast<QObject *>(m_graph); }
    void setGraphItem(QObject *);


signals:

    void modelChanged();
    void graphTypChanged();
    void graphItemChanged();


public slots:

    Q_INVOKABLE void runSimulation();
    Q_INVOKABLE void loadSimulationfromFile(const QString&);
    Q_INVOKABLE void loadSimulationfromUrl(const QUrl& url) { loadSimulationfromFile(url.toLocalFile()); }
    Q_INVOKABLE void printModelParameters() const { if (m_modelPointer) m_modelPointer->printParamset(); }


protected:

    QString m_modelName;
    baseModel *m_modelPointer;
    conParam m_conBlock;
    MacrodynGraphicsItem *m_graph;

    int m_graphTyp;
    int m_paramCount;
};

#endif // SIMLOADER_H
