#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "IVHDx.h"
#include <QMainWindow>
#include <QString>
#include <QLibrary>
#include <QStandardItemModel>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    //Read点击事件
    void eventButtonClickRead();
    //ReadFileSelect点击事件
    void eventButtonClickReadFileSelect();
    //Write点击事件
    void eventButtonClickWrite();
    //WriteFileSelect点击事件
    void eventButtonClickWriteFileSelect();
    //VHDFileSelect点击事件
    void eventButtonClickVHDFileSelect();
    //按钮点击事件槽
    Q_SLOT void slotButtonClickEvent();

private:
    //连接按钮点击信号
    void connectButtonClickSignal();
    //初始化属性表
    void initPropertiesView();

private:
    //初始化窗体
    void initMainWindow();

private:
    //窗体关闭事件
    virtual void closeEvent(QCloseEvent* pCloseEvent) override;
    //窗体显示事件
    virtual void showEvent(QShowEvent* pShowEvent) override;


private:
    //VHDx
    IVHDx* m_pVHDx;
    QString m_strVHDx;

private:
    QLibrary* m_pLibrary;
    //属性表头
    QStandardItemModel* m_pStandardItemModel;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
