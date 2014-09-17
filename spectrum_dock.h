/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2010, 2011, 2012, 2013, 2014 Teunis van Beelen
*
* teuniz@gmail.com
*
***************************************************************************
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation version 2 of the License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
***************************************************************************
*
* This version of GPL is at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*
***************************************************************************
*/



#ifndef SPECTRUMANALYZERDOCK_H
#define SPECTRUMANALYZERDOCK_H



#include <QApplication>
#include <QDialog>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QSlider>
#include <QLabel>
#include <QProgressDialog>
#include <QCheckBox>
#include <QString>
#include <QStringList>
#include <QMessageBox>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "global.h"
#include "mainwindow.h"
#include "signalcurve.h"
#include "popup_messagewindow.h"
#include "utils.h"
#include "filter.h"
#include "ravg_filter.h"
#include "flywheel.h"

#include "third_party/fidlib/fidlib.h"

#include "third_party/kiss_fft/kiss_fftr.h"



class UI_Mainwindow;



class UI_SpectrumDockWindow : public QObject
{
  Q_OBJECT

public:
  UI_SpectrumDockWindow(QWidget *parent);

  ~UI_SpectrumDockWindow();

  UI_Mainwindow *mainwindow;

  QDialog *SpectrumDialog;

  QDockWidget *dock;

  struct signalcompblock *signalcomp;

  struct spectrum_markersblock *spectrum_color;

void rescan();
void init(int);
void clear();
int getsignalnr(void);

private:

  QWidget *parentwindow;

  QHBoxLayout *hlayout1,
              *hlayout2,
              *hlayout3,
              *hlayout4;

  QVBoxLayout *vlayout1,
              *vlayout2,
              *vlayout3;

  SignalCurve *curve1;

  QTimer *t1;

  QSlider *amplitudeSlider,
          *spanSlider,
          *centerSlider;

  QLabel *spanLabel,
         *centerLabel,
         *amplitudeLabel;

  QCheckBox *sqrtButton,
            *vlogButton,
            *colorBarButton;

  UI_Flywheel *flywheel1;

  int samples,
      steps,
      spectrumdock_sqrt,
      spectrumdock_vlog,
      dashboard,
      flywheel_value,
      init_maxvalue,
      signal_nr;

  volatile int busy;

  double samplefreq,
         freqstep,
         maxvalue,
         maxvalue_sqrt,
         maxvalue_vlog,
         maxvalue_sqrt_vlog,
         minvalue_vlog,
         minvalue_sqrt_vlog,
         *buf1,
         *buf2,
         *buf3,
         *buf4,
         *buf5;

  char *viewbuf,
       signallabel[512],
       physdimension[9];


private slots:

void update_curve();
void sliderMoved(int);
void sqrtButtonClicked(bool);
void vlogButtonClicked(bool);
void colorBarButtonClicked(bool);
void print_to_txt();
void setdashboard();
void update_flywheel(int);

};


#endif



















