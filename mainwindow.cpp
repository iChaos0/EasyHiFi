#include "mainwindow.h"
#include "qcustomplot.h"
#include "wavHead.h"

#include <QFileDialog>
#include <QWidget>
#include <QDebug>
#include <QString>
#include <QMediaMetaData>

int OutputMode = 1;
int SlienceCount = 1;
int ModeCount = 1;
int PlaybackCount = 1;
bool PlayorPause = false;
static QAudioFormat format;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    white.setColor(QPalette::WindowText,Qt::white);
    whiteB.setColor(Qt::white);

    //主窗口界面
    this->setGeometry(700,250,1400,840);        //设置主窗口位置和大小
    this->setFixedSize(1400,840);
    this->setWindowIcon(QIcon(":/new/icon/MainWindow.svg"));
    setBackground(QImage(":/new/mainwindow/bg1.png"));
    ListWidget = new QListWidget(this);
    ListWidget->setGeometry(500,145,800,445);
    ListWidget->setStyleSheet("background-color:transparent");
    ListWidget->setFrameShape(QListWidget::NoFrame);
    CurMedia = new QLabel(this);
    CurMedia->setGeometry(1050,720,80,30);

    setImage = new QLabel(this);
    setImage->setGeometry(50,170,400,400);

    nowMode = new QLabel(this);
    nowMode->setPalette(white);
    nowMode->setGeometry(200,660,150,30);
    nowMode->setText("现在是兼容模式");

    VideoWidget = new QVideoWidget;
    VideoWidget->setGeometry(1200,395,800,450);
    VideoWidget->setWindowFlags(Qt::FramelessWindowHint);
    VideoWidget->setAspectRatioMode(Qt::IgnoreAspectRatio);
    VideoWidget->setAttribute(Qt::WA_QuitOnClose,false);

    StatusBar = new QStatusBar(this);           //状态栏
    StatusBar = statusBar();
    StatusBar->setStyleSheet("QStatusBar::item{border: 250px}");
    StatusBar->setStyleSheet("background-color:white");
    status1 = new QLabel(this);
    status2 = new QLabel(this);
    status3 = new QLabel(this);
    status4 = new QLabel(this);
    StatusBar->addWidget(status1);
    StatusBar->addWidget(status2);
    StatusBar->addWidget(status3);
    StatusBar->addWidget(status4);


    //按钮
    PlayButton = new QToolButton(this);         //播放/暂停按钮
    PlayButton->setGeometry(150,715,90,90);
    PlayButton->setIconSize(QSize(90,90));
    PlayButton->setToolTip("播放/暂停");
    PlayButton->setStyleSheet("QToolButton{background-color:transparent;}");    //背景透明
    PlayButton->setIcon(QIcon(":/new/icon/Play.svg"));
    PlayButton->setShortcut(Qt::Key_F3);

    /*
    SuspendButton = new QToolButton(this);          //暂停按钮
    SuspendButton->setGeometry(120,720,70,70);
    SuspendButton->setIconSize(QSize(70,70));
    SuspendButton->setStyleSheet("QToolButton{background-color:transparent;}");    //背景透明
    SuspendButton->setIcon(QIcon(":/new/icon/Suspend.svg"));
    */


    PreButton = new QToolButton(this);           //上一首按钮
    PreButton->setGeometry(50,720,80,80);
    PreButton->setIconSize(QSize(80,80));
    PreButton->setToolTip("上一首");
    PreButton->setStyleSheet("QToolButton{background-color:transparent;}");     //背景透明
    PreButton->setIcon(QIcon(":/new/icon/Pre.svg"));
    PreButton->setShortcut(Qt::Key_F2);

    NextButton = new QToolButton(this);          //下一首按钮
    NextButton->setGeometry(260,720,80,80);
    NextButton->setIconSize(QSize(80,80));
    NextButton->setToolTip("下一首");
    NextButton->setStyleSheet("QToolButton{background-color:transparent;}");    //背景透明
    NextButton->setIcon(QIcon(":/new/icon/Next.svg"));
    NextButton->setShortcut(Qt::Key_F4);

    SlienceButton = new QPushButton(this);
    SlienceButton->setGeometry(1300,775,40,40);
    SlienceButton->setToolTip("静音");
    SlienceButton->setStyleSheet("QPushButton{background-color:transparent;}");
    SlienceButton->setIconSize(QSize(40,40));
    SlienceButton->setIcon(QIcon(":/new/icon/Volumn.svg"));
    SlienceButton->setShortcut(Qt::Key_F5);

    FileButton = new QPushButton(this);          //文件按钮
    FileButton->setGeometry(30,50,100,30);
    FileButton->setText("打开文件");
    FileButton->setShortcut(Qt::Key_F1);

    RemoveButton = new QPushButton(this);
    RemoveButton->setGeometry(140,50,100,30);
    RemoveButton->setText("移除文件");

    ClearButton = new QPushButton(this);
    ClearButton->setGeometry(250,50,100,30);
    ClearButton->setText("清空文件");

    VideoButton = new QPushButton(this);
    VideoButton->setGeometry(400,50,100,30);
    VideoButton->setText("打开MV");

    upScaleVideo = new QPushButton(this);
    upScaleVideo->setGeometry(510,50,100,30);
    upScaleVideo->setText("放大MV");

    closeVideo = new QPushButton(this);
    closeVideo->setGeometry(620,50,100,30);
    closeVideo->setText("关闭当前MV");

    PlaybackMode = new QPushButton(this);
    PlaybackMode->setGeometry(1150,720,70,70);
    PlaybackMode->setToolTip("切换播放顺序");
    PlaybackMode->setStyleSheet("QPushButton{background-color:transparent;}");
    PlaybackMode->setIconSize(QSize(70,70));
    PlaybackMode->setIcon(QIcon(":/new/icon/Loop.svg"));

    ModeSelect = new QPushButton(this);        //输出模式选择
    ModeSelect->setGeometry(150,660,30,30);
    ModeSelect->setStyleSheet("QPushButton{background-color:transparent;}");
    ModeSelect->setIconSize(QSize(30,30));
    ModeSelect->setIcon(QIcon(":/new/icon/ModeChange.svg"));

//    attenton1 = new QLabel(this);
//    attenton1->setPalette(white);
//    attenton1->setGeometry(30,650,400,30);
//    attenton1->setText("注意：HiFi模式只支持PCM裸流输出，建议");
//    attenton2 = new QLabel(this);
//    attenton2->setPalette(white);
//    attenton2->setGeometry(30,680,400,30);
//    attenton2->setText("播放wave格式文件,选取歌曲时只支持单曲选择。");
    OutputModeSelect = new QLabel(this);
    OutputModeSelect->setPalette(white);
    OutputModeSelect->setText("输出模式");
    OutputModeSelect->setGeometry(60,660,80,30);

    lineEdit = new QLineEdit(this);
    lineEdit->setGeometry(120,100,1000,30);

    /*实现音频可视化(显示频谱)，实际运行极易闪退，故先注释掉
    if(OutputMode % 2 == 0)
    {
        customPlot = new QCustomPlot(this);
        customPlot->setGeometry(50,530,350,50);
        customPlot->setStyleSheet("background-color:black");

        QFile FileInfo(lineEdit->text());
        if (!FileInfo.open(QIODevice::ReadOnly))
        {
            return ;
        }
        FileInfo.read(WavFileHeader.RiffName, sizeof(WavFileHeader.RiffName));
        FileInfo.read((char*)&WavFileHeader.nRiffLength, sizeof(WavFileHeader.nRiffLength));
        FileInfo.read(WavFileHeader.WavName, sizeof(WavFileHeader.WavName));
        FileInfo.read(WavFileHeader.FmtName, sizeof(WavFileHeader.FmtName));
        FileInfo.read((char*)&WavFileHeader.nFmtLength, sizeof(WavFileHeader.nFmtLength));
        FileInfo.read((char*)&WavFileHeader.nAudioFormat, sizeof(WavFileHeader.nAudioFormat));
        FileInfo.read((char*)&WavFileHeader.nChannleNumber, sizeof(WavFileHeader.nChannleNumber));
        FileInfo.read((char*)&WavFileHeader.nSampleRate, sizeof(WavFileHeader.nSampleRate));
        FileInfo.read((char*)&WavFileHeader.nBytesPerSecond, sizeof(WavFileHeader.nBytesPerSecond));
        FileInfo.read((char*)&WavFileHeader.nBytesPerSample, sizeof(WavFileHeader.nBytesPerSample));
        FileInfo.read((char*)&WavFileHeader.nBitsPerSample, sizeof(WavFileHeader.nBitsPerSample));

        QString strAppendMessageData;
        if (WavFileHeader.nFmtLength >= 18)
        {
            FileInfo.read((char*)&WavFileHeader.nAppendMessage, sizeof(WavFileHeader.nAppendMessage));

            int appendMessageLength = WavFileHeader.nFmtLength - 18;
            WavFileHeader.AppendMessageData = new char[appendMessageLength];
            FileInfo.read(WavFileHeader.AppendMessageData, appendMessageLength);
            strAppendMessageData = QString(WavFileHeader.AppendMessageData);
        }
        char chunkName[5];
        FileInfo.read(chunkName, sizeof(chunkName) - 1);
        chunkName[4] = '\0';
        QString strChunkName(chunkName);
        if (strChunkName.compare("fact") == 0)
        {
            strcpy(WavFileHeader.FactName, chunkName);
            FileInfo.read((char*)&WavFileHeader.nFactLength, sizeof(WavFileHeader.nFactLength));
            FileInfo.read(WavFileHeader.FactData, sizeof(WavFileHeader.FactData));
            FileInfo.read(WavFileHeader.DATANAME, sizeof(WavFileHeader.DATANAME));
        }
        else
        {
            strcpy(WavFileHeader.DATANAME, chunkName);
        }

        FileInfo.read((char*)&WavFileHeader.nDataLength, sizeof(WavFileHeader.nDataLength));

        pcmData = FileInfo.readAll();
        WavFileHeader.fileDataSize = pcmData.size();
        WavFileHeader.fileTotalSize = WavFileHeader.nRiffLength + 8;
        WavFileHeader.fileHeaderSize = WavFileHeader.fileTotalSize - WavFileHeader.fileDataSize;

        FileInfo.close();

        QVector<double> waveData;
            uint len = WavFileHeader.fileDataSize/WavFileHeader.nBytesPerSample;
            qDebug()<<__FUNCTION__<<pcmData.size()<<len;
            short *data = (short *)pcmData.data();
            for (uint i = 0; i < len; i++)
            {
                waveData.append(data[i]);
            }
            customPlot->addGraph();
            customPlot->graph(0)->setPen(QPen(Qt::blue));

            customPlot->addGraph();
            customPlot->graph(1)->setPen(QPen(Qt::red));

            QVector<double> x(len);
            for (uint i=0; i<len; ++i)
            {
                x[i] = i;
            }
            QVector<double> lineX(2),lineY(2);
            lineX[0]=lineX[1]=0;
            lineY[0]=-100000;
            lineY[1]=100000;
            customPlot->graph(0)->setData(x, waveData);
            customPlot->graph(1)->setData(lineX,lineY);
            customPlot->graph(0)->rescaleAxes();
    }
    */


    //滑条样式
    QString sliderSheet=
            "QSlider::groove:horizontal {"
            "border: 1px solid #00B0AE;"
            "background: #00B0AE;"
            "height: 5px;"
            "border-radius: 1px;"
            "padding-left:0px;"
            "padding-right:0px;"
            "}"

            "QSlider::sub-page:horizontal {"

            "background: #00B0AE;"
            "border: 1px solid #00B0AE;"
            "height: 2px;"
            "border-radius: 2px;"
            "}"

            "QSlider::add-page:horizontal {"
            "background: #EAEAEA;"
            "border: 0px solid #EAEAEA;"
            "height: 2px;"
            "border-radius: 2px;"
            "}"
            "QSlider::handle:horizontal "
            "{"
                "background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, "
                "stop:0.6 #00B0AE,stop:0.98409 rgba(255, 255, 255, 255));"

                "width: 30px;"
                "margin-top: -11px;"
                "margin-bottom: -11px;"
                "border-radius: 13px;"
            "}"

            "QSlider::handle:horizontal:hover {"
                "background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,"
                "stop:0.6 #00B0AE,stop:0.98409 rgba(255, 255, 255, 255));"

                "width: 30px;"
                "margin-top: -11px;"
                "margin-bottom: -11px;"
                "border-radius: 13px;"
            "}";

    //播放进度条
    sliderPosition = new QSlider(this);
    sliderPosition->setOrientation(Qt::Horizontal);
    sliderPosition->setStyleSheet(sliderSheet);
    sliderPosition->setGeometry(400,740,700,30);

    //音量条
    sliderVolumn = new QSlider(this);
    sliderVolumn->setStyleSheet(sliderSheet);
    sliderVolumn->setGeometry(1300,670,30,100);
    sliderVolumn->setMinimum(0);
    sliderVolumn->setMaximum(100);
    sliderVolumn->setValue(100);
//    int nValue = sliderVolumn->value();
//    QString nowValue = QString::number(nValue);
//    sliderVolumn->setToolTip(nowValue);


    //播放MV视频
    VideoList = new QListWidget(this);
    VideoList->setGeometry(500,600,800,100);
    VideoList->setStyleSheet("background-color:transparent");
    VideoList->setFrameShape(QListWidget::NoFrame);

    VideoPlayer = new QMediaPlayer(this);
    VideoPlayer->setVideoOutput(VideoWidget);
    VideoPlayList = new QMediaPlaylist(this);
    VideoPlayList->setPlaybackMode(QMediaPlaylist::Loop);
    VideoPlayer->setPlaylist(VideoPlayList);

    connect(PlayButton,SIGNAL(clicked()),this,SLOT(on_PlayButton_clicked()));
    connect(FileButton,SIGNAL(clicked()),this,SLOT(on_FileButton_clicked()));
    connect(PreButton,SIGNAL(clicked()),this,SLOT(on_PreButton_clicked()));
    connect(NextButton,SIGNAL(clicked()),this,SLOT(on_NextButton_clicked()));
    connect(RemoveButton,SIGNAL(clicked()),this,SLOT(on_RemoveButton_clicked()));
    connect(ClearButton,SIGNAL(clicked()),this,SLOT(on_ClearButton_clicked()));
    connect(SlienceButton,SIGNAL(clicked()),this,SLOT(on_SlienceButton_clicked()));
    connect(VideoButton,SIGNAL(clicked()),this,SLOT(on_VideoButton_clicked()));
    connect(ListWidget,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(on_listWidget_doubleClicked(const QModelIndex &)));
    connect(ModeSelect,SIGNAL(clicked()),this,SLOT(on_ModeSelect_clicked()));
    connect(sliderPosition,SIGNAL(sliderMoved(int)),this,SLOT(on_sliderPosition_sliderMoved(int)));
    connect(sliderVolumn,SIGNAL(sliderMoved(int)),this,SLOT(on_sliderVolumn_sliderMoved(int)));
    connect(PlaybackMode,SIGNAL(clicked()),this,SLOT(on_PlaybackMode_clicked()));
    connect(closeVideo,SIGNAL(clicked()),this,SLOT(on_closeVideo_clicked()));
    connect(upScaleVideo,SIGNAL(clicked()),this,SLOT(on_upScaleVideo_clicked()));

    //菜单
    mb = menuBar();                             //菜单栏
    settings = new QMenu("设置");                 //设置菜单
    settings0 = new QMenu("帮助");                 //帮助菜单
    setting1 = new QAction("输出",NULL);
    setting2 = new QAction("主题",NULL);
    setting3 = new QAction("使用说明",NULL);
    setting4 = new QAction("关于",NULL);
    settings->addAction(setting1);
    settings->addAction(setting2);
    settings0->addAction(setting3);
    settings0->addAction(setting4);
    mb->addMenu(settings);
    mb->addMenu(settings0);
    mb->addSeparator();
    setting1->setIcon(QIcon(":/new/icon/setting1.svg"));
    setting2->setIcon(QIcon(":/new/icon/setting2.svg"));
    setting3->setIcon(QIcon(":/new/icon/Help.svg"));
    setting4->setIcon(QIcon(":/new/icon/About.svg"));
    connect(setting1,SIGNAL(triggered()),this,SLOT(on_setting1_triggered()));
    connect(setting2,SIGNAL(triggered()),this,SLOT(on_setting2_triggered()));
    connect(setting3,SIGNAL(triggered()),this,SLOT(on_setting3_triggered()));
    connect(setting4,SIGNAL(triggered()),this,SLOT(on_setting4_triggered()));



    //pcm/wav音频输出
    format.setSampleRate(44100);        //默认采样率
    format.setChannelCount(2);          //默认通道
    format.setSampleSize(16);           //默认采样位数
    format.setByteOrder(QAudioFormat::LittleEndian);    //设置端字节序：小端序
    format.setSampleType(QAudioFormat::UnSignedInt);    //存储样本数据类型
    format.setCodec("audio/pcm");               //编码器


    //监测当前设置是否支持format设置的格式
     QAudioDeviceInfo infodev = QAudioDeviceInfo::defaultInputDevice();
     if(!infodev.isFormatSupported(format))
     {
         qDebug()<<"format";
     }

     output = new QAudioOutput(format,this);
     connect(output,&QAudioOutput::stateChanged,this,&MainWindow::onStateChanged1);


     //普通模式音频输出
     Player = new QMediaPlayer(this);
     PlayList = new QMediaPlaylist(this);
     PlayList->setPlaybackMode(QMediaPlaylist::Loop);
     Player->setPlaylist(PlayList);

     connect(VideoList,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(on_VideoList_doubleClicked(const QModelIndex &)));
     connect(Player,SIGNAL(positionChanged(qint64)),this,SLOT(onPositionChanged(qint64)));
     connect(Player,SIGNAL(durationChanged(qint64)),this,SLOT(onDurationChanged(qint64)));
     connect(PlayList,SIGNAL(currentIndexChanged(int)),this,SLOT(onPlaylistChanged(int)));
     connect(Player,&QMediaPlayer::metaDataAvailableChanged,[=](){

         //状态栏第一格显示曲目名称
         auto title = Player->metaData(QMediaMetaData::Title);
         QString title1 = title.toString();
         status1->setText(title1 + " ");
         status1->setToolTip("歌曲名称");

         //状态栏第二格显示曲目作者
         auto Author = Player->metaData(QMediaMetaData::Author);
         QString Author1 = Author.toString();
         status2->setText(Author1 + " ");
         status2->setToolTip("作者");

         //状态栏第三格显示曲目专辑
         auto AlbumTitle = Player->metaData(QMediaMetaData::AlbumTitle);
         QString AlbumTitle1 = AlbumTitle.toString();
         status3->setText(AlbumTitle1 + " ");
         status3->setToolTip("专辑");

         //状态栏第四格显示曲目比特率
         auto BitRate = Player->metaData(QMediaMetaData::AudioBitRate);
         float BitRate1 = BitRate.toFloat();
         float BitRate2 = BitRate1 / 1000.00;
         QString BitRate0 = QString("%1").arg(BitRate2);
         status4->setText(BitRate0 + " kBps" + " ");
         status4->setToolTip("比特率");

         //显示专辑封面
         auto image = Player->metaData(QMediaMetaData::ThumbnailImage);
         QImage image1 = image.value<QImage>();
         if(image1.height() != 0)
         {
             image1 = image1.scaled(350,350);
             setImage->setPixmap(QPixmap::fromImage(image1));
         }
         else
         {
             QPixmap image0 = QPixmap(":/new/mainwindow/noImage.png");
             image0 = image0.scaled(350,350);
             setImage->setPixmap(image0);
         }
     });
}


MainWindow::~MainWindow()
{

}


//设置背景图片
void MainWindow::setBackground(QImage(image))
{
    QPalette pa(this->palette());
    QImage backgroundImage = QImage(image);
    backgroundImage =backgroundImage.scaled(this->size());
    QBrush *pic = new QBrush(backgroundImage);
    pa.setBrush(QPalette::Window,*pic);
    this->setPalette(pa);
}


//转换输出模式按钮
void MainWindow::on_ModeSelect_clicked()
{
    ++ ModeCount;
    if(ModeCount % 2 == 0)
    {
        OutputMode = 2;
        ModeSelect->setIcon(QIcon(":/new/icon/ModeChanged.svg"));
        nowMode->setText("现在是HiFi模式");
    }
    else if (ModeCount % 2 == 1)
    {
        OutputMode = 1;
        ModeSelect->setIcon(QIcon(":/new/icon/ModeChange.svg"));
        nowMode->setText("现在是兼容模式");
    }
    VideoWidget->raise();
}


//状态变化(HiFi模式)
void MainWindow::onStateChanged1(QAudio::State state)
{
    if(state == QAudio::IdleState)
    {
        SaveFile.close();
    }
}


//播放列表变化
void MainWindow::onPlaylistChanged(int position)
{
    ListWidget->setCurrentRow(position);
    QListWidgetItem *item = ListWidget->currentItem();
    if(item)
    {
        CurMedia->setText(item->text());
    }
}


//更新进度
void MainWindow::onDurationChanged(qint64 duration)
{
    sliderPosition->setMaximum(int(duration));
    int sec = int(duration) / 1000;
    int min = sec / 60;
    sec = sec % 60;
    durationTime = QString::asprintf("%d:%d",min,sec);
    CurMedia->setText(positionTime + "/" + durationTime);
}


//打开文件
void MainWindow::on_FileButton_clicked()
{
    if(OutputMode == 1)
    {
        QString curPath = QDir::homePath();
        QString dlgTitle = "选择音频文件";
        QString filter = "音频文件(*.mp3 *.wav *.wma *.flac *.ogg)";
        QStringList fileList = QFileDialog::getOpenFileNames(this,dlgTitle,curPath,filter);
        if(fileList.count() < 1)
        {
            return;
        }
        for(int i = 0;i < fileList.count();++ i)
        {
            QString aFile = fileList.at(i);
            qDebug()<<QUrl::fromLocalFile(aFile)<<endl;
            PlayList->addMedia(QUrl::fromLocalFile(aFile));     //添加音频文件
            QFileInfo fileInfo(aFile); 
            itemN = new QListWidgetItem(fileInfo.fileName());
            itemN->setForeground(whiteB);
            ListWidget->addItem(itemN);
        }
        if(Player->state() != QMediaPlayer::PlayingState)
        {
            PlayList->setCurrentIndex(0);
        }
    }
    if(OutputMode == 2)
    {
        QString fileName = QFileDialog::getOpenFileName();//弹出文件对话框获取文件路径与名称
        lineEdit->setText(fileName);
    }
    VideoWidget->raise();
}


//打开MV
void MainWindow::on_VideoButton_clicked()
{

    QString curPath = QDir::homePath();
    QString dlgTitle = "选择MV视频文件";
    QString filter = "视频文件(*.mp4)";
    QStringList videoList = QFileDialog::getOpenFileNames(this,dlgTitle,curPath,filter);
    if(videoList.count() < 1)
    {
        return;
    }
    for(int i = 0;i < videoList.count();++ i)
    {
        QString aVideo = videoList.at(i);
        qDebug()<<QUrl::fromLocalFile(aVideo);
        VideoPlayList->addMedia(QUrl::fromLocalFile(aVideo));     //添加视频文件
        QFileInfo fileInfo(aVideo);
        itemN = new QListWidgetItem(fileInfo.fileName());
        itemN->setForeground(whiteB);
        VideoList->addItem(itemN);
    }
    if(VideoPlayer->state() != QMediaPlayer::PlayingState)
    {
        VideoPlayList->setCurrentIndex(0);
    }
}

//播放位置变化更新进度条
void MainWindow::onPositionChanged(qint64 position)
{
    if(sliderPosition->isSliderDown())
    {
        return;
    }
    sliderPosition->setSliderPosition(int(position));
    int sec = int(position) / 1000;
    int min = sec / 60;
    sec = sec % 60;
    positionTime = QString::asprintf("%d:%d",min,sec);
    CurMedia->setText(positionTime + "/" + durationTime);
}



/*
//强制HiFi模式获取文件
void MainWindow::on_FileButton2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName();//弹出文件对话框获取文件路径与名称
    lineEdit->setText(fileName);
}
*/


//播放音频
void MainWindow::on_PlayButton_clicked()
{
    if(PlayorPause)
    {
        Player->pause();
        output->suspend();
        PlayorPause = false;
        PlayButton->setIcon(QIcon(":/new/icon/Play.svg"));

    }
    else
    {
        if(OutputMode == 1)
        {
            if(PlayList->currentIndex() < 0)
            {
                PlayList->setCurrentIndex(0);
            }
            Player->play();
            output->stop();
            auto title = Player->metaData(QMediaMetaData::Title);
            QString title1 = title.toString();
            status1->setText(title1);
            auto image = Player->metaData(QMediaMetaData::ThumbnailImage);
            QImage image1 = image.value<QImage>();
            if(image1.height() != 0)
            {
                image1 = image1.scaled(350,350);
                setImage->setPixmap(QPixmap::fromImage(image1));
            }
            else
            {
                QPixmap image0 = QPixmap(":/new/mainwindow/noImage.png");
                image0 = image0.scaled(350,350);
                setImage->setPixmap(image0);
            }
        }
        if(OutputMode == 2)
        {
            QString name = lineEdit->text();
            SaveFile.setFileName(name);
            SaveFile.open(QFileDevice::ReadOnly);   //打开音频文件只读
            output->start(&SaveFile);
            Player->stop();
        }
        PlayorPause = true;
        PlayButton->setIcon(QIcon(":/new/icon/Suspend.svg"));
    }
    on_closeVideo_clicked();
}


//切换播放顺序
void MainWindow::on_PlaybackMode_clicked()
{
    PlaybackCount ++;
    if(PlaybackCount % 5 == 0)
    {
        PlayList->setPlaybackMode(QMediaPlaylist::Random);
        PlaybackMode->setIcon(QIcon(":/new/icon/Random.svg"));
    }
    if(PlaybackCount % 5 == 1)
    {
        PlayList->setPlaybackMode(QMediaPlaylist::Loop);
        PlaybackMode->setIcon(QIcon(":/new/icon/Loop.svg"));
    }
    if(PlaybackCount % 5 == 2)
    {
        PlayList->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        PlaybackMode->setIcon(QIcon(":/new/icon/CurrentItemOnce.svg"));
    }
    if(PlaybackCount % 5 == 3)
    {
        PlayList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        PlaybackMode->setIcon(QIcon(":/new/icon/CurrentItemInLoop.svg"));
    }
    if(PlaybackCount % 5 == 4)
    {
        PlayList->setPlaybackMode(QMediaPlaylist::Sequential);
        PlaybackMode->setIcon(QIcon(":/new/icon/Sequential.svg"));
    }
    VideoWidget->raise();
}

//移除文件
void MainWindow::on_RemoveButton_clicked()
{
    int pos = ListWidget->currentRow();
    QListWidgetItem *item = ListWidget->takeItem(pos);
    delete item;                        //在ListWidget中移除
    PlayList->removeMedia(pos);         //在播放列表中移除
    VideoWidget->raise();
}


//清空列表
void MainWindow::on_ClearButton_clicked()
{
    PlayList->clear();
    ListWidget->clear();
    Player->stop();
    VideoWidget->raise();
}


//关闭MV
void MainWindow::on_closeVideo_clicked()
{
    VideoWidget->close();
    VideoPlayer->stop();
}


//放大MV
void MainWindow::on_upScaleVideo_clicked()
{
    VideoWidget->setGeometry(700,330,1400,787);
    VideoWidget->raise();
}


//静音
void MainWindow::on_SlienceButton_clicked()
{
    bool mute = Player->isMuted();
    Player->setMuted(!mute);
    bool mute1 = VideoPlayer->isMuted();
    VideoPlayer->setMuted(!mute1);
    SlienceCount ++;
    if(SlienceCount % 2 != 0)
    {
        SlienceButton->setIcon(QIcon(":/new/icon/Volumn.svg"));
    }
    else
    {
        SlienceButton->setIcon(QIcon(":/new/icon/Slience.svg"));
    }
    VideoWidget->raise();
}


//音乐播放进度条控制
void MainWindow::on_sliderPosition_sliderMoved(int value)
{
    Player->setPosition(value);
    VideoWidget->raise();
}


//声音进度条控制
void MainWindow::on_sliderVolumn_sliderMoved(int value)
{
    Player->setVolume(value);
    output->setVolume(value);
    VideoPlayer->setVolume(value);
    VideoWidget->raise();
}


//上一首
void MainWindow::on_PreButton_clicked()
{
    PlayList->previous();
}


//下一首
void MainWindow::on_NextButton_clicked()
{
    PlayList->next();
}


//输出设置界面
void MainWindow::on_setting1_triggered()
{
    set1 = new QWidget;
    set1->setGeometry(200,250,500,800);
    set1->setWindowTitle("输出设置");
    set1->setWindowIcon(QIcon(":/new/icon/setting1.svg"));
    set1->setAttribute(Qt::WA_QuitOnClose,false);

    SampleRate = new QComboBox(set1);               //设置采样率
    SampleRate->setGeometry(120,100,150,30);
    SampleRate->addItem("44100");
    SampleRate->addItem("48000");
    SampleRate->addItem("96000");
    SampleRate->addItem("192000");
    SampleRate->addItem("384000");
    Hz = new QLabel(set1);
    Hz->setGeometry(270,100,50,30);
    Hz->setText("Hz");

    ChannelCount = new QComboBox(set1);             //设置音频通道数
    ChannelCount->setGeometry(120,200,150,30);
    ChannelCount->addItem("2");
    ChannelCount->addItem("1");

    SampleSize = new QComboBox(set1);               //设置采样位数
    SampleSize->setGeometry(120,300,150,30);
    SampleSize->addItem("16");
    SampleSize->addItem("8");
    SampleSize->addItem("24");
    SampleSize->addItem("32");

    sLabel1 = new QLabel(set1);
    sLabel2 = new QLabel(set1);
    sLabel3 = new QLabel(set1);
    sLabel1->setGeometry(20,100,80,30);
    sLabel2->setGeometry(20,200,80,30);
    sLabel3->setGeometry(20,300,80,30);
    sLabel1->setText("采样率");
    sLabel2->setText("音频通道");
    sLabel3->setText("采样位数");

    connect(SampleRate,SIGNAL(currentIndexChanged(int)), this, SLOT(on_Combobox1_changed(int)));
    connect(ChannelCount,SIGNAL(currentIndexChanged(int)), this, SLOT(on_Combobox2_changed(int)));
    connect(SampleSize,SIGNAL(currentIndexChanged(int)), this, SLOT(on_Combobox3_changed(int)));

    attenton3 = new QLabel(set1);
    attenton3->setGeometry(20,400,300,30);
    attenton3->setText("注意：以上设置仅对HiFi模式生效");
    attenton4 = new QLabel(set1);
    attenton4->setGeometry(20,430,400,30);
    attenton4->setText("若音频本身质量达不到要求，请谨慎调整");


    /*
    mode1 = new QLabel(set1);
    mode1->setGeometry(150,30,80,30);
    mode1->setText("兼容模式");
    mode2 = new QLabel(set1);
    mode2->setGeometry(310,30,80,30);
    mode2->setText("HiFi模式");

    attenton1 = new QLabel(set1);
    attenton1->setGeometry(100,60,400,30);
    attenton1->setText("注意：HiFi模式只支持PCM裸流输出，建议");
    attenton2 = new QLabel(set1);
    attenton2->setGeometry(100,90,400,30);
    attenton2->setText("播放wave格式文件,选取歌曲时只支持单曲选择。");

    OutputModeSelect = new QLabel(set1);
    OutputModeSelect->setText("输出模式");
    OutputModeSelect->setGeometry(20,30,80,30);
    ModeSelect1 = new QRadioButton(set1);           //设置radio button
    ModeSelect1->setGeometry(120,30,30,30);
    ModeSelect2 = new QRadioButton(set1);
    ModeSelect2->setGeometry(280,30,30,30);
    group1 = new QButtonGroup(set1);
    group1->addButton(ModeSelect1,1);
    group1->addButton(ModeSelect2,2);
    ModeSelect1->setChecked(1);
    if(group1->checkedId() == 1)
    {
        OutputMode = 1;
        qDebug("1");
        ModeSelect1->setChecked(1);
    }
    else
    {
        OutputMode = 2;
        qDebug("2");
        ModeSelect2->setChecked(1);
    }
    */

    set1->show();
}


//主题设置界面
void MainWindow::on_setting2_triggered()
{
    set2 = new QWidget;
    set2->setGeometry(200,250,500,400);
    set2->setWindowTitle("主题设置");
    set2->setWindowIcon(QIcon(":/new/icon/setting2.svg"));
    set2->setAttribute(Qt::WA_QuitOnClose,false);

    background = new QComboBox(set2);
    background->setGeometry(120,150,150,30);
    background->addItem("预设一");
    background->addItem("预设二");
    background->addItem("预设三");
    background->addItem("预设四");

    tip1 = new QLabel(set2);
    tip1->setGeometry(20,150,80,30);
    tip1->setText("预设背景");

    setbg = new QPushButton(set2);
    setbg->setGeometry(120,220,150,30);
    setbg->setText("自定义背景图片");

    connect(background,SIGNAL(currentIndexChanged(int)), this, SLOT(on_Combobox4_changed(int)));
    connect(setbg,SIGNAL(clicked()),this,SLOT(on_setbg_clicked()));

    set2->show();
}

void MainWindow::on_setting3_triggered()
{
    set3 = new QWidget;
    set3->setGeometry(200,250,500,800);
    set3->setWindowTitle("使用说明");
    set3->setWindowIcon(QIcon(":/new/icon/Help.svg"));
    set3->setAttribute(Qt::WA_QuitOnClose,false);

    HelpText = new QTextBrowser(set3);
    HelpText->setGeometry(0,0,500,800);
    HelpText->setText("本程序是一款本地HiFi音乐播放器，可以播放本地音乐和MV视频。\n"
                      "以下是功能上的使用说明：\n\n"
                      "添加音乐：通过界面左上角的打开文件按钮可以选择电脑中的音频文件,兼容模式下可以多选，HiFi模式下只支持单选。\n\n"
                      "播放音乐：点击播放列表中的音乐后单击播放按钮，或者直接双击列表中的歌曲。\n\n"
                      "切换播放顺序：点击右下角切换播放顺序按钮，播放器支持列表循环，单曲循环，列表顺序播放，随机播放，单曲播放一次后停止等播放顺序。\n\n"
                      "删除列表中歌曲：单击列表中音乐后点击右上角移除文件按钮可以将此歌曲移出列表，点击清空文件将删除列表中所有歌曲。\n\n"
                      "切换播放模式：播放器包括兼容模式和HiFi模式两种播放模式，通过左中下的模式切换按钮进行切换，播放器默认是兼容模式，此时模式切换按钮显示为蓝色，兼容模式中歌曲可以以列表呈现，并且支持多种格式的音乐。"
                      "HiFi模式下模式切换按钮显示为红色，添加歌曲只支持单曲，并且歌曲格式最好为wav，此模式可以调整音乐播放的参数。\n\n"
                      "快捷键一览：\n\n"
                      "F1 添加文件 \n"
                      "F2 上一首 \n"
                      "F3 播放/暂停 \n"
                      "F4 下一首 \n"
                      "F5 静音 \n");

    set3->show();
}

void MainWindow::on_setting4_triggered()
{
    set4 = new QWidget;
    set4->setGeometry(200,250,500,400);
    set4->setWindowTitle("关于");
    set4->setWindowIcon(QIcon(":/new/icon/About.svg"));
    set4->setAttribute(Qt::WA_QuitOnClose,false);

    AboutText = new QTextBrowser(set4);
    AboutText->setGeometry(0,150,500,250);
    AboutText->setText("EasyHiFi ver2.0 \n\n\n"
                       "基于QT C++的本地音乐、视频播放器\n"
                       "By CSU_lyz");
    Icon = new QLabel(set4);
    Icon->setPixmap(QPixmap(":/new/mainwindow/icon.png"));
    Icon->setGeometry(0,0,500,150);

    set4->show();
}

//输出设置采样率选项
void MainWindow::on_Combobox1_changed(int index)
{
    QString str1 = SampleRate->itemText(index);
    int num1 = str1.toInt();
    format.setSampleRate(num1);
}


//输出设置音频通道选项
void MainWindow::on_Combobox2_changed(int index)
{
    QString str2 = SampleRate->itemText(index);
    int num2 = str2.toInt();
    format.setChannelCount(num2);
}


//输出设置采样位数选项
void MainWindow::on_Combobox3_changed(int index)
{
    QString str3 = SampleRate->itemText(index);
    int num3 = str3.toInt();
    format.setSampleSize(num3);
}


//选择预设背景
void MainWindow::on_Combobox4_changed(int index)
{
    switch(index)
    {
case 0:
    setBackground(QImage(":/new/mainwindow/bg1.png"));
        break;
case 1:
    setBackground(QImage(":/new/mainwindow/bg2.png"));
        break;
case 2:
    setBackground(QImage(":/new/mainwindow/bg3.png"));
        break;
case 3:
    setBackground(QImage(":/new/mainwindow/bg4.png"));
    }
}


//自定义背景图片
void MainWindow::on_setbg_clicked()
{
    QString curPath = QDir::homePath();
    QString dlgTitle = "选择背景";
    QString filter = "图片文件(*.jpg *.png *.jpeg)";
    QString file1 = QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);
    setBackground(QImage(file1));
}


//双击歌曲列表播放
void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    int rowNow=index.row();
    PlayList->setCurrentIndex(rowNow);
    Player->play();
    PlayorPause = true;
    PlayButton->setIcon(QIcon(":/new/icon/Suspend.svg"));
    on_closeVideo_clicked();
}

//双击MV列表播放
void MainWindow::on_VideoList_doubleClicked(const QModelIndex &index)
{
    Player->stop();
    PlayorPause = false;
    on_closeVideo_clicked();
    PlayButton->setIcon(QIcon(":/new/icon/Play.svg"));
    int rowNow=index.row();
    VideoPlayList->setCurrentIndex(rowNow);
    VideoPlayer->play();
    VideoWidget->setGeometry(1200,395,800,450);
    VideoWidget->show();
}
