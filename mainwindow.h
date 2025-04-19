#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QToolButton>
#include <QToolBar>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QtMultimedia/QMediaPlayer>
#include <QWidget>
#include <QMessageBox>  
#include <QAudioOutput>
#include <QFile>
#include <QLineEdit>
#include <QStringList>
#include <QDir>
#include <QListWidget>
#include <QMediaPlaylist>
#include <QLabel>
#include <QSlider>
#include <QRadioButton>
#include <QButtonGroup>
#include <QComboBox>
#include <QMediaMetaData>
#include <QStatusBar>
#include "qcustomplot.h"
#include <QVideoWidget>
#include <QGraphicsVideoItem>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPalette white;
    QBrush whiteB;
    QPushButton *menuButton;
    QMenu *settings,*settings0;     //菜单
    QMenuBar *mb;                   //菜单栏
    QAction *setting1,*setting2,*setting3,*setting4;    //设置菜单动作
    QToolButton *PlayButton;        //播放按钮
    QToolButton *StopButton;        //停止按钮
    QToolButton *PreButton;         //上一首
    QToolButton *NextButton;        //下一首
    QPushButton *FileButton;        //添加文件
    QPushButton *SlienceButton;     //静音
    QPushButton *RemoveButton;      //移除文件
    QPushButton *ClearButton;       //清空文件
    QAudioOutput *output;           //pcm/wav音频输出
    QFile SaveFile;                 //用于每次打开保存的文件路径
    QFile OpenFile;                 //打开播放的文件路径
    QLineEdit *lineEdit;            //文件文本框
    QMediaPlayer *Player;           //普通音频输出
    QMediaPlaylist *PlayList;       //音频列表
    QListWidget *ListWidget;        //列表窗口
    QString durationTime;
    QString positionTime;
    QLabel *CurMedia;               //进度时间
    QSlider *sliderPosition;        //进度条
    QSlider *sliderVolumn;          //音量条
    QWidget *set1;                  //输出设置窗口
    QWidget *set2;                  //主题设置窗口
    QWidget *set3;                  //使用说明窗口
    QWidget *set4;                  //关于窗口
    QTextBrowser *HelpText;
    QTextBrowser *AboutText;
    QLabel *Icon;
    QLabel *OutputModeSelect;       //输出模式选择
    QLabel *attenton1;              //注意事项
    QLabel *attenton2;
    QLabel *attenton3;
    QLabel *attenton4;
    QPushButton *ModeSelect;        //输出模式选择
    QLabel *nowMode;                //现在的播放模式
    QPushButton *PlaybackMode;      //播放顺序选择
    QComboBox *SampleRate;          //设置采样率
    QComboBox *ChannelCount;        //设置音频通道
    QComboBox *SampleSize;          //设置采样位数
    QLabel *sLabel1;                //Combobox的标签
    QLabel *sLabel2;
    QLabel *sLabel3;
    QLabel *Hz;                     //赫兹
    QLabel *tip1;
    void setBackground(QImage);
    QComboBox *background;          //设置预选的背景图片
    QPushButton *setbg;             //自定义背景图片
    QStatusBar *StatusBar;          //状态栏
    QLabel *status1;                //状态栏显示项
    QLabel *status2;
    QLabel *status3;
    QLabel *status4;
    QLabel *setImage;                  //显示图片
    QCustomPlot *customPlot;           //音频可视化
    QVideoWidget *VideoWidget;              //播放MV视频窗口
    QListWidget *VideoList;            //MV视频列表
    QPushButton *VideoButton;          //选取视频按钮
    QMediaPlayer *VideoPlayer;
    QMediaPlaylist *VideoPlayList;
    QLabel *CurMedia1;
    QPushButton *closeVideo;           //关闭MV视频
    QPushButton *upScaleVideo;         //视频放大
    QListWidgetItem *itemN;


protected slots:
    void onStateChanged1(QAudio::State state);

private slots:
    void on_PlayButton_clicked();       //播放音频
    void on_PreButton_clicked();        //上一首
    void on_NextButton_clicked();       //下一首
    void on_SlienceButton_clicked();    //静音
    void on_FileButton_clicked();       //获取文件
    void on_RemoveButton_clicked();     //移除文件
    void on_ClearButton_clicked();      //清空文件
    void on_PlaybackMode_clicked();     //切换播放顺序
    void onPlaylistChanged(int position);       //播放列表文件变化时发射
    void onDurationChanged(qint64 duration);    //更新文件的时间长度显示
    void onPositionChanged(qint64 position);    //在当前文件播放为变化时发射
    void on_sliderPosition_sliderMoved(int value);      //进度条
    void on_sliderVolumn_sliderMoved(int value);        //音量
    void on_listWidget_doubleClicked(const QModelIndex &);
    void on_setting1_triggered();
    void on_ModeSelect_clicked();                       //输出模式切换
    void on_Combobox1_changed(int index);               //选项改变
    void on_Combobox2_changed(int index);
    void on_Combobox3_changed(int index);
    void on_Combobox4_changed(int index);
    void on_setting2_triggered();
    void on_setting3_triggered();
    void on_setting4_triggered();
    void on_setbg_clicked();                            //自定义背景图片
    void on_VideoButton_clicked();                      //打开MV视频
    void on_VideoList_doubleClicked(const QModelIndex &);
    void on_closeVideo_clicked();                       //关闭MV视频
    void on_upScaleVideo_clicked();                       //放大视频
};

#endif // MAINWINDOW_H
