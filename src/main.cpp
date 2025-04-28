#include <QApplication>
#include "CacheLoader.hpp"
#include "OptionExecutor.hpp"
#include "Util.hpp"
#include "WallpaperLoader.hpp"
#include "gui/MainWindow.hpp"

using namespace std;
using namespace filesystem;
using nlohmann::json;

int main(int argc, char *argv[]) {
    ConfigReader::readConfig();
    WallpaperLoader::loadWallpapers();

    if(argc >= 2) {
        OptionExecutor::getInstance().execute(argc, argv);
        return 0;
    }

    QApplication app(argc, argv);

    CacheLoader::loadCache();

    MainWindow::getInstance().show();
    MainWindow::getInstance().fillWidgets();

    return QApplication::exec();
}
