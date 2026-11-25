#include "LoginDialog.h"
#include "../repositories/UserRepository.h"
#include "../utils/PasswordHasher.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <plog/Log.h>


LoginDialog::LoginDialog(QWidget* parent)
	: QDialog(parent)
{
	setupUi();
	initDefaultAdmin();
}

LoginDialog::~LoginDialog()
{
}

void LoginDialog::setupUi()
{
	setWindowTitle("device system manager - login");
	setFixedSize(350, 220);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setSpacing(10);
	//mainLayout->setDpacing(15);
	mainLayout->setContentsMargins(30, 30, 30, 30);

	// 标题
	m_lblTitle = new QLabel("设备管理系统", this);
	m_lblTitle->setAlignment(Qt::AlignCenter);
	m_lblTitle->setStyleSheet("font-size: 18px; font-weight: bold; padding: 10px;");
	mainLayout->addWidget(m_lblTitle);

	// 用户名
	QHBoxLayout* userLayout = new QHBoxLayout();
	QLabel* lblUser = new QLabel("用户名:", this);
	lblUser->setFixedWidth(60);
	m_txtUsername = new QLineEdit(this);
	m_txtUsername->setPlaceholderText("请输入用户名");
	userLayout->addWidget(lblUser);
	userLayout->addWidget(m_txtUsername);
	mainLayout->addLayout(userLayout);

	// 密码
	QHBoxLayout* passLayout = new QHBoxLayout();
	QLabel* lblPass = new QLabel("密码:", this);
	lblPass->setFixedWidth(60);
	m_txtPassword = new QLineEdit(this);
	m_txtPassword->setPlaceholderText("请输入密码");
	m_txtPassword->setEchoMode(QLineEdit::Password);
	passLayout->addWidget(lblPass);
	passLayout->addWidget(m_txtPassword);
	mainLayout->addLayout(passLayout);

	// 按钮
	QHBoxLayout* buttonLayout = new QHBoxLayout();
	m_btnLogin = new QPushButton("登录", this);
	m_btnCancel = new QPushButton("取消", this);
	m_btnLogin->setFixedWidth(100);
	m_btnCancel->setFixedWidth(100);
	buttonLayout->addStretch();
	buttonLayout->addWidget(m_btnLogin);
	buttonLayout->addWidget(m_btnCancel);
	mainLayout->addStretch();
	mainLayout->addLayout(buttonLayout);

	// 回车登录
	m_btnLogin->setDefault(true);

	// 信号连接
	connect(m_btnLogin, &QPushButton::clicked, this, &LoginDialog::onLogin);
	connect(m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

void LoginDialog::initDefaultAdmin()
{
	// 检查是否存在管理员账号，不存在则创建默认账号
	UserRepository repo;
	User admin = repo.findByUsername("admin");

	if (!admin.isValid()) {
		admin.username = "admin";
		admin.passwordHash = PasswordHasher::hashPassword("admin123");
		admin.displayName = "系统管理员";
		admin.role = "admin";

		if (repo.create(admin)) {
			PLOG_INFO << "创建默认管理员账号: admin / admin123";
		}
	}
}

void LoginDialog::onLogin() {
	QString username = m_txtUsername->text().trimmed();
	QString password = m_txtPassword->text();

	if (username.isEmpty() || password.isEmpty()) {
		QMessageBox::warning(this, "notice", "username and password not null");
		return;
	}

	UserRepository reop;
	User user = reop.findByUsername(username);

	if (!user.isValid()) {
		QMessageBox::warning(this, "登录失败", "用户名不存在");
		PLOG_WARNING << "登录失败: 用户不存在 - " << username.toStdString();
		return;
	}

	if (!PasswordHasher::verifyPassword(password, user.passwordHash)) {
		QMessageBox::warning(this, "登录失败", "密码错误");
		PLOG_WARNING << "登录失败: 密码错误 - " << username.toStdString();
		return;
	}

	m_user = user;
	PLOG_INFO << "用户登录成功: " << user.username.toStdString()
		<< " (" << user.displayName.toStdString() << ")";

	accept();
}

User LoginDialog::getUser() const
{
	return m_user;
}

