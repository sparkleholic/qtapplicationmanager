/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.pelagicore.com/
**
** This file is part of the Pelagicore Application Manager.
**
** SPDX-License-Identifier: GPL-3.0
**
** $QT_BEGIN_LICENSE:GPL3$
** Commercial License Usage
** Licensees holding valid commercial Pelagicore Application Manager
** licenses may use this file in accordance with the commercial license
** agreement provided with the Software or, alternatively, in accordance
** with the terms contained in a written agreement between you and
** Pelagicore. For licensing terms and conditions, contact us at:
** http://www.pelagicore.com.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU General Public License version 3 requirements will be
** met: http://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

pragma Singleton
import QtQuick 2.2
import "ApplicationManager.js" as ApplicationManager

QtObject {
    id: root
    property int count: ApplicationManager.count
    property var surfaceItems: []
    signal surfaceItemReady(int index, Item item)
    signal surfaceItemClosing()
    signal surfaceItemLost()
    signal raiseApplicationWindow()
    signal surfaceWindowPropertyChanged(Item surfaceItem, string name, var value)

    function setSurfaceWindowProperty(appItem, type, status) {
        appItem.windowPropertyChanged(type, status)
    }

    function surfaceWindowProperty(item, type) {
        return false
    }

    function get(index) {
        var entry = ApplicationManager.get(index)
        entry.surfaceItem = surfaceItems[index]
        return entry
    }

    Component.onCompleted: {
        for (var i = 0; i < root.count; i++) {
            surfaceItems.push(null)
        }
    }

}
