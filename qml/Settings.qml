import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Configuration 1.0

Page {
    id: page

    ConfigurationValue {
        id: gestures
        key: "/apps/motogestures/enabled-gestures"
        defaultValue: [ "chopchopsensor", "cameragesturesensor", "liftgesturesensor" ]

        function set(gesture, isEnabled) {
            var savedIndex = value.indexOf(gesture)
            var enabledGestures = value

            if (isEnabled && savedIndex === -1) {
                enabledGestures.push(gesture)
            } else if (!isEnabled && savedIndex > -1) {
                enabledGestures.splice(savedIndex, 1)
            }

            value = enabledGestures
        }

        function get(gesture) {
            return (value.indexOf(gesture) > -1)
        }
    }


    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTrId("motorola-gestures-settings-title")
            }

            TextSwitch {
                text: qsTrId("motorola-gestures-chopchop")
                description: qsTrId("motorola-gestures-chopchop-description")
                checked: gestures.get("chopchopsensor")
                onCheckedChanged: gestures.set("chopchopsensor", checked)
            }

            TextSwitch {
                text: qsTrId("motorola-gestures-camera")
                description: qsTrId("motorola-gestures-camera-description")
                checked: gestures.get("cameragesturesensor")
                onCheckedChanged: gestures.set("cameragesturesensor", checked)
            }

            TextSwitch {
                text: qsTrId("motorola-gestures-lift")
                description: qsTrId("motorola-gestures-lift-description")
                checked: gestures.get("liftgesturesensor")
                onCheckedChanged: gestures.set("liftgesturesensor", checked)
            }
        }
    }
}
