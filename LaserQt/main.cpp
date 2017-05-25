#include "stack_window.h"
#include <QApplication>
#include <QElapsedTimer>
#include <QFile>
#include <QSplashScreen>
#include <QTranslator>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);  // 创建Qt主应用进程

    QSplashScreen splash(QPixmap::fromImage(QImage(":/img/ui/logo_72px.png")));
    splash.setDisabled(true);
    splash.show();
    splash.showMessage(QObject::tr("..."), Qt::AlignCenter, Qt::black);

    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < 500) {
        QApplication::processEvents();
    }

    /* app全局属性配置 */
    app.setApplicationName("LaserQt");
    app.setApplicationDisplayName("LaserQt-v1.0");
    app.setApplicationVersion("version 1.0");
    app.setWindowIcon(QIcon(":/img/ui/logo_32px.png"));

    /* app全局样式配置 */
    QFile fd(":/style/style.qss");
    if (fd.open(QFile::ReadOnly | QFile::Text)) {
        app.setStyleSheet(fd.readAll());
    }

    /* app全局字体配置 */
    QTranslator translator;
    if (QLocale::system().name() == "en_US") {
        translator.load(":/qm/font/qt_en.qm");
    }
    else if (QLocale::system().name() == "zh_CN") {
        translator.load(":/qm/font/qt_zh_CN.qm");
    }
    else if (QLocale::system().name() == "zh_TW") {
        translator.load(":/qm/font/qt_zh_TW.qm");
    }
    app.installTranslator(&translator);

    StackWindow window;
    window.show();
    splash.finish(&window);

    return app.exec();
}
