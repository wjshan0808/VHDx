#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QPushButton>
#include <QStringList>
#include <QList>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QByteArray>
#include <QDir>
#include <QCoreApplication>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_pVHDx(nullptr)
    , m_strVHDx("VHDX.dll")
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化窗体
    this->initMainWindow();
}
MainWindow::~MainWindow()
{
    delete ui;
}




//Read点击事件
void MainWindow::eventButtonClickRead()
{
    //检查
    if(nullptr == m_pVHDx)
    {
        return;
    }

    //获取LBA
    bool bIsOkLBA = false;
    unsigned int uiLBA = ui->cmbLBAReadStart->currentText().toUInt(&bIsOkLBA, 10);
    if(!bIsOkLBA)
    {
        //LBA消息
        ui->statusbar->showMessage("invalid 'LBA' value.");
        return;
    }

    //获取读取数据大小
    bool bIsOkDataSize = false;
    long long llDataSize = ui->txtReadDataSize->text().toUInt(&bIsOkDataSize, 10);
    if((!bIsOkDataSize)
        || (0x00 > llDataSize))
    {
        //LBA消息
        ui->statusbar->showMessage("invalid 'Size' value.");
        return;
    }

    //读取数据
    char* szDataBuffer = new char[llDataSize + 0x01]();
    long long llDataSizeRead = m_pVHDx->Read(uiLBA, szDataBuffer, llDataSize);

    //数据显示
    ui->txtReadData->insertPlainText(QString(szDataBuffer));

    //状态栏消息
    QString strStatusMessage = QString::asprintf("Read '%lld/%lld' data, from '%d' LBA.", llDataSizeRead, llDataSize, uiLBA);

    //写入文件
    if(!ui->txtReadFile->text().isEmpty())
    {
        QFile qFile(ui->txtReadFile->text());
        if(qFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            qFile.write(szDataBuffer, llDataSizeRead);
            qFile.close();
        }
        else
        {
            //状态栏消息
            strStatusMessage = QString::asprintf("open '%s' failure.", qFile.fileName().toStdString().c_str());
        }
    }

    //释放资源
    delete[] szDataBuffer;

    //状态栏消息
    ui->statusbar->showMessage(strStatusMessage);

}
//ReadFileSelect点击事件
void MainWindow::eventButtonClickReadFileSelect()
{
    //保存文件
    QString strFileName("");
    strFileName = QFileDialog::getSaveFileName(this,
                                 "Save File",
                                 "",
                                 "Any files (*)");
    //文件路劲
    ui->txtReadFile->setText(strFileName);
}
//Write点击事件
void MainWindow::eventButtonClickWrite()
{
    //检查
    if(nullptr == m_pVHDx)
    {
        return;
    }

    //获取LBA
    bool bIsOkLBA = false;
    unsigned int uiLBA = ui->cmbLBAWriteStart->currentText().toUInt(&bIsOkLBA, 10);
    if(!bIsOkLBA)
    {
        //LBA消息
        ui->statusbar->showMessage("invalid 'LBA' value.");
        return;
    }

    //文件级别优先, 其次是数据区
    long long llDataSize = 0x00;
    long long llDataSizeWrite = 0x00;

    //数据文件存在
    if(QFile::exists(ui->txtWriteFile->text()))
    {
        //读取文件数据
        QFile qFile(ui->txtWriteFile->text());
        if(qFile.open(QIODevice::ReadOnly))
        {
            llDataSize = qFile.bytesAvailable();
            QByteArray qFileByteArray = qFile.readAll();

            //qDebug("%lld/%d", llDataSize, qFileByteArray.size());

            //写入文件数据
            llDataSizeWrite = m_pVHDx->Write(uiLBA, qFileByteArray.constData(), llDataSize);
            qFile.close();
        }
        else
        {
            //状态栏消息
            ui->statusbar->showMessage(QString::asprintf("open '%s' failure.", qFile.fileName().toStdString().c_str()));
            return;
        }
    }
    //数据区有数据
    else if(!ui->txtWriteData->toPlainText().isEmpty())
    {
        //读数据区数据
        QString strData = ui->txtWriteData->toPlainText();
        llDataSize = strData.size();

        //写入文件数据
        llDataSizeWrite = m_pVHDx->Write(uiLBA, strData.toStdString().c_str(), llDataSize);
    }
    else
    {
        return;
    }

    //状态栏消息
    ui->statusbar->showMessage(QString::asprintf("Write '%lld/%lld' data, from '%d' LBA.", llDataSizeWrite, llDataSize, uiLBA));

}
//WriteFileSelect点击事件
void MainWindow::eventButtonClickWriteFileSelect()
{
    //选择文件
    QString strFileName("");
    strFileName = QFileDialog::getOpenFileName(this,
                                 "Select File",
                                 "",
                                 "Any files (*)");
    //文件路劲
    ui->txtWriteFile->setText(strFileName);

}
//VHDFileSelect点击事件
void MainWindow::eventButtonClickVHDFileSelect()
{
    //打开文件
    QString strFileName = QFileDialog::getOpenFileName(this,
                                 "Open File",
                                 "",
                                 "VHD files (*.vhd *.vhdx);;Any files (*)");
    //文件路劲
    ui->txtVHDFile->setText(strFileName);

    //操作选择的VHD文件
    if (nullptr != m_pVHDx)
    {
        //关闭VHD文件
        m_pVHDx->CloseVHD();

        //打开VHD文件
        m_pVHDx->OpenVHD(strFileName.toStdString().c_str());

        //读取VHD文件Footer信息
        m_pVHDx->ReadFooterInfo();

        //获取VHD文件Footer信息
        const char* szFooterInfo = m_pVHDx->ToString();

        //显示VHD文件Footer信息
        if (nullptr != szFooterInfo)
        {
            //清除行
            QStandardItem* pRootItem = this->m_pStandardItemModel->invisibleRootItem();
            pRootItem->removeRows(0x00, pRootItem->rowCount());

            //添加行
            QStringList lstFooterInfo = QString(szFooterInfo).split("\r\n");
            foreach(const QString& item, lstFooterInfo)
            {
                QStringList lstItem = item.split(": ");
                if(0x01 == lstItem.count())
                {
                    pRootItem->appendRow(new QStandardItem(lstItem.first()));
                }
                if(0x02 == lstItem.count())
                {
                    pRootItem->appendRow(//new QList<QStandardItem*>
                                         {
                                             new QStandardItem(lstItem.first()),
                                             new QStandardItem(lstItem.last()),
                                         });
                }
            }
        }
        //
    }
    //
}
//按钮点击事件槽
void MainWindow::slotButtonClickEvent()
{
    //检查
    QPushButton* pButton = qobject_cast<QPushButton*>(sender());
    if(nullptr == pButton)
    {
        return;
    }
    //qDebug("%s", qPrintable(pButton->objectName()));

    //按钮
    if(ui->btnRead == pButton)
    {
        eventButtonClickRead();
    }
    if(ui->btnReadFileSelect == pButton)
    {
        eventButtonClickReadFileSelect();
    }
    if(ui->btnWrite == pButton)
    {
        eventButtonClickWrite();
    }
    if(ui->btnWriteFileSelect == pButton)
    {
        eventButtonClickWriteFileSelect();
    }
    if(ui->btnVHDFileSelect == pButton)
    {
        eventButtonClickVHDFileSelect();
    }
}

//连接按钮点击信号
void MainWindow::connectButtonClickSignal()
{
    //Read点击事件
    connect(ui->btnRead, SIGNAL(clicked()),
            this, SLOT(slotButtonClickEvent()));
    //ReadFileSelect点击事件
    connect(ui->btnReadFileSelect, SIGNAL(clicked()),
            this, SLOT(slotButtonClickEvent()));
    //Write点击事件
    connect(ui->btnWrite, SIGNAL(clicked()),
            this, SLOT(slotButtonClickEvent()));
    //WriteFileSelect点击事件
    connect(ui->btnWriteFileSelect, SIGNAL(clicked()),
            this, SLOT(slotButtonClickEvent()));
    //VHDFileSelect点击事件
    connect(ui->btnVHDFileSelect, SIGNAL(clicked()),
            this, SLOT(slotButtonClickEvent()));
}
//初始化属性表
void MainWindow::initPropertiesView()
{
    //设置列表
    this->m_pStandardItemModel->setHorizontalHeaderItem(0x00, new QStandardItem("Field"));
    this->m_pStandardItemModel->setHorizontalHeaderItem(0x01, new QStandardItem("Value"));

    //
    //QStandardItem* pRootItem = this->m_pStandardItemModel->invisibleRootItem();

    //应用到树表
    ui->treeVHDFileInfo->setModel(m_pStandardItemModel);
}

//初始化窗体
void MainWindow::initMainWindow()
{
    //初始化变量
    m_pLibrary = new QLibrary(this);
    m_pStandardItemModel = new QStandardItemModel(this);

    //初始化属性表
    this->initPropertiesView();
    //连接按钮点击信号
    this->connectButtonClickSignal();

}


//窗体关闭事件
void MainWindow::closeEvent(QCloseEvent*)
{
    //检查
    if (nullptr != m_pVHDx)
    {
        m_pVHDx->CloseVHD();
        m_pVHDx->Dispose();
    }

    //
    //accept();
}
//窗体显示事件
void MainWindow::showEvent(QShowEvent*)
{
    if(nullptr != m_pVHDx)
    {
        return;
    }

    //检查VHDX动态库文件
    QDir qDir(QCoreApplication::applicationDirPath());
    QString strVHDx = qDir.absoluteFilePath(m_strVHDx);
    if(!qDir.exists(strVHDx))
    {
        QMessageBox::warning(this, "Warning", "'" + strVHDx + "', not exist.");
        return;
    }

    //加载VHDX动态库文件
    m_pLibrary->setFileName(strVHDx);
    if(!m_pLibrary->load())
    {
        //0x000000c1 平台(x86,x64)不一致
        QMessageBox::critical(this, "Error"
                              , "Load '" + strVHDx + "' failure."
                              + "\r\n" + m_pLibrary->errorString());
        return;
    }

    //获取VHDx对象
    typedef IVHDx* (*pfnNewVHDx)(const char*);
    pfnNewVHDx funcNewVHDx = (pfnNewVHDx)m_pLibrary->resolve("NewVHDx");
    if (nullptr == funcNewVHDx)
    {
        QMessageBox::critical(this, "Error", "New VHDx object failure.\r\n" + m_pLibrary->errorString());
        return;
    }
    m_pVHDx = funcNewVHDx(nullptr);

    //状态栏消息
    ui->statusbar->showMessage("file '" + m_strVHDx + "' load.");

}

