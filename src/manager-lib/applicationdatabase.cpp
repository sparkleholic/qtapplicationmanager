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

#include <QFile>
#include <QDataStream>
#include <QTemporaryFile>

#include "application.h"
#include "applicationdatabase.h"

class ApplicationDatabasePrivate
{
public:
    QFile *file;

    ApplicationDatabasePrivate()
        : file(0)
    { }
    ~ApplicationDatabasePrivate()
    { delete file; }
};

ApplicationDatabase::ApplicationDatabase(const QString &fileName)
    : d(new ApplicationDatabasePrivate())
{
    d->file = new QFile(fileName);
    d->file->open(QFile::ReadWrite);
}

ApplicationDatabase::ApplicationDatabase()
    : d(new ApplicationDatabasePrivate())
{
    d->file = new QTemporaryFile("am-apps-db");
    d->file->open(QFile::ReadWrite);
}

ApplicationDatabase::~ApplicationDatabase()
{
    delete d;
}

bool ApplicationDatabase::isValid() const
{
    return d->file && d->file->isOpen();
}

bool ApplicationDatabase::isTemporary() const
{
    return qobject_cast<QTemporaryFile *>(d->file);
}

QString ApplicationDatabase::errorString() const
{
    return d->file->errorString();
}

QString ApplicationDatabase::name() const
{
    return d->file->fileName();
}

QList<const Application *> ApplicationDatabase::read() throw (Exception)
{
    QList<const Application *> apps;

    if (d->file->seek(0)) {
        QDataStream ds(d->file);

        forever {
            Application *app = new Application();
            ds >> *app;

            if (ds.status() != QDataStream::Ok) {
                if (ds.status() != QDataStream::ReadPastEnd) {
                    qDeleteAll(apps);
                    throw Exception(Error::System, "could not read from application database %1").arg(d->file->fileName());
                }
                break;
            }
            apps << app;
        }
    }

    return apps;
}

void ApplicationDatabase::write(const QList<const Application *> &apps) throw (Exception)
{
    if (!d->file->seek(0))
        throw Exception(*d->file, "could not not seek to position 0 in the application database");
    if (!d->file->resize(0))
        throw Exception(*d->file, "could not truncate the application database");

    QDataStream ds(d->file);
    foreach (const Application *app, apps)
        ds << *app;
    if (ds.status() != QDataStream::Ok)
        throw Exception(*d->file, "could not write to application database");
}