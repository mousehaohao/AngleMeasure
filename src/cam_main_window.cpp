#include "cam_main_window.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>
#include <QSettings>
#include "event_label.h"
#include "cam_main_widget.h"

CAMMainWindow::CAMMainWindow(QWidget *parent)
	: QMainWindow(parent) {
	ui.setupUi(this);

	this->setWindowTitle(QStringLiteral("接触角测量 http://www.coronalab.com/"));
	this->setWindowIcon(QIcon(":/CAMMainWindow/Resources/kenny.jpg"));  

	open_image_action_ = new QAction(QIcon(":/CAMMainWindow/Resources/file_open.png"), tr("&Open..."), this);
	open_image_action_->setShortcuts(QKeySequence::Open);
	open_image_action_->setStatusTip(QStringLiteral("打开一个文件"));
	connect(open_image_action_, &QAction::triggered, this, &CAMMainWindow::OpenImageFile);
	//
	save_image_action_ = new QAction(QIcon(":/CAMMainWindow/Resources/file_save.png"), tr("&Save..."), this);
	save_image_action_->setShortcuts(QKeySequence::Save);
	save_image_action_->setStatusTip(QStringLiteral("保存图像"));
	connect(save_image_action_, &QAction::triggered, this, &CAMMainWindow::SaveImageFile);

	//
	QMenu* menu_open_file = menuBar()->addMenu(tr("&File"));
	menu_open_file->addAction(open_image_action_);
	menu_open_file->addAction(save_image_action_);
	//
	QToolBar* tool_bar = addToolBar(tr("&File"));
	tool_bar->addAction(open_image_action_);
	tool_bar->addAction(save_image_action_);
	tool_bar->setFloatable(false);
	tool_bar->setMovable(false);
	//
	CAMMainWidget* main_widget = new CAMMainWidget(this);
	QObject::connect(this, &CAMMainWindow::SIGSelectImageFileComplete,
		main_widget,&CAMMainWidget::OnSIGSelectImageFileComplete);

	this->setCentralWidget(main_widget);

	statusBar();
}

void CAMMainWindow::SaveImageFile() {
	//

}

void CAMMainWindow::OpenImageFile() {
	const QString kLastOpenDir(".");
	QSettings my_settings;


	QString open_file_path = QFileDialog::getOpenFileName(this,tr("Open File"),
		my_settings.value(kLastOpenDir).toString(),
		tr("*.*"));
	//
	if(open_file_path.isEmpty()) {
		return;
	}
	//
	QDir current_dir;
	my_settings.setValue(kLastOpenDir, current_dir.absoluteFilePath(open_file_path));
	//
	emit SIGSelectImageFileComplete(open_file_path);
	return;
}

CAMMainWindow::~CAMMainWindow() {

}