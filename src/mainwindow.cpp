#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    ui->tabWidget->setTabText(0, "Rachis");

    // Create the OpenGL widget
    m_gl = new NGLScene(this);
    
    // Add the OpenGL widget to the grid layout in column 0, spanning 2 rows
    // This will place it on the left side of the UI
    ui->gridLayout_2->addWidget(m_gl, 0, 0, 2, 1);
    
    // Initialize UI with default values
    ui->P0_X->setValue(0.0);
    ui->P0_Y->setValue(0.0);
    ui->P0_Z->setValue(0.0);
    
    ui->P1_X->setValue(0.3);
    ui->P1_Y->setValue(2.0);
    ui->P1_Z->setValue(0.0);
    
    ui->P2_X->setValue(0.5);
    ui->P2_Y->setValue(4.0);
    ui->P2_Z->setValue(0.0);
    
    ui->P3_X->setValue(0.2);
    ui->P3_Y->setValue(6.0);
    ui->P3_Z->setValue(0.0);
    
    ui->sample->setValue(200);
    
    // Set up signal-slot connections
    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{

    // Connect buttons
    connect(ui->resetBtn, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    connect(ui->renderBtn, &QPushButton::clicked, this, &MainWindow::onRenderClicked);
    
    // Connect tab changes
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
}

void MainWindow::onResetClicked()
{
    // Reset UI to default values
    ui->P0_X->setValue(0.0);
    ui->P0_Y->setValue(0.0);
    ui->P0_Z->setValue(0.0);
    
    ui->P1_X->setValue(0.3);
    ui->P1_Y->setValue(2.0);
    ui->P1_Z->setValue(0.0);
    
    ui->P2_X->setValue(0.5);
    ui->P2_Y->setValue(4.0);
    ui->P2_Z->setValue(0.0);
    
    ui->P3_X->setValue(0.2);
    ui->P3_Y->setValue(6.0);
    ui->P3_Z->setValue(0.0);
    
    ui->sample->setValue(200);
    
}

void MainWindow::onRenderClicked()
{
    updateRachis();
    m_gl->update();
}

void MainWindow::onTabChanged(int index)
{
    if (m_gl) {
        switch (index) {
            case 0: // Rachis tab
                m_gl->setDrawMode(DrawMode::RACHIS_ONLY);
                break;
            case 1: // Tab 2 (future outlines tab)
                m_gl->setDrawMode(DrawMode::OUTLINES_ONLY);
                break;
            default:
                m_gl->setDrawMode(DrawMode::RACHIS_ONLY);
                break;
        }
    }
}

void MainWindow::updateRachis()
{
    // Get values from UI spin boxes
    ngl::Vec3 p0(ui->P0_X->value(), ui->P0_Y->value(), ui->P0_Z->value());
    ngl::Vec3 p1(ui->P1_X->value(), ui->P1_Y->value(), ui->P1_Z->value());
    ngl::Vec3 p2(ui->P2_X->value(), ui->P2_Y->value(), ui->P2_Z->value());
    ngl::Vec3 p3(ui->P3_X->value(), ui->P3_Y->value(), ui->P3_Z->value());
    
    int sampleNum = ui->sample->value();
    
    // Update the rachis using the safer NGLScene method
    if (m_gl) {
        m_gl->getFeather()->setSampleNum(sampleNum);
        m_gl->getFeather()->setRachisControlPoints(p0, p1, p2, p3);
        auto controlPoints = m_gl->getFeather()->m_rachis->getCPs();
    }
}
