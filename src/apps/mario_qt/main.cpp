#include "mario_game_widget.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
    spdlog::info("Starting Super Mario Platform Game...");
    
    QApplication app(argc, argv);
    app.setApplicationName("Super Mario");
    app.setApplicationVersion("1.0");
    
    // Configure OpenGL surface format
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(4); // 4x MSAA
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSwapInterval(1); // VSync
    QSurfaceFormat::setDefaultFormat(format);
    
    MarioGameWidget game;
    game.setWindowTitle("Super Mario - Qt6 (OpenGL)");
    game.show();
    
    spdlog::info("Mario game window created successfully");
    
    return app.exec();
}
