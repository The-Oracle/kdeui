/* This file is part of the KDE libraries
    Copyright (C) 2004 Antonio Larrosa <larrosa@kde.org

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "kpixmapregionselectordialog.h"

#include <QtGui/QDialog>
#include <QtGui/QDesktopWidget>
#include <QImage>
#include <QtGui/QLabel>

#include <klocale.h>
#include <kdialog.h>
#include <kpixmapregionselectorwidget.h>
#include <kvbox.h>

class KPixmapRegionSelectorDialog::Private
{
public:
    Private(KPixmapRegionSelectorDialog * parent)
        : pixmapSelectorWidget( 0 ), q(parent)
    {
    }

    KPixmapRegionSelectorWidget *pixmapSelectorWidget;
    KPixmapRegionSelectorDialog *q;

    void init() {
        //When the image is rotated we need to enforce the maximum width&height into the
        //KPixmapRegionSelectorWidget; in order to avoid the dialog to get out of the screen
        q->connect(pixmapSelectorWidget, SIGNAL(pixmapRotated()), q, SLOT(_k_adjustPixmapSize()));
    }

    void _k_adjustPixmapSize() {
        if (pixmapSelectorWidget) {
            //Set maximum size for picture
            QDesktopWidget desktopWidget;
            QRect screen = desktopWidget.availableGeometry();
            pixmapSelectorWidget->setMaximumWidgetSize(
                (int)(screen.width()*4.0/5), (int)(screen.height()*4.0/5));
        }
    }
};

KPixmapRegionSelectorDialog::KPixmapRegionSelectorDialog( QWidget *parent )
  : KDialog( parent ),
    d( new Private(this) )
{
  setCaption( i18n("Select Region of Image") );
  setButtons( Help|Ok|Cancel );

  KVBox *vbox=new KVBox(this);
  new QLabel(i18n("Please click and drag on the image to select the region of interest:"), vbox);
  d->pixmapSelectorWidget= new KPixmapRegionSelectorWidget(vbox);

  vbox->setSpacing( -1 );

  setMainWidget(vbox);

  d->init();
}

KPixmapRegionSelectorDialog::~KPixmapRegionSelectorDialog()
{
  delete d;
}

KPixmapRegionSelectorWidget *KPixmapRegionSelectorDialog::pixmapRegionSelectorWidget() const
{
  return d->pixmapSelectorWidget;
}

void KPixmapRegionSelectorDialog::adjustRegionSelectorWidgetSizeToFitScreen()
{
  d->_k_adjustPixmapSize();
}

QRect KPixmapRegionSelectorDialog::getSelectedRegion(const QPixmap &pixmap, QWidget *parent )
{
  KPixmapRegionSelectorDialog dialog(parent);

  dialog.pixmapRegionSelectorWidget()->setPixmap(pixmap);
  dialog.adjustRegionSelectorWidgetSizeToFitScreen();

  int result = dialog.exec();

  QRect rect;

  if ( result == QDialog::Accepted )
    rect = dialog.pixmapRegionSelectorWidget()->unzoomedSelectedRegion();

  return rect;
}

QRect KPixmapRegionSelectorDialog::getSelectedRegion(const QPixmap &pixmap, int aspectRatioWidth, int aspectRatioHeight, QWidget *parent )
{
  KPixmapRegionSelectorDialog dialog(parent);

  dialog.pixmapRegionSelectorWidget()->setPixmap(pixmap);
  dialog.pixmapRegionSelectorWidget()->setSelectionAspectRatio(aspectRatioWidth,aspectRatioHeight);
  dialog.adjustRegionSelectorWidgetSizeToFitScreen();

  int result = dialog.exec();

  QRect rect;

  if ( result == QDialog::Accepted )
    rect = dialog.pixmapRegionSelectorWidget()->unzoomedSelectedRegion();

  return rect;
}

QImage KPixmapRegionSelectorDialog::getSelectedImage(const QPixmap &pixmap, QWidget *parent )
{
  KPixmapRegionSelectorDialog dialog(parent);

  dialog.pixmapRegionSelectorWidget()->setPixmap(pixmap);
  dialog.adjustRegionSelectorWidgetSizeToFitScreen();

  int result = dialog.exec();

  QImage image;

  if ( result == QDialog::Accepted )
    image = dialog.pixmapRegionSelectorWidget()->selectedImage();

  return image;
}

QImage KPixmapRegionSelectorDialog::getSelectedImage(const QPixmap &pixmap, int aspectRatioWidth, int aspectRatioHeight, QWidget *parent )
{
  KPixmapRegionSelectorDialog dialog(parent);

  dialog.pixmapRegionSelectorWidget()->setPixmap(pixmap);
  dialog.pixmapRegionSelectorWidget()->setSelectionAspectRatio(aspectRatioWidth,aspectRatioHeight);
  dialog.adjustRegionSelectorWidgetSizeToFitScreen();

  int result = dialog.exec();

  QImage image;

  if ( result == QDialog::Accepted )
    image = dialog.pixmapRegionSelectorWidget()->selectedImage();

  return image;
}

#include "kpixmapregionselectordialog.moc"
