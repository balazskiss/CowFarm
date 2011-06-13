import QtQuick 1.0
import QtWebKit 1.0

Rectangle {
    width: 900
    height: 600

    WebView{
        url: "http://balazskiss.name/projects/CowFarm/CowFarm-Client/www"
        settings.pluginsEnabled: true
        anchors.fill: parent
        anchors.centerIn: parent
    }
}
