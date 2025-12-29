#include "game_widget.h"
#include "gl_game_widget.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QSurfaceFormat>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Breakout Game");
    app.setApplicationVersion("2.0");
    
    // Command line parser
    QCommandLineParser parser;
    parser.setApplicationDescription("Breakout Game - Qt6 with OpenGL acceleration");
    parser.addHelpOption();
    parser.addVersionOption();
    
    QCommandLineOption openglOption(QStringList() << "gl" << "opengl",
                                   "Use OpenGL accelerated rendering (recommended)");
    parser.addOption(openglOption);
    
    QCommandLineOption legacyOption(QStringList() << "legacy" << "qpainter",
                                   "Use legacy QPainter rendering");
    parser.addOption(legacyOption);
    
    parser.process(app);
    
    // Determine rendering mode
    bool useOpenGL = true; // Default to OpenGL
    
    if (parser.isSet(legacyOption)) {
        useOpenGL = false;
        spdlog::info("Starting Breakout Game with QPainter rendering...");
    } else {
        spdlog::info("Starting Breakout Game with OpenGL rendering...");
        
        // Configure OpenGL surface format
        QSurfaceFormat format;
        format.setVersion(3, 3);
        format.setProfile(QSurfaceFormat::CoreProfile);
        format.setSamples(4); // 4x MSAA
        format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
        format.setSwapInterval(1); // VSync
        QSurfaceFormat::setDefaultFormat(format);
    }
    
    if (useOpenGL) {
        GLGameWidget game;
        game.setWindowTitle("Breakout Game - Qt6 (OpenGL Accelerated)");
        game.show();
        spdlog::info("OpenGL game window created successfully");
        return app.exec();
    } else {
        GameWidget game;
        game.setWindowTitle("Breakout Game - Qt6 (Legacy)");
        game.show();
        spdlog::info("Legacy game window created successfully");
        return app.exec();
    }
}
