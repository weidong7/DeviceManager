#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "../models/User.h"

class QLineEdit;
class QPushButton;
class QLabel;

class LoginDialog : public QDialog
{
	Q_OBJECT

public:
	explicit LoginDialog(QWidget* parent = nullptr);
	~LoginDialog();

	// 获取登录的用户（登录成功后调用）
	User getUser() const;

private slots:
	void onLogin();

private:
	void setupUi();
	void initDefaultAdmin();  // 初始化默认管理员账号

	User m_user;

	QLabel* m_lblTitle;
	QLineEdit* m_txtUsername;
	QLineEdit* m_txtPassword;
	QPushButton* m_btnLogin;
	QPushButton* m_btnCancel;
};

#endif // LOGINDIALOG_H
