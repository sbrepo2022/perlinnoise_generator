#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->createTextureButton, SIGNAL(clicked()), this, SLOT(createTexture()));
    connect(ui->saveTextureButton, SIGNAL(clicked()), this, SLOT(saveTexture()));
    connect(ui->saveLandscapeButton, SIGNAL(clicked()), this, SLOT(saveLandscape()));
    srand(time(nullptr));

    createTexture();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createTexture() {
    int width = ui->widthSpinBox->value();
    int height = ui->heightSpinBox->value();
    int quality = ui->qualitySpinBox->value();
    int interpolation = ui->interpolationComboBox->currentIndex();
    int seed = 0;

    float streight = ui->streihtSpinBox->value();

    if (ui->randomSeedCheckBox->isChecked()) {
        seed = rand() % 65536;
        ui->seedSpinBox->setValue(seed);
    }
    else {
        seed = ui->seedSpinBox->value();
    }

    generator.initNoise(width, height, quality, seed, interpolation);
    this->HeightMap = generator.getNoiseTexture();
    ui->heightmapOutputLabel->setPixmap(QPixmap::fromImage(this->HeightMap));

    view3d.createLandscape(this->HeightMap, 1.0f, streight);
}

void MainWindow::saveTexture() {
    QString path = QFileDialog::getSaveFileName(0, "Export heightmap", "", "*.bmp;;*.png;;*.jpg;;*.jpeg");
    if (path != "")
      this->HeightMap.save(path);
}

void MainWindow::saveLandscape() {
    float scale = ui->scaleSpinBox->value();
    QString path = QFileDialog::getSaveFileName(0, "Export landscape", "", "*.obj");
    if (path != "")
      exportLandscape(path, scale);
}
