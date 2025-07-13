#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    ui->tabWidget->setTabText(0, "Rachis");
    ui->tabWidget->setTabText(1, "Outlines");
    ui->tabWidget->setTabText(2, "Barb");
    ui->tabWidget->setTabText(3, "Feather");
    // Note: Full feather view will use ALL_COMPONENTS draw mode

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
    
    ui->P3_X->setValue(0.20);
    ui->P3_Y->setValue(9.50);
    ui->P3_Z->setValue(0.00);
    
    ui->sample->setValue(200);
    
    // Initialize outline UI with default values
    ui->O_P1_X->setValue(-1.8);
    ui->O_P1_Y->setValue(3.5);
    ui->O_P1_Z->setValue(0.0);
    
    ui->O_P2_X->setValue(-1.5);
    ui->O_P2_Y->setValue(10.0);
    ui->O_P2_Z->setValue(0.0);
    
    ui->O_P3_X->setValue(0.2);
    ui->O_P3_Y->setValue(9.5);
    ui->O_P3_Z->setValue(0.0);
    
    ui->O_P4_X->setValue(0.6);
    ui->O_P4_Y->setValue(2.75);
    ui->O_P4_Z->setValue(0.0);
    
    ui->O_P5_X->setValue(0.5);
    ui->O_P5_Y->setValue(10.0);
    ui->O_P5_Z->setValue(0.0);
    
    ui->f0->setValue(0.25);
    
    // Initialize barb UI with default values
    ui->fb->setValue(0.50);
    ui->spinBox->setValue(20);
    ui->l_b_p3_f->setValue(0.55);
    ui->doubleSpinBox->setValue(0.51);
    ui->barb_p1_x_f->setValue(0.3);
    ui->barb_p1_y_f->setValue(1.0);
    ui->barb_p2_x_f->setValue(0.1);
    ui->barb_p2_y_f->setValue(0.0);
    
    // Initialize "All" tab controls with default values
    ui->barbnum->setValue(100);
    ui->barblod->setValue(20);
    ui->showOutline->setChecked(false);
    ui->tip->setValue(0.99);
    ui->m_outlineMappingStart->setValue(0.0);
    ui->m_outlineMappingEnd->setValue(1.0);
    
    // Set up signal-slot connections
    setupConnections();
    
    // Initialize outline controls state (UI only - feather object not ready yet)
    ui->groupBox_8->setEnabled(!ui->symmetrical->isChecked());  // R_P1 group
    ui->groupBox_9->setEnabled(!ui->symmetrical->isChecked());  // R_P2 group
    
    // Enable render button (it was disabled in UI file)
    ui->renderBtn->setEnabled(true);
    ui->resetBtn->setEnabled(true);
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
    
    // Connect outline controls
    connect(ui->symmetrical, &QCheckBox::toggled, this, &MainWindow::onSymmetricalChanged);
}

void MainWindow::onResetClicked()
{
    // Reset rachis UI to default values
    ui->P0_X->setValue(0.0);
    ui->P0_Y->setValue(0.0);
    ui->P0_Z->setValue(0.0);
    
    ui->P1_X->setValue(0.3);
    ui->P1_Y->setValue(2.0);
    ui->P1_Z->setValue(0.0);
    
    ui->P2_X->setValue(0.5);
    ui->P2_Y->setValue(4.0);
    ui->P2_Z->setValue(0.0);
    
    ui->P3_X->setValue(0.20);
    ui->P3_Y->setValue(9.50);
    ui->P3_Z->setValue(0.00);
    
    ui->sample->setValue(200);
    
    // Reset outline UI to default values
    ui->symmetrical->setChecked(true);
    
    ui->O_P1_X->setValue(-1.8);
    ui->O_P1_Y->setValue(3.5);
    ui->O_P1_Z->setValue(0.0);
    
    ui->O_P2_X->setValue(-1.5);
    ui->O_P2_Y->setValue(10.0);
    ui->O_P2_Z->setValue(0.0);
    
    ui->O_P3_X->setValue(0.2);
    ui->O_P3_Y->setValue(9.5);
    ui->O_P3_Z->setValue(0.0);
    
    ui->O_P4_X->setValue(0.6);
    ui->O_P4_Y->setValue(2.75);
    ui->O_P4_Z->setValue(0.0);
    
    ui->O_P5_X->setValue(0.5);
    ui->O_P5_Y->setValue(10.0);
    ui->O_P5_Z->setValue(0.0);
    
    ui->f0->setValue(0.25);
    
    // Reset barb UI to default values
    ui->fb->setValue(0.50);
    ui->spinBox->setValue(20);
    ui->l_b_p3_f->setValue(0.55);
    ui->doubleSpinBox->setValue(0.51);
    ui->barb_p1_x_f->setValue(0.3);
    ui->barb_p1_y_f->setValue(1.0);
    ui->barb_p2_x_f->setValue(0.1);
    ui->barb_p2_y_f->setValue(0.0);
    
    // Reset "All" tab UI to default values
    ui->barbnum->setValue(100);
    ui->barblod->setValue(20);
    ui->showOutline->setChecked(false);
    ui->tip->setValue(0.99);
    ui->m_outlineMappingStart->setValue(0.0);
    ui->m_outlineMappingEnd->setValue(1.0);
    
}

void MainWindow::onRenderClicked()
{
    updateRachis();
    updateOutlines();
    updateBarbs();
    updateAllFeather();
    m_gl->update();
}

void MainWindow::onTabChanged(int index)
{
    if (m_gl) {
        switch (index) {
            case 0: // Rachis tab
                m_gl->setDrawMode(DrawMode::RACHIS);
                break;
            case 1: // outlines tab
                m_gl->setDrawMode(DrawMode::OUTLINES);
                break;
            case 2: // barbs tab
                m_gl->setDrawMode(DrawMode::BARB);
                break;
            case 3: // feather tab
                m_gl->setDrawMode(DrawMode::ALL_COMPONENTS);
                break;
            default:
                m_gl->setDrawMode(DrawMode::RACHIS);
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

    if (m_gl) {
        m_gl->getFeather()->setSampleNum(sampleNum);
        m_gl->getFeather()->setRachisControlPoints(p0, p1, p2, p3);
    }
}

void MainWindow::updateOutlines()
{
    if (!m_gl) return;
    
    // Get outline control points from UI
    ngl::Vec3 p1(ui->O_P1_X->value(), ui->O_P1_Y->value(), ui->O_P1_Z->value());
    ngl::Vec3 p2(ui->O_P2_X->value(), ui->O_P2_Y->value(), ui->O_P2_Z->value());
    ngl::Vec3 p3(ui->O_P3_X->value(), ui->O_P3_Y->value(), ui->O_P3_Z->value());
    
    // Get F0 factor from UI
    double f0 = ui->f0->value();
    m_gl->getFeather()->setF0(f0);
    
    bool symmetric = ui->symmetrical->isChecked();
    
    if (symmetric) {
        // Use symmetrical outline control points
        m_gl->getFeather()->setSymmetricOutlineControlPoints(p1, p2, p3);
    } else {
        // Use separate control points for left and right
        ngl::Vec3 rightP1(ui->O_P4_X->value(), ui->O_P4_Y->value(), ui->O_P4_Z->value());
        ngl::Vec3 rightP2(ui->O_P5_X->value(), ui->O_P5_Y->value(), ui->O_P5_Z->value());
        m_gl->getFeather()->setOutlineControlPoints(p1, p2, rightP1, rightP2, p3);
    }
}

void MainWindow::updateBarbs()
{
    if (!m_gl) return;
    
    // Get barb parameters from UI
    double fb = ui->fb->value();
    int lod = ui->spinBox->value();
    double leftEndFactor = ui->l_b_p3_f->value();
    double rightEndFactor = ui->doubleSpinBox->value();
    double p1XFactor = ui->barb_p1_x_f->value();
    double p1YFactor = ui->barb_p1_y_f->value();
    double p2XFactor = ui->barb_p2_x_f->value();
    double p2YFactor = ui->barb_p2_y_f->value();
    
    // Update feather barb parameters
    m_gl->getFeather()->setFb(fb);
    m_gl->getFeather()->setBarbLOD(lod);
    m_gl->getFeather()->setBarbsOutlineFactor(leftEndFactor, rightEndFactor);
    m_gl->getFeather()->setBarbControlFactors(p1XFactor, p1YFactor, p2XFactor, p2YFactor);
}

void MainWindow::updateAllFeather()
{
    if (!m_gl) return;
    
    // Get "All" tab parameters from UI
    int barbNum = ui->barbnum->value();
    int barbLOD = ui->barblod->value();
    bool showOutlines = ui->showOutline->isChecked();
    double tipFactor = ui->tip->value();
    double mappingStart = ui->m_outlineMappingStart->value();
    double mappingEnd = ui->m_outlineMappingEnd->value();
    
    // Update feather parameters for full feather generation
    m_gl->getFeather()->setNumBarbs(barbNum);
    m_gl->getFeather()->setBarbLOD(barbLOD);
    m_gl->getFeather()->setShowOutlines(showOutlines);
    m_gl->getFeather()->setFn(tipFactor);
    m_gl->getFeather()->setOutlineMappingRange(mappingStart, mappingEnd);
}

void MainWindow::onSymmetricalChanged(bool checked)
{
    // Enable/disable right outline controls based on symmetrical checkbox
    ui->groupBox_8->setEnabled(!checked);  // R_P1 group
    ui->groupBox_9->setEnabled(!checked);  // R_P2 group
    
    // Update feather symmetry setting (only if feather object exists)
    if (m_gl && m_gl->getFeather()) {
        m_gl->getFeather()->setOutlineSymmetric(checked);
    }
}
