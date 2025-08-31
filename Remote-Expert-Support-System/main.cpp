#include <QApplication>
#include "server/core/server.h"
#include "ui/login/register_dialog.h"
#include "utils/network_interface_constants.h"
#include "ui/conference/conference_ui.h"
#include "ui/index/factory/main_window.h"
#include "ui/knowledge/data_viewer.h"
#include "test/login_test.h"
#include "test/video_test.h"
#include "test/conference_test.h"

int main(int argc, char *argv[])
{

    // loginTest(argc, argv);
    videoTest(argc, argv);
    // conferenceTest(argc, argv);
}
