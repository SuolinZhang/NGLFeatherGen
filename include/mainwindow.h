#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "NGLScene.h"
#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    
    // Slots for buttons
    void onResetClicked();
    void onRenderClicked();
    
    // Slot for tab changes
    void onTabChanged(int index);
    
    // Slot for outline controls
    void onSymmetricalChanged(bool checked);

private:
    void updateRachis();
    void updateOutlines();
    void updateBarbs();
    void updateAllFeather();
    void setupConnections();
    
    Ui::MainWindow *ui;
    NGLScene *m_gl;
};

#endif // MAINWINDOW_H
