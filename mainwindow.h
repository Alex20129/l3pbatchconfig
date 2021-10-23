#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QUrl>
#include <QtNetwork/QtNetwork>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
signals:
    void deviceSettingsObtained();
    void deviceRebootRequested();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void PerformGETrequest(QHostAddress host, QString path, QUrlQuery *query=nullptr);
    void PerformPOSTrequest(QHostAddress host, QString path, QByteArray *dataToSend);

    QString UserName, Password;
    QVector <QHostAddress> HostsToSetup;
    int WEBPort;
public slots:
    void uploadSettings();
    void rebootDevices();
    void onAuthenticationNeeded(QNetworkReply *reply, QAuthenticator *authenticator);
    void onNetManagerFinished(QNetworkReply *reply);
private slots:
    void on_applyButton_clicked();
    void on_rebootButton_clicked();
    void on_progressBar_valueChanged(int value);
private:
    QNetworkAccessManager *netManager;
    QStringList NewDeviceSettings;
    uint ActiveThreadsNum;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
