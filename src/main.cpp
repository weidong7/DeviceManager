#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDir>
#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Appenders/ConsoleAppender.h>
#include "repositories/DatabaseManager.h"
#include "tests/RepositoryTests.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 初始化 plog（日志文件 + 控制台输出）
    QDir().mkpath("logs");
    static plog::RollingFileAppender<plog::TxtFormatter> fileAppender("logs/app.log", 1024 * 1024 * 5, 3);
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &fileAppender).addAppender(&consoleAppender);

    PLOG_INFO << "========== 设备管理系统启动 ==========";

    // 初始化数据库
    DatabaseManager &dbManager = DatabaseManager::instance();
    if (!dbManager.initialize()) {
        PLOG_FATAL << "数据库初始化失败";
        return -1;
    }

    // 运行测试（临时，后续Phase 4会移除）
    RepositoryTests::runAll();

    // 主窗口（临时占位，Phase 4会替换）
    QWidget window;
    window.setWindowTitle("设备管理系统");
    window.resize(800, 600);

    QVBoxLayout *layout = new QVBoxLayout(&window);

    QLabel *label = new QLabel("数据库已初始化完成\n\n检查控制台日志查看测试结果");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    QPushButton *btnClose = new QPushButton("关闭");
    layout->addWidget(btnClose);
    QObject::connect(btnClose, &QPushButton::clicked, &window, &QWidget::close);

    window.show();

    return app.exec();
}
