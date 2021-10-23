#include "main.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ActiveThreadsNum=0;
    WEBPort=80;
    UserName=QString("root");
    Password=QString("root");
    netManager=new QNetworkAccessManager;

    connect(netManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(onNetManagerFinished(QNetworkReply *)));
    connect(netManager, SIGNAL(authenticationRequired(QNetworkReply *, QAuthenticator *)), this, SLOT(onAuthenticationNeeded(QNetworkReply *, QAuthenticator *)));
    connect(this, SIGNAL(deviceSettingsObtained()), this, SLOT(uploadSettings()));
    connect(this, SIGNAL(deviceRebootRequested()), this, SLOT(rebootDevices()));

    ui->setupUi(this);
    this->setWindowTitle(QString(PROGRAM_NAME " " PROGRAM_VERSION));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::uploadSettings()
{
    if(HostsToSetup.isEmpty())
    {
        fprintf(stdout, "Host list is empty =/\nnothing will be done.");
        return;
    }
    QByteArray DataToSend;
    int setting;
    for(int host=0; host<HostsToSetup.count(); host++)
    {
        DataToSend.clear();
        for(setting=0; setting<NewDeviceSettings.count(); setting++)
        {
            if(NewDeviceSettings.at(setting).split('=').first()==QString("pool1user"))
            {
                DataToSend.append(QString("_ant_pool1user="));
                if(2==NewDeviceSettings.at(setting).split('=', QString::SkipEmptyParts).count())
                {
                    DataToSend.append(NewDeviceSettings.at(setting).split('=').last()+
                                      QString(".")+
                                      QString::number((HostsToSetup.at(host).toIPv4Address()>>24)&0xff)+
                                      QString("x")+
                                      QString::number((HostsToSetup.at(host).toIPv4Address()>>16)&0xff)+
                                      QString("x")+
                                      QString::number((HostsToSetup.at(host).toIPv4Address()>>8)&0xff)+
                                      QString("x")+
                                      QString::number((HostsToSetup.at(host).toIPv4Address())&0xff));
                }
                DataToSend.append(" ");
            }
            else if(NewDeviceSettings.at(setting).split('=').first()==QString("pool2user"))
            {
                DataToSend.append(QString("_ant_pool2user="));
                if(2==NewDeviceSettings.at(setting).split('=', QString::SkipEmptyParts).count())
                {
                    DataToSend.append(NewDeviceSettings.at(setting).split('=').last()+
                                      QString(".")+
                                      QString::number((HostsToSetup.at(host).toIPv4Address()>>24)&0xff)+
                                      QString("x")+
                                      QString::number((HostsToSetup.at(host).toIPv4Address()>>16)&0xff)+
                                      QString("x")+
                                      QString::number((HostsToSetup.at(host).toIPv4Address()>>8)&0xff)+
                                      QString("x")+
                                      QString::number((HostsToSetup.at(host).toIPv4Address())&0xff));
                }
                DataToSend.append(" ");
            }
            else if(NewDeviceSettings.at(setting).split('=').first()==QString("pool3user"))
            {
                DataToSend.append(QString("_ant_pool3user="));
                if(2==NewDeviceSettings.at(setting).split('=', QString::SkipEmptyParts).count())
                {
                    DataToSend.append(NewDeviceSettings.at(setting).split('=').last()+
                                      QString(".")+
                                      QString::number((HostsToSetup.at(host).toIPv4Address()>>24)&0xff)+
                                      QString("x")+
                                      QString::number((HostsToSetup.at(host).toIPv4Address()>>16)&0xff)+
                                      QString("x")+
                                      QString::number((HostsToSetup.at(host).toIPv4Address()>>8)&0xff)+
                                      QString("x")+
                                      QString::number((HostsToSetup.at(host).toIPv4Address())&0xff));
                }
                DataToSend.append(" ");
            }
            else
            {
                DataToSend.append(QString("_ant_")+NewDeviceSettings.at(setting)+QString(" "));
            }
        }
        PerformPOSTrequest(HostsToSetup.at(host), "/cgi-bin/set_miner_conf.cgi", &DataToSend);
    }
}

void MainWindow::rebootDevices()
{
    if(HostsToSetup.isEmpty())
    {
        fprintf(stdout, "Host list is empty =/\nnothing will be done.");
        return;
    }
    for(int host=0; host<HostsToSetup.count(); host++)
    {
        PerformGETrequest(HostsToSetup.at(host), "/cgi-bin/reboot.cgi");
    }
}

void MainWindow::PerformGETrequest(QHostAddress host, QString path, QUrlQuery *query)
{
    QUrl deviceURL;
    deviceURL.setScheme("http");
    deviceURL.setHost(host.toString());
    deviceURL.setPort(this->WEBPort);
    deviceURL.setUserName(this->UserName);
    deviceURL.setPassword(this->Password);
    deviceURL.setPath(path);
    if(query)
    {
        if(!query->isEmpty())
        {
            deviceURL.setQuery(*query);
        }
    }
    QNetworkRequest ExecRequest;
    ExecRequest.setUrl(deviceURL);
    ExecRequest.setHeader(QNetworkRequest::UserAgentHeader, API_HEADER_USER_AGENT);
    fprintf(stdout, "sending GET request to\n%s\n", deviceURL.toString().toStdString().data());
    ActiveThreadsNum++;
    while(ActiveThreadsNum>ACTIVE_THREADS_MAX_NUM)
    {
         QApplication::processEvents();
    }
    netManager->get(ExecRequest);
}

void MainWindow::PerformPOSTrequest(QHostAddress host, QString path, QByteArray *dataToSend)
{
    QUrl deviceURL;
    deviceURL.setScheme("http");
    deviceURL.setHost(host.toString());
    deviceURL.setPort(this->WEBPort);
    deviceURL.setUserName(this->UserName);
    deviceURL.setPassword(this->Password);
    deviceURL.setPath(path);
    QNetworkRequest setingsRequest;
    setingsRequest.setUrl(deviceURL);
    setingsRequest.setHeader(QNetworkRequest::UserAgentHeader, API_HEADER_USER_AGENT);
    setingsRequest.setHeader(QNetworkRequest::ContentTypeHeader, API_HEADER_CONTENT_TYPE);
    fprintf(stdout, "sending POST request to\n%s\n", deviceURL.toString().toStdString().data());
    ActiveThreadsNum++;
    while(ActiveThreadsNum>ACTIVE_THREADS_MAX_NUM)
    {
         QApplication::processEvents();
    }
    netManager->post(setingsRequest, *dataToSend);
}

void MainWindow::onAuthenticationNeeded(QNetworkReply *reply, QAuthenticator *authenticator)
{
    if(reply->error()==QNetworkReply::NoError)
    {
        //AppLogger->Log("onAuthenticationNeeded reply success");
    }
    else
    {
        //AppLogger->Log("onAuthenticationNeeded reply error");
        //AppLogger->Log(reply->errorString());
    }
    authenticator->setPassword(Password);
    authenticator->setUser(UserName);
}

void MainWindow::onNetManagerFinished(QNetworkReply *reply)
{
    QByteArray ReceivedData;
    fprintf(stdout, "%s", reply->url().toString().toStdString().data());
    if(reply->error()==QNetworkReply::NoError)
    {
        fprintf(stdout, " [success]\n");
    }
    else
    {
        fprintf(stdout, " [error]\n");
    }
    QVariant statusCode=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
    {
        fprintf(stdout, "HttpStatusCode=%u\n", statusCode.toUInt());
    }
    if(reply->isReadable())
    {
        ReceivedData=reply->readAll();
        /*
        fprintf(stdout, "device replied:\n");
        fprintf(stdout, ReceivedData.toStdString().data());
        fprintf(stdout, "\n");
        */
    }
    reply->deleteLater();
    ui->progressBar->setValue(ui->progressBar->value()+1);
    ActiveThreadsNum--;
}

void MainWindow::on_applyButton_clicked()
{
    NewDeviceSettings.clear();
    NewDeviceSettings.append(QString("pool1url=")+ui->pool1url->text());
    NewDeviceSettings.append(QString("pool1user=")+ui->pooluser->text());
    NewDeviceSettings.append(QString("pool1pw=")+ui->poolpassword->text());
    NewDeviceSettings.append(QString("pool2url=")+ui->pool2url->text());
    NewDeviceSettings.append(QString("pool2user=")+ui->pooluser->text());
    NewDeviceSettings.append(QString("pool2pw=")+ui->poolpassword->text());
    NewDeviceSettings.append(QString("pool3url=")+ui->pool3url->text());
    NewDeviceSettings.append(QString("pool3user=")+ui->pooluser->text());
    NewDeviceSettings.append(QString("pool3pw=")+ui->poolpassword->text());
    NewDeviceSettings.append(QString("nobeeper=")+QString(ui->nobeeper->isChecked() ? "true" : "false"));
    NewDeviceSettings.append(QString("notempoverctrl=")+QString(ui->notempoverctrl->isChecked() ? "true" : "false"));
    NewDeviceSettings.append(QString("fan_mode=")+QString::number(ui->fan_mode->currentIndex()));
    NewDeviceSettings.append(QString("fan_customize_value=")+QString::number(ui->fan_customize_value->text().toUInt()));
    NewDeviceSettings.append(QString("freq=")+QString::number(ui->freq->text().toUInt()));
    int volt=255-ui->voltage->text().toInt()*10;
    if(volt<0){volt=0;}
    if(volt>255){volt=255;}
    NewDeviceSettings.append(QString("voltage=")+QString::number(volt));
    HostsToSetup.clear();
    for(uint addr=QHostAddress(ui->ipstart->text()).toIPv4Address(); addr<=QHostAddress(ui->ipend->text()).toIPv4Address(); addr++)
    {
        HostsToSetup.append(QHostAddress(addr));
    }
    ui->progressBar->setMaximum(HostsToSetup.count());
    ui->progressBar->setValue(0);
    ui->applyButton->setEnabled(false);
    ui->rebootButton->setEnabled(false);
    emit(deviceSettingsObtained());
}

void MainWindow::on_rebootButton_clicked()
{
    HostsToSetup.clear();
    for(uint addr=QHostAddress(ui->ipstart->text()).toIPv4Address(); addr<=QHostAddress(ui->ipend->text()).toIPv4Address(); addr++)
    {
        HostsToSetup.append(QHostAddress(addr));
    }
    ui->progressBar->setMaximum(HostsToSetup.count());
    ui->progressBar->setValue(0);
    ui->applyButton->setEnabled(false);
    ui->rebootButton->setEnabled(false);
    emit(deviceRebootRequested());
}

void MainWindow::on_progressBar_valueChanged(int value)
{
    if(value==ui->progressBar->maximum())
    {
        ui->applyButton->setEnabled(true);
        ui->rebootButton->setEnabled(true);
        ui->progressBar->setValue(0);
    }
}
