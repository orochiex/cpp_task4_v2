#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include "Graph.h"
#include "Logger.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadButton_clicked();
    void on_drawButton_clicked();

private:
    Graph* graph;
    Logger* logger;
    QGraphicsView* view;
    QGraphicsScene* scene;
    QPushButton* loadButton;
    QPushButton* drawButton;
    QLineEdit* vertexInput;
    void drawGraph();
};
