#include <QApplication>
#include <QDir>
#include <QFile>
#include <QWidget>
#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Appenders/ConsoleAppender.h>
#include "repositories/DatabaseManager.h"
#include "utils/SessionManager.h"
#include "views/LoginDialog.h"
#include "views/MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 初始化 plog（日志文件 + 控制台）
    QDir().mkpath("logs");
    static plog::RollingFileAppender<plog::TxtFormatter> fileAppender(
        "logs/app.log", 1024 * 1024 * 5, 3);
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &fileAppender).addAppender(&consoleAppender);

    PLOG_INFO << "========== 设备管理系统启动 ==========";

    // 初始化数据库
    DatabaseManager& dbManager = DatabaseManager::instance();
    if (!dbManager.initialize()) {
        PLOG_FATAL << "数据库初始化失败";
        return -1;
    }

    //// 显示登录窗口
    LoginDialog loginDlg;
    //if (loginDlg.exec() != QDialog::Accepted) {
    //    PLOG_INFO << "用户取消登录，程序退出";
    //    return 0;
    //}

    //// 保存当前用户到 Session (保留一套即可)
    //User user = loginDlg.getUser();
    //bool isAdmin = (user.role == "admin");
    //SessionManager::getInstance().setCurrentUser(user.username, isAdmin);

    //PLOG_INFO << "当前用户: " << SessionManager::getInstance().getCurrentUser()
    //          << " (管理员: " << (SessionManager::getInstance().isAdmin() ? "是" : "否") << ")";

    Mainwindow window;
    window.show();

    PLOG_INFO << "进入主窗口";

    return app.exec();
}
