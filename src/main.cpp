#include <QApplication>
#include "CacheLoader.hpp"
#include "OptionExecutor.hpp"
#include "Util.hpp"
#include "WallpaperManager.hpp"
#include "gui/MainWindow.hpp"

using namespace std;
using namespace filesystem;
using nlohmann::json;

int main(int argc, char *argv[]) {
    ConfigManager::readConfig();
    WallpaperManager::loadWallpapers();

    if(argc >= 2) {
        OptionExecutor::execute(argc, argv);
        return 0;
    }

    QApplication app(argc, argv);

    CacheLoader::loadCache();
    MainWindow::getInstance().show();
    MainWindow::getInstance().fillWidgets();

    return QApplication::exec();
}
