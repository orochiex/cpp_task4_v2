#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <queue>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), graph(nullptr), logger(new Logger("log.txt")), scene(new QGraphicsScene(this)) {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    loadButton = new QPushButton("Load Graph", this);
    drawButton = new QPushButton("Draw Graph", this);
    vertexInput = new QLineEdit(this);
    vertexInput->setPlaceholderText("Enter starting vertex");

    layout->addWidget(loadButton);
    layout->addWidget(new QLabel("Starting Vertex:", this));
    layout->addWidget(vertexInput);
    layout->addWidget(drawButton);

    view = new QGraphicsView(scene, this);
    layout->addWidget(view);

    setCentralWidget(centralWidget);

    connect(loadButton, &QPushButton::clicked, this, &MainWindow::on_loadButton_clicked);
    connect(drawButton, &QPushButton::clicked, this, &MainWindow::on_drawButton_clicked);
}

MainWindow::~MainWindow() {
    delete graph;
    delete logger;
}

void MainWindow::on_loadButton_clicked() {
    QString filename = QFileDialog::getOpenFileName(this, "Open Graph File", "", "Text Files (*.txt);;All Files (*)");
    if (filename.isEmpty()) {
        return;
    }
    try {
        graph = new Graph(filename.toStdString());
        logger->log("Graph loaded from file: " + filename.toStdString(), "INFO");
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", e.what());
        logger->log("Error loading graph: " + std::string(e.what()), "ERROR");
    }
}

void MainWindow::on_drawButton_clicked() {
    if (!graph) {
        QMessageBox::warning(this, "Warning", "Load a graph first.");
        return;
    }
    bool ok;
    int start_vertex = vertexInput->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Warning", "Invalid vertex number.");
        return;
    }
    try {
        graph->renumber(start_vertex);
        drawGraph();
        logger->log("Graph renumbered starting from vertex: " + std::to_string(start_vertex), "INFO");
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", e.what());
        logger->log("Error renumbering graph: " + std::string(e.what()), "ERROR");
    }
}

void MainWindow::drawGraph() {
    scene->clear();
    const auto &adjacency_list = graph->getAdjacencyList();

    std::unordered_map<int, QPointF> vertex_positions;
    std::unordered_map<int, int> layers;
    std::queue<int> vertex_queue;
    int radius = 30;
    int layer_distance = 100;
    int layer_width = 100;

    for (const auto &pair: adjacency_list) {
        int vertex = pair.first;
        if (layers.find(vertex) == layers.end()) {
            vertex_queue.push(vertex);
            layers[vertex] = 0;
        }
        while (!vertex_queue.empty()) {
            int current_vertex = vertex_queue.front();
            vertex_queue.pop();
            int current_layer = layers[current_vertex];

            for (const auto &neighbor: adjacency_list.at(current_vertex)) {
                if (layers.find(neighbor) == layers.end()) {
                    layers[neighbor] = current_layer + 1;
                    vertex_queue.push(neighbor);
                }
            }
        }
    }

// Place vertices based on their layers
    std::unordered_map<int, std::vector<int>> layer_map;
    for (const auto &pair: layers) {
        int vertex = pair.first;
        int layer = pair.second;
        layer_map[layer].push_back(vertex);
    }

    for (const auto &pair: layer_map) {
        int layer = pair.first;
        const auto &vertices = pair.second;
        int y = layer * layer_distance;
        int x_offset = (layer_width - vertices.size() * radius) / 2;
        for (size_t i = 0; i < vertices.size(); ++i) {
            int x = i * radius
                    + x_offset;
            int vertex = vertices[i];
            QPointF position(x, y);
            vertex_positions[vertex] = position;

            QGraphicsEllipseItem *ellipse = scene->addEllipse(position.x() - radius / 2, position.y() - radius / 2,
                                                              radius, radius);
            QGraphicsTextItem *text = scene->addText(QString::number(vertex));
            text->setPos(position.x() - radius / 4, position.y() - radius / 4);
        }
    }

// Draw edges
    for (const auto &pair: adjacency_list) {
        int vertex = pair.first;
        const auto &neighbors = pair.second;
        for (int neighbor: neighbors) {
            if (vertex_positions.find(neighbor) != vertex_positions.end()) {
                scene->addLine(QLineF(vertex_positions[vertex], vertex_positions[neighbor]));
            }
        }
    }
}