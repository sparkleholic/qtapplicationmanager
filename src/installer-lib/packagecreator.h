/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.pelagicore.com/
**
** This file is part of the Pelagicore Application Manager.
**
** $QT_BEGIN_LICENSE:LGPL3$
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

#pragma once

#include <QObject>

#include "error.h"

QT_FORWARD_DECLARE_CLASS(QIODevice)
QT_FORWARD_DECLARE_CLASS(QDir)

class PackageCreatorPrivate;
class InstallationReport;


class PackageCreator : public QObject
{
    Q_OBJECT

public:
    PackageCreator(const QDir &sourceDir, QIODevice *output, const InstallationReport &report, QObject *parent = 0);

    QDir sourceDirectory() const;
    void setSourceDirectory(const QDir &sourceDir);

    bool create();

    QByteArray createdDigest() const;

    bool hasFailed() const;
    bool wasCanceled() const;

    Error errorCode() const;
    QString errorString() const;

public slots:
    void cancel();

signals:
    void progress(qreal progress);

private:
    PackageCreatorPrivate *d;

    friend class PackageCreatorPrivate;
};